#!/bin/sh

set -xeu

cp $HOST_DIR/opt/ext-toolchain/arm-buildroot-linux-gnueabihf/lib/libasan.so* $TARGET_DIR/usr/lib
cp $HOST_DIR/opt/ext-toolchain/arm-buildroot-linux-gnueabihf/lib/libubsan.so* $TARGET_DIR/usr/lib
