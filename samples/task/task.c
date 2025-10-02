// SPDX-License-Identifier: GPL-2.0

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/string.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/module.h>

struct identity {
	char name[20];
	int id;
	bool hired;
	struct list_head node;
};

static LIST_HEAD(identities);

static int identity_create(char *name, int id)
{
	struct identity *tmp;
	int name_len = strnlen(name, 20);

	if (name_len >= 20)
		return -ENAMETOOLONG;
	tmp = kzalloc(sizeof(*tmp), GFP_KERNEL);
	if (!tmp)
		return -ENOMEM;
	strscpy(tmp->name, name);
	tmp->id = id;
	tmp->hired = false;
	list_add_tail(&tmp->node, &identities);
	return 0;
}

static struct identity *identity_find(int id)
{
	struct identity *cur;

	list_for_each_entry(cur, &identities, node) {
		if (cur->id == id)
			return cur;
	}
	return NULL;
}

static void identity_destroy(int id)
{
	struct identity *tmp = identity_find(id);

	if (!tmp)
		return;
	list_del(&tmp->node);
}

static int identity_hire(int id)
{
	struct identity *tmp = identity_find(id);

	if (!tmp)
		return -ENOENT;
	tmp->hired = true;
	return 0;
}

static int print_create_error_and_error(int ret)
{
	WARN_ON(ret != -ENOMEM || ret != -ENAMETOOLONG);

	pr_err("unable to create identity: ");
	if (ret == -ENOMEM)
		pr_err("out of memory\n");
	else
		pr_err("identity name is too long\n");
	return ret;
}

static int __init task3_init(void)
{
	struct identity *temp;
	int ret;

	pr_debug("Loading\n");

	ret = identity_create("Volodymyr Azarov", 1);
	if (ret != 0)
		return print_create_error_and_error(ret);

	ret = identity_create("Tomasz Bukowski", 2);
	if (ret != 0)
		return print_create_error_and_error(ret);

	temp = identity_find(1);
	if (!temp) {
		pr_err("no identity with id = 1\n");
		return -ENOENT;
	}
	pr_debug("id 1 = %s\n", temp->name);

	if (identity_hire(1) != 0) {
		pr_err("unable to hire candidate with id = 1\n");
		return -EINVAL;
	}

	temp = identity_find(10);
	if (temp) {
		pr_err("id 10 found, even though if was not added\n");
		return -EINVAL;
	}

	identity_destroy(2);
	identity_destroy(1);

	if (!list_empty(&identities)) {
		pr_err("identity list is not empty, even though it should be\n");
		return -EINVAL;
	}
	return 0;
}

static void __exit task3_exit(void)
{
	pr_debug("Unloading\n");
}

module_init(task3_init);
module_exit(task3_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Task 3");
