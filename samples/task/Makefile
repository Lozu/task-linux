ifeq ($(KERNELRELEASE),)

KDIR ?= ../..

prefix := $(MAKE) -C $(KDIR) M=$$PWD

default:
	$(prefix)

clean:
	$(prefix) $@

else

CONFIG_TASK3 ?= m

ccflags-y += -DDEBUG
obj-$(CONFIG_TASK3) := task.o

endif
