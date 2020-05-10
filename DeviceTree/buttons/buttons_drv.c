#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_gpio.h>


#define DRIVER_NAME "buttons_test"

static int gpio_pin = 0;

static int buttons_probe(struct platform_device *pdev)
{
	struct device_node *node = pdev->dev.of_node;
	int ret;

	printk(KERN_ALERT "buttons_probe\n");

	gpio_pin = of_get_named_gpio(node, "gpioin", 0);

	ret = gpio_request(gpio_pin, "buttons");
	if (ret != 0)
	{
		printk(KERN_ALERT "gpio_pin request %d failed\n", gpio_pin);
		return ret;
	}

	gpio_free(gpio_pin);

	gpio_direction_input(gpio_pin);
	int i = gpio_get_value(gpio_pin);
	printk(KERN_ALERT "i = %d\n", i);

	return 0;
}

static int buttons_remove(struct platform_device *pdev)
{
	printk(KERN_ALERT "buttons_remove\n");

	return 0;
}

static const struct of_device_id of_buttons_dt_match[] = {
		{.compatible = DRIVER_NAME},
		{},
};

static struct platform_driver buttons_drv = {
		.probe = buttons_probe,
		.remove = buttons_remove,
		.driver = {
			.name = DRIVER_NAME,
			.owner = THIS_MODULE,
			.of_match_table = of_buttons_dt_match,
		},
};

static int __init buttons_init(void)
{
	printk(KERN_ALERT "buttons init\n");

	platform_driver_register(&buttons_drv);

	return 0;
}

static void __exit buttons_exit(void)
{
	printk(KERN_ALERT "leds_exit\n");

	platform_driver_unregister(&buttons_drv);
}

module_init(buttons_init);
module_exit(buttons_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("LSQ");
