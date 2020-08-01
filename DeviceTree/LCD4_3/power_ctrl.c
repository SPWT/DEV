/*
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>

#include <linux/of_gpio.h>

#define DEVICE_NAME	"power_ctl"
#define DRIVER_NAME "power_ctl"


//#define POWER_GPIO			100//IMX_GPIO_NR(1, 19)

uint32_t POWER_GPIO = 0;
uint32_t LVDS_GPIO = 0;
uint32_t PWM_GPIO = 0;
uint32_t TP_GPIO = 0;
uint32_t USB_RST = 0;
uint32_t USB_CON = 0;
uint32_t ME3630_POW = 0;
uint32_t ME3630_RST = 0;


static int power_open(struct inode *inode, struct file *file) {
		return 0;
}

static int power_close(struct inode *inode, struct file *file) {
	return 0;
}

static long power_ioctl(struct file *filep, unsigned int cmd,
		unsigned long arg)
{
	printk("%s: cmd = %d\n", __FUNCTION__, cmd);
	switch(cmd) {
		case 0:
			gpio_set_value(POWER_GPIO, 0);
			break;
		case 1:
			gpio_set_value(POWER_GPIO, 1);
			break;
		default:
			return -EINVAL;
	}

	return 0;
}

static ssize_t power_write(struct file *filp, const char __user *buffer, size_t count, loff_t *ppos)
{
	char str[20];

	memset(str, 0, 20);

	if(copy_from_user(str, buffer, count))
	{
		printk("Error\n");

		return -EINVAL;
	}

	printk("%s", str);
#if 1
	if(!strncmp(str, "1", 1))
		gpio_set_value(POWER_GPIO, 1);
	else
		gpio_set_value(POWER_GPIO, 0);
#endif
	return count;
}

static struct file_operations power_ops = {
	.owner			= THIS_MODULE,
	.open			= power_open,
	.release		= power_close, 
	.unlocked_ioctl	= power_ioctl,
	.write			= power_write,
};

static struct miscdevice power_misc_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &power_ops,
};

static int power_probe(struct platform_device *pdev)
{
	int ret;

	struct device_node *np = pdev->dev.of_node;

	POWER_GPIO = of_get_named_gpio(np, "gpios", 0);
	if (POWER_GPIO == -EPROBE_DEFER)
                return POWER_GPIO;
        if (POWER_GPIO < 0) {
                dev_err(&pdev->dev, "error acquiring power gpio: %d\n", POWER_GPIO);
                return POWER_GPIO;
        }

        ret = devm_gpio_request_one(&pdev->dev, POWER_GPIO, 0, "power-gpio");
        if(ret) {
                dev_err(&pdev->dev, "error requesting power gpio: %d\n", ret);
                return ret;
        }

	gpio_direction_output(POWER_GPIO, 1);
	
	LVDS_GPIO = of_get_named_gpio(np, "gpios", 1);
	if (LVDS_GPIO == -EPROBE_DEFER)
                return LVDS_GPIO;
        if (LVDS_GPIO < 0) {
                dev_err(&pdev->dev, "error acquiring power gpio: %d\n", LVDS_GPIO);
                return LVDS_GPIO;
        }

        ret = devm_gpio_request_one(&pdev->dev, LVDS_GPIO, 0, "power-gpio");
        if(ret) {
                dev_err(&pdev->dev, "error requesting power gpio: %d\n", ret);
                return ret;
        }

	gpio_direction_output(LVDS_GPIO, 1);

	
	PWM_GPIO = of_get_named_gpio(np, "gpios", 2);
	if (PWM_GPIO == -EPROBE_DEFER)
                return PWM_GPIO;
        if (PWM_GPIO < 0) {
                dev_err(&pdev->dev, "error acquiring power gpio: %d\n", PWM_GPIO);
                return PWM_GPIO;
        }

        ret = devm_gpio_request_one(&pdev->dev, PWM_GPIO, 0, "power-gpio");
        if(ret) {
                dev_err(&pdev->dev, "error requesting power gpio: %d\n", ret);
                return ret;
        }

	gpio_direction_output(PWM_GPIO, 1);

	TP_GPIO = of_get_named_gpio(np, "gpios", 3);
	if (TP_GPIO == -EPROBE_DEFER)
                return TP_GPIO;
        if (TP_GPIO < 0) {
                dev_err(&pdev->dev, "error acquiring power gpio: %d\n", TP_GPIO);
                return TP_GPIO;
        }

        ret = devm_gpio_request_one(&pdev->dev, TP_GPIO, 0, "power-gpio");
        if(ret) {
                dev_err(&pdev->dev, "error requesting power gpio: %d\n", ret);
                return ret;
        }

	gpio_direction_output(TP_GPIO, 1);


	USB_RST = of_get_named_gpio(np, "usb-ctl", 0);
	if (USB_RST == -EPROBE_DEFER)
                return USB_RST;
        if (USB_RST < 0) {
                dev_err(&pdev->dev, "error acquiring power gpio: %d\n", USB_RST);
                return USB_RST;
        }

        ret = devm_gpio_request_one(&pdev->dev, USB_RST, 0, "usb-gpio");
        if(ret) {
                dev_err(&pdev->dev, "error requesting power gpio: %d\n", ret);
                return ret;
        }

	gpio_direction_output(USB_RST, 1);


	USB_CON = of_get_named_gpio(np, "usb-ctl", 1);
	if (USB_CON == -EPROBE_DEFER)
                return USB_CON;
        if (USB_CON < 0) {
                dev_err(&pdev->dev, "error acquiring power gpio: %d\n", USB_CON);
                return USB_CON;
        }

        ret = devm_gpio_request_one(&pdev->dev, USB_CON, 0, "usb-gpio");
        if(ret) {
                dev_err(&pdev->dev, "error requesting power gpio: %d\n", ret);
                return ret;
        }

	gpio_direction_output(USB_CON, 1);

	ME3630_POW = of_get_named_gpio(np, "me3630-ctl", 1);
	if (ME3630_POW == -EPROBE_DEFER)
                return ME3630_POW;
        if (ME3630_POW < 0) {
                dev_err(&pdev->dev, "error acquiring power gpio: %d\n", ME3630_POW);
                return ME3630_POW;
        }

        ret = devm_gpio_request_one(&pdev->dev, ME3630_POW, 0, "usb-gpio");
        if(ret) {
                dev_err(&pdev->dev, "error requesting power gpio: %d\n", ret);
                return ret;
        }

	gpio_direction_output(ME3630_POW, 0);

	ME3630_RST = of_get_named_gpio(np, "me3630-ctl", 0);
	if (ME3630_RST == -EPROBE_DEFER)
                return ME3630_RST;
        if (ME3630_RST < 0) {
                dev_err(&pdev->dev, "error acquiring power gpio: %d\n", ME3630_RST);
                return ME3630_RST;
        }

        ret = devm_gpio_request_one(&pdev->dev, ME3630_RST, 0, "usb-gpio");
        if(ret) {
                dev_err(&pdev->dev, "error requesting power gpio: %d\n", ret);
                return ret;
        }

	gpio_direction_output(ME3630_RST, 0);
	msleep(100);
	gpio_direction_output(ME3630_RST, 1);
	

	ret = misc_register(&power_misc_dev);

	printk(DEVICE_NAME "\tinitialized\n");

	return 0;
}

static int power_remove (struct platform_device *pdev)
{
	misc_deregister(&power_misc_dev);
	gpio_free(POWER_GPIO);	

	return 0;
}

static int power_suspend (struct platform_device *pdev, pm_message_t state)
{
	printk("led_ctl suspend:power off!\n");
	return 0;
}

static int power_resume (struct platform_device *pdev)
{
	printk("led_ctl resume:power on!\n");
	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id power_of_match[] = {
        { .compatible = "powerctrl-gpio" },
        { /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, power_of_match);
#endif

static struct platform_driver power_driver = {
	.probe = power_probe,
	.remove = power_remove,
	.suspend = power_suspend,
	.resume = power_resume,
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(power_of_match),
	},
};

static int __init power_dev_init(void) {
	return platform_driver_register(&power_driver);
}

static void __exit power_dev_exit(void) {
	platform_driver_unregister(&power_driver);
}

module_init(power_dev_init);
module_exit(power_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TOPEET Inc.");
