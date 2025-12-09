/** ****************************************************************************
  * \file    sysmon_thread.h
  * \author  Jang Ho Jong
  * \version V0.0.1
  * \date    2023.04.05
  * \brief   System Monitoring Thread Header
  * ***************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYSMON_THREAD_H
#define __SYSMON_THREAD_H

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

#define RUN_LED_TOGGLE_TIME          500U     // 500msec  
#define HCR_INPUT_TIME               500U     // 500msec
#define WATCH_DOG_TOGGLE_TIME         15U     // 15msec                    
#define TEMPERATOR_CHECK_TIME      60000U	    // 10sec	
#define POWER_OK_CHECK_TIME         1000U     // 1sec
#define HIGH_TEMPER              80.00000    // +85도
#define LOW_TEMPER			              40U    // -40 C
#define LOW_WARNING_TEMPER            30U    // -30도 경고 온도
#define HIGH_WARNING_TEMPER           70U    // 70도 온도 경고
#define MINUS_TEMPER               32768U    // 영하온도 시작
#define TMP117_RESOLUTION       0.0078125
#define FLT_SIGNAL_CHECK_TIME      10000U
#define RLY_FB_CHECK_TIME         600000U    
#define SENSOR_A12_CHECK_TIME     10000U
#define WATCHDOG_CHECK_PIN_TIME       10U     // 10msec

#define     TMP117_DeviceID  0x48 <<1    //	GND
#define     TMP117_TemperatureRegister     0x00
/*
#define     TMP117_ConfigurationRegister   0x01
#define     TMP117_TemperatureHighLimit    0x02
#define     TMP117_TemperatureLowLimit     0x03
#define     TMP117_Temperature_Offset      0x07
#define     TMP117_EEPROM_Uclock           0x04
*/
/** \} Sysmon_Thread_Exported_Variables */

/* Exported functions --------------------------------------------------------*/
/** \defgroup Sysmon_Thread_Exported_Functions 시스템 모니터링 쓰레드 공개 함수
  * \{ */
extern Bool_t g_Master_Board;
extern u32_t  g_Run_Led_timer;
extern u16_t  g_Watch_Dog_timer;
extern u16_t  g_Watchdog_Check_Pin_Timer;
extern u32_t  g_Temp_check_timer;
extern u32_t  g_Pwr_Ok_timer;
extern u32_t  g_Relay_FB_check_timer;
extern void   Sysmon_Thread(void);            /* Sysmon 쓰레드 생성한다. */
extern void   Check_Temperature(void);
extern float  g_device_temper;
extern u32_t  g_Flt_Signal_timer;
extern u32_t  g_Sensor_Check_timer;
extern u08_t  g_State_Code;
extern u08_t  g_Detail_Code; 
/** \} Sysmon_Thread_Exported_Functions */

/** \} Sysmon_Thread */

/** \} Applications */

#endif /* __SYSMON_THREAD_H */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
