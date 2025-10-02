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
#ccflags-y += -DSLOW_OPS

obj-$(CONFIG_TASK2) := task.o

endif
