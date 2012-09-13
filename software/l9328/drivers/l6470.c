/*
 * File      : l6470.c
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

#include "l6470.h"
#include "stm32f10x.h"

static struct spi_l6470_device l6470;

static void l6470_lock(struct spi_l6470_device *dev)
{
    rt_mutex_take(&dev->lock, RT_WAITING_FOREVER);
}

static void l6470_unlock(struct spi_l6470_device *dev)
{
    rt_mutex_release(&dev->lock);
}

static rt_err_t l6470_init(rt_device_t dev)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    /* Configure dSPIN - PAN Busy pin */
    GPIO_InitStructure.GPIO_Pin = PAN_BUSY_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(PAN_BUSY_Port, &GPIO_InitStructure);

    /* Configure dSPIN - PAN Flag pin */
    GPIO_InitStructure.GPIO_Pin = PAN_FLAG_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(PAN_FLAG_Port, &GPIO_InitStructure);

    /* Configure dSPIN - TILT Busy pin */
    GPIO_InitStructure.GPIO_Pin = TILT_BUSY_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(TILT_BUSY_Port, &GPIO_InitStructure);

    /* Configure dSPIN - TILT Flag pin */
    GPIO_InitStructure.GPIO_Pin = TILT_FLAG_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(TILT_FLAG_Port, &GPIO_InitStructure);

    return RT_EOK;
}

static rt_err_t l6470_open(rt_device_t dev, rt_uint16_t oflag)
{
    return RT_EOK;
}

static rt_err_t l6470_close(rt_device_t dev)
{
    return RT_EOK;
}

static void set_param(struct spi_l6470_device *device, struct param_data *data)
{
    rt_uint8_t send_buf[2];

    l6470_lock(device);
    
    /* Send SetParam operation code to dSPIN */
    send_buf[TILT] = dSPIN_SET_PARAM | data->param;
    send_buf[PAN]  = dSPIN_SET_PARAM | data->param; 
    rt_spi_send(device->spi_dev, send_buf, 2);

    switch (data->param)
    {
        case dSPIN_ABS_POS: ;
        case dSPIN_MARK: ;
        case dSPIN_SPEED:
            /* Send parameter - byte 2 to dSPIN */
            send_buf[TILT] = (uint8_t)(data->tilt_value >> 16);
            send_buf[PAN]  = (uint8_t)(data->pan_value >> 16); 
            rt_spi_send(device->spi_dev, send_buf, 2);
        case dSPIN_ACC: ;
        case dSPIN_DEC: ;
        case dSPIN_MAX_SPEED: ;
        case dSPIN_MIN_SPEED: ;
        case dSPIN_FS_SPD: ;
        case dSPIN_INT_SPD: ;
        case dSPIN_CONFIG: ;
        case dSPIN_STATUS:
            /* Send parameter - byte 1 to dSPIN */
            send_buf[TILT] = (uint8_t)(data->tilt_value >> 8);
            send_buf[PAN]  = (uint8_t)(data->pan_value >> 8); 
            rt_spi_send(device->spi_dev, send_buf, 2);
        default:
            /* Send parameter - byte 0 to dSPIN */
            send_buf[TILT] = (uint8_t)(data->tilt_value);
            send_buf[PAN]  = (uint8_t)(data->pan_value); 
            rt_spi_send(device->spi_dev, send_buf, 2);
    }

    l6470_unlock(device);
}

static void pan_set_param(struct spi_l6470_device *device, struct param_data *data)
{
    rt_uint8_t send_buf[2];

    l6470_lock(device);
    
    send_buf[TILT] = dSPIN_NOP;

    /* Send SetParam operation code to dSPIN */
    send_buf[PAN]  = dSPIN_SET_PARAM | data->param; 
    rt_spi_send(device->spi_dev, send_buf, 2);

    switch (data->param)
    {
        case dSPIN_ABS_POS: ;
        case dSPIN_MARK: ;
        case dSPIN_SPEED:
            /* Send parameter - byte 2 to dSPIN */
            send_buf[PAN]  = (uint8_t)(data->pan_value >> 16); 
            rt_spi_send(device->spi_dev, send_buf, 2);
        case dSPIN_ACC: ;
        case dSPIN_DEC: ;
        case dSPIN_MAX_SPEED: ;
        case dSPIN_MIN_SPEED: ;
        case dSPIN_FS_SPD: ;
        case dSPIN_INT_SPD: ;
        case dSPIN_CONFIG: ;
        case dSPIN_STATUS:
            /* Send parameter - byte 1 to dSPIN */
            send_buf[PAN]  = (uint8_t)(data->pan_value >> 8); 
            rt_spi_send(device->spi_dev, send_buf, 2);
        default:
            /* Send parameter - byte 0 to dSPIN */
            send_buf[PAN]  = (uint8_t)(data->pan_value); 
            rt_spi_send(device->spi_dev, send_buf, 2);
    }

    l6470_unlock(device);
}

static void tilt_set_param(struct spi_l6470_device *device, struct param_data *data)
{
    rt_uint8_t send_buf[2];

    l6470_lock(device);
    
    send_buf[PAN]  = dSPIN_NOP; 

    /* Send SetParam operation code to dSPIN */
    send_buf[TILT] = dSPIN_SET_PARAM | data->param;
    rt_spi_send(device->spi_dev, send_buf, 2);

    switch (data->param)
    {
        case dSPIN_ABS_POS: ;
        case dSPIN_MARK: ;
        case dSPIN_SPEED:
            /* Send parameter - byte 2 to dSPIN */
            send_buf[TILT] = (uint8_t)(data->tilt_value >> 16);
            rt_spi_send(device->spi_dev, send_buf, 2);
        case dSPIN_ACC: ;
        case dSPIN_DEC: ;
        case dSPIN_MAX_SPEED: ;
        case dSPIN_MIN_SPEED: ;
        case dSPIN_FS_SPD: ;
        case dSPIN_INT_SPD: ;
        case dSPIN_CONFIG: ;
        case dSPIN_STATUS:
            /* Send parameter - byte 1 to dSPIN */
            send_buf[TILT] = (uint8_t)(data->tilt_value >> 8);
            rt_spi_send(device->spi_dev, send_buf, 2);
        default:
            /* Send parameter - byte 0 to dSPIN */
            send_buf[TILT] = (uint8_t)(data->tilt_value);
            rt_spi_send(device->spi_dev, send_buf, 2);
    }

    l6470_unlock(device);
}

static void get_param(struct spi_l6470_device *device, struct param_data *data)
{
    uint32_t temp = 0;
    rt_uint8_t send_buf[2];
    rt_uint8_t recv_buf[2];

    data->pan_value  = 0;
    data->tilt_value = 0;
    
    l6470_lock(device);
    
    /* Send GetParam operation code to dSPIN */
    send_buf[PAN]  = dSPIN_GET_PARAM | data->param;
    send_buf[TILT] = dSPIN_GET_PARAM | data->param;
    rt_spi_transfer(device->spi_dev, send_buf, recv_buf, 2);

    temp = recv_buf[PAN];
    /* MSB which should be 0 */
    temp = temp << 24;
    data->pan_value |= temp;

    temp = recv_buf[TILT];
    /* MSB which should be 0 */
    temp = temp << 24;
    data->tilt_value |= temp;

    switch (data->param)
    {
        case dSPIN_ABS_POS: ;
        case dSPIN_MARK: ;
        case dSPIN_SPEED:
            send_buf[PAN]  = (uint8_t)(0x00);
            send_buf[TILT] = (uint8_t)(0x00);
            rt_spi_transfer(device->spi_dev, send_buf, recv_buf, 2);

            temp = recv_buf[PAN];
            temp = temp << 16;
            data->pan_value |= temp;

            temp = recv_buf[TILT];
            temp = temp << 16;
            data->tilt_value |= temp;
        case dSPIN_ACC: ;
        case dSPIN_DEC: ;
        case dSPIN_MAX_SPEED: ;
        case dSPIN_MIN_SPEED: ;
        case dSPIN_FS_SPD: ;
        case dSPIN_INT_SPD: ;
        case dSPIN_CONFIG: ;
        case dSPIN_STATUS:
            send_buf[PAN] = (uint8_t)(0x00);
            send_buf[TILT] = (uint8_t)(0x00);
            rt_spi_transfer(device->spi_dev, send_buf, recv_buf, 2);

            temp = recv_buf[PAN];
            temp = temp << 8;
            data->pan_value |= temp;

            temp = recv_buf[TILT];
            temp = temp << 8;
            data->tilt_value |= temp;
        default:
            send_buf[PAN] = (uint8_t)(0x00);
            send_buf[TILT] = (uint8_t)(0x00);
            rt_spi_transfer(device->spi_dev, send_buf, recv_buf, 2);
            
            temp = recv_buf[PAN];
            data->pan_value |= temp;

            temp = recv_buf[TILT];
            data->tilt_value |= temp;
    }

    l6470_lock(device);

    rt_kprintf("PARAM(%d) of PAN  is 0x%x\r\n", data->param, data->pan_value);
    rt_kprintf("PARAM(%d) of TILT is 0x%x\r\n", data->param, data->tilt_value);
}

static void pan_get_param(struct spi_l6470_device *device, struct param_data *data)
{
    uint32_t temp = 0;
    rt_uint8_t send_buf[2];
    rt_uint8_t recv_buf[2];

    data->pan_value = 0;
    
    l6470_lock(device);
    
    send_buf[TILT] = dSPIN_NOP; 

    /* Send GetParam operation code to dSPIN */
    send_buf[PAN] = dSPIN_GET_PARAM | data->param;
    rt_spi_transfer(device->spi_dev, send_buf, recv_buf, 2);
    temp = recv_buf[PAN];
    /* MSB which should be 0 */
    temp = temp << 24;
    data->pan_value |= temp;
    switch (data->param)
    {
        case dSPIN_ABS_POS: ;
        case dSPIN_MARK: ;
        case dSPIN_SPEED:
            send_buf[PAN] = (uint8_t)(0x00);
            rt_spi_transfer(device->spi_dev, send_buf, recv_buf, 2);
            temp = recv_buf[PAN];
            temp = temp << 16;
            data->pan_value |= temp;
        case dSPIN_ACC: ;
        case dSPIN_DEC: ;
        case dSPIN_MAX_SPEED: ;
        case dSPIN_MIN_SPEED: ;
        case dSPIN_FS_SPD: ;
        case dSPIN_INT_SPD: ;
        case dSPIN_CONFIG: ;
        case dSPIN_STATUS:
            send_buf[PAN] = (uint8_t)(0x00);
            rt_spi_transfer(device->spi_dev, send_buf, recv_buf, 2);
            temp = recv_buf[PAN];
            temp = temp << 8;
            data->pan_value |= temp;
        default:
            send_buf[PAN] = (uint8_t)(0x00);
            rt_spi_transfer(device->spi_dev, send_buf, recv_buf, 2);
            temp = recv_buf[PAN];
            data->pan_value |= temp;
    }

    l6470_lock(device);

    rt_kprintf("PARAM(%d) of PAN is 0x%x\r\n", data->param, data->pan_value);
}

static void tilt_get_param(struct spi_l6470_device *device, struct param_data *data)
{
    uint32_t temp = 0;
    rt_uint8_t send_buf[2];
    rt_uint8_t recv_buf[2];

    data->tilt_value = 0;
    
    l6470_lock(device);
    
    send_buf[PAN] = dSPIN_NOP; 

    /* Send GetParam operation code to dSPIN */
    send_buf[TILT] = dSPIN_GET_PARAM | data->param;
    rt_spi_transfer(device->spi_dev, send_buf, recv_buf, 2);
    temp = recv_buf[TILT];
    /* MSB which should be 0 */
    temp = temp << 24;
    data->tilt_value |= temp;
    switch (data->param)
    {
        case dSPIN_ABS_POS: ;
        case dSPIN_MARK: ;
        case dSPIN_SPEED:
            send_buf[TILT] = (uint8_t)(0x00);
            rt_spi_transfer(device->spi_dev, send_buf, recv_buf, 2);
            temp = recv_buf[TILT];
            temp = temp << 16;
            data->tilt_value |= temp;
        case dSPIN_ACC: ;
        case dSPIN_DEC: ;
        case dSPIN_MAX_SPEED: ;
        case dSPIN_MIN_SPEED: ;
        case dSPIN_FS_SPD: ;
        case dSPIN_INT_SPD: ;
        case dSPIN_CONFIG: ;
        case dSPIN_STATUS:
            send_buf[TILT] = (uint8_t)(0x00);
            rt_spi_transfer(device->spi_dev, send_buf, recv_buf, 2);
            temp = recv_buf[TILT];
            temp = temp << 8;
            data->tilt_value |= temp;
        default:
            send_buf[TILT] = (uint8_t)(0x00);
            rt_spi_transfer(device->spi_dev, send_buf, recv_buf, 2);
            temp = recv_buf[TILT];
            data->tilt_value |= temp;
    }

    l6470_lock(device);

    rt_kprintf("PARAM(%d) of TILT is 0x%x\r\n", data->param, data->tilt_value);
}


static void move(struct spi_l6470_device *device, struct move_data *data)
{
    rt_uint8_t send_buf[2];

    l6470_lock(device);
    
    /* Send Move operation code to dSPIN */
    send_buf[TILT] = dSPIN_MOVE | data->tilt_dir;
    send_buf[PAN]  = dSPIN_MOVE | data->pan_dir; 
    rt_spi_send(device->spi_dev, send_buf, 2);

    /* Send n_step - byte 2 data dSPIN */
    send_buf[TILT] = (uint8_t)(data->tilt_steps >> 16);
    send_buf[PAN]  = (uint8_t)(data->pan_steps >> 16); 
    rt_spi_send(device->spi_dev, send_buf, 2);

    /* Send n_step - byte 1 data dSPIN */
    send_buf[TILT] = (uint8_t)(data->tilt_steps >> 8);
    send_buf[PAN]  = (uint8_t)(data->pan_steps >> 8); 
    rt_spi_send(device->spi_dev, send_buf, 2);

    /* Send n_step - byte 0 data dSPIN */
    send_buf[TILT] = (uint8_t)(data->tilt_steps);
    send_buf[PAN]  = (uint8_t)(data->pan_steps); 
    rt_spi_send(device->spi_dev, send_buf, 2);
        
    l6470_unlock(device);
}

static void pan_move(struct spi_l6470_device *device, struct move_data *data)
{
    rt_uint8_t send_buf[2];

    l6470_lock(device);
    
    send_buf[TILT] = dSPIN_NOP;

    /* Send Move operation code to dSPIN */
    send_buf[PAN]  = dSPIN_MOVE | data->pan_dir; 
    rt_spi_send(device->spi_dev, send_buf, 2);

    /* Send n_step - byte 2 data dSPIN */
    send_buf[PAN]  = (uint8_t)(data->pan_steps >> 16); 
    rt_spi_send(device->spi_dev, send_buf, 2);

    /* Send n_step - byte 1 data dSPIN */
    send_buf[PAN]  = (uint8_t)(data->pan_steps >> 8); 
    rt_spi_send(device->spi_dev, send_buf, 2);

    /* Send n_step - byte 0 data dSPIN */
    send_buf[PAN]  = (uint8_t)(data->pan_steps); 
    rt_spi_send(device->spi_dev, send_buf, 2);
        
    l6470_unlock(device);
}

static void tilt_move(struct spi_l6470_device *device, struct move_data *data)
{
    rt_uint8_t send_buf[2];

    l6470_lock(device);
    
    send_buf[PAN]  = dSPIN_NOP; 

    /* Send Move operation code to dSPIN */
    send_buf[TILT] = dSPIN_MOVE | data->tilt_dir;
    rt_spi_send(device->spi_dev, send_buf, 2);

    /* Send n_step - byte 2 data dSPIN */
    send_buf[TILT] = (uint8_t)(data->tilt_steps >> 16);
    rt_spi_send(device->spi_dev, send_buf, 2);

    /* Send n_step - byte 1 data dSPIN */
    send_buf[TILT] = (uint8_t)(data->tilt_steps >> 8);
    rt_spi_send(device->spi_dev, send_buf, 2);

    /* Send n_step - byte 0 data dSPIN */
    send_buf[TILT] = (uint8_t)(data->tilt_steps);
    rt_spi_send(device->spi_dev, send_buf, 2);
        
    l6470_unlock(device);
}

static void get_status(struct spi_l6470_device *dev, struct motor_status *status)
{
    rt_uint8_t send_buf[2];
    rt_uint8_t recv_buf[2];

    l6470_lock(dev);
    
    /* Send GetParam operation code to dSPIN */
    send_buf[PAN]  = dSPIN_GET_STATUS;
    send_buf[TILT] = dSPIN_GET_STATUS;
    rt_spi_transfer(dev->spi_dev, send_buf, recv_buf, 2);

    send_buf[PAN]  = (uint8_t)(0x00);
    send_buf[TILT] = (uint8_t)(0x00);
    rt_spi_transfer(dev->spi_dev, send_buf, recv_buf, 2);

    status->pan_status.value  = recv_buf[PAN];
    status->tilt_status.value = recv_buf[TILT];
    
    send_buf[PAN]  = (uint8_t)(0x00);
    send_buf[TILT] = (uint8_t)(0x00);
    rt_spi_transfer(dev->spi_dev, send_buf, recv_buf, 2);

    status->pan_status.value  = (status->pan_status.value << 8) | recv_buf[PAN];
    status->tilt_status.value = (status->tilt_status.value << 8) | recv_buf[TILT];

    rt_kprintf("pan_status  = 0x%x\r\n", status->pan_status.value);
    rt_kprintf("tilt_status = 0x%x\r\n", status->tilt_status.value);

    l6470_lock(dev);
}

static rt_err_t l6470_control(rt_device_t dev, rt_uint8_t cmd, void *args)
{
    struct spi_l6470_device *device = (struct spi_l6470_device *)dev;

    RT_ASSERT(dev != RT_NULL);

    switch (cmd)
    {
    case SET_PARAM:
        set_param(device, (struct param_data *)args);
        break;
    case PAN_SET_PARAM:
        pan_set_param(device, (struct param_data *)args);
        break;
    case TILT_SET_PARAM:
        tilt_set_param(device, (struct param_data *)args);
        break;      
    case GET_PARAM:
        get_param(device, (struct param_data *)args);
        break;
    case PAN_GET_PARAM:
        pan_get_param(device, (struct param_data *)args);
        break;
    case TILT_GET_PARAM:
        tilt_get_param(device, (struct param_data *)args);
        break;      
    case RUN:
        break;
    case MOVE:
        move(device, (struct move_data *)args);
        break;
    case PAN_MOVE:
        pan_move(device, (struct move_data *)args);
        break;
    case TILT_MOVE:
        tilt_move(device, (struct move_data *)args);
        break;
    case GET_STATUS:
        get_status(device, (struct motor_status *)args);
        break;
    default:
        break;
    }

    return RT_EOK;
}

static rt_size_t l6470_read(rt_device_t dev,
                            rt_off_t    pos,
                            void       *buffer,
                            rt_size_t   size)
{
    return size;
}

static rt_size_t l6470_write(rt_device_t dev,
                             rt_off_t    pos,
                             const void *buffer,
                             rt_size_t   size)
{
    return size;
}

rt_err_t rt_hw_l6470_init(const char *l6470_device_name, const char *spi_device_name)
{
    struct rt_spi_device *spi_dev;

    /* initialize mutex */
    if (rt_mutex_init(&l6470.lock, spi_device_name, RT_IPC_FLAG_FIFO) != RT_EOK)
    {
        rt_kprintf("init L6470 lock mutex failed\n");

        return -RT_ENOSYS;
    }

    spi_dev = (struct rt_spi_device*)rt_device_find(spi_device_name);
    if (spi_dev == RT_NULL)
    {
        rt_kprintf("spi device %s not found!\r\n", spi_device_name);

        return -RT_ENOSYS;
    }
    l6470.spi_dev = spi_dev;

    /* config spi */
    {
        struct rt_spi_configuration cfg;
        cfg.data_width = 8;
        cfg.mode = RT_SPI_MODE_0 | RT_SPI_MSB; /* SPI Compatible: Mode 0 and Mode 3 */
        cfg.max_hz = 3 * 1000 * 1000; /* 3M */
        rt_spi_configure(l6470.spi_dev, &cfg);
    }

    /* register device */
    l6470.device.init    = l6470_init;
    l6470.device.open    = l6470_open;
    l6470.device.close   = l6470_close;
    l6470.device.read    = l6470_read;
    l6470.device.write   = l6470_write;
    l6470.device.control = l6470_control;
    /* no private */
    l6470.device.user_data = RT_NULL;

    rt_device_register(&l6470.device,
                       l6470_device_name,
                       RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_STANDALONE);

    return RT_EOK;
}

#ifdef RT_USING_FINSH
#include <finsh.h>

static rt_err_t move_cmd(dSPIN_Direction_TypeDef pan_dir,
                         rt_uint32_t             pan_steps,
                         dSPIN_Direction_TypeDef tilt_dir,
                         rt_uint32_t             tilt_steps)
{
    rt_device_t device;
    struct move_data data;

    data.pan_dir    = pan_dir;
    data.pan_steps  = pan_steps;
    data.tilt_dir   = tilt_dir;
    data.tilt_steps = tilt_steps;

    device = rt_device_find("l6470");
    if (device == RT_NULL)
    {
        return -RT_ERROR;
    }

    return rt_device_control(device, MOVE, &data);  
}
FINSH_FUNCTION_EXPORT(move_cmd, move the pan and tilt motor)

static rt_err_t pan_move_cmd(dSPIN_Direction_TypeDef pan_dir,
                             rt_uint32_t             pan_steps)
{
    rt_device_t device;
    struct move_data data;

    data.pan_dir    = pan_dir;
    data.pan_steps  = pan_steps;

    device = rt_device_find("l6470");
    if (device == RT_NULL)
    {
        return -RT_ERROR;
    }

    return rt_device_control(device, PAN_MOVE, &data);  
}
FINSH_FUNCTION_EXPORT(pan_move_cmd, move the pan motor)

static rt_err_t tilt_move_cmd(dSPIN_Direction_TypeDef tilt_dir,
                              rt_uint32_t             tilt_steps)
{
    rt_device_t device;
    struct move_data data;

    data.tilt_dir    = tilt_dir;
    data.tilt_steps  = tilt_steps;

    device = rt_device_find("l6470");
    if (device == RT_NULL)
    {
        return -RT_ERROR;
    }

    return rt_device_control(device, TILT_MOVE, &data); 
}
FINSH_FUNCTION_EXPORT(tilt_move_cmd, move the tilt motor)

static rt_err_t pan_get_params(dSPIN_Registers_TypeDef param)
{
    rt_device_t dev;
    struct param_data data;

    data.param      = param;
    data.pan_value  = 0;
    data.tilt_value = 0;
    
    dev = rt_device_find("l6470");
    if (dev == RT_NULL)
    {
        return -RT_ERROR;
    }

    return rt_device_control(dev, PAN_GET_PARAM, &data);    
}
FINSH_FUNCTION_EXPORT(pan_get_params, get parameters of pan)

static rt_err_t get_params(dSPIN_Registers_TypeDef param)
{
    rt_device_t dev;
    struct param_data data;

    data.param      = param;
    data.pan_value  = 0;
    data.tilt_value = 0;
    
    dev = rt_device_find("l6470");
    if (dev == RT_NULL)
    {
        return -RT_ERROR;
    }

    return rt_device_control(dev, GET_PARAM, &data);    
}
FINSH_FUNCTION_EXPORT(get_params, get parameters of pan and tilt)

static rt_err_t get_status_cmd(void)
{
    rt_device_t dev;
    struct motor_status status;

    dev = rt_device_find("l6470");
    if (dev == RT_NULL)
    {
        return -RT_ERROR;
    }

    return rt_device_control(dev, GET_STATUS, &status); 
}
FINSH_FUNCTION_EXPORT(get_status_cmd, get status of pan and tilt)

#endif

