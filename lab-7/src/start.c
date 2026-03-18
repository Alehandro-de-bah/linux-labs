/*
 * start.c - Пример модуля из нескольких файлов
 */
#include <linux/kernel.h>
#include <linux/module.h>

// Обязательные макросы - их достаточно добавить в любой из файлов,
// но лучше в оба для надёжности
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Multi-file kernel module example (start part)");

int init_module(void)
{
    printk("Hello, world - this is the kernel speaking\n");
    return 0;
}
