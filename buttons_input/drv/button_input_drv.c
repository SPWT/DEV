#include <linux/module.h>

#include <linux/init.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/sched.h>
#include <linux/pm.h>
#include <linux/slab.h>
#include <linux/sysctl.h>
#include <linux/proc_fs.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/workqueue.h>
#include <linux/gpio.h>

struct pin_desc{
	int irq;
	char *name;
	unsigned int pin;
	unsigned int key_val;
};

struct pin_desc pins_desc[4] = {
		{IRQ_EINT(9), "Home", EXYNOS4_GPX1(1), KEY_L},
		{IRQ_EINT(10), "Back", EXYNOS4_GPX1(2), KEY_S},
		{IRQ_EINT(27), "Sleep", EXYNOS4_GPX3(3), KEY_ENTER},
		{IRQ_EINT(17), "Vol+", EXYNOS4_GPX2(1), KEY_LEFTSHIFT},
};

static struct input_dev *buttons_dev;
static struct pin_desc *irq_pd;
static struct timer_list buttons_timer;

static irqreturn_t buttons_irq(int irq, void *dev_id)
{
	irq_pd = (struct pin_desc *)dev_id;
	mod_timer(&buttons_timer, jiffies+HZ/100);

	return IRQ_RETVAL(IRQ_HANDLED);
}

static void buttons_timer_function(unsigned long data)
{
	struct pin_desc *pindesc = irq_pd;
	unsigned int pinval;

	if (!pindesc)
		return;

	pinval = gpio_get_value(pindesc->pin);

	if (pinval)
	{
		input_event(buttons_dev, EV_KEY, pindesc->key_val, 0);
		input_sync(buttons_dev);
	}
	else
	{
		input_event(buttons_dev, EV_KEY, pindesc->key_val, 1);
		input_sync(buttons_dev);
	}
}

static int buttons_init(void)
{
	int i, err;

	buttons_dev = input_allocate_device();

	set_bit(EV_KEY, buttons_dev->evbit);
	set_bit(EV_REP, buttons_dev->evbit);

	set_bit(KEY_L, buttons_dev->keybit);
	set_bit(KEY_S, buttons_dev->keybit);
	set_bit(KEY_ENTER, buttons_dev->keybit);
	set_bit(KEY_LEFTSHIFT, buttons_dev->keybit);

	input_register_device(buttons_dev);

	init_timer(&buttons_timer);
	buttons_timer.function = buttons_timer_function;
	add_timer(&buttons_timer);

	for (i = 0; i < 4; i++)
	{
		err = request_irq(pins_desc[i].irq, buttons_irq, IRQ_TYPE_EDGE_BOTH, pins_desc[i].name, &pins_desc[i]);
		if (err)
		{
			printk(KERN_EMERG"request_irq failed\r\n");
		}
	}

	return 0;
}

static void buttons_exit(void)
{
	int i;

	del_timer(&buttons_timer);
	for (i = 0; i < 0; i++)
	{
		free_irq(pins_desc[i].irq, &pins_desc[i]);
	}

	input_unregister_device(buttons_dev);
	input_free_device(buttons_dev);
}

module_init(buttons_init);
module_exit(buttons_exit);

MODULE_LICENSE("GPL");
