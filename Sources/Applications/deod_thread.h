/** ****************************************************************************
  * \file    deod_thread.h
  * \author  Jang Ho Jong
  * \version V0.0.1
  * \date    2023.03.09
  * \brief   DEOD_Thread Header
  * ***************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DEOD_THREAD_H
#define __DEOD_THREAD_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** \addtogroup Applications
  * \{ */

/** \addtogroup Sysmon_Thread
  * \{ */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/** \defgroup Sysmon_Thread_Exported_Variables 시스템 모니터링 쓰레드 공개 변수
  * \{ */
\
/** \} DEOD_Thread_Exported_Variables */

/* Exported functions --------------------------------------------------------*/
/** \defgroup Sysmon_Thread_Exported_Functions 시스템 모니터링 쓰레드 공개 함수
  * \{ */
//#define DEOD_SYS_CHECK_TIME     1000U  
#define  Derail_Fail            0x01
#define  Self_Fail              0x02

#define  STL_Fail               0x01
#define  Voltage_Fail           0x02
#define  Relay_Fail             0x03
#define  Temp_Fail_Maj          0x04
#define  Temp_Fail_Min          0x05
#define  Power_Fail             0x06
#define  In_Out_Isol_Fail       0x07
#define  Watchdog_Fail          0x08
#define  Sensor_Fail            0x09
#define DEOD_SYS_CHECK_TIME     10U                         

extern void Deod_Thread(void);
extern u32_t g_DEOD_Check_timer;
extern u32_t g_Low_Voltage_Check_timer;
 
/** \} Deod_Thread_Exported_Functions */

/** \} Deod_Thread */

/** \} Applications */

#endif /* __DEOD_THREAD_H */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
