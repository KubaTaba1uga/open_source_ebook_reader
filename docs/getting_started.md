Getting started
=====================

```{note}
All commands mentioned in this docs were tested on Debian Trixie.
```

We use invoke to simplify usage of the repo so pls install it in your system if it's not there yet:
```bash
sudo apt install python3-invoke
```

To start development we need source code of the ebook, so download it with git:
```bash
git clone https://github.com/KubaTaba1uga/open_source_ebook_reader.git && \
cd open_source_ebook_reader
```

We also need to install all repo's dependencies:
```bash
inv install-deps
```

The ebook reader is booted from SD card, so first thing to learn is to how generate 
SD card image, which will include linux, rootfs and ebook reader app itself.
Building the image is done with `inv build` command, but to generate proper image you need to specify `-c` argument with one of the following boards we support:

% TO-DO: insert normal board here

| Board                 | Config                              | Link                                                        |
|-----------------------|-------------------------------------|-------------------------------------------------------------|
| STM32MP135D Odyssey   | stm32mp135d_odyssey_defconfig       | https://www.seeedstudio.com/Odyssey-MP135D-eMMC-p-5728.html |
| STM32MP135D Odyssey   | stm32mp135d_odyssey_debug_defconfig | https://www.seeedstudio.com/Odyssey-MP135D-eMMC-p-5728.html |

```bash
inv build -c stm32mp135d_odyssey_defconfig
```

Build will take some time so feel free to take a brake, make a coffe etc.

Once build is done, you should have sdcard image in `build/buildroot/images/sdcard.img`.
Flash it onto the SD card with following command:
```bash
EBK_SD_CARD=/dev/sda
sudo dd if=./build/buildroot/images/sdcard.img of=$EBK_SD_CARD bs=1M status=progress
```

```{warning}
Before executing `dd` command adjust EBK_SD_CARD variable accordingly to your SD card path.
```

Once flashing is done insert sd card into the device and press power ON button, the device should turn on with newly build image.


## Debug build

We usually have two builds per board, debug build and prod build.

Debug build is meant for fast development cycle, it have enabled DFU, NFS root in linux etc.

To use debug build you need to configure your PC first. To configure your developemnt machine perform following tutorials:
- [Set up USB NIC](debug/usb_net.md)
- [Set up NFS server](debug/nfs.md)

### DFU

Enabling DFU varies between boards. For stm32mp135d odyssey you need to short boot pins in row 0 and 2.

Once you build the sdcard you can flash it with DFU using following command:
```bash
python tools/dfu/flash.py
```
