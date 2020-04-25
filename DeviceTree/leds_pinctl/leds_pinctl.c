#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/pinctrl/consumer.h>


#define DRIVER_NAME "leds_test"

static int leds_probe(struct platform_device *pdev)
{
	struct pinctrl *leds_pin_ctrl;
	struct pinctrl_state *pin_state;

	printk(KERN_ALERT "leds_probe\n");

	leds_pin_ctrl = devm_pinctrl_get(&pdev->dev);
	pin_state = pinctrl_lookup_state(leds_pin_ctrl, "leds_gpios1_on");
	pinctrl_select_state(leds_pin_ctrl, pin_state);
	devm_pinctrl_put(leds_pin_ctrl);

	return 0;
}

static int leds_remove(struct platform_device *pdev)
{
	printk(KERN_ALERT "leds_remove\n");

	devm_pinctrl_get_select(&pdev->dev, "leds_gpios1_off");

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
