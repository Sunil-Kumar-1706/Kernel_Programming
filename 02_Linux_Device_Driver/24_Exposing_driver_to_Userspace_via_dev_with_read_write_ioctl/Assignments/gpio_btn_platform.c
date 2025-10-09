#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/gpio/consumer.h>

#define DEVICE_NAME "gpiobtn"
#define CLASS_NAME  "gpiobtnclass"
#define GPIOBTN_MAGIC 'G'
#define GPIOBTN_RESET _IO(GPIOBTN_MAGIC, 0)

struct gpiobtn_data {
    dev_t dev_num;
    struct cdev cdev;
    struct class *devclass;
    struct device *device;
    struct gpio_desc *led_gpiod;
    int press_count;
    bool led_state;
};

static struct gpiobtn_data *btn_data;

static int gpiobtn_open(struct inode *inode, struct file *file) { return 0; }
static int gpiobtn_release(struct inode *inode, struct file *file) { return 0; }

static ssize_t gpiobtn_read(struct file *file, char __user *buf, size_t len, loff_t *off)
{
    char msg[64];
    int msg_len = snprintf(msg, sizeof(msg), "Press Count: %d\n", btn_data->press_count);

    if (*off >= msg_len) return 0;
    if (copy_to_user(buf, msg, msg_len)) return -EFAULT;

    *off += msg_len;
    return msg_len;
}

static ssize_t gpiobtn_write(struct file *file, const char __user *buf, size_t len, loff_t *off)
{
    char input[8];
    if (len > sizeof(input)-1) len = sizeof(input)-1;
    if (copy_from_user(input, buf, len)) return -EFAULT;
    input[len] = '\0';

    if (input[0] == '1') {
        btn_data->led_state = !btn_data->led_state;
        if (btn_data->led_gpiod) gpiod_set_value(btn_data->led_gpiod, btn_data->led_state);
    }

    return len;
}

static long gpiobtn_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    if (cmd == GPIOBTN_RESET) btn_data->press_count = 0;
    else return -EINVAL;
    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = gpiobtn_open,
    .release = gpiobtn_release,
    .read = gpiobtn_read,
    .write = gpiobtn_write,
    .unlocked_ioctl = gpiobtn_ioctl,
};

static int gpiobtn_probe(struct platform_device *pdev)
{
    int ret;
    struct gpiobtn_data *data;

    data = devm_kzalloc(&pdev->dev, sizeof(*data), GFP_KERNEL);
    if (!data) return -ENOMEM;
    btn_data = data;

    ret = alloc_chrdev_region(&data->dev_num, 0, 1, DEVICE_NAME);
    if (ret) return ret;

    cdev_init(&data->cdev, &fops);
    ret = cdev_add(&data->cdev, data->dev_num, 1);
    if (ret) {
        unregister_chrdev_region(data->dev_num, 1);
        return ret;
    }

    data->devclass = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(data->devclass)) {
        cdev_del(&data->cdev);
        unregister_chrdev_region(data->dev_num, 1);
        return PTR_ERR(data->devclass);
    }

    data->device = device_create(data->devclass, NULL, data->dev_num, NULL, DEVICE_NAME);
    if (IS_ERR(data->device)) {
        class_destroy(data->devclass);
        cdev_del(&data->cdev);
        unregister_chrdev_region(data->dev_num, 1);
        return PTR_ERR(data->device);
    }

    data->led_gpiod = devm_gpiod_get(&pdev->dev, "led", GPIOD_OUT_LOW);
    if (IS_ERR(data->led_gpiod)) data->led_gpiod = NULL;

    pr_info("[gpiobtn] Probe done, /dev/%s created\n", DEVICE_NAME);
    return 0;
}

static int gpiobtn_remove(struct platform_device *pdev)
{
    struct gpiobtn_data *data = btn_data;

    device_destroy(data->devclass, data->dev_num);
    class_destroy(data->devclass);
    cdev_del(&data->cdev);
    unregister_chrdev_region(data->dev_num, 1);

    pr_info("[gpiobtn] Remove done\n");
    return 0;
}

static struct platform_driver gpiobtn_driver = {
    .driver = { .name = "gpiobtn", .owner = THIS_MODULE },
    .probe = gpiobtn_probe,
    .remove = gpiobtn_remove,
};

module_platform_driver(gpiobtn_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SUNIL");
MODULE_DESCRIPTION("GPIO Button Platform Driver without interrupts");
