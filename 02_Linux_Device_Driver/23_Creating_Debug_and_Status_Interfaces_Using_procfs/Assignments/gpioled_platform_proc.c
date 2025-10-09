#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/gpio/consumer.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SUNIL");
MODULE_DESCRIPTION("GPIO LED platform driver with /proc");

static struct gpio_desc *led_gpiod;
static struct proc_dir_entry *proc_entry;
static bool led_state = false;

static int gpioled_proc_show(struct seq_file *m, void *v)
{
    seq_printf(m, "Driver: GPIO LED\n");
    seq_printf(m, "LED State: %s\n", led_state ? "ON" : "OFF");
    return 0;
}

static int gpioled_proc_open(struct inode *inode, struct file *file)
{
    return single_open(file, gpioled_proc_show, NULL);
}

static const struct proc_ops gpioled_proc_ops = {
    .proc_open    = gpioled_proc_open,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
};

static int gpioled_probe(struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;

    led_gpiod = gpiod_get(dev, NULL, GPIOD_OUT_LOW);
    if (IS_ERR(led_gpiod))
        return PTR_ERR(led_gpiod);

    led_state = true;
    gpiod_set_value(led_gpiod, 1);

    proc_entry = proc_create("gpioled_info", 0, NULL, &gpioled_proc_ops);
    if (!proc_entry)
        dev_warn(dev, "Failed to create /proc/gpioled_info\n");

    return 0;
}

static int gpioled_remove(struct platform_device *pdev)
{
    gpiod_set_value(led_gpiod, 0);
    led_state = false;
    gpiod_put(led_gpiod);

    proc_remove(proc_entry);
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
