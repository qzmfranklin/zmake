TARGET_MODULE:=hello

ifneq ($(KERNELRELEASE),)
	obj-m:=$(TARGET_MODULE).o
else
	KERNEL_DIR:=/lib/modules/$(shell uname -r)/build
	PWD:=$(shell pwd)
all:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean
help:
	@echo "    all        build this module"
	@echo "    clean      clean"
	@echo "    help       print this message"
endif
