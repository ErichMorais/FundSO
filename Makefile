obj-m	:=	driver.o

KDIR	:= /lib/modules/$(shell uname -r)/build
PWD	:= $(shell pwd)
EXTRAVERSION = .7-I900XXJVP-CL264642

default:
	make -C $(KDIR) SUBDIRS=$(PWD) modules
