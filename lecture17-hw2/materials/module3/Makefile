ifneq ($(KERNELRELEASE),) # kbuild part of makefile
CFLAGS_hw3.o := -DDEBUG
obj-m := hw3.o
else # normal makefile
KDIR ?= /lib/modules/$(shell uname -r)/build

default: module app
module:
	$(MAKE) -C $(KDIR) M=$(PWD) C=1 modules
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) C=1 clean
	rm -f hw3-app

CC = $(CROSS_COMPILE)gcc
CFLAGS = -O2 -Wall

app:
	$(CC) $(CFLAGS) hw3-app.c -o hw3-app

.PHONY: module clean app
endif
