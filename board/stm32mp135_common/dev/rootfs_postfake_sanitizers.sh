#!/bin/sh

set -xeu

env

cp $HOST_DIR/arm-buildroot-linux-gnueabihf/lib/libasan* $TARGET_DIR/usr/lib
cp $HOST_DIR/arm-buildroot-linux-gnueabihf/lib/liblsan* $TARGET_DIR/usr/lib
cp $HOST_DIR/arm-buildroot-linux-gnueabihf/lib/libubsan* $TARGET_DIR/usr/lib

# cp $HOST_DIR/opt/ext-toolchain/arm-buildroot-linux-gnueabihf/lib/libasan.so* $TARGET_DIR/usr/lib
# cp $HOST_DIR/opt/ext-toolchain/arm-buildroot-linux-gnueabihf/lib/libubsan.so* $TARGET_DIR/usr/lib

