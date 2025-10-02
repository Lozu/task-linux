// SPDX-License-Identifier: GPL-2.0

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/printk.h>
#include <linux/module.h>

static int __init task1_init(void)
{
	pr_debug("Hello, KernelCare!\n");
	return 0;
}

static void __exit task1_exit(void)
{
	pr_debug("Unloading\n");
}

module_init(task1_init);
module_exit(task1_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Task 1");
