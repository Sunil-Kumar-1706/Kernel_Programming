#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/string.h>

#define PROC_CONTROL "control"
#define PROC_STATUS  "status"
#define MAX_LEN 100

static char state[MAX_LEN] = "Stopped";

ssize_t status_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    char status_buf[120];
    int len = snprintf(status_buf, sizeof(status_buf), "Status: %s\n", state);
    return simple_read_from_buffer(buf, count, ppos, status_buf, len);
}

ssize_t control_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    char command[MAX_LEN];

    if (count > MAX_LEN - 1)
        count = MAX_LEN - 1;

    if (copy_from_user(command, buf, count))
        return -EFAULT;

    command[count] = '\0';

    if (strncmp(command, "Start", 5) == 0)
        strncpy(state, "Running", MAX_LEN);
    else if (strncmp(command, "Stop", 4) == 0)
        strncpy(state, "Stopped", MAX_LEN);

    return count;
}

static const struct proc_ops control_fops = {
    .proc_write = control_write,
};

static const struct proc_ops status_fops = {
    .proc_read = status_read,
};

static int __init proc_cmd_init(void)
{
    proc_create(PROC_CONTROL, 0666, NULL, &control_fops);
    proc_create(PROC_STATUS, 0444, NULL, &status_fops);
    return 0;
}

static void __exit proc_cmd_exit(void)
{
    remove_proc_entry(PROC_CONTROL, NULL);
    remove_proc_entry(PROC_STATUS, NULL);
}

module_init(proc_cmd_init);
module_exit(proc_cmd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SUNIL");
MODULE_DESCRIPTION("Procfs command-response interface (Start/Stop only)");
