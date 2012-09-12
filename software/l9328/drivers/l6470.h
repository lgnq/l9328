/*
 * File      : l6470.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2012, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2012-09-09     lgnq         the first version
 */

#ifndef __L6470_H__
#define __L6470_H__

#include <rtthread.h>
#include <drivers/spi.h>

#include "dspin.h"

#define PAN_BUSY_Pin		GPIO_Pin_8	//PAN BUSY
#define PAN_BUSY_Port		GPIOC

#define PAN_FLAG_Pin		GPIO_Pin_6	//PAN FLAG
#define PAN_FLAG_Port		GPIOC

#define TILT_BUSY_Pin		GPIO_Pin_9	//TILT BUSY
#define TILT_BUSY_Port		GPIOC

#define TILT_FLAG_Pin		GPIO_Pin_7	//TILT FLAG
#define TILT_FLAG_Port		GPIOC

enum
{
	TILT = 0,
	PAN = 1,
};

/* L6470 command set */
enum l6470_cmd
{
	SET_PARAM = 0,
	PAN_SET_PARAM,
	TILT_SET_PARAM,
	GET_PARAM,
	PAN_GET_PARAM,
	TILT_GET_PARAM,
	RUN,
	STEP_CLOCK,
	MOVE,
	PAN_MOVE,
	TILT_MOVE,
	GO_TO,
	GO_TO_DIR,
	GO_UNTIL,
	RELEASE_SW,
	GO_HOME,
	GO_MARK,
	RESET_POS,
	RESET_DEVICE,
	SOFT_STOP,
	HARD_STOP,
	SOFT_HIZ,
	HARD_HIZ,
	GET_STATUS,
};

struct spi_l6470_device
{
    struct rt_device      device;
    struct rt_spi_device *spi_dev;
    struct rt_mutex       lock;
};

struct param_data
{
	dSPIN_Registers_TypeDef param;
    rt_uint32_t             pan_value;
    rt_uint32_t             tilt_value;
};

struct move_data
{
	dSPIN_Direction_TypeDef pan_dir;
	rt_uint32_t			    pan_steps;
	dSPIN_Direction_TypeDef tilt_dir;
	rt_uint32_t			    tilt_steps;
};

extern rt_err_t rt_hw_l6470_init(const char *l6470_device_name,
                                 const char *spi_device_name);

#endif
