/*
 * stop.c - Пример модуля из нескольких файлов
 */
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Multi-file kernel module example (stop part)");

void cleanup_module(void)
{
    printk("<1>Short is the life of a kernel module\n");
}
