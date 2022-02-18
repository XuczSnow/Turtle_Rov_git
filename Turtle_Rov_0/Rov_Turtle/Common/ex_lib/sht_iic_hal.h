/**
  ******************************************************************************
  * @file    sht_iic_hal.h
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   SHT 系列传感器官方例程中 i2c_hal.h 文件移植
  *
  ******************************************************************************
  * @attention
  *	未做更改
  *
  ******************************************************************************
  */


//=============================================================================
//    S E N S I R I O N   AG,  Laubisruetistr. 50, CH-8712 Staefa, Switzerland
//=============================================================================
// Project   :  SHT3x Sample Code (V1.1)
// File      :  i2c_hal.h (V1.1)
// Author    :  RFU
// Date      :  6-Mai-2015
// Controller:  STM32F100RB
// IDE       :  礦ision V5.12.0.0
// Compiler  :  Armcc
// Brief     :  I2C hardware abstraction layer 
//=============================================================================

#ifndef SOFT_IIC_SHT_H
#define SOFT_IIC_SHT_H

//-- Includes -----------------------------------------------------------------
#include "sht_system.h"

//-- Enumerations -------------------------------------------------------------

// I2C acknowledge
typedef enum{
  ACK  = 0,
  NACK = 1,
}etI2cAck;

//=============================================================================
void I2c_Init(void);
//=============================================================================
// Initializes the ports for I2C interface.
//-----------------------------------------------------------------------------

//=============================================================================
void I2c_StartCondition(void);
//=============================================================================
// Writes a start condition on I2C-Bus.
//-----------------------------------------------------------------------------
// remark: Timing (delay) may have to be changed for different microcontroller.
//       _____
// SDA:       |_____
//       _______
// SCL:         |___

//=============================================================================
void I2c_StopCondition(void);
//=============================================================================
// Writes a stop condition on I2C-Bus.
//-----------------------------------------------------------------------------
// remark: Timing (delay) may have to be changed for different microcontroller.
//              _____
// SDA:   _____|
//            _______
// SCL:   ___|

//=============================================================================
etError I2c_WriteByte(u8t txByte);
//=============================================================================
// Writes a byte to I2C-Bus and checks acknowledge.
//-----------------------------------------------------------------------------
// input:  txByte       transmit byte
//
// return: error:       ACK_ERROR = no acknowledgment from sensor
//                      NO_ERROR  = no error
//
// remark: Timing (delay) may have to be changed for different microcontroller.

//=============================================================================
etError I2c_ReadByte(u8t *rxByte, etI2cAck ack, u8t timeout);

etError I2c_GeneralCallReset(void);

#endif