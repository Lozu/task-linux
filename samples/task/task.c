// SPDX-License-Identifier: GPL-2.0

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/printk.h>
#include <linux/debugfs.h>
#include <linux/jiffies.h>
#include <linux/module.h>

static struct dentry *debugfs_root;
static struct dentry *jiffies_file;

static int show_jiffies(struct seq_file *s, void *)
{
	seq_printf(s, "%llu\n", get_jiffies_64());
	return 0;
}

static int jiffies_open(struct inode *inode, struct file *file)
{
	return single_open(file, show_jiffies, NULL);
}

static const struct file_operations jiffies_ops = {
	.owner = THIS_MODULE,
	.open = jiffies_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release
};

static void make_fs(void)
{
	debugfs_root = debugfs_create_dir("kernelcare", NULL);
	jiffies_file = debugfs_create_file("jiffies", 0444, debugfs_root,
					   NULL, &jiffies_ops);
}

static int __init task2_init(void)
{
	pr_debug("Loading\n");
	make_fs();
	return 0;
}

static void __exit task2_exit(void)
{
	pr_debug("Unloading\n");
	debugfs_remove(debugfs_root);
}

module_init(task2_init);
module_exit(task2_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Task 2");
