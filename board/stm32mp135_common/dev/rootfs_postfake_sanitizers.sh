#!/bin/sh

set -xeu

cp $HOST_DIR/arm-buildroot-linux-gnueabihf/lib/libasan* $TARGET_DIR/usr/lib
cp $HOST_DIR/arm-buildroot-linux-gnueabihf/lib/liblsan* $TARGET_DIR/usr/lib
cp $HOST_DIR/arm-buildroot-linux-gnueabihf/lib/libubsan* $TARGET_DIR/usr/lib
