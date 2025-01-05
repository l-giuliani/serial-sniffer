KERNEL_SRC = /lib/modules/$(shell uname -r)/build
SOURCE := $(shell pwd)

obj-m := serial_bridge.o
ccflags-y += -I./services -I./include
serial_bridge-objs := serial_bridge_main.o services/user_comm_services.o

all:
	$(MAKE) -C $(KERNEL_SRC)  M=$(SOURCE)

#install: modules_install

modules_install:
	$(MAKE) -C $(KERNEL_SRC) M=$(SOURCE) modules_install

clean: 
	rm -f *.o *~ core .depend .*.cmd *.ko *.mod.c *.a
	rm -f Module.markers Module.symvers modules.order
	rm -rf .tmp_versions Modules.symvers
