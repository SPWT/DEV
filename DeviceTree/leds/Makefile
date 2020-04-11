#!/bin/bash
export ARCH=arm
obj-m += leds_drv.o 

KDIR := /home/lsq/kernel/itop4412_kernel_4_14_2_bsp/linux-4.14.2_iTop-4412_scp

PWD ?= $(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
	
cp:
	#cp -i *.ko /home/lsq/tftpboot
	cp -i *.ko /home/minilinux
		
clean:
	rm *.ko *.o  *.mod.c *.order *.symvers
