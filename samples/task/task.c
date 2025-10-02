// SPDX-License-Identifier: GPL-2.0

#include <linux/init.h>
#include <linux/printk.h>
#include <linux/debugfs.h>
#include <linux/fs.h>
#include <linux/jiffies.h>
#include <linux/delay.h>
#include <linux/random.h>
#include <linux/rwsem.h>
#include <linux/module.h>

#undef pr_fmt
#define pr_fmt(fmt) KBUILD_MODNAME " [%08x]: " fmt, hash

#define DATA_FILE_LIMIT BIT(CONFIG_PAGE_SHIFT)

static struct dentry *debugfs_root;
static struct dentry *jiffies_file;
static struct dentry *data_file;

static char data_buf[DATA_FILE_LIMIT];

static int show_jiffies(struct seq_file *s, void *)
{
	seq_printf(s, "%llu\n", get_jiffies_64());
	return 0;
}

static int jiffies_open(struct inode *inode, struct file *file)
{
	return single_open(file, show_jiffies, NULL);
}

static int data_open(struct inode *inode, struct file *file)
{
	if (file->f_flags & O_APPEND)
		generic_file_llseek(file, 0, SEEK_END);

	return generic_file_open(inode, file);
}

static ssize_t data_read(struct file *file, char __user *user_buf,
			 size_t size, loff_t *ppos)
{
	ssize_t ret;
	u32 hash = get_random_u16();
	loff_t pos = *ppos;

	down_read(&file->f_inode->i_rwsem);
	pr_debug("data read: locked\n");

	loff_t file_size = file->f_inode->i_size;

	ret = simple_read_from_buffer(user_buf, size, ppos, data_buf,
				      file_size);

#ifdef SLOW_OPS
	fsleep(2 * 1000000);
#endif

	pr_debug("read (%lu, %llu): %ld\n", size, pos, ret);
	up_read(&file->f_inode->i_rwsem);
	pr_debug("data read: unlock");

	return ret;
}

static ssize_t data_write(struct file *file, const char __user *user_buf,
			  size_t size, loff_t *ppos)
{
	ssize_t ret;
	u32 hash = get_random_u16() << 16;
	loff_t file_size;
	loff_t pos = *ppos;

	down_write(&file->f_inode->i_rwsem);
	pr_debug("data write: locked\n");

	file_size = file->f_inode->i_size;

	ret = simple_write_to_buffer(data_buf, DATA_FILE_LIMIT, ppos,
				     user_buf, size);
	pr_debug("write (%lu, %llu): %ld\n", size, pos, ret);

	if (ret > 0 && *ppos > file_size)
		file->f_inode->i_size = *ppos;

#ifdef SLOW_OPS
	fsleep(2 * 1000000);
#endif

	pr_debug("size = %llu\n", file->f_inode->i_size);
	up_write(&file->f_inode->i_rwsem);
	pr_debug("data write: unlocked\n");

	return ret;
}

static int data_release(struct inode *inode, struct file *file)
{
	return 0;
}

static loff_t data_llseek(struct file *file, loff_t offset, int whence)
{
	return generic_file_llseek_size(file, offset, whence,
			DATA_FILE_LIMIT, i_size_read(file->f_inode));
}

static const struct file_operations jiffies_ops = {
	.owner = THIS_MODULE,
	.open = jiffies_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release
};

static const struct file_operations data_ops = {
	.owner = THIS_MODULE,
	.open = data_open,
	.read = data_read,
	.write = data_write,
	.llseek = data_llseek,
	.release = data_release
};

static void make_fs(void)
{
	debugfs_root = debugfs_create_dir("kernelcare", NULL);
	jiffies_file = debugfs_create_file("jiffies", 0444, debugfs_root,
					   NULL, &jiffies_ops);
	data_file = debugfs_create_file("data", 0644, debugfs_root,
					NULL, &data_ops);
}

#undef pr_fmt
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

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
