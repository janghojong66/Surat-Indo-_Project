/** ****************************************************************************
  * \file    rs485com_thread.h
  * \author  Jang Ho Jong
  * \version V1.0.0
  * \date    2024.07.10
  * \brief   Rs485com Thread Header
  * ***************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RS485COM_THREAD_H
#define __RS485COM_THREAD_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "rs232com_thread.h"
#include "device.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

typedef struct
{
	uint8_t     stx;
	uint8_t     data_len;
	uint8_t     seq_no;     			 
	uint8_t     depc_id;	
	uint8_t     msg_type;
	uint8_t     derail_info;
	uint8_t     sensor_info;
	uint8_t     crc_low;				 
	uint8_t     crc_high;
	uint8_t	    etx;
}Rs232_Packet_t;  /* 10bytes */

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/** \defgroup Rs232com_Thread_Exported_Variables Rs485com 쓰레드 공개 변수
  * \{ */

/** \} Rs232com_Thread_Exported_Variables */

/* Exported functions --------------------------------------------------------*/
/** \defgroup Rs232com_Thread_Exported_Functions Rs485com 쓰레드 공개 함수
  * \{ */
#define RS232COM_REQUEST_TIME          500U     // 500msec  
extern void Rs232com_Thread(void);
extern u32_t  g_Rs232com_request_timer;
extern Bool_t g_monitor_232_tx;
extern Bool_t g_monitor_232_rx;
//extern Bool_t g_temper_flag;
//extern void Console_Thread(void);
/** \} Console_Thread_Exported_Functions */

/** \} Rs232com_Thread */

/** \} Applications */

#endif /* __RS232COM_THREAD_H */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
