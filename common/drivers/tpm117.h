/** ****************************************************************************
  * \file    tpm117.h
  * \author  Jang Ho Jong
  * \version V0.0.1
  * \date    2023.05.30
  * \brief   I2C Driver Header
  * ***************************************************************************/
/** \remark  Version History
  * V0.0.1   - 최초 버전 - 2016.10.17
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef      __TPM117_H
#define      __TPM117_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"
#include "device.h"

/*
Check the model of your MCU model
*/
#ifndef     __STM32L0xx_HAL_I2C_H
#define     __STM32L0xx_HAL_I2C_H
#endif	

#define     TMP117_DeviceID1 0x48<<1    //	GND
#define     TMP117_DeviceID2 0x49<<1    //	Vcc
#define     TMP117_DeviceID3 0x4A<<1    //	SDA
#define     TMP117_DeviceID4 0x4B<<1    //	SCL


/*
Pointer Registers
*/
#define     TMP117_TemperatureRegister     0x00
#define     TMP117_ConfigurationRegister   0x01
#define     TMP117_TemperatureHighLimit    0x02
#define     TMP117_TemperatureLowLimit     0x03

#define     TMP117_EEPROM_Uclock           0x04
#define     TMP117_EEPROM1                 0x05
#define     TMP117_EEPROM2                 0x06
#define     TMP117_EEPROM3                 0x08

#define     TMP117_Temperature_Offset      0x07
#define     TMP117_ID_Register             0x0F

/*  
   @Brief         Get temperature basically
   @Description   Function gives to us ambient temperature
   @Parameter     I2C_HandleTypeDef ->  HAL_I2C Handle
   @Return value  Float
 */
float TMP117_get_Temperature             (I2C_HandleTypeDef i2c);





#endif






/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
