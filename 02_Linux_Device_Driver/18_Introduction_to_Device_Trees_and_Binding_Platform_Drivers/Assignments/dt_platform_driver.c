#include <linux/init.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SUNIL");
MODULE_DESCRIPTION("Platform driver using device tree");

struct mydt_data {
    int id;
    const char *label;
};

static int dt_probe(struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;
    struct device_node *np = dev->of_node;
    struct mydt_data data;

    if (!np) {
        dev_err(dev, "No device tree node found\n");
        return -EINVAL;
    }

    if (of_property_read_u32(np, "id", &data.id))
        data.id = -1;

    if (of_property_read_string(np, "label", &data.label))
        data.label = "unknown";

    dev_info(dev, "Probed via DT: id=%d, label=%s\n", data.id, data.label);
    return 0;
}

static int dt_remove(struct platform_device *pdev)
{
    dev_info(&pdev->dev, "Device removed\n");
    return 0;
}

static const struct of_device_id dt_ids[] = {
    { .compatible = "myvendor,mydevice" },
    { }
};
MODULE_DEVICE_TABLE(of, dt_ids);

static struct platform_driver dt_driver = {
    .probe = dt_probe,
    .remove = dt_remove,
    .driver = {
        .name = "mydevice",
        .of_match_table = dt_ids,
    },
};


 
module_platform_driver(dt_driver);