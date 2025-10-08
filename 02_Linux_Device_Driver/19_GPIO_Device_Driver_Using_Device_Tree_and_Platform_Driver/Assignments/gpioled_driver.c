#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/gpio/consumer.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SUNIL");
MODULE_DESCRIPTION("GPIO LED platform driver");

static struct gpio_desc *led_gpiod;

static int gpioled_probe(struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;

    led_gpiod = gpiod_get(dev, NULL, GPIOD_OUT_LOW);
    if (IS_ERR(led_gpiod)) {
        dev_err(dev, "Failed to get GPIO\n");
        return PTR_ERR(led_gpiod);
    }

    dev_info(dev, "GPIO LED driver bound successfully\n");

 
    gpiod_set_value(led_gpiod, 1);
    dev_info(dev, "LED turned ON\n");

    return 0;
}

static int gpioled_remove(struct platform_device *pdev)
{
    gpiod_set_value(led_gpiod, 0);  // Turn off LED
    gpiod_put(led_gpiod);
    dev_info(&pdev->dev, "GPIO LED driver removed\n");
    return 0;
}

static const struct of_device_id gpioled_of_ids[] = {
    { .compatible = "myvendor,gpioled" },
    { }
};
MODULE_DEVICE_TABLE(of, gpioled_of_ids);

static struct platform_driver gpioled_driver = {
    .driver = {
        .name = "gpioled",
        .of_match_table = gpioled_of_ids,
    },
    .probe = gpioled_probe,
    .remove = gpioled_remove,
};
 
module_platform_driver(gpioled_driver);