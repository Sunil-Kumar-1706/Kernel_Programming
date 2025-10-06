#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define BUFFER_SIZE 256

static dev_t dev_number;
static struct cdev my_cdev;
static struct class *my_class;
static struct device *my_device;
static char message[BUFFER_SIZE] = "Hello from kernel space!\n";

static char *device_name = "autodev";
static char *class_name  = "myclass";

module_param(device_name, charp, 0660);
MODULE_PARM_DESC(device_name, "Device name under /dev/");
module_param(class_name, charp, 0660);
MODULE_PARM_DESC(class_name, "Class name under /sys/class/");

static int dev_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "[%s] Opened\n", device_name);
    return 0;
}

static int dev_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "[%s] Closed\n", device_name);
    return 0;
}

static ssize_t dev_read(struct file *file, char __user *buf, size_t len, loff_t *offset)
{
    return simple_read_from_buffer(buf, len, offset, message, strlen(message));
}

static ssize_t dev_write(struct file *file, const char __user *buf, size_t len, loff_t *offset)
{
    if (len > BUFFER_SIZE - 1)
        len = BUFFER_SIZE - 1;

    if (copy_from_user(message, buf, len))
        return -EFAULT;

    message[len] = '\0';
    printk(KERN_INFO "[%s] Received: %s\n", device_name, message);
    return len;
}

static struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = dev_open,
    .read    = dev_read,
    .write   = dev_write,
    .release = dev_release
};

static int __init autodev_init(void)
{
    int ret;

    ret = alloc_chrdev_region(&dev_number, 0, 1, device_name);
    if (ret < 0) {
        printk(KERN_ALERT "alloc_chrdev_region failed\n");
        return ret;
    }

    cdev_init(&my_cdev, &fops);
    my_cdev.owner = THIS_MODULE;
    ret = cdev_add(&my_cdev, dev_number, 1);
    if (ret < 0) {
        unregister_chrdev_region(dev_number, 1);
        printk(KERN_ALERT "cdev_add failed\n");
        return ret;
    }

    my_class = class_create(class_name);
    if (IS_ERR(my_class)) {
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_number, 1);
        return PTR_ERR(my_class);
    }

    my_device = device_create(my_class, NULL, dev_number, NULL, device_name);
    if (IS_ERR(my_device)) {
        class_destroy(my_class);
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_number, 1);
        return PTR_ERR(my_device);
    }

    printk(KERN_INFO "[%s] Device created: /dev/%s\n", device_name, device_name);
    return 0;
}

static void __exit autodev_exit(void)
{
    device_destroy(my_class, dev_number);
    class_destroy(my_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_number, 1);
    printk(KERN_INFO "[%s] Device removed\n", device_name);
}

module_init(autodev_init);
module_exit(autodev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SUNIL");
MODULE_DESCRIPTION("Char device with automatic /dev node creation and custom naming");
