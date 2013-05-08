#!/bin/sh

sh configure LDFLAGS="$LDFLAGS" CFLAGS="$CFLAGS -fPIC -fno-strict-aliasing -fasynchronous-unwind-tables -fno-omit-frame-pointer -fno-optimize-sibling-calls -fvisibility=hidden" \
  --enable-lib \
  --enable-static \
  --disable-shared \
  --disable-util \
  --disable-example
