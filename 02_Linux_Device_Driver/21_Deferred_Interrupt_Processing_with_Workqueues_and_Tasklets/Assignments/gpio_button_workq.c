#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>
#include <linux/of.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SUNIL");
MODULE_DESCRIPTION("GPIO button driver with deferred work");

static struct gpio_desc *btn_gpiod;
static int irq_num;
static int press_count = 0;

static struct work_struct btn_work;

static void btn_work_handler(struct work_struct *work)
{
    press_count++;
    pr_info("[gpiobtn] Button pressed %d times (in workqueue)\n", press_count);
}

static irqreturn_t btn_irq_handler(int irq, void *dev_id)
{
    schedule_work(&btn_work);  // Defer to workqueue
    return IRQ_HANDLED;
}

static int btn_probe(struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;

    INIT_WORK(&btn_work, btn_work_handler);

    btn_gpiod = gpiod_get(dev, NULL, GPIOD_IN);
    if (IS_ERR(btn_gpiod))
        return PTR_ERR(btn_gpiod);

    irq_num = gpiod_to_irq(btn_gpiod);
    if (irq_num < 0)
        return irq_num;

    if (request_irq(irq_num, btn_irq_handler, IRQF_TRIGGER_FALLING,
                    "gpiobtn_irq", NULL)) {
        gpiod_put(btn_gpiod);
        return -EBUSY;
    }

    dev_info(dev, "Driver loaded with IRQ %d\n", irq_num);
    return 0;
}

static int btn_remove(struct platform_device *pdev)
{
    free_irq(irq_num, NULL);
    gpiod_put(btn_gpiod);
    cancel_work_sync(&btn_work);
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