/*
 * File      : application.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 */

/**
 * @addtogroup STM32
 */

/*@{*/

#include <board.h>
#include <rtthread.h>

#ifdef RT_USING_COMPONENTS_INIT
#include <components_init.h>
#endif

#include "led.h"

ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t led_stack[512];
static struct rt_thread led_thread;
static void led_thread_entry(void *parameter)
{
    unsigned int count=0;

    rt_hw_led_init();

    while (1)
    {
        /* led1 on */
#ifndef RT_USING_FINSH
        rt_kprintf("led on, count : %d\r\n",count);
#endif
        count ++;
        rt_hw_led_on();
        rt_thread_delay(RT_TICK_PER_SECOND); /* sleep 1 second and switch to other thread */

        /* led1 off */
#ifndef RT_USING_FINSH
        rt_kprintf("led off\r\n");
#endif
        rt_hw_led_off();
        rt_thread_delay(RT_TICK_PER_SECOND);
    }
}

void rt_init_thread_entry(void *parameter)
{
#ifdef RT_USING_COMPONENTS_INIT
    /* initialization RT-Thread Components */
    rt_components_init();
#endif
}

int rt_application_init(void)
{
    rt_thread_t init_thread;

    rt_err_t result;

    /* init led thread */
    result = rt_thread_init(&led_thread,
        "led",
        led_thread_entry, RT_NULL,
        (rt_uint8_t *)&led_stack[0], sizeof(led_stack), 20, 5);
    if (result == RT_EOK)
    {
        rt_thread_startup(&led_thread);
    }

#if (RT_THREAD_PRIORITY_MAX == 32)
    init_thread = rt_thread_create("init",
                                rt_init_thread_entry, RT_NULL,
                                2048, 8, 20);
#else
    init_thread = rt_thread_create("init",
                                rt_init_thread_entry, RT_NULL,
                                2048, 80, 20);
#endif

    if (init_thread != RT_NULL)
        rt_thread_startup(init_thread);

    return 0;
}

/*@}*/
