#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
//#include <linux/of.h>
//#include <linux/of_gpio.h>


#define DRIVER_NAME "leds_test"

static int leds_probe(struct platform_device *pdev)
{
	printk(KERN_ALERT "leds_probe\n");

	return 0;
}

static int leds_remove(struct platform_device *pdev)
{
	printk(KERN_ALERT "leds_remove\n");

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
}

module_init(leds_init);
module_exit(leds_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("LSQ");