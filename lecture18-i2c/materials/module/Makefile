ifneq ($(KERNELRELEASE),) # kbuild part of makefile

CFLAGS_ds1307x.o := -DDEBUG
obj-m := ds1307x.o

else # normal makefile

KDIR ?= /lib/modules/$(shell uname -r)/build

default: module
module:
	$(MAKE) -C $(KDIR) M=$(PWD) C=1 modules
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) C=1 clean

CC = $(CROSS_COMPILE)gcc
CFLAGS = -O2 -Wall

.PHONY: module clean

endif
