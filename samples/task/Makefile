ifeq ($(KERNELRELEASE),)

KDIR ?= ../..

prefix := $(MAKE) -C $(KDIR) M=$$PWD

default:
	$(prefix)

clean:
	$(prefix) $@

else

CONFIG_TASK2 ?= m

ccflags-y += -DDEBUG
obj-$(CONFIG_TASK2) := task.o

endif
