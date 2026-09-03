#!/bin/bash

set -xeu

scp build/buildroot/images/boot.ext2 \
    build/buildroot/images/fip.bin \
    build/buildroot/images/tf-a-ebook-reader-debug.stm32 \
    root@192.168.7.2:/tmp/

ssh root@192.168.7.2 'dd if=/tmp/boot.ext2 of=/dev/mmcblk0p4 bs=4M'
ssh root@192.168.7.2 'dd if=/tmp/fip.bin of=/dev/mmcblk0p3 bs=4M'
ssh root@192.168.7.2 'dd if=/tmp/tf-a-ebook-reader-debug.stm32 of=/dev/mmcblk0p2 bs=4M'
ssh root@192.168.7.2 'dd if=/tmp/tf-a-ebook-reader-debug.stm32 of=/dev/mmcblk0p1 bs=4M'
ssh root@192.168.7.2 'reboot'
