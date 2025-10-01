#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/jiffies.h>
#include <linux/param.h>

#define PROC_NAME "myinfo"

static char *author = "SUNIL";
module_param(author, charp, 0);
MODULE_PARM_DESC(author, "Author name");

static int counter = 0;

static int show_myinfo(struct seq_file *m, void *v)
{
    unsigned long uptime = jiffies / HZ; // uptime in seconds
    counter++;

    seq_printf(m, "Hello from kernel space!\n");
    seq_printf(m, "Module: proc_demo\n");
    seq_printf(m, "Status: Running and happy\n");
    seq_printf(m, "System Uptime: %lu seconds\n", uptime);
    seq_printf(m, "Author: %s\n", author);
    seq_printf(m, "Read Counter: %d\n", counter);

    return 0;
}

static int open_myinfo(struct inode *inode, struct file *file)
{
    return single_open(file, show_myinfo, NULL);
}

static const struct proc_ops myinfo_fops = {
    .proc_open    = open_myinfo,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
};

static int __init proc_demo_init(void)
{
    proc_create(PROC_NAME, 0, NULL, &myinfo_fops);
    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
    return 0;
}

static void __exit proc_demo_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

module_init(proc_demo_init);
module_exit(proc_demo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SUNIL");
MODULE_DESCRIPTION("Kernel module with /proc entry, uptime, author, and counter");
