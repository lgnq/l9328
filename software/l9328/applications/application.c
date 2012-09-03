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
#include "dspin.h"

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
	dSPIN_RegsStruct_TypeDef pan_regs;
	dSPIN_RegsStruct_TypeDef tilt_regs;

#ifdef RT_USING_COMPONENTS_INIT
    /* initialization RT-Thread Components */
    rt_components_init();
#endif

	/* Initialize peripherals used by dSPIN */
	dSPIN_Peripherals_Init();

	/* Structure initialization by default values, in order to avoid blank records */
	dSPIN_Regs_Struct_Reset(&pan_regs);

	/* Acceleration rate settings to 466 steps/s2, range 14.55 to 59590 steps/s2 */
	pan_regs.ACC		= AccDec_Steps_to_Par(466);
	/* Deceleration rate settings to 466 steps/s2, range 14.55 to 59590 steps/s2 */
	pan_regs.DEC		= AccDec_Steps_to_Par(466); 
	/* Maximum speed settings to 488 steps/s, range 15.25 to 15610 steps/s */
	pan_regs.MAX_SPEED	= MaxSpd_Steps_to_Par(488);
	/* Minimum speed settings to 0 steps/s, range 0 to 976.3 steps/s */
	pan_regs.MIN_SPEED	= MinSpd_Steps_to_Par(0);
	/* Full step speed settings 252 steps/s, range 7.63 to 15625 steps/s */
	pan_regs.FS_SPD 	= FSSpd_Steps_to_Par(252);
	/* Hold duty cycle (torque) settings to 10%, range 0 to 99.6% */
	pan_regs.KVAL_HOLD	= Kval_Perc_to_Par(10);
	/* Run duty cycle (torque) settings to 10%, range 0 to 99.6% */
	pan_regs.KVAL_RUN	= Kval_Perc_to_Par(10);
	/* Acceleration duty cycle (torque) settings to 10%, range 0 to 99.6% */
	pan_regs.KVAL_ACC	= Kval_Perc_to_Par(10);
	/* Deceleration duty cycle (torque) settings to 10%, range 0 to 99.6% */
	pan_regs.KVAL_DEC	= Kval_Perc_to_Par(10); 	
	/* Intersect speed settings for BEMF compensation to 200 steps/s, range 0 to 3906 steps/s */
	pan_regs.INT_SPD	= IntSpd_Steps_to_Par(200);
	/* BEMF start slope settings for BEMF compensation to 0.038% step/s, range 0 to 0.4% s/step */
	pan_regs.ST_SLP 	= BEMF_Slope_Perc_to_Par(0.038);
	/* BEMF final acc slope settings for BEMF compensation to 0.063% step/s, range 0 to 0.4% s/step */
	pan_regs.FN_SLP_ACC = BEMF_Slope_Perc_to_Par(0.063);
	/* BEMF final dec slope settings for BEMF compensation to 0.063% step/s, range 0 to 0.4% s/step */
	pan_regs.FN_SLP_DEC = BEMF_Slope_Perc_to_Par(0.063);
	/* Thermal compensation param settings to 1, range 1 to 1.46875 */
	pan_regs.K_THERM	= KTherm_to_Par(1);
	/* Overcurrent threshold settings to 1500mA */
	pan_regs.OCD_TH 	= dSPIN_OCD_TH_1500mA;
	/* Stall threshold settings to 1000mA, range 31.25 to 4000mA */
	pan_regs.STALL_TH	= StallTh_to_Par(1000);
	/* Step mode settings to 128 microsteps */
	pan_regs.STEP_MODE	= dSPIN_STEP_SEL_1_128;
	/* Alarm settings - all alarms enabled */
	pan_regs.ALARM_EN	= dSPIN_ALARM_EN_OVERCURRENT | dSPIN_ALARM_EN_THERMAL_SHUTDOWN
		| dSPIN_ALARM_EN_THERMAL_WARNING | dSPIN_ALARM_EN_UNDER_VOLTAGE | dSPIN_ALARM_EN_STALL_DET_A
		| dSPIN_ALARM_EN_STALL_DET_B | dSPIN_ALARM_EN_SW_TURN_ON | dSPIN_ALARM_EN_WRONG_NPERF_CMD;
	/* Internal oscillator, 2MHz OSCOUT clock, supply voltage compensation disabled, *
	 * overcurrent shutdown enabled, slew-rate = 290 V/us, PWM frequency = 15.6kHz	 */
	pan_regs.CONFIG 	= dSPIN_CONFIG_INT_16MHZ_OSCOUT_2MHZ | dSPIN_CONFIG_SW_HARD_STOP
		| dSPIN_CONFIG_VS_COMP_DISABLE | dSPIN_CONFIG_OC_SD_ENABLE | dSPIN_CONFIG_SR_290V_us
		| dSPIN_CONFIG_PWM_DIV_2 | dSPIN_CONFIG_PWM_MUL_1;	

	/* Structure initialization by default values, in order to avoid blank records */
	dSPIN_Regs_Struct_Reset(&tilt_regs);

	/* Acceleration rate settings to 466 steps/s2, range 14.55 to 59590 steps/s2 */
	tilt_regs.ACC		= AccDec_Steps_to_Par(466);
	/* Deceleration rate settings to 466 steps/s2, range 14.55 to 59590 steps/s2 */
	tilt_regs.DEC		= AccDec_Steps_to_Par(466); 
	/* Maximum speed settings to 488 steps/s, range 15.25 to 15610 steps/s */
	tilt_regs.MAX_SPEED	= MaxSpd_Steps_to_Par(488);
	/* Minimum speed settings to 0 steps/s, range 0 to 976.3 steps/s */
	tilt_regs.MIN_SPEED	= MinSpd_Steps_to_Par(0);
	/* Full step speed settings 252 steps/s, range 7.63 to 15625 steps/s */
	tilt_regs.FS_SPD 	= FSSpd_Steps_to_Par(252);
	/* Hold duty cycle (torque) settings to 10%, range 0 to 99.6% */
	tilt_regs.KVAL_HOLD	= Kval_Perc_to_Par(10);
	/* Run duty cycle (torque) settings to 10%, range 0 to 99.6% */
	tilt_regs.KVAL_RUN	= Kval_Perc_to_Par(10);
	/* Acceleration duty cycle (torque) settings to 10%, range 0 to 99.6% */
	tilt_regs.KVAL_ACC	= Kval_Perc_to_Par(10);
	/* Deceleration duty cycle (torque) settings to 10%, range 0 to 99.6% */
	tilt_regs.KVAL_DEC	= Kval_Perc_to_Par(10); 	
	/* Intersect speed settings for BEMF compensation to 200 steps/s, range 0 to 3906 steps/s */
	tilt_regs.INT_SPD	= IntSpd_Steps_to_Par(200);
	/* BEMF start slope settings for BEMF compensation to 0.038% step/s, range 0 to 0.4% s/step */
	tilt_regs.ST_SLP 	= BEMF_Slope_Perc_to_Par(0.038);
	/* BEMF final acc slope settings for BEMF compensation to 0.063% step/s, range 0 to 0.4% s/step */
	tilt_regs.FN_SLP_ACC = BEMF_Slope_Perc_to_Par(0.063);
	/* BEMF final dec slope settings for BEMF compensation to 0.063% step/s, range 0 to 0.4% s/step */
	tilt_regs.FN_SLP_DEC = BEMF_Slope_Perc_to_Par(0.063);
	/* Thermal compensation param settings to 1, range 1 to 1.46875 */
	tilt_regs.K_THERM	= KTherm_to_Par(1);
	/* Overcurrent threshold settings to 1500mA */
	tilt_regs.OCD_TH 	= dSPIN_OCD_TH_1500mA;
	/* Stall threshold settings to 1000mA, range 31.25 to 4000mA */
	tilt_regs.STALL_TH	= StallTh_to_Par(1000);
	/* Step mode settings to 128 microsteps */
	tilt_regs.STEP_MODE	= dSPIN_STEP_SEL_1_128;
	/* Alarm settings - all alarms enabled */
	tilt_regs.ALARM_EN	= dSPIN_ALARM_EN_OVERCURRENT | dSPIN_ALARM_EN_THERMAL_SHUTDOWN
		| dSPIN_ALARM_EN_THERMAL_WARNING | dSPIN_ALARM_EN_UNDER_VOLTAGE | dSPIN_ALARM_EN_STALL_DET_A
		| dSPIN_ALARM_EN_STALL_DET_B | dSPIN_ALARM_EN_SW_TURN_ON | dSPIN_ALARM_EN_WRONG_NPERF_CMD;
	/* Internal oscillator, 2MHz OSCOUT clock, supply voltage compensation disabled, *
	 * overcurrent shutdown enabled, slew-rate = 290 V/us, PWM frequency = 15.6kHz	 */
	tilt_regs.CONFIG 	= dSPIN_CONFIG_INT_16MHZ_OSCOUT_2MHZ | dSPIN_CONFIG_SW_HARD_STOP
		| dSPIN_CONFIG_VS_COMP_DISABLE | dSPIN_CONFIG_OC_SD_ENABLE | dSPIN_CONFIG_SR_290V_us
		| dSPIN_CONFIG_PWM_DIV_2 | dSPIN_CONFIG_PWM_MUL_1;	

	/* Program all dSPIN registers */
	registers_set(&pan_regs, &tilt_regs);
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
