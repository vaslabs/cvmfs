#!/bin/sh

sh configure LDFLAGS="$LDFLAGS" CFLAGS="$CFLAGS -fno-strict-aliasing -fasynchronous-unwind-tables -fno-omit-frame-pointer -fno-optimize-sibling-calls -fvisibility=hidden" \
  --enable-lib \
  --enable-static \
  --with-pic \
  --disable-shared \
  --disable-util \
  --disable-example
