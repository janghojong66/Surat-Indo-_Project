/** ****************************************************************************
  * \file    TMP117.c
  * \author  Jang Ho Jong
  * \version V1.0.0
  * \date    2023.03.15
  * \brief   This Libray for TMP117 Temperature Sensor Device Driver Body
  * ***************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "tpm117.h"
#include "device.h"
#include "sysmon_thread.h"

/** \addtogroup Drivers
  * \{ */

/** \defgroupTMP117 Temperature Sensor Device Drvier
  * \brief TMP117 Temperature Sensor 디바이스를 제어하는 드라이버 입니다.
  * \{ */

/*  
   @Brief         Get temperature basically
   @Description   Function gives to us ambient temperature
   @Parameter     I2C_HandleTypeDef ->  HAL_I2C Handle
   @Return value  Float
 */
float TMP117_get_Temperature(I2C_HandleTypeDef hi2c1)
{
      static u08_t buf[2];
	    uint16_t nTempVal = 0;
      buf[0]=TMP117_TemperatureRegister;
#ifndef __NOT_USE_ST__
#endif
      HAL_Delay(1);
#ifndef __NOT_USE_ST__
      HAL_I2C_Master_Receive(&hi2c1,TMP117_DeviceID,buf,2U,10U);
#endif
//      return ((((buf[0]<<8)|buf[1]))*0.0078125);
      //return (((buf[0]<<8)|buf[1]));

      nTempVal = (uint16_t)(((uint16_t)buf[0]<<8)|buf[1]);
      return (float)nTempVal;
}

/** \} TPM117_Driver_Exported_Functions */

/* Private functions ---------------------------------------------------------*/

/** \} TMP117_Driver */

/** \} Drivers */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
