KERNEL_SRC = /lib/modules/$(shell uname -r)/build
SOURCE := $(shell pwd)

INCLUDE_DIRS := -I./

obj-m := serial_bridge.o

all:
	$(MAKE) -C $(KERNEL_SRC) M=$(SOURCE)

#install: modules_install

modules_install:
	$(MAKE) -C $(KERNEL_SRC) M=$(SOURCE) modules_install

clean: 
	rm -f *.o *~ core .depend .*.cmd *.ko *.mod.c *.a
	rm -f Module.markers Module.symvers modules.order
	rm -rf .tmp_versions Modules.symvers
