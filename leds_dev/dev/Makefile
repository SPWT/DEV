prom = led_dev
#cc = arm-none-linux-gnueabi-gcc
#$(prom):main.c
#	$(cc) $^ -o $@ #-static
#
#cp:
#	cp $(prom) /home/lsq/tftpboot
#clean:
#	rm -rf $(prom)

obj-m += $(prom).o

KDIR := /home/lsq/kernel/iTop4412_Kernel_3.0
#KDIR := /home/lsq/kernel/linux-3.0.10
#/lib/modules/$(shell uname -r)/build

PWD ?= $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

cp:
	cp -i *.ko /home/lsq/tftpboot

clean_o:
	rm -rf *.o

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean 



#PWD := $(shell pwd)
#DOC := /usr/src/linux-headers-4.4.0-31/include
#
#obj-m := hello.o
# 
#all :
#	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
#
#clean:
#	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
