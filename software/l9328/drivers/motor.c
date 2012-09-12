#include <rtthread.h>

#include "motor.h"
#include "dspin.h"
#include "l6470.h"

void dSPIN_Regs_Struct_Reset(dSPIN_RegsStruct_TypeDef *dSPIN_RegsStruct)
{
    dSPIN_RegsStruct->ABS_POS = 0;
    dSPIN_RegsStruct->EL_POS = 0;
    dSPIN_RegsStruct->MARK = 0;
    dSPIN_RegsStruct->SPEED = 0;
    dSPIN_RegsStruct->ACC = 0x08A;
    dSPIN_RegsStruct->DEC = 0x08A;
    dSPIN_RegsStruct->MAX_SPEED = 0x041;
    dSPIN_RegsStruct->MIN_SPEED = 0;
    dSPIN_RegsStruct->FS_SPD = 0x027;
    dSPIN_RegsStruct->KVAL_HOLD = 0x29;
    dSPIN_RegsStruct->KVAL_RUN = 0x29;
    dSPIN_RegsStruct->KVAL_ACC = 0x29;
    dSPIN_RegsStruct->KVAL_DEC = 0x29;
    dSPIN_RegsStruct->INT_SPD = 0x0408;
    dSPIN_RegsStruct->ST_SLP = 0x19;
    dSPIN_RegsStruct->FN_SLP_ACC = 0x29;
    dSPIN_RegsStruct->FN_SLP_DEC = 0x29;
    dSPIN_RegsStruct->K_THERM = 0;
    dSPIN_RegsStruct->OCD_TH = 0x8;
    dSPIN_RegsStruct->STALL_TH = 0x40;
    dSPIN_RegsStruct->STEP_MODE = 0x7;
    dSPIN_RegsStruct->ALARM_EN = 0xFF;
    dSPIN_RegsStruct->CONFIG = 0x2E88;
}

rt_err_t set_init_param(rt_bool_t is_pan)
{
    rt_device_t dev;
    struct param_data data;
	dSPIN_RegsStruct_TypeDef registers;

	/* Structure initialization by default values, in order to avoid blank records */
	dSPIN_Regs_Struct_Reset(&registers);

	/* Acceleration rate settings to 466 steps/s2, range 14.55 to 59590 steps/s2 */
	registers.ACC		= AccDec_Steps_to_Par(466);
	/* Deceleration rate settings to 466 steps/s2, range 14.55 to 59590 steps/s2 */
	registers.DEC		= AccDec_Steps_to_Par(466); 
	/* Maximum speed settings to 488 steps/s, range 15.25 to 15610 steps/s */
	registers.MAX_SPEED	= MaxSpd_Steps_to_Par(488);
	/* Minimum speed settings to 0 steps/s, range 0 to 976.3 steps/s */
	registers.MIN_SPEED	= MinSpd_Steps_to_Par(0);
	/* Full step speed settings 252 steps/s, range 7.63 to 15625 steps/s */
	registers.FS_SPD 	= FSSpd_Steps_to_Par(252);
	/* Hold duty cycle (torque) settings to 10%, range 0 to 99.6% */
	registers.KVAL_HOLD	= Kval_Perc_to_Par(10);
	/* Run duty cycle (torque) settings to 10%, range 0 to 99.6% */
	registers.KVAL_RUN	= Kval_Perc_to_Par(10);
	/* Acceleration duty cycle (torque) settings to 10%, range 0 to 99.6% */
	registers.KVAL_ACC	= Kval_Perc_to_Par(10);
	/* Deceleration duty cycle (torque) settings to 10%, range 0 to 99.6% */
	registers.KVAL_DEC	= Kval_Perc_to_Par(10); 	
	/* Intersect speed settings for BEMF compensation to 200 steps/s, range 0 to 3906 steps/s */
	registers.INT_SPD	= IntSpd_Steps_to_Par(200);
	/* BEMF start slope settings for BEMF compensation to 0.038% step/s, range 0 to 0.4% s/step */
	registers.ST_SLP 	= BEMF_Slope_Perc_to_Par(0.038);
	/* BEMF final acc slope settings for BEMF compensation to 0.063% step/s, range 0 to 0.4% s/step */
	registers.FN_SLP_ACC = BEMF_Slope_Perc_to_Par(0.063);
	/* BEMF final dec slope settings for BEMF compensation to 0.063% step/s, range 0 to 0.4% s/step */
	registers.FN_SLP_DEC = BEMF_Slope_Perc_to_Par(0.063);
	/* Thermal compensation param settings to 1, range 1 to 1.46875 */
	registers.K_THERM	= KTherm_to_Par(1);
	/* Overcurrent threshold settings to 1500mA */
	registers.OCD_TH 	= dSPIN_OCD_TH_1500mA;
	/* Stall threshold settings to 1000mA, range 31.25 to 4000mA */
	registers.STALL_TH	= StallTh_to_Par(1000);
	/* Step mode settings to 128 microsteps */
	registers.STEP_MODE	= dSPIN_STEP_SEL_1_128;
	/* Alarm settings - all alarms enabled */
	registers.ALARM_EN	= dSPIN_ALARM_EN_OVERCURRENT | dSPIN_ALARM_EN_THERMAL_SHUTDOWN
		| dSPIN_ALARM_EN_THERMAL_WARNING | dSPIN_ALARM_EN_UNDER_VOLTAGE | dSPIN_ALARM_EN_STALL_DET_A
		| dSPIN_ALARM_EN_STALL_DET_B | dSPIN_ALARM_EN_SW_TURN_ON | dSPIN_ALARM_EN_WRONG_NPERF_CMD;
	/* Internal oscillator, 2MHz OSCOUT clock, supply voltage compensation disabled, *
	 * overcurrent shutdown enabled, slew-rate = 290 V/us, PWM frequency = 15.6kHz	 */
	registers.CONFIG 	= dSPIN_CONFIG_INT_16MHZ_OSCOUT_2MHZ | dSPIN_CONFIG_SW_HARD_STOP
		| dSPIN_CONFIG_VS_COMP_DISABLE | dSPIN_CONFIG_OC_SD_ENABLE | dSPIN_CONFIG_SR_290V_us
		| dSPIN_CONFIG_PWM_DIV_2 | dSPIN_CONFIG_PWM_MUL_1;	

    dev = rt_device_find("l6470");

    if (dev == RT_NULL)
    {
        return -RT_ERROR;
    }

	if (is_pan)
	{
		data.param = dSPIN_ABS_POS;
		data.pan_value = registers.ABS_POS;
	    rt_device_control(dev, PAN_SET_PARAM, &data);

		data.param = dSPIN_EL_POS;
		data.pan_value = registers.EL_POS;
	    rt_device_control(dev, PAN_SET_PARAM, &data);

		data.param = dSPIN_MARK;
		data.pan_value = registers.MARK;
	    rt_device_control(dev, PAN_SET_PARAM, &data);

		data.param = dSPIN_SPEED;
		data.pan_value = registers.SPEED;
	    rt_device_control(dev, PAN_SET_PARAM, &data);

		data.param = dSPIN_ACC;
		data.pan_value = registers.ACC;
	    rt_device_control(dev, PAN_SET_PARAM, &data);

		data.param = dSPIN_DEC;
		data.pan_value = registers.DEC;
	    rt_device_control(dev, PAN_SET_PARAM, &data);

		data.param = dSPIN_DEC;
		data.pan_value = registers.DEC;
	    rt_device_control(dev, PAN_SET_PARAM, &data);

		data.param = dSPIN_MAX_SPEED;
		data.pan_value = registers.MAX_SPEED;
	    rt_device_control(dev, PAN_SET_PARAM, &data);

		data.param = dSPIN_MIN_SPEED;
		data.pan_value = registers.MIN_SPEED;
	    rt_device_control(dev, PAN_SET_PARAM, &data);

		data.param = dSPIN_FS_SPD;
		data.pan_value = registers.FS_SPD;
	    rt_device_control(dev, PAN_SET_PARAM, &data);

		data.param = dSPIN_KVAL_HOLD;
		data.pan_value = registers.KVAL_HOLD;
	    rt_device_control(dev, PAN_SET_PARAM, &data);

		data.param = dSPIN_KVAL_RUN;
		data.pan_value = registers.KVAL_RUN;
	    rt_device_control(dev, PAN_SET_PARAM, &data);

		data.param = dSPIN_KVAL_ACC;
		data.pan_value = registers.KVAL_ACC;
	    rt_device_control(dev, PAN_SET_PARAM, &data);

		data.param = dSPIN_KVAL_DEC;
		data.pan_value = registers.KVAL_DEC;
	    rt_device_control(dev, PAN_SET_PARAM, &data);

		data.param = dSPIN_INT_SPD;
		data.pan_value = registers.INT_SPD;
	    rt_device_control(dev, PAN_SET_PARAM, &data);

		data.param = dSPIN_ST_SLP;
		data.pan_value = registers.ST_SLP;
	    rt_device_control(dev, PAN_SET_PARAM, &data);

		data.param = dSPIN_FN_SLP_ACC;
		data.pan_value = registers.FN_SLP_ACC;
	    rt_device_control(dev, PAN_SET_PARAM, &data);

		data.param = dSPIN_FN_SLP_DEC;
		data.pan_value = registers.FN_SLP_DEC;
	    rt_device_control(dev, PAN_SET_PARAM, &data);

		data.param = dSPIN_K_THERM;
		data.pan_value = registers.K_THERM;
	    rt_device_control(dev, PAN_SET_PARAM, &data);

		data.param = dSPIN_OCD_TH;
		data.pan_value = registers.OCD_TH;
	    rt_device_control(dev, PAN_SET_PARAM, &data);

		data.param = dSPIN_STALL_TH;
		data.pan_value = registers.STALL_TH;
	    rt_device_control(dev, PAN_SET_PARAM, &data);

		data.param = dSPIN_STEP_MODE;
		data.pan_value = registers.STEP_MODE;
	    rt_device_control(dev, PAN_SET_PARAM, &data);

		data.param = dSPIN_ALARM_EN;
		data.pan_value = registers.ALARM_EN;
	    rt_device_control(dev, PAN_SET_PARAM, &data);

		data.param = dSPIN_CONFIG;
		data.pan_value = registers.CONFIG;
	    rt_device_control(dev, PAN_SET_PARAM, &data);
	}
	else
	{
		data.param = dSPIN_ABS_POS;
		data.tilt_value = registers.ABS_POS;
	    rt_device_control(dev, TILT_SET_PARAM, &data);

		data.param = dSPIN_EL_POS;
		data.tilt_value = registers.EL_POS;
	    rt_device_control(dev, TILT_SET_PARAM, &data);

		data.param = dSPIN_MARK;
		data.tilt_value = registers.MARK;
	    rt_device_control(dev, TILT_SET_PARAM, &data);

		data.param = dSPIN_SPEED;
		data.tilt_value = registers.SPEED;
	    rt_device_control(dev, TILT_SET_PARAM, &data);

		data.param = dSPIN_ACC;
		data.tilt_value = registers.ACC;
	    rt_device_control(dev, TILT_SET_PARAM, &data);

		data.param = dSPIN_DEC;
		data.tilt_value = registers.DEC;
	    rt_device_control(dev, TILT_SET_PARAM, &data);

		data.param = dSPIN_DEC;
		data.tilt_value = registers.DEC;
	    rt_device_control(dev, TILT_SET_PARAM, &data);

		data.param = dSPIN_MAX_SPEED;
		data.tilt_value = registers.MAX_SPEED;
	    rt_device_control(dev, TILT_SET_PARAM, &data);

		data.param = dSPIN_MIN_SPEED;
		data.tilt_value = registers.MIN_SPEED;
	    rt_device_control(dev, TILT_SET_PARAM, &data);

		data.param = dSPIN_FS_SPD;
		data.tilt_value = registers.FS_SPD;
	    rt_device_control(dev, TILT_SET_PARAM, &data);

		data.param = dSPIN_KVAL_HOLD;
		data.tilt_value = registers.KVAL_HOLD;
	    rt_device_control(dev, TILT_SET_PARAM, &data);

		data.param = dSPIN_KVAL_RUN;
		data.tilt_value = registers.KVAL_RUN;
	    rt_device_control(dev, TILT_SET_PARAM, &data);

		data.param = dSPIN_KVAL_ACC;
		data.tilt_value = registers.KVAL_ACC;
	    rt_device_control(dev, TILT_SET_PARAM, &data);

		data.param = dSPIN_KVAL_DEC;
		data.tilt_value = registers.KVAL_DEC;
	    rt_device_control(dev, TILT_SET_PARAM, &data);

		data.param = dSPIN_INT_SPD;
		data.tilt_value = registers.INT_SPD;
	    rt_device_control(dev, TILT_SET_PARAM, &data);

		data.param = dSPIN_ST_SLP;
		data.tilt_value = registers.ST_SLP;
	    rt_device_control(dev, TILT_SET_PARAM, &data);

		data.param = dSPIN_FN_SLP_ACC;
		data.tilt_value = registers.FN_SLP_ACC;
	    rt_device_control(dev, TILT_SET_PARAM, &data);

		data.param = dSPIN_FN_SLP_DEC;
		data.tilt_value = registers.FN_SLP_DEC;
	    rt_device_control(dev, TILT_SET_PARAM, &data);

		data.param = dSPIN_K_THERM;
		data.tilt_value = registers.K_THERM;
	    rt_device_control(dev, TILT_SET_PARAM, &data);

		data.param = dSPIN_OCD_TH;
		data.tilt_value = registers.OCD_TH;
	    rt_device_control(dev, TILT_SET_PARAM, &data);

		data.param = dSPIN_STALL_TH;
		data.tilt_value = registers.STALL_TH;
	    rt_device_control(dev, TILT_SET_PARAM, &data);

		data.param = dSPIN_STEP_MODE;
		data.tilt_value = registers.STEP_MODE;
	    rt_device_control(dev, TILT_SET_PARAM, &data);

		data.param = dSPIN_ALARM_EN;
		data.tilt_value = registers.ALARM_EN;
	    rt_device_control(dev, TILT_SET_PARAM, &data);

		data.param = dSPIN_CONFIG;
		data.tilt_value = registers.CONFIG;
	    rt_device_control(dev, TILT_SET_PARAM, &data);
	}

	return RT_EOK;
}

void pan_thread_entry(void *parameter)
{
	set_init_param(RT_TRUE);

    while (1)
    {
        rt_thread_delay(1);
    }
}

void tilt_thread_entry(void *parameter)
{
	set_init_param(RT_FALSE);

    while (1)
    {
        rt_thread_delay(1);
    }
}
