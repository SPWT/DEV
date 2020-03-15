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

#include <asm/io.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("LSQ");
MODULE_DESCRIPTION("A simple LED Module");
MODULE_ALIAS("a simple led module");

static int major;
static struct class *cls;

static volatile unsigned long *gpio_con;
static volatile unsigned long *gpio_dat;
static unsigned int pin;

static int ledOpen(struct inode *p, struct file *f)
{
	printk(KERN_EMERG"ledOpen\r\n");

	*gpio_con &= ~(0xf << (pin * 4));
	*gpio_con |= 0x1 << (pin * 4);
	*gpio_dat &= ~(1 << (pin * 1));
//	*gpio_dat |= 1 << (pin * 1);

	return 0;
}

static long ledIoctl(struct file *f, unsigned int cmd, unsigned long ul)
{
	printk(KERN_EMERG"ledIoctl, cmd: %d\r\n", cmd);

	if (cmd > 0)
	{
		cmd = 1;
	}
	else
	{
		cmd = 0;
	}

	*gpio_dat &= ~(1 << (pin * 1));
	*gpio_dat |= cmd << (pin * 1);

	return 0;
}

static struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.open = ledOpen,
	.unlocked_ioctl = ledIoctl,
};

static int led_probe(struct platform_device *pdev)
{
	struct resource *res;
	printk(KERN_EMERG"led_probe##############\r\n");

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	gpio_con = ioremap(res->start, res->end - res->start + 1);
	gpio_dat = gpio_con + 1;

	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	pin = res->start;

	major = register_chrdev(0, "myled", &led_fops);

	cls = class_create(THIS_MODULE, "myled");

	device_create(cls, NULL, MKDEV(major, 0), NULL, "led"); /* /dev/led */

	return 0;
}

static int led_remove (struct platform_device *pdev)
{
	printk("led_remove, remove led\n");

	device_destroy(cls, MKDEV(major, 0));
	class_destroy(cls);
	unregister_chrdev(major, "myled");
	iounmap(gpio_con);

	return 0;
}

static struct platform_driver led_drv = {
	.probe = led_probe,
	.remove = led_remove,
	.driver = {
		.name = "myled",
	}
};

static int led_init()
{
	platform_driver_register(&led_drv);

	return 0;
}

static int led_exit()
{
	platform_driver_unregister(&led_drv);
	return 0;
}

module_init(led_init);
module_exit(led_exit);

#if 0
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

#include <asm/io.h>

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

int ledOpen(struct inode *p, struct file *f)
{
	printk(KERN_EMERG"ledOpen\r\n");

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

static int led_init()
{
	devNum = MKDEV(reg_major, reg_minor);
	printk(KERN_EMERG"devNum is %d\r\n", devNum);
	if (OK == register_chrdev_region(devNum, subDevNum, "ledDev"))
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

	gFile->open = ledOpen;
	gFile->read = ledRead;
	gFile->write = ledWrite;
	gFile->unlocked_ioctl = ledIoctl;
	gFile->owner = THIS_MODULE;

	cdev_init(gDev, gFile);
	int ret = cdev_add(gDev, devNum, 3);
	printk(KERN_EMERG"ret: %d\n", ret);
//	platform_driver_register();

	unsigned int gpiocon = 0x11000000 + 0x60;
	unsigned int gpiodat = gpiocon + 4;
	volatile unsigned long *gpkcon;
	volatile unsigned long *gpkdat;
	gpkcon = (volatile unsigned long *)ioremap(gpiocon, 1);
	gpkdat = gpkcon + 1;
//	gpkdat = (volatile unsigned long *)ioremap(gpiodat, 8);

	*gpkcon &= ~(0xf << 4);
	*gpkcon |= 0x1 << 4;

	*gpkdat |= 1 << 1;

	return 0;
}

static int led_exit()
{
	gpio_free(EXYNOS4_GPL2(0));
	cdev_del(gDev);
	unregister_chrdev_region(devNum, subDevNum);
	return 0;
}

module_init(led_init);
module_exit(led_exit);

#endif

