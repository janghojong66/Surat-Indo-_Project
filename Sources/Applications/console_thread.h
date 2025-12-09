/** ****************************************************************************
  * \file    console_thread.c
  * \author  Jang Ho Jong
  * \version V1.0.0
  * \date    2023.06.10
  * \brief   Console Thread Body
  * ***************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CONSOLE_THREAD_H
#define __CONSOLE_THREAD_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** \addtogroup Applications
  * \{ */

/** \addtogroup Console_Thread
  * \{ */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/** \defgroup Console_Thread_Exported_Variables Console 쓰레드 공개 변수
  * \{ */

/** \} Console_Thread_Exported_Variables */

/* Exported functions --------------------------------------------------------*/
/** \defgroup Console_Thread_Exported_Functions Console 쓰레드 공개 함수
  * \{ */
#define MAX_SIZE_CMD_BUF 20U          /**< 명령어 버퍼 크기 */
extern Bool_t g_monitor_flag;
extern Bool_t g_temper_flag;
extern void Console_Thread(void);
extern u08_t g_OpMode;
extern u08_t g_IdCnt;
/** \} Console_Thread_Exported_Functions */

/** \} Console_Thread */

/** \} Applications */

#endif /* __CONSOLE_THREAD_H */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
