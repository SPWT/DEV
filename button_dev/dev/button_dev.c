#include <linux/init.h>
#include <linux/module.h>

#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>

#include <linux/moduleparam.h>
#include <linux/cdev.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/sched.h>
#include <linux/slab.h>

#include <linux/gpio.h>
#include <plat/gpio-cfg.h>
#include <mach/gpio.h>
#include <mach/gpio-exynos4.h>

#include <linux/interrupt.h>
#include <linux/irq.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("LSQ");
MODULE_DESCRIPTION("A simple LED Module");
MODULE_ALIAS("a simple led module");

#define BUFFER_MAX	(10)
#define OK			(0)
#define ERROR		(-1)

struct cdev *gDev;
struct file_operations *gFile;
dev_t devNum;
unsigned int subDevNum = 1;
int reg_major = 232;
int reg_minor = 0;
char *buffer;
int flag = 0;

static irq_handler_t eint9_interrupt(int irq, void *dev_id)
{
	int iValue;
//	printk(KERN_EMERG"interrupt9 \r\n");

	iValue = gpio_get_value(EXYNOS4_GPX1(1));

	printk(KERN_EMERG"iValue: %d", iValue);

	return IRQ_HANDLED;
}

int butOpen(struct inode *p, struct file *f)
{
	int irq;
	int err = 0;

	printk(KERN_EMERG"butOpen\r\n");

	irq = gpio_to_irq(EXYNOS4_GPX1(1));
	printk(KERN_EMERG"irq: %d EINT(9): %d\r\n", irq, IRQ_EINT(9));
	err = request_irq(IRQ_EINT(9), eint9_interrupt, IRQ_TYPE_EDGE_BOTH, "eint9", NULL);
	if(err)
	{
		printk(KERN_EMERG"request_irq failed\r\n");
	}

	return 0;
}

ssize_t ledWrite(struct file *f, const char __user *u, size_t s, loff_t *l)
{
	printk(KERN_EMERG"ledWrite\r\n");

	gpio_set_value(EXYNOS4_GPL2(0), 1);

	return 0;
}

ssize_t ledRead (struct file *f, char __user *u, size_t s, loff_t *l)
{
	printk(KERN_EMERG"ledRead\r\n");

	return 0;
}

long ledIoctl(struct file *f, unsigned int cmd, unsigned long ul)
{
	printk(KERN_EMERG"ledIoctl, cmd: %d\r\n", cmd);

	gpio_set_value(EXYNOS4_GPL2(0), cmd);

	return 0;
}

static int but_init()
{
	devNum = MKDEV(reg_major, reg_minor);
	printk(KERN_EMERG"devNum is %d\r\n", devNum);
	if (OK == register_chrdev_region(devNum, subDevNum, "butDev"))
	{
		printk(KERN_EMERG"register_chrdev_region ok\r\n");
	}
	else
	{
		printk(KERN_EMERG"register_chrdev_region error\r\n");
		return ERROR;
	}
	printk(KERN_EMERG"devNum is %d\r\n", devNum);
	gDev = kzalloc(sizeof(struct cdev), GFP_KERNEL);
	gFile = kzalloc(sizeof(struct file_operations), GFP_KERNEL);

	gFile->open = butOpen;
//	gFile->read = butRead;
//	gFile->write = butWrite;
//	gFile->unlocked_ioctl = butIoctl;
	gFile->owner = THIS_MODULE;

	cdev_init(gDev, gFile);
	int ret = cdev_add(gDev, devNum, 3);
	printk(KERN_EMERG"ret: %d\n", ret);
//	platform_driver_register();

	ret = gpio_request(EXYNOS4_GPX1(1), "HOME");
	if (ret < 0)
	{
		printk(KERN_EMERG "butOpen error: %d\n", ret);
		return ret;
	}

//	s3c_gpio_cfgpin(EXYNOS4_GPL2(0), S3C_GPIO_OUTPUT);
	s3c_gpio_cfgpin(EXYNOS4_GPX1(1), S3C_GPIO_SFN(0xF));

//	gpio_set_value(EXYNOS4_GPL2(0), 0);

	return 0;
}

static int but_exit()
{
	free_irq(IRQ_EINT(9), NULL);
	gpio_free(EXYNOS4_GPX1(1));
	cdev_del(gDev);
	unregister_chrdev_region(devNum, subDevNum);
	return 0;
}

module_init(but_init);
module_exit(but_exit);
