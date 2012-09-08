/**
  ******************************************************************************
  * @file    dspin.c 
  * @author  IMMCC Prague FA - MH
  * @modify  lgnq
  * @version V1.0.1
  * @date    3-Sep-2012
  * @brief   dSPIN (L6470) product related routines
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  */ 

#include <rtthread.h>

#include "dspin.h"
#include "stm32f10x_spi.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStructure;
SPI_InitTypeDef SPI_InitStructure;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes uC peripherals, GPIOs, clocks used by dSPIN.
  * @param  None
  * @retval None
  */
void dSPIN_Peripherals_Init(void)
{
    /* Used peripherals clock enable */
    RCC_APB1PeriphClockCmd(dSPIN_PERIPHERAL_CLKs_APB1, ENABLE);
    RCC_APB2PeriphClockCmd(dSPIN_PERIPHERAL_CLKs_APB2, ENABLE);

    /* Configure pins used by dSPIN */

    /* Configure SPI pin: SCK */
    GPIO_InitStructure.GPIO_Pin = dSPIN_SCK_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(dSPIN_SCK_Port, &GPIO_InitStructure);

    /* Configure SPI pin: MOSI */
    GPIO_InitStructure.GPIO_Pin = dSPIN_MOSI_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(dSPIN_MOSI_Port, &GPIO_InitStructure);

    /* Configure SPI pin: nSS */
    GPIO_InitStructure.GPIO_Pin = dSPIN_nSS_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(dSPIN_nSS_Port, &GPIO_InitStructure);

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

    /* SPI configuration */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(dSPIN_SPI, &SPI_InitStructure);

    /* Enable SPI */
    SPI_Cmd(dSPIN_SPI, ENABLE);
}

/**
  * @brief  Fills-in dSPIN configuration structure with default values.
  * @param  Structure address (pointer to struct)
  * @retval None
  */
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

/**
  * @brief  Configures dSPIN internal registers with values in the config structure.
  * @param  Configuration structure address (pointer to configuration structure)
  * @retval None
  */
void dSPIN_Registers_Set(dSPIN_RegsStruct_TypeDef *dSPIN_RegsStruct)
{
    dSPIN_Set_Param(dSPIN_ABS_POS, dSPIN_RegsStruct->ABS_POS);
    dSPIN_Set_Param(dSPIN_EL_POS, dSPIN_RegsStruct->EL_POS);
    dSPIN_Set_Param(dSPIN_MARK, dSPIN_RegsStruct->MARK);
    dSPIN_Set_Param(dSPIN_SPEED, dSPIN_RegsStruct->SPEED);
    dSPIN_Set_Param(dSPIN_ACC, dSPIN_RegsStruct->ACC);
    dSPIN_Set_Param(dSPIN_DEC, dSPIN_RegsStruct->DEC);
    dSPIN_Set_Param(dSPIN_MAX_SPEED, dSPIN_RegsStruct->MAX_SPEED);
    dSPIN_Set_Param(dSPIN_MIN_SPEED, dSPIN_RegsStruct->MIN_SPEED);
    dSPIN_Set_Param(dSPIN_FS_SPD, dSPIN_RegsStruct->FS_SPD);
    dSPIN_Set_Param(dSPIN_KVAL_HOLD, dSPIN_RegsStruct->KVAL_HOLD);
    dSPIN_Set_Param(dSPIN_KVAL_RUN, dSPIN_RegsStruct->KVAL_RUN);
    dSPIN_Set_Param(dSPIN_KVAL_ACC, dSPIN_RegsStruct->KVAL_ACC);
    dSPIN_Set_Param(dSPIN_KVAL_DEC, dSPIN_RegsStruct->KVAL_DEC);
    dSPIN_Set_Param(dSPIN_INT_SPD, dSPIN_RegsStruct->INT_SPD);
    dSPIN_Set_Param(dSPIN_ST_SLP, dSPIN_RegsStruct->ST_SLP);
    dSPIN_Set_Param(dSPIN_FN_SLP_ACC, dSPIN_RegsStruct->FN_SLP_ACC);
    dSPIN_Set_Param(dSPIN_FN_SLP_DEC, dSPIN_RegsStruct->FN_SLP_DEC);
    dSPIN_Set_Param(dSPIN_K_THERM, dSPIN_RegsStruct->K_THERM);
    dSPIN_Set_Param(dSPIN_OCD_TH, dSPIN_RegsStruct->OCD_TH);
    dSPIN_Set_Param(dSPIN_STALL_TH, dSPIN_RegsStruct->STALL_TH);
    dSPIN_Set_Param(dSPIN_STEP_MODE, dSPIN_RegsStruct->STEP_MODE);
    dSPIN_Set_Param(dSPIN_ALARM_EN, dSPIN_RegsStruct->ALARM_EN);
    dSPIN_Set_Param(dSPIN_CONFIG, dSPIN_RegsStruct->CONFIG);
}

void registers_set(dSPIN_RegsStruct_TypeDef *pan_regs, dSPIN_RegsStruct_TypeDef *tilt_regs)
{
    set_param(dSPIN_ABS_POS, pan_regs->ABS_POS, tilt_regs->ABS_POS);
    set_param(dSPIN_EL_POS, pan_regs->EL_POS, tilt_regs->EL_POS);
    set_param(dSPIN_MARK, pan_regs->MARK, tilt_regs->MARK);
    set_param(dSPIN_SPEED, pan_regs->SPEED, tilt_regs->SPEED);
    set_param(dSPIN_ACC, pan_regs->ACC, tilt_regs->ACC);
    set_param(dSPIN_DEC, pan_regs->DEC, tilt_regs->DEC);
    set_param(dSPIN_MAX_SPEED, pan_regs->MAX_SPEED, tilt_regs->MAX_SPEED);
    set_param(dSPIN_MIN_SPEED, pan_regs->MIN_SPEED, tilt_regs->MIN_SPEED);
    set_param(dSPIN_FS_SPD, pan_regs->FS_SPD, tilt_regs->FS_SPD);
    set_param(dSPIN_KVAL_HOLD, pan_regs->KVAL_HOLD, tilt_regs->KVAL_HOLD);
    set_param(dSPIN_KVAL_RUN, pan_regs->KVAL_RUN, tilt_regs->KVAL_RUN);
    set_param(dSPIN_KVAL_ACC, pan_regs->KVAL_ACC, tilt_regs->KVAL_ACC);
    set_param(dSPIN_KVAL_DEC, pan_regs->KVAL_DEC, tilt_regs->KVAL_DEC);
    set_param(dSPIN_INT_SPD, pan_regs->INT_SPD, tilt_regs->INT_SPD);
    set_param(dSPIN_ST_SLP, pan_regs->ST_SLP, tilt_regs->ST_SLP);
    set_param(dSPIN_FN_SLP_ACC, pan_regs->FN_SLP_ACC, tilt_regs->FN_SLP_ACC);
    set_param(dSPIN_FN_SLP_DEC, pan_regs->FN_SLP_DEC, tilt_regs->FN_SLP_DEC);
    set_param(dSPIN_K_THERM, pan_regs->K_THERM, tilt_regs->K_THERM);
    set_param(dSPIN_OCD_TH, pan_regs->OCD_TH, tilt_regs->OCD_TH);
    set_param(dSPIN_STALL_TH, pan_regs->STALL_TH, tilt_regs->STALL_TH);
    set_param(dSPIN_STEP_MODE, pan_regs->STEP_MODE, tilt_regs->STEP_MODE);
    set_param(dSPIN_ALARM_EN, pan_regs->ALARM_EN, tilt_regs->ALARM_EN);
    set_param(dSPIN_CONFIG, pan_regs->CONFIG, tilt_regs->CONFIG);
}

/**
  * @brief  Issues dSPIN NOP command.
  * @param  None
  * @retval None
  */
void dSPIN_Nop(void)
{
    /* Send NOP operation code to dSPIN */
    dSPIN_Write_Byte(dSPIN_NOP);
}

/**
  * @brief  Issues dSPIN Set Param command.
  * @param  dSPIN register address, value to be set
  * @retval None
  */
void dSPIN_Set_Param(dSPIN_Registers_TypeDef param, uint32_t value)
{
    /* Send SetParam operation code to dSPIN */
    dSPIN_Write_Byte(dSPIN_SET_PARAM | param);
    switch (param)
    {
        case dSPIN_ABS_POS: ;
        case dSPIN_MARK: ;
        case dSPIN_SPEED:
            /* Send parameter - byte 2 to dSPIN */
            dSPIN_Write_Byte((uint8_t)(value >> 16));
        case dSPIN_ACC: ;
        case dSPIN_DEC: ;
        case dSPIN_MAX_SPEED: ;
        case dSPIN_MIN_SPEED: ;
        case dSPIN_FS_SPD: ;
        case dSPIN_INT_SPD: ;
        case dSPIN_CONFIG: ;
        case dSPIN_STATUS:
            /* Send parameter - byte 1 to dSPIN */
            dSPIN_Write_Byte((uint8_t)(value >> 8));
        default:
            /* Send parameter - byte 0 to dSPIN */
            dSPIN_Write_Byte((uint8_t)(value));
    }
}

void set_param(dSPIN_Registers_TypeDef param,
               uint32_t                pan_value,
               uint32_t                tilt_value)
{
    /* Send SetParam operation code to dSPIN */
    write_byte(dSPIN_SET_PARAM | param, dSPIN_SET_PARAM | param);
    switch (param)
    {
        case dSPIN_ABS_POS: ;
        case dSPIN_MARK: ;
        case dSPIN_SPEED:
            /* Send parameter - byte 2 to dSPIN */
            write_byte((uint8_t)(pan_value >> 16), (uint8_t)(tilt_value >> 16));
        case dSPIN_ACC: ;
        case dSPIN_DEC: ;
        case dSPIN_MAX_SPEED: ;
        case dSPIN_MIN_SPEED: ;
        case dSPIN_FS_SPD: ;
        case dSPIN_INT_SPD: ;
        case dSPIN_CONFIG: ;
        case dSPIN_STATUS:
            /* Send parameter - byte 1 to dSPIN */
            write_byte((uint8_t)(pan_value >> 8), (uint8_t)(tilt_value >> 8));
        default:
            /* Send parameter - byte 0 to dSPIN */
            write_byte((uint8_t)(pan_value), (uint8_t)(tilt_value));
    }
}

void pan_set_param(dSPIN_Registers_TypeDef param, uint32_t pan_value)
{
    /* Send SetParam operation code to dSPIN */
    write_byte(dSPIN_SET_PARAM | param, dSPIN_NOP);
    switch (param)
    {
        case dSPIN_ABS_POS: ;
        case dSPIN_MARK: ;
        case dSPIN_SPEED:
            /* Send parameter - byte 2 to dSPIN */
            write_byte((uint8_t)(pan_value >> 16), dSPIN_NOP);
        case dSPIN_ACC: ;
        case dSPIN_DEC: ;
        case dSPIN_MAX_SPEED: ;
        case dSPIN_MIN_SPEED: ;
        case dSPIN_FS_SPD: ;
        case dSPIN_INT_SPD: ;
        case dSPIN_CONFIG: ;
        case dSPIN_STATUS:
            /* Send parameter - byte 1 to dSPIN */
            write_byte((uint8_t)(pan_value >> 8), dSPIN_NOP);
        default:
            /* Send parameter - byte 0 to dSPIN */
            write_byte((uint8_t)(pan_value), dSPIN_NOP);
    }
}

void tilt_set_param(dSPIN_Registers_TypeDef param, uint32_t tilt_value)
{
    /* Send SetParam operation code to dSPIN */
    write_byte(dSPIN_NOP, dSPIN_SET_PARAM | param);
    switch (param)
    {
        case dSPIN_ABS_POS: ;
        case dSPIN_MARK: ;
        case dSPIN_SPEED:
            /* Send parameter - byte 2 to dSPIN */
            write_byte(dSPIN_NOP, (uint8_t)(tilt_value >> 16));
        case dSPIN_ACC: ;
        case dSPIN_DEC: ;
        case dSPIN_MAX_SPEED: ;
        case dSPIN_MIN_SPEED: ;
        case dSPIN_FS_SPD: ;
        case dSPIN_INT_SPD: ;
        case dSPIN_CONFIG: ;
        case dSPIN_STATUS:
            /* Send parameter - byte 1 to dSPIN */
            write_byte(dSPIN_NOP, (uint8_t)(tilt_value >> 8));
        default:
            /* Send parameter - byte 0 to dSPIN */
            write_byte(dSPIN_NOP, (uint8_t)(tilt_value));
    }
}

/**
  * @brief  Issues dSPIN Get Param command.
  * @param  dSPIN register address
  * @retval Register value - 1 to 3 bytes (depends on register)
  */
uint32_t dSPIN_Get_Param(dSPIN_Registers_TypeDef param)
{
    uint32_t temp = 0;
    uint32_t rx = 0;

    /* Send GetParam operation code to dSPIN */
    temp = dSPIN_Write_Byte(dSPIN_GET_PARAM | param);
    /* MSB which should be 0 */
    temp = temp << 24;
    rx |= temp;
    switch (param)
    {
        case dSPIN_ABS_POS: ;
        case dSPIN_MARK: ;
        case dSPIN_SPEED:
            temp = dSPIN_Write_Byte((uint8_t)(0x00));
            temp = temp << 16;
            rx |= temp;
        case dSPIN_ACC: ;
        case dSPIN_DEC: ;
        case dSPIN_MAX_SPEED: ;
        case dSPIN_MIN_SPEED: ;
        case dSPIN_FS_SPD: ;
        case dSPIN_INT_SPD: ;
        case dSPIN_CONFIG: ;
        case dSPIN_STATUS:
            temp = dSPIN_Write_Byte((uint8_t)(0x00));
            temp = temp << 8;
            rx |= temp;
        default:
            temp = dSPIN_Write_Byte((uint8_t)(0x00));
            rx |= temp;
    }
    return rx;
}

uint32_t pan_get_param(dSPIN_Registers_TypeDef param)
{
    uint32_t temp = 0;
    uint32_t rx = 0;

    /* Send GetParam operation code to dSPIN */
    temp = write_byte(dSPIN_GET_PARAM | param, dSPIN_NOP);
    /* MSB which should be 0 */
    temp = temp << 24;
    rx |= temp;
    switch (param)
    {
        case dSPIN_ABS_POS: ;
        case dSPIN_MARK: ;
        case dSPIN_SPEED:
            temp = write_byte((uint8_t)(0x00), dSPIN_NOP) & 0xff;
            temp = temp << 16;
            rx |= temp;
        case dSPIN_ACC: ;
        case dSPIN_DEC: ;
        case dSPIN_MAX_SPEED: ;
        case dSPIN_MIN_SPEED: ;
        case dSPIN_FS_SPD: ;
        case dSPIN_INT_SPD: ;
        case dSPIN_CONFIG: ;
        case dSPIN_STATUS:
            temp = write_byte((uint8_t)(0x00), dSPIN_NOP) & 0xff;
            temp = temp << 8;
            rx |= temp;
        default:
            temp = write_byte((uint8_t)(0x00), dSPIN_NOP) & 0xff;
            rx |= temp;
    }

    rt_kprintf("PARAM(%d) of PAN is 0x%x\r\n", param, rx);

    return rx;
}

uint32_t tilt_get_param(dSPIN_Registers_TypeDef param)
{
    uint32_t temp = 0;
    uint32_t rx = 0;

    /* Send GetParam operation code to dSPIN */
    temp = write_byte(dSPIN_NOP, dSPIN_GET_PARAM | param);
    /* MSB which should be 0 */
    temp = temp << 24;
    rx |= temp;
    switch (param)
    {
        case dSPIN_ABS_POS: ;
        case dSPIN_MARK: ;
        case dSPIN_SPEED:
            temp = (write_byte(dSPIN_NOP, (uint8_t)(0x00)) & 0xff00) >> 8;
            temp = temp << 16;
            rx |= temp;
        case dSPIN_ACC: ;
        case dSPIN_DEC: ;
        case dSPIN_MAX_SPEED: ;
        case dSPIN_MIN_SPEED: ;
        case dSPIN_FS_SPD: ;
        case dSPIN_INT_SPD: ;
        case dSPIN_CONFIG: ;
        case dSPIN_STATUS:
            temp = (write_byte(dSPIN_NOP, (uint8_t)(0x00)) & 0xff00) >> 8;
            temp = temp << 8;
            rx |= temp;
        default:
            temp = (write_byte(dSPIN_NOP, (uint8_t)(0x00)) & 0xff00) >> 8;
            rx |= temp;
    }

    rt_kprintf("PARAM(%d) of TILT is 0x%x\r\n", param, rx);

    return rx;
}


/**
  * @brief  Issues dSPIN Run command.
  * @param  Movement direction (FWD, REV), Speed - 3 bytes
  * @retval None
  */
void dSPIN_Run(dSPIN_Direction_TypeDef direction, uint32_t speed)
{
    /* Send RUN operation code to dSPIN */
    dSPIN_Write_Byte(dSPIN_RUN | direction);
    /* Send speed - byte 2 data dSPIN */
    dSPIN_Write_Byte((uint8_t)(speed >> 16));
    /* Send speed - byte 1 data dSPIN */
    dSPIN_Write_Byte((uint8_t)(speed >> 8));
    /* Send speed - byte 0 data dSPIN */
    dSPIN_Write_Byte((uint8_t)(speed));
}

void run(dSPIN_Direction_TypeDef pan_dir, 
         uint32_t                pan_speed,
         dSPIN_Direction_TypeDef tilt_dir,
         uint32_t                tilt_speed)
{
    /* Send RUN operation code to dSPIN */
    write_byte(dSPIN_RUN | pan_dir, dSPIN_RUN | tilt_dir);
    /* Send speed - byte 2 data dSPIN */
    write_byte((uint8_t)(pan_speed >> 16), (uint8_t)(tilt_speed >> 16));
    /* Send speed - byte 1 data dSPIN */
    write_byte((uint8_t)(pan_speed >> 8), (uint8_t)(tilt_speed >> 8));
    /* Send speed - byte 0 data dSPIN */
    write_byte((uint8_t)(pan_speed), (uint8_t)(tilt_speed));
}

void pan_run(dSPIN_Direction_TypeDef pan_dir, uint32_t pan_speed)
{
    /* Send RUN operation code to dSPIN */
    write_byte(dSPIN_RUN | pan_dir, dSPIN_NOP);
    /* Send speed - byte 2 data dSPIN */
    write_byte((uint8_t)(pan_speed >> 16), dSPIN_NOP);
    /* Send speed - byte 1 data dSPIN */
    write_byte((uint8_t)(pan_speed >> 8), dSPIN_NOP);
    /* Send speed - byte 0 data dSPIN */
    write_byte((uint8_t)(pan_speed), dSPIN_NOP);
}

void tilt_run(dSPIN_Direction_TypeDef tilt_dir, uint32_t tilt_speed)
{
    /* Send RUN operation code to dSPIN */
    write_byte(dSPIN_NOP, dSPIN_RUN | tilt_dir);
    /* Send speed - byte 2 data dSPIN */
    write_byte(dSPIN_NOP, (uint8_t)(tilt_speed >> 16));
    /* Send speed - byte 1 data dSPIN */
    write_byte(dSPIN_NOP, (uint8_t)(tilt_speed >> 8));
    /* Send speed - byte 0 data dSPIN */
    write_byte(dSPIN_NOP, (uint8_t)(tilt_speed));
}

/**
  * @brief  Issues dSPIN Step Clock command.
  * @param  Movement direction (FWD, REV)
  * @retval None
  */
void dSPIN_Step_Clock(dSPIN_Direction_TypeDef direction)
{
    /* Send StepClock operation code to dSPIN */
    dSPIN_Write_Byte(dSPIN_STEP_CLOCK | direction);
}

/**
  * @brief  Issues dSPIN Move command.
  * @param  Movement direction, Number of steps
  * @retval None
  */
void dSPIN_Move(dSPIN_Direction_TypeDef direction, uint32_t n_step)
{
    /* Send Move operation code to dSPIN */
    dSPIN_Write_Byte(dSPIN_MOVE | direction);
    /* Send n_step - byte 2 data dSPIN */
    dSPIN_Write_Byte((uint8_t)(n_step >> 16));
    /* Send n_step - byte 1 data dSPIN */
    dSPIN_Write_Byte((uint8_t)(n_step >> 8));
    /* Send n_step - byte 0 data dSPIN */
    dSPIN_Write_Byte((uint8_t)(n_step));
}

void move(dSPIN_Direction_TypeDef pan_dir,
          uint32_t                pan_steps,
          dSPIN_Direction_TypeDef tilt_dir,
          uint32_t                tilt_steps)
{
    /* Send Move operation code to dSPIN */
    write_byte(dSPIN_MOVE | pan_dir, dSPIN_MOVE | tilt_dir);
    /* Send n_step - byte 2 data dSPIN */
    write_byte((uint8_t)(pan_steps >> 16), (uint8_t)(tilt_steps >> 16));
    /* Send n_step - byte 1 data dSPIN */
    write_byte((uint8_t)(pan_steps >> 8), (uint8_t)(tilt_steps >> 8));
    /* Send n_step - byte 0 data dSPIN */
    write_byte((uint8_t)(pan_steps), (uint8_t)(tilt_steps));
}

void pan_move(dSPIN_Direction_TypeDef pan_dir, uint32_t pan_steps)
{
    /* Send Move operation code to dSPIN */
    write_byte(dSPIN_MOVE | pan_dir, dSPIN_NOP);
    /* Send n_step - byte 2 data dSPIN */
    write_byte((uint8_t)(pan_steps >> 16), dSPIN_NOP);
    /* Send n_step - byte 1 data dSPIN */
    write_byte((uint8_t)(pan_steps >> 8), dSPIN_NOP);
    /* Send n_step - byte 0 data dSPIN */
    write_byte((uint8_t)(pan_steps), dSPIN_NOP);
}

void tilt_move(dSPIN_Direction_TypeDef tilt_dir, uint32_t tilt_steps)
{
    /* Send Move operation code to dSPIN */
    write_byte(dSPIN_NOP, dSPIN_MOVE | tilt_dir);
    /* Send n_step - byte 2 data dSPIN */
    write_byte(dSPIN_NOP, (uint8_t)(tilt_steps >> 16));
    /* Send n_step - byte 1 data dSPIN */
    write_byte(dSPIN_NOP, (uint8_t)(tilt_steps >> 8));
    /* Send n_step - byte 0 data dSPIN */
    write_byte(dSPIN_NOP, (uint8_t)(tilt_steps));
}

/**
  * @brief  Issues dSPIN Go To command.
  * @param  Absolute position where requested to move
  * @retval None
  */
void dSPIN_Go_To(uint32_t abs_pos)
{
    /* Send GoTo operation code to dSPIN */
    dSPIN_Write_Byte(dSPIN_GO_TO);
    /* Send absolute position parameter - byte 2 data to dSPIN */
    dSPIN_Write_Byte((uint8_t)(abs_pos >> 16));
    /* Send absolute position parameter - byte 1 data to dSPIN */
    dSPIN_Write_Byte((uint8_t)(abs_pos >> 8));
    /* Send absolute position parameter - byte 0 data to dSPIN */
    dSPIN_Write_Byte((uint8_t)(abs_pos));
}

void go_to(uint32_t pan_abs_pos, uint32_t tilt_abs_pos)
{
    /* Send GoTo operation code to dSPIN */
    write_byte(dSPIN_GO_TO, dSPIN_GO_TO);
    /* Send absolute position parameter - byte 2 data to dSPIN */
    write_byte((uint8_t)(pan_abs_pos >> 16), (uint8_t)(tilt_abs_pos >> 16));
    /* Send absolute position parameter - byte 1 data to dSPIN */
    write_byte((uint8_t)(pan_abs_pos >> 8), (uint8_t)(tilt_abs_pos >> 8));
    /* Send absolute position parameter - byte 0 data to dSPIN */
    write_byte((uint8_t)(pan_abs_pos), (uint8_t)(tilt_abs_pos));
}

void pan_go_to(uint32_t pan_abs_pos)
{
    /* Send GoTo operation code to dSPIN */
    write_byte(dSPIN_GO_TO, dSPIN_NOP);
    /* Send absolute position parameter - byte 2 data to dSPIN */
    write_byte((uint8_t)(pan_abs_pos >> 16), dSPIN_NOP);
    /* Send absolute position parameter - byte 1 data to dSPIN */
    write_byte((uint8_t)(pan_abs_pos >> 8), dSPIN_NOP);
    /* Send absolute position parameter - byte 0 data to dSPIN */
    write_byte((uint8_t)(pan_abs_pos), dSPIN_NOP);
}

void tilt_go_to(uint32_t tilt_abs_pos)
{
    /* Send GoTo operation code to dSPIN */
    write_byte(dSPIN_NOP, dSPIN_GO_TO);
    /* Send absolute position parameter - byte 2 data to dSPIN */
    write_byte(dSPIN_NOP, (uint8_t)(tilt_abs_pos >> 16));
    /* Send absolute position parameter - byte 1 data to dSPIN */
    write_byte(dSPIN_NOP, (uint8_t)(tilt_abs_pos >> 8));
    /* Send absolute position parameter - byte 0 data to dSPIN */
    write_byte(dSPIN_NOP, (uint8_t)(tilt_abs_pos));
}

/**
  * @brief  Issues dSPIN Go To Dir command.
  * @param  Movement direction, Absolute position where requested to move
  * @retval None
  */
void dSPIN_Go_To_Dir(dSPIN_Direction_TypeDef direction, uint32_t abs_pos)
{
    /* Send GoTo_DIR operation code to dSPIN */
    dSPIN_Write_Byte(dSPIN_GO_TO_DIR | direction);
    /* Send absolute position parameter - byte 2 data to dSPIN */
    dSPIN_Write_Byte((uint8_t)(abs_pos >> 16));
    /* Send absolute position parameter - byte 1 data to dSPIN */
    dSPIN_Write_Byte((uint8_t)(abs_pos >> 8));
    /* Send absolute position parameter - byte 0 data to dSPIN */
    dSPIN_Write_Byte((uint8_t)(abs_pos));
}

void go_to_dir(dSPIN_Direction_TypeDef pan_dir,
               uint32_t                pan_abs_pos,
               dSPIN_Direction_TypeDef tilt_dir,
               uint32_t                tilt_abs_pos)
{
    /* Send GoTo_DIR operation code to dSPIN */
    write_byte(dSPIN_GO_TO_DIR | pan_dir, dSPIN_GO_TO_DIR | tilt_dir);
    /* Send absolute position parameter - byte 2 data to dSPIN */
    write_byte((uint8_t)(pan_abs_pos >> 16), (uint8_t)(tilt_abs_pos >> 16));
    /* Send absolute position parameter - byte 1 data to dSPIN */
    write_byte((uint8_t)(pan_abs_pos >> 8), (uint8_t)(tilt_abs_pos >> 8));
    /* Send absolute position parameter - byte 0 data to dSPIN */
    write_byte((uint8_t)(pan_abs_pos), (uint8_t)(tilt_abs_pos));
}

void pan_go_to_dir(dSPIN_Direction_TypeDef pan_dir, uint32_t pan_abs_pos)
{
    /* Send GoTo_DIR operation code to dSPIN */
    write_byte(dSPIN_GO_TO_DIR | pan_dir, dSPIN_NOP);
    /* Send absolute position parameter - byte 2 data to dSPIN */
    write_byte((uint8_t)(pan_abs_pos >> 16), dSPIN_NOP);
    /* Send absolute position parameter - byte 1 data to dSPIN */
    write_byte((uint8_t)(pan_abs_pos >> 8), dSPIN_NOP);
    /* Send absolute position parameter - byte 0 data to dSPIN */
    write_byte((uint8_t)(pan_abs_pos), dSPIN_NOP);
}

void tilt_go_to_dir(dSPIN_Direction_TypeDef tilt_dir, uint32_t tilt_abs_pos)
{
    /* Send GoTo_DIR operation code to dSPIN */
    write_byte(dSPIN_NOP, dSPIN_GO_TO_DIR | tilt_dir);
    /* Send absolute position parameter - byte 2 data to dSPIN */
    write_byte(dSPIN_NOP, (uint8_t)(tilt_abs_pos >> 16));
    /* Send absolute position parameter - byte 1 data to dSPIN */
    write_byte(dSPIN_NOP, (uint8_t)(tilt_abs_pos >> 8));
    /* Send absolute position parameter - byte 0 data to dSPIN */
    write_byte(dSPIN_NOP, (uint8_t)(tilt_abs_pos));
}

/**
  * @brief  Issues dSPIN Go Until command.
  * @param  Action, Movement direction, Speed
  * @retval None
  */
void dSPIN_Go_Until(dSPIN_Action_TypeDef    action,
                    dSPIN_Direction_TypeDef direction,
                    uint32_t                speed)
{
    /* Send GoUntil operation code to dSPIN */
    dSPIN_Write_Byte(dSPIN_GO_UNTIL | action | direction);
    /* Send speed parameter - byte 2 data to dSPIN */
    dSPIN_Write_Byte((uint8_t)(speed >> 16));
    /* Send speed parameter - byte 1 data to dSPIN */
    dSPIN_Write_Byte((uint8_t)(speed >> 8));
    /* Send speed parameter - byte 0 data to dSPIN */
    dSPIN_Write_Byte((uint8_t)(speed));
}

void go_until(dSPIN_Action_TypeDef    pan_action,
              dSPIN_Direction_TypeDef pan_dir,
              uint32_t                pan_speed,
              dSPIN_Action_TypeDef    tilt_action,
              dSPIN_Direction_TypeDef tilt_dir,
              uint32_t                tilt_speed)
{
    /* Send GoUntil operation code to dSPIN */
    write_byte(dSPIN_GO_UNTIL | pan_action | pan_dir, dSPIN_GO_UNTIL | tilt_action | tilt_dir);
    /* Send speed parameter - byte 2 data to dSPIN */
    write_byte((uint8_t)(pan_speed >> 16), (uint8_t)(tilt_speed >> 16));
    /* Send speed parameter - byte 1 data to dSPIN */
    write_byte((uint8_t)(pan_speed >> 8), (uint8_t)(tilt_speed >> 8));
    /* Send speed parameter - byte 0 data to dSPIN */
    write_byte((uint8_t)(pan_speed), (uint8_t)(tilt_speed));
}

void pan_go_until(dSPIN_Action_TypeDef    pan_action,
                  dSPIN_Direction_TypeDef pan_dir,
                  uint32_t                pan_speed)
{
    /* Send GoUntil operation code to dSPIN */
    write_byte(dSPIN_GO_UNTIL | pan_action | pan_dir, dSPIN_NOP);
    /* Send speed parameter - byte 2 data to dSPIN */
    write_byte((uint8_t)(pan_speed >> 16), dSPIN_NOP);
    /* Send speed parameter - byte 1 data to dSPIN */
    write_byte((uint8_t)(pan_speed >> 8), dSPIN_NOP);
    /* Send speed parameter - byte 0 data to dSPIN */
    write_byte((uint8_t)(pan_speed), dSPIN_NOP);
}

void tilt_go_until(dSPIN_Action_TypeDef    tilt_action,
                   dSPIN_Direction_TypeDef tilt_dir,
                   uint32_t                tilt_speed)
{
    /* Send GoUntil operation code to dSPIN */
    write_byte(dSPIN_NOP, dSPIN_GO_UNTIL | tilt_action | tilt_dir);
    /* Send speed parameter - byte 2 data to dSPIN */
    write_byte(dSPIN_NOP, (uint8_t)(tilt_speed >> 16));
    /* Send speed parameter - byte 1 data to dSPIN */
    write_byte(dSPIN_NOP, (uint8_t)(tilt_speed >> 8));
    /* Send speed parameter - byte 0 data to dSPIN */
    write_byte(dSPIN_NOP, (uint8_t)(tilt_speed));
}

/**
  * @brief  Issues dSPIN Release SW command.
  * @param  Action, Movement direction
  * @retval None
  */
void dSPIN_Release_SW(dSPIN_Action_TypeDef    action,
                      dSPIN_Direction_TypeDef direction)
{
    /* Send ReleaseSW operation code to dSPIN */
    dSPIN_Write_Byte(dSPIN_RELEASE_SW | action | direction);
}

void release_sw(dSPIN_Action_TypeDef    pan_action,
                dSPIN_Direction_TypeDef pan_dir,
                dSPIN_Action_TypeDef    tilt_action,
                dSPIN_Direction_TypeDef tilt_dir)
{
    /* Send ReleaseSW operation code to dSPIN */
    write_byte(dSPIN_RELEASE_SW | pan_action | pan_dir, dSPIN_RELEASE_SW | tilt_action | tilt_dir);
}

void pan_release_sw(dSPIN_Action_TypeDef    pan_action,
                    dSPIN_Direction_TypeDef pan_dir)
{
    /* Send ReleaseSW operation code to dSPIN */
    write_byte(dSPIN_RELEASE_SW | pan_action | pan_dir, dSPIN_NOP);
}

void tilt_release_sw(dSPIN_Action_TypeDef    tilt_action,
                     dSPIN_Direction_TypeDef tilt_dir)
{
    /* Send ReleaseSW operation code to dSPIN */
    write_byte(dSPIN_NOP, dSPIN_RELEASE_SW | tilt_action | tilt_dir);
}

/**
  * @brief  Issues dSPIN Go Home command. (Shorted path to zero position)
  * @param  None
  * @retval None
  */
void dSPIN_Go_Home(void)
{
    /* Send GoHome operation code to dSPIN */
    dSPIN_Write_Byte(dSPIN_GO_HOME);
}

void go_home(void)
{
    /* Send GoHome operation code to dSPIN */
    write_byte(dSPIN_GO_HOME, dSPIN_GO_HOME);
}

void pan_go_home(void)
{
    /* Send GoHome operation code to dSPIN */
    write_byte(dSPIN_GO_HOME, dSPIN_NOP);
}

void tilt_go_home(void)
{
    /* Send GoHome operation code to dSPIN */
    write_byte(dSPIN_NOP, dSPIN_GO_HOME);
}

/**
  * @brief  Issues dSPIN Go Mark command.
  * @param  None
  * @retval None
  */
void dSPIN_Go_Mark(void)
{
    /* Send GoMark operation code to dSPIN */
    dSPIN_Write_Byte(dSPIN_GO_MARK);
}

void go_mark(void)
{
    /* Send GoMark operation code to dSPIN */
    write_byte(dSPIN_GO_MARK, dSPIN_GO_MARK);
}

void pan_go_mark(void)
{
    /* Send GoMark operation code to dSPIN */
    write_byte(dSPIN_GO_MARK, dSPIN_NOP);
}

void tilt_go_mark(void)
{
    /* Send GoMark operation code to dSPIN */
    write_byte(dSPIN_NOP, dSPIN_GO_MARK);
}

/**
  * @brief  Issues dSPIN Reset Pos command.
  * @param  None
  * @retval None
  */
void dSPIN_Reset_Pos(void)
{
    /* Send ResetPos operation code to dSPIN */
    dSPIN_Write_Byte(dSPIN_RESET_POS);
}

void reset_pos(void)
{
    /* Send ResetPos operation code to dSPIN */
    write_byte(dSPIN_RESET_POS, dSPIN_RESET_POS);
}

void pan_reset_pos(void)
{
    /* Send ResetPos operation code to dSPIN */
    write_byte(dSPIN_RESET_POS, dSPIN_NOP);
}

void tilt_reset_pos(void)
{
    /* Send ResetPos operation code to dSPIN */
    write_byte(dSPIN_NOP, dSPIN_RESET_POS);
}

/**
  * @brief  Issues dSPIN Reset Device command.
  * @param  None
  * @retval None
  */
void dSPIN_Reset_Device(void)
{
    /* Send ResetDevice operation code to dSPIN */
    dSPIN_Write_Byte(dSPIN_RESET_DEVICE);
}

void reset_device(void)
{
    /* Send ResetDevice operation code to dSPIN */
    write_byte(dSPIN_RESET_DEVICE, dSPIN_RESET_DEVICE);
}

void pan_reset_device(void)
{
    /* Send ResetDevice operation code to dSPIN */
    write_byte(dSPIN_RESET_DEVICE, dSPIN_NOP);
}

void tilt_reset_device(void)
{
    /* Send ResetDevice operation code to dSPIN */
    write_byte(dSPIN_NOP, dSPIN_RESET_DEVICE);
}

/**
  * @brief  Issues dSPIN Soft Stop command.
  * @param  None
  * @retval None
  */
void dSPIN_Soft_Stop(void)
{
    /* Send SoftStop operation code to dSPIN */
    dSPIN_Write_Byte(dSPIN_SOFT_STOP);
}

void soft_stop(void)
{
    /* Send SoftStop operation code to dSPIN */
    write_byte(dSPIN_SOFT_STOP, dSPIN_SOFT_STOP);
}

void pan_soft_stop(void)
{
    /* Send SoftStop operation code to dSPIN */
    write_byte(dSPIN_SOFT_STOP, dSPIN_NOP);
}

void tilt_soft_stop(void)
{
    /* Send SoftStop operation code to dSPIN */
    write_byte(dSPIN_NOP, dSPIN_SOFT_STOP);
}

/**
  * @brief  Issues dSPIN Hard Stop command.
  * @param  None
  * @retval None
  */
void dSPIN_Hard_Stop(void)
{
    /* Send HardStop operation code to dSPIN */
    dSPIN_Write_Byte(dSPIN_HARD_STOP);
}

void hard_stop(void)
{
    /* Send HardStop operation code to dSPIN */
    write_byte(dSPIN_HARD_STOP, dSPIN_HARD_STOP);
}

void pan_hard_stop(void)
{
    /* Send HardStop operation code to dSPIN */
    write_byte(dSPIN_HARD_STOP, dSPIN_NOP);
}

void tilt_hard_stop(void)
{
    /* Send HardStop operation code to dSPIN */
    write_byte(dSPIN_NOP, dSPIN_HARD_STOP);
}


/**
  * @brief  Issues dSPIN Soft HiZ command.
  * @param  None
  * @retval None
  */
void dSPIN_Soft_HiZ(void)
{
    /* Send SoftHiZ operation code to dSPIN */
    dSPIN_Write_Byte(dSPIN_SOFT_HIZ);
}

void soft_hiz(void)
{
    /* Send SoftHiZ operation code to dSPIN */
    write_byte(dSPIN_SOFT_HIZ, dSPIN_SOFT_HIZ);
}

void pan_soft_hiz(void)
{
    /* Send SoftHiZ operation code to dSPIN */
    write_byte(dSPIN_SOFT_HIZ, dSPIN_NOP);
}

void tilt_soft_hiz(void)
{
    /* Send SoftHiZ operation code to dSPIN */
    write_byte(dSPIN_NOP, dSPIN_SOFT_HIZ);
}

/**
  * @brief  Issues dSPIN Hard HiZ command.
  * @param  None
  * @retval None
  */
void dSPIN_Hard_HiZ(void)
{
    /* Send HardHiZ operation code to dSPIN */
    dSPIN_Write_Byte(dSPIN_HARD_HIZ);
}

void hard_hiz(void)
{
    /* Send HardHiZ operation code to dSPIN */
    write_byte(dSPIN_HARD_HIZ, dSPIN_HARD_HIZ);
}

void pan_hard_hiz(void)
{
    /* Send HardHiZ operation code to dSPIN */
    write_byte(dSPIN_HARD_HIZ, dSPIN_NOP);
}

void tilt_hard_hiz(void)
{
    /* Send HardHiZ operation code to dSPIN */
    write_byte(dSPIN_NOP, dSPIN_HARD_HIZ);
}

/**
  * @brief  Issues dSPIN Get Status command.
  * @param  None
  * @retval Status Register content
  */
uint16_t dSPIN_Get_Status(void)
{
    uint16_t temp = 0;
    uint16_t rx = 0;

    /* Send GetStatus operation code to dSPIN */
    dSPIN_Write_Byte(dSPIN_GET_STATUS);
    /* Send zero byte / receive MSByte from dSPIN */
    temp = dSPIN_Write_Byte((uint8_t)(0x00));
    temp = temp << 8;
    rx |= temp;
    /* Send zero byte / receive LSByte from dSPIN */
    temp = dSPIN_Write_Byte((uint8_t)(0x00));
    rx |= temp;
    return rx;
}

struct motor_status get_status(void)
{
    uint16_t temp = 0;
    struct motor_status status;

    /* Send GetStatus operation code to dSPIN */
    temp = write_byte(dSPIN_GET_STATUS, dSPIN_GET_STATUS);
    
    /* Send zero byte / receive MSByte from dSPIN */
    temp = write_byte((uint8_t)(0x00), (uint8_t)(0x00));
    
    status.pan_status.value = (temp & 0xff) << 8;
    status.tilt_status.value = temp & 0xff00;

    /* Send zero byte / receive LSByte from dSPIN */
    temp = write_byte((uint8_t)(0x00), (uint8_t)(0x00));
    
    status.pan_status.value |= (temp & 0xff);
    status.tilt_status.value |= (temp & 0xff00) >> 8;

    rt_kprintf("pan_status  = 0x%x\r\n", status.pan_status.value);
    rt_kprintf("tilt_status = 0x%x\r\n", status.tilt_status.value);

    return status;
}

/**
  * @brief  Checks if the dSPIN is Busy by hardware - active Busy signal.
  * @param  None
  * @retval one if chip is busy, otherwise zero
  */
uint8_t pan_busy_hw(void)
{
    if (!(GPIO_ReadInputDataBit(PAN_BUSY_Port, PAN_BUSY_Pin)))
        return 0x01;
    else
        return 0x00;
}

uint8_t tilt_busy_hw(void)
{
    if (!(GPIO_ReadInputDataBit(TILT_BUSY_Port, TILT_BUSY_Pin)))
        return 0x01;
    else
        return 0x00;
}

/**
  * @brief  Checks if the dSPIN is Busy by SPI - Busy flag bit in Status Register.
  * @param  None
  * @retval one if chip is busy, otherwise zero
  */
uint8_t dSPIN_Busy_SW(void)
{
    if(!(dSPIN_Get_Status() & dSPIN_STATUS_BUSY)) return 0x01;
    else return 0x00;
}

/**
  * @brief  Checks dSPIN Flag signal.
  * @param  None
  * @retval one if Flag signal is active, otherwise zero
  */
uint8_t pan_flag(void)
{
    if (!(GPIO_ReadInputDataBit(PAN_FLAG_Port, PAN_FLAG_Pin)))
        return 0x01;
    else
        return 0x00;
}

uint8_t tilt_flag(void)
{
    if (!(GPIO_ReadInputDataBit(TILT_FLAG_Port, TILT_FLAG_Pin)))
        return 0x01;
    else
        return 0x00;
}

/**
  * @brief  Transmits/Receives one byte to/from dSPIN over SPI.
  * @param  Transmited byte
  * @retval Received byte
  */
uint8_t dSPIN_Write_Byte(uint8_t byte)
{
    /* nSS signal activation - low */
    GPIO_ResetBits(dSPIN_nSS_Port, dSPIN_nSS_Pin);
    /* SPI byte send */
    SPI_I2S_SendData(dSPIN_SPI, byte);
    /* Wait for SPIx Busy flag */
    while (SPI_I2S_GetFlagStatus(dSPIN_SPI, SPI_I2S_FLAG_BSY) != RESET);
    /* nSS signal deactivation - high */
    GPIO_SetBits(dSPIN_nSS_Port, dSPIN_nSS_Pin);
    return (uint8_t)(SPI_I2S_ReceiveData(dSPIN_SPI));
}

uint16_t write_byte(uint8_t pan_byte, uint8_t tilt_byte)
{
    uint16_t return_value;
    
    /* nSS signal activation - low */
    GPIO_ResetBits(dSPIN_nSS_Port, dSPIN_nSS_Pin);

    /* SPI byte send */
    SPI_I2S_SendData(dSPIN_SPI, tilt_byte);
    /* Wait for SPIx Busy flag */
    while (SPI_I2S_GetFlagStatus(dSPIN_SPI, SPI_I2S_FLAG_BSY) != RESET)
        ;

    /* SPI byte read */
    return_value = SPI_I2S_ReceiveData(dSPIN_SPI)&0xff;
    
    /* SPI byte send */
    SPI_I2S_SendData(dSPIN_SPI, pan_byte);
    /* Wait for SPIx Busy flag */
    while (SPI_I2S_GetFlagStatus(dSPIN_SPI, SPI_I2S_FLAG_BSY) != RESET)
        ;

    /* SPI byte read */
    return_value = (return_value << 8) | SPI_I2S_ReceiveData(dSPIN_SPI)&0xff;

    /* nSS signal deactivation - high */
    GPIO_SetBits(dSPIN_nSS_Port, dSPIN_nSS_Pin);

    return return_value;
}

void send_bytes(uint8_t size, uint8_t *in, uint8_t *out)
{
    uint8_t i = 0;
    
    /* nSS signal activation - low */
    GPIO_ResetBits(dSPIN_nSS_Port, dSPIN_nSS_Pin);

    for (i = 0; i < size; i++)
    {
        /* SPI byte send */
        SPI_I2S_SendData(dSPIN_SPI, in[size - 1 - i]);
        /* Wait for SPIx Busy flag */
        while (SPI_I2S_GetFlagStatus(dSPIN_SPI, SPI_I2S_FLAG_BSY) != RESET)
            ;

        /* SPI byte read */
        out[size - 1 - i] = SPI_I2S_ReceiveData(dSPIN_SPI)&0xff;
    }

    /* nSS signal deactivation - high */
    GPIO_SetBits(dSPIN_nSS_Port, dSPIN_nSS_Pin);
}

#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(move, move pan and tilt motor at the same time)
FINSH_FUNCTION_EXPORT(pan_move, move pan motor)
FINSH_FUNCTION_EXPORT(tilt_move, move tilt motor)
FINSH_FUNCTION_EXPORT(run, start pan and tilt motor at the same time)
FINSH_FUNCTION_EXPORT(pan_run, start pan motor)
FINSH_FUNCTION_EXPORT(tilt_run, start tilt motor)
FINSH_FUNCTION_EXPORT(go_to, go to pan and tilt motor at the same time)
FINSH_FUNCTION_EXPORT(pan_go_to, pan go to)
FINSH_FUNCTION_EXPORT(tilt_go_to, tilt go to)
FINSH_FUNCTION_EXPORT(go_to_dir, go to pan and tilt motor at the same time)
FINSH_FUNCTION_EXPORT(pan_go_to_dir, pan go to in direction)
FINSH_FUNCTION_EXPORT(tilt_go_to_dir, tilt go to in direction)
FINSH_FUNCTION_EXPORT(go_until, The GoUntil command produces a motion at SPD speed imposing a forware or a reverse direction)
FINSH_FUNCTION_EXPORT(pan_go_until, The GoUntil command produces a motion at SPD speed imposing a forware or a reverse direction)
FINSH_FUNCTION_EXPORT(tilt_go_until, The GoUntil command produces a motion at SPD speed imposing a forware or a reverse direction)
FINSH_FUNCTION_EXPORT(release_sw, The ReleaseSW command produces a motion at minimum speed imposing a forware or a reverse rotation)
FINSH_FUNCTION_EXPORT(pan_release_sw, The ReleaseSW command produces a motion at minimum speed imposing a forware or a reverse rotation)
FINSH_FUNCTION_EXPORT(tilt_release_sw, The ReleaseSW command produces a motion at minimum speed imposing a forware or a reverse rotation)
FINSH_FUNCTION_EXPORT(go_home, The GoHome command produces a motion to the HOME position-zero position via the shortest path)
FINSH_FUNCTION_EXPORT(pan_go_home, The GoHome command produces a motion to the HOME position-zero position via the shortest path)
FINSH_FUNCTION_EXPORT(tilt_go_home, The GoHome command produces a motion to the HOME position-zero position via the shortest path)
FINSH_FUNCTION_EXPORT(go_mark, The GoMark command produces a motion to MARK position performing the minimum path)
FINSH_FUNCTION_EXPORT(pan_go_mark, The GoMark command produces a motion to MARK position performing the minimum path)
FINSH_FUNCTION_EXPORT(tilt_go_mark, The GoMark command produces a motion to MARK position performing the minimum path)
FINSH_FUNCTION_EXPORT(reset_pos, The ResetPos command resets the ABS_POS register to zero(HOME position).)
FINSH_FUNCTION_EXPORT(pan_reset_pos, The ResetPos command resets the ABS_POS register to zero(HOME position).)
FINSH_FUNCTION_EXPORT(tilt_reset_pos, The ResetPos command resets the ABS_POS register to zero(HOME position).)
FINSH_FUNCTION_EXPORT(reset_device, The ResetDevice command resets the device to power-up conditions)
FINSH_FUNCTION_EXPORT(pan_reset_device, The ResetDevice command resets the device to power-up conditions)
FINSH_FUNCTION_EXPORT(tilt_reset_device, The ResetDevice command resets the device to power-up conditions)
FINSH_FUNCTION_EXPORT(soft_stop, The SoftStop command causes an immediate deceleration to zero speed and a consequent motor stop)
FINSH_FUNCTION_EXPORT(pan_soft_stop, The SoftStop command causes an immediate deceleration to zero speed and a consequent motor stop)
FINSH_FUNCTION_EXPORT(tilt_soft_stop, The SoftStop command causes an immediate deceleration to zero speed and a consequent motor stop)
FINSH_FUNCTION_EXPORT(hard_stop, The HardStop command causes an immediate motor stop with infinite deceleration)
FINSH_FUNCTION_EXPORT(pan_hard_stop, The HardStop command causes an immediate motor stop with infinite deceleration)
FINSH_FUNCTION_EXPORT(tilt_hard_stop, The HardStop command causes an immediate motor stop with infinite deceleration)
FINSH_FUNCTION_EXPORT(soft_hiz, The SoftHiZ command disables the power bridges after a deceleration to zero)
FINSH_FUNCTION_EXPORT(pan_soft_hiz, The SoftHiZ command disables the power bridges after a deceleration to zero)
FINSH_FUNCTION_EXPORT(tilt_soft_hiz, The SoftHiZ command disables the power bridges after a deceleration to zero)
FINSH_FUNCTION_EXPORT(hard_hiz, The HardHiZ command immediately disables the power bridges and raises the HiZ flag)
FINSH_FUNCTION_EXPORT(pan_hard_hiz, The HardHiZ command immediately disables the power bridges and raises the HiZ flag)
FINSH_FUNCTION_EXPORT(tilt_hard_hiz, The HardHiZ command immediately disables the power bridges and raises the HiZ flag)
FINSH_FUNCTION_EXPORT(get_status, The GetStatus command returns the Status register value)
FINSH_FUNCTION_EXPORT(pan_get_param, This command reads the PARAM register value)
FINSH_FUNCTION_EXPORT(tilt_get_param, This command reads the PARAM register value)
FINSH_FUNCTION_EXPORT(pan_set_param, The SetParam command sets the PARAM register value equal to VALUE)
FINSH_FUNCTION_EXPORT(tilt_set_param, The SetParam command sets the PARAM register value equal to VALUE)
#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
