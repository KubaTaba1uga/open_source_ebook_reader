#!/bin/bash

set -xeu

rm -rf build/buildroot/build/ebook_reader-v0.0.1/src/
cp -r package/ebook_reader/src build/buildroot/build/ebook_reader-v0.0.1/src
meson compile -C build/buildroot/build/ebook_reader-v0.0.1/build/
scp build/buildroot/build/ebook_reader-v0.0.1/build/ebook_reader root@192.168.7.2:/root/
