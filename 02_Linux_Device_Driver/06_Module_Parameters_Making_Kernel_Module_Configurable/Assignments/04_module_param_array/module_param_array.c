#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/moduleparam.h>

static int arr[3] = {0,0,0};
static int count = 0;

module_param_array(arr, int, &count,0660);

static int __init array_module_init(void)
{
        int i;
        int sum = 0;
        int avg;

        pr_info("Array_Module loaded\n");
        pr_info("Number of Elements Passed: %d\n",count);

        for(i=0; i<count; i++)
        {
                pr_info("arr[%d] = %d\n",i,arr[i]);
                sum += arr[i];
        }

        if(count > 0)
                 avg = sum / count;
        else
                avg = 0;

        pr_info("Average = %d\n",avg);

        return 0;
}


static void __exit array_module_exit(void)
{
        pr_info("Array_Module removed\n");
}

module_init(array_module_init);
module_exit(array_module_exit);

MODULE_LICENSE("GPL");
