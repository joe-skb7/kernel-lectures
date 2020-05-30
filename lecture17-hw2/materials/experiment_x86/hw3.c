// SPDX-License-Identifier: GPL-2.0
/*
 * Driver for handling externally connected button and LED.
 */

#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/spinlock.h>
#include <linux/poll.h>
#include <linux/sched/signal.h>
#include <linux/slab.h>
#include <linux/delay.h>

#define DRIVER_NAME	"hw3"
#define WRITE_BUF_LEN	10
#define READ_BUF_LEN	2

static struct miscdevice mdev;
static bool first_read = true;

static ssize_t hw3_read(struct file *file, char __user *buf, size_t count,
			loff_t *ppos)
{
	unsigned long copy_len;
	ssize_t ret;
	const char *val;

	if (!first_read) {
		pr_err("not a first read\n");
		//return -EAGAIN;
		return 0; // no data available (EOF)
	}

	pr_err("first read\n");

	copy_len = min(count, (size_t)READ_BUF_LEN);
	pr_err("count requested: %zu\n", count);
	pr_err("copy_len = %lu\n", copy_len);

	val = "1";

	/* Do not advance ppos, do not use simple_read_from_buffer() */
	if (copy_to_user(buf, val, copy_len))
		ret = -EFAULT;
	else {
		// ret = copy_len;
		ret = count;
		*ppos += count;
	}

	first_read = false;

	return ret;
}

static __poll_t hw3_poll(struct file *file, poll_table *wait)
{

	__poll_t mask = 0;

	mdelay(1000);

	//	mask = EPOLLIN | EPOLLRDNORM;

	return mask;
}

static ssize_t hw3_write(struct file *file, const char __user *buf,
			 size_t count, loff_t *ppos)
{
	first_read = true;
	return count;
}

static const struct file_operations hw3_fops = {
	.owner		= THIS_MODULE,
	.read		= hw3_read,
	.write		= hw3_write,
	.poll		= hw3_poll,
	.llseek		= no_llseek,
};

static int hw3_init(void)
{
	int ret;

	memset(&mdev, 0, sizeof(mdev));
	mdev.minor		= MISC_DYNAMIC_MINOR;
	mdev.name		= DRIVER_NAME;
	mdev.fops		= &hw3_fops;
	ret = misc_register(&mdev);
	if (ret)
		return ret;

	pr_err("### probe finished OK\n");
	return 0;
}

static void hw3_exit(void)
{
	misc_deregister(&mdev);
}

module_init(hw3_init);
module_exit(hw3_exit);

MODULE_AUTHOR("Sam Protsenko <joe.skb7@gmail.com>");
MODULE_DESCRIPTION("Test module 3");
MODULE_LICENSE("GPL");
