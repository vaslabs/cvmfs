/**
 * \file cvmfs_sync.cc
 *
 * This tool figures out the changes made to a cvmfs repository by means
 * of a union file system mounted on top of a cvmfs volume.
 * We take all three volumes (namely union, overlay and repository) into
 * account to sync the changes back into the repository.
 *
 * On the repository side we have a catalogs directory that mimicks the
 * shadow directory structure and stores compressed and uncompressed
 * versions of all catalogs.  The raw data are stored in the data
 * subdirectory in zlib-compressed form.  They are named with their SHA1
 * hash of the compressed file (like in CVMFS client cache, but with a
 * 2-level cache hierarchy).  Symlinks from the catalog directory to the
 * data directory form the connection. If necessary, add a .htaccess file
 * to allow Apache to follow the symlinks.
 *
 * Developed by Jakob Blomer 2010 at CERN
 * Adapted for Union File Systems by René Meusel 2011 at CERN
 * jakob.blomer@cern.ch
 * rene@renemeusel.de
 */


#define _FILE_OFFSET_BITS 64

#include <string>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <set>

#include "SyncUnionAufs.h"
#include "SyncMediator.h"

#include "WritableCatalogManager.h"

#include "cvmfs_config.h"
#include "cvmfs_sync.h"
#include "util.h"
#include "monitor.h"

#include "platform.h"

using namespace std;
using namespace cvmfs;

static void usage() {
   cout << "CernVM-FS sync shadow tree with repository" << endl;
   cout << "Usage: cvmfs_sync -s <union volume> -o <overlay directory> -c <cvmfs mounted volume> -r <repository store>" << endl
        << "                  [-p(rint change set)] [-d(ry run)] [-i <immutable dir(,dir)*>]" << endl
        << "                  [-k(ey file)] [-z (lazy attach of catalogs)] [-b(ookkeeping of dirty catalogs)]" << endl
        << "                  [-t <threads>] [-m(ucatalogs)]" << endl << endl
        << "Make sure that a 'data' and a 'catalogs' subdirectory exist in your repository store." << endl
        << "Also, your webserver must be able to follow symlinks in the catalogs subdirectory." << endl
        << "For Apache, you can add 'Options +FollowSymLinks' to a '.htaccess' file."
        << endl << endl;
}


bool parseParameters(int argc, char **argv, SyncParameters *p) {
	// print some help if needed
	if ((argc < 2) || (string(argv[1]) == "-h") || (string(argv[1]) == "--help") ||
		(string(argv[1]) == "-v") || (string(argv[1]) == "--version")){
		usage();
		return false;
	}

	// set defaults
	p->print_changeset = false;
	p->dry_run = false;
	p->lazy_attach = false;
	p->sync_threads = 0;
	p->mucatalogs = false;

	// read the parameters
	char c;
	while ((c = getopt(argc, argv, "s:o:c:r:pdi:k:zbt:m")) != -1) {
		switch (c) {

		// directories
		case 's':
			p->dir_shadow = MakeCanonicalPath(optarg);
			break;
		case 'c':
			p->dir_cvmfs = MakeCanonicalPath(optarg);
			break;
		case 'r': {
			const string path = MakeCanonicalPath(optarg);
			p->dir_data = path + "/data";
			p->dir_catalogs = path + "/catalogs";
			break;
		}
		case 'o':
			p->dir_overlay = MakeCanonicalPath(optarg);
			break;

		// switches
		case 'p':
			p->print_changeset = true;
			break;
		case 'd':
			p->dry_run = true;
			break;
		case 'z':
			p->lazy_attach = true;
			break;
		case 'm':
			p->mucatalogs = true;
			break;

		// misc
		case 'i': {
			char *token = strtok(optarg, ",");
			while (token != NULL) {
				p->immutables.insert(MakeCanonicalPath(token));
				token = strtok(NULL, ",");
			}
			break;
		}
		case 'k':
			p->keyfile = optarg;
			break;
		case 't':
			p->sync_threads = atoi(optarg);
			break;
		case '?':
		default:
			usage();
			return false;
		}
	}

	return true;
}

// TODO monitor::fini
bool initWatchdog() {
	umask(022);

	if (!monitor::Init(".", false)) {
		PrintError("Failed to init watchdog");
		return false;
	}
	monitor::Spawn();

	return true;
}

bool doSanityChecks(SyncParameters *p) {
	if (not DirectoryExists(p->dir_overlay)) {
		PrintError("overlay (copy on write) directory does not exist");
		return false;
	}

	if (not DirectoryExists(p->dir_shadow)) {
		PrintError("shadow directory does not exist");
		return false;
	}

	if (not DirectoryExists(p->dir_cvmfs)) {
		PrintError("mounted cvmfs repository does not exist");
		return false;
	}

	if (not DirectoryExists(p->dir_data)) {
		PrintError("data store directory does not exist");
		return false;
	}

	if (not DirectoryExists(p->dir_catalogs)) {
		PrintError("catalog store directory does not exist");
		return false;
	}

	return true;
}

bool createCacheDir(SyncParameters *p) {
	if (!MakeCacheDirectories(p->dir_data, 0755)) {
		PrintError("could not initialize data store");
		return false;
	}

	return true;
}

catalog::WritableCatalogManager* createWritableCatalogManager(const SyncParameters &p) {
  return new catalog::WritableCatalogManager(MakeCanonicalPath(p.dir_catalogs),
                                    MakeCanonicalPath(p.dir_data),
                                    p.lazy_attach);
}

SyncMediator* createSyncMediator(catalog::WritableCatalogManager* catalogManager,
                                 const SyncParameters &p) {
  return new SyncMediator(catalogManager,
                          MakeCanonicalPath(p.dir_data),
                          p.dry_run,
                          p.print_changeset);
}

SyncUnion* createSynchronisationEngine(SyncMediator* mediator,
                                       const SyncParameters &p) {
  return new SyncUnionAufs(mediator,
                           MakeCanonicalPath(p.dir_cvmfs),
                           MakeCanonicalPath(p.dir_shadow),
                           MakeCanonicalPath(p.dir_overlay));
}

int main(int argc, char **argv) {
	SyncParameters parameters;

	// do some initialization
	if (not parseParameters(argc, argv, &parameters)) return 1;
	if (not initWatchdog()) return 1;
	if (not doSanityChecks(&parameters)) return 2;
	if (not createCacheDir(&parameters)) return 3;

	// create worker objects
  catalog::WritableCatalogManager *catalogManager = createWritableCatalogManager(parameters);
  SyncMediator *mediator = createSyncMediator(catalogManager, parameters);
  SyncUnion *sync = createSynchronisationEngine(mediator, parameters);

	// sync
	if (not sync->DoYourMagic()) {
		PrintError("something went wrong during sync");
		return 4;
	}

	// clean up
	delete mediator;
	delete catalogManager;
  delete sync;
  
  std::cout << "done" << std::endl;

	return 0;
}