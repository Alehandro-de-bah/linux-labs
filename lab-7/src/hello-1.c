#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple Hello World kernel module");

int init_module(void)
{
    printk("<1>Hello world 1.\n");
    return 0;
}

void cleanup_module(void)
{
    printk(KERN_ALERT "Goodbye world 1.\n");
}

