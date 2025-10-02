ifeq ($(KERNELRELEASE),)

KDIR ?= ../..

prefix := $(MAKE) -C $(KDIR) M=$$PWD

default:
	$(prefix)

clean:
	$(prefix) $@

else

CONFIG_TASK1 ?= m

ccflags-y += -DDEBUG
obj-$(CONFIG_TASK1) := task.o

endif
