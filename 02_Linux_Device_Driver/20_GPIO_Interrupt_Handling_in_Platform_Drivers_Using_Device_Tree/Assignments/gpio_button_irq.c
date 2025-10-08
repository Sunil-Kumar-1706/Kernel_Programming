#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/gpio/consumer.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/irqreturn.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SUNIL");
MODULE_DESCRIPTION("GPIO button with interrupt");

static struct gpio_desc *btn_gpiod;
static int irq_num;

static irqreturn_t btn_irq_handler(int irq, void *dev_id)
{
    pr_info("[gpiobtn] Interrupt received! Button pressed.\n");
    return IRQ_HANDLED;
}

static int btn_probe(struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;

    btn_gpiod = gpiod_get(dev, NULL, GPIOD_IN);
    if (IS_ERR(btn_gpiod)) {
        dev_err(dev, "Failed to get GPIO\n");
        return PTR_ERR(btn_gpiod);
    }

    irq_num = gpiod_to_irq(btn_gpiod);
    if (irq_num < 0) {
        dev_err(dev, "Failed to get IRQ from GPIO\n");
        return irq_num;
    }

    if (request_irq(irq_num, btn_irq_handler, IRQF_TRIGGER_FALLING,
                    "gpiobtn_irq", NULL)) {
        dev_err(dev, "Failed to request IRQ\n");
        return -EBUSY;
    }

    dev_info(dev, "Button driver initialized, IRQ %d\n", irq_num);
    return 0;
}

static int btn_remove(struct platform_device *pdev)
{
    free_irq(irq_num, NULL);
    gpiod_put(btn_gpiod);
    pr_info("[gpiobtn] Driver removed\n");
    return 0;
}

static const struct of_device_id btn_of_match[] = {
    { .compatible = "myvendor,gpiobtn" },
    { }
};
MODULE_DEVICE_TABLE(of, btn_of_match);

static struct platform_driver btn_driver = {
    .driver = {
        .name = "gpiobtn",
        .of_match_table = btn_of_match,
    },
    .probe = btn_probe,
    .remove = btn_remove,
};

module_platform_driver(btn_driver);