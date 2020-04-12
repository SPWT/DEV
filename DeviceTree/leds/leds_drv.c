#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_gpio.h>


#define DRIVER_NAME "leds_test"

static int gpio_pin[2] = {0};

static int leds_probe(struct platform_device *pdev)
{
	struct device_node *node = pdev->dev.of_node;
	int ret;

	printk(KERN_ALERT "leds_probe\n");

	gpio_pin[0] = of_get_named_gpio(node, "gpios1", 0);
	gpio_pin[1] = of_get_named_gpio(node, "gpios2", 0);

	ret = gpio_request(gpio_pin[0], "les1");
	if (ret != 0)
	{
		printk(KERN_ALERT "gpio_pin[0] request %d failed\n", gpio_pin[0]);
		return ret;
	}
	ret = gpio_request(gpio_pin[1], "les2");
	if (ret != 0)
	{
		printk(KERN_ALERT "gpio_pin[0] request %d failed\n", gpio_pin[0]);
		return ret;
	}

	gpio_free(gpio_pin[0]);
	gpio_free(gpio_pin[1]);

	gpio_direction_output(gpio_pin[0],0);
	gpio_set_value(gpio_pin[0], 1);
	gpio_direction_output(gpio_pin[1],0);
	gpio_set_value(gpio_pin[1], 1);

	return 0;
}

static int leds_remove(struct platform_device *pdev)
{
	printk(KERN_ALERT "leds_remove\n");

	gpio_set_value(gpio_pin[0], 0);
	gpio_set_value(gpio_pin[1], 0);

	return 0;
}

static const struct of_device_id of_leds_dt_match[] = {
		{.compatible = DRIVER_NAME},
		{},
};

static struct platform_driver leds_drv = {
		.probe = leds_probe,
		.remove = leds_remove,
		.driver = {
			.name = DRIVER_NAME,
			.owner = THIS_MODULE,
			.of_match_table = of_leds_dt_match,
		},
};

static int __init leds_init(void)
{
	printk(KERN_ALERT "leds init\n");

	platform_driver_register(&leds_drv);

	return 0;
}

static void __exit leds_exit(void)
{
	printk(KERN_ALERT "leds_exit\n");

	platform_driver_unregister(&leds_drv);
}

module_init(leds_init);
module_exit(leds_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("LSQ");
