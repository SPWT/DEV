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

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("LSQ");
MODULE_DESCRIPTION("A simple LED Module");
MODULE_ALIAS("a simple timer module");

#define BUFFER_MAX	(10)
#define OK			(0)
#define ERROR		(-1)

struct timer_list mytimer;

static void timerFunc(unsigned long data)
{
	printk("%s, time(secs) = %ld\n", (char *)data, jiffies/HZ);
	mod_timer(&mytimer, jiffies + 3 * HZ);
}

static int __init timer_init()
{
	printk(KERN_INFO "init timer...\n");
	setup_timer(&mytimer, timerFunc, (unsigned long)"my timer");
	mytimer.expires = jiffies + 1*HZ;
	add_timer(&mytimer);

	return 0;
}

static void __exit timer_exit()
{
	printk(KERN_INFO "del timer...\n");
	del_timer(&mytimer);
	return ;
}

module_init(timer_init);
module_exit(timer_exit);
