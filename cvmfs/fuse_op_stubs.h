/**
 * This file is part of the CernVM File System.
 *
 * Contains all unimplemented FUSE low level callbacks.
 */

#ifndef CVMFS_FUSE_OP_STUBS_H_
#define CVMFS_FUSE_OP_STUBS_H_

#include <errno.h>
#include <fuse/fuse_lowlevel.h>
#include "logging.h"

#define DEFAULT_STUB_ERROR_CODE EROFS  // read only file system

static void cvmfs_setattr(fuse_req_t req, fuse_ino_t ino, struct stat *attr,
                          int to_set, struct fuse_file_info *fi)
{
  LogCvmfs(kLogFuse, kLogDebug, "cvmfs_setattr on inode: %d", ino);
  fuse_reply_err(req, DEFAULT_STUB_ERROR_CODE);
}

static void cvmfs_mknod(fuse_req_t req, fuse_ino_t parent, const char *name,
                        mode_t mode, dev_t rdev)
{
  LogCvmfs(kLogFuse, kLogDebug, "cvmfs_mknod with name: %s", name);
  fuse_reply_err(req, DEFAULT_STUB_ERROR_CODE);
}

static void cvmfs_mkdir(fuse_req_t req, fuse_ino_t parent, const char *name,
                        mode_t mode)
{
  LogCvmfs(kLogFuse, kLogDebug, "cvmfs_mkdir with name: %s", name);
  fuse_reply_err(req, DEFAULT_STUB_ERROR_CODE);
}

static void cvmfs_unlink(fuse_req_t req, fuse_ino_t parent, const char *name) {
  LogCvmfs(kLogFuse, kLogDebug, "cvmfs_unlink for name: %s", name);
  fuse_reply_err(req, DEFAULT_STUB_ERROR_CODE);
}

static void cvmfs_rmdir(fuse_req_t req, fuse_ino_t parent, const char *name) {
  LogCvmfs(kLogFuse, kLogDebug, "cvmfs_rmdir for name: %s", name);
  fuse_reply_err(req, DEFAULT_STUB_ERROR_CODE);
}

static void cvmfs_symlink(fuse_req_t req, const char *link, fuse_ino_t parent,
                          const char *name)
{
  LogCvmfs(kLogFuse, kLogDebug, "cvmfs_symlink with name: %s and link: %s",
           name, link);
  fuse_reply_err(req, DEFAULT_STUB_ERROR_CODE);
}

static void cvmfs_rename(fuse_req_t req, fuse_ino_t parent, const char *name,
                         fuse_ino_t newparent, const char *newname)
{
  LogCvmfs(kLogFuse, kLogDebug, "cvmfs_rename from name: %s to name: %s",
           name, newname);
  fuse_reply_err(req, DEFAULT_STUB_ERROR_CODE);
}

static void cvmfs_link(fuse_req_t req, fuse_ino_t ino, fuse_ino_t newparent,
                       const char *newname)
{
  LogCvmfs(kLogFuse, kLogDebug, "cvmfs_link on inode: %d with name: %s",
           ino, newname);
  fuse_reply_err(req, DEFAULT_STUB_ERROR_CODE);
}

static void cvmfs_write(fuse_req_t req, fuse_ino_t ino, const char *buf,
                        size_t size, off_t off, struct fuse_file_info *fi)
{
  LogCvmfs(kLogFuse, kLogDebug, "cvmfs_write on inode: %d", ino);
  fuse_reply_err(req, DEFAULT_STUB_ERROR_CODE);
}

static void cvmfs_flush(fuse_req_t req, fuse_ino_t ino,
                        struct fuse_file_info *fi)
{
  LogCvmfs(kLogFuse, kLogDebug, "cvmfs_flush on inode: %d", ino);
  fuse_reply_err(req, 0);
}

static void cvmfs_fsync(fuse_req_t req, fuse_ino_t ino, int datasync,
                        struct fuse_file_info *fi)
{
  LogCvmfs(kLogFuse, kLogDebug, "cvmfs_fsync on inode: %d", ino);
  fuse_reply_err(req, DEFAULT_STUB_ERROR_CODE);
}

static void cvmfs_fsyncdir(fuse_req_t req, fuse_ino_t ino, int datasync,
                           struct fuse_file_info *fi)
{
  LogCvmfs(kLogFuse, kLogDebug, "cvmfs_fsyncdir on inode: %d", ino);
  fuse_reply_err(req, DEFAULT_STUB_ERROR_CODE);
}

#ifdef __APPLE__
static void cvmfs_setxattr(fuse_req_t req, fuse_ino_t ino, const char *name,
                           const char *value, size_t size, int flags,
                           uint32_t position)
#else
static void cvmfs_setxattr(fuse_req_t req, fuse_ino_t ino, const char *name,
                           const char *value, size_t size, int flags)
#endif
{
  LogCvmfs(kLogFuse, kLogDebug,
           "cvmfs_setxattr on inode: %d with xattrname: %s and value: %s",
           ino, name, value);
  fuse_reply_err(req, DEFAULT_STUB_ERROR_CODE);
}

static void cvmfs_removexattr(fuse_req_t req, fuse_ino_t ino, const char *name)
{
  LogCvmfs(kLogFuse, kLogDebug, "cvmfs_removexattr on inode: %d with name: %s",
           ino, name);
  fuse_reply_err(req, DEFAULT_STUB_ERROR_CODE);
}

static void cvmfs_create(fuse_req_t req, fuse_ino_t parent, const char *name,
                         mode_t mode, struct fuse_file_info *fi)
{
  LogCvmfs(kLogFuse, kLogDebug,
           "cvmfs_create on parent inode: %d with name: %s", parent, name);
  fuse_reply_err(req, DEFAULT_STUB_ERROR_CODE);
}

static void cvmfs_getlk(fuse_req_t req, fuse_ino_t ino,
                        struct fuse_file_info *fi, struct flock *lock)
{
  LogCvmfs(kLogFuse, kLogDebug, "cvmfs_getlk on inode: %d", ino);
  fuse_reply_err(req, DEFAULT_STUB_ERROR_CODE);
}

static void cvmfs_setlk(fuse_req_t req, fuse_ino_t ino,
                        struct fuse_file_info *fi, struct flock *lock,
                        int sleep)
{
  LogCvmfs(kLogFuse, kLogDebug, "cvmfs_setlk on inode: %d", ino);
  fuse_reply_err(req, DEFAULT_STUB_ERROR_CODE);
}

static void cvmfs_bmap(fuse_req_t req, fuse_ino_t ino, size_t blocksize,
                       uint64_t idx)
{
  LogCvmfs(kLogFuse, kLogDebug, "cvmfs_bmap on inode: %d", ino);
  fuse_reply_err(req, DEFAULT_STUB_ERROR_CODE);
}

#endif  // CVMFS_FUSE_OP_STUBS_H_