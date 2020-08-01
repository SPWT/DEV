#!/bin/bash
export ARCH=arm
obj-m += lcd_dev.o 

KDIR := /home/lsq/kernel/linux-5.4.49

PWD ?= $(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
	
cp:
	#cp -i *.ko /home/lsq/tftpboot
	cp -i *.ko /home/minilinux/model
		
clean:
	rm *.ko *.o  *.mod.c *.order *.symvers .*.cmd
