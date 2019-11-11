#!/bin/sh

export ARCH=arm
export PATH=/opt/gcc-linaro-7.3.1-2018.05-x86_64_arm-linux-gnueabihf/bin:$PATH
export CROSS_COMPILE=arm-linux-gnueabihf-
export KDIR=~/repos/linux-stable

make $*
