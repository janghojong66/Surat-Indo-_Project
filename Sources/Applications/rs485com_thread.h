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
#include "rs485com_thread.h"
#include "device.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define DERAILMENT_FAIL    0x01
#define SELFTEST_FAIL      0x02
#define cT_10msec          10U
#define REQUEST            0x01
#define RESPONCE           0x02
#define Packet_Lenth       0x05
#define NOT_USE            0x00
#define STX                0x02
#define ETX                0x03
#define DEFAULT_DATA_LENGTH  0x05


typedef struct
{
	uint8_t     stx;
	uint8_t     data_len;
	uint8_t     heartbit;     			 
	uint8_t     dmpc_id;	
	uint8_t     msg_type;
  uint8_t     state_code;
	uint8_t     detail_code; 
	uint8_t     crc_high;				 
	uint8_t     crc_low;
	uint8_t	    etx;
}Curs485_Packet_t;  /* 10bytes */

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/** \defgroup Rs232com_Thread_Exported_Variables Rs485com 쓰레드 공개 변수
  * \{ */

/** \} Rs232com_Thread_Exported_Variables */

/* Exported functions --------------------------------------------------------*/
/** \defgroup Rs232com_Thread_Exported_Functions Rs485com 쓰레드 공개 함수
  * \{ */
//#define RS485COM_REQUEST_TIME         150U     // 150msec  
//#define RS485COM_COMM_OK_TIME         5000U     // 5초동안 데이타 통신 상태 점검
extern void Rs485com_Thread(void);
//extern u32_t  g_Rs485com_request_timer;
//extern u32_t  g_Rs485com_comm_ok_timer;
extern u16_t  Rx_Wait_Timer1d;
extern u08_t   Rx_Wait_Timer1c;
extern Bool_t g_monitor_485_tx;
extern Bool_t g_monitor_485_rx;
extern u08_t   g_send_dmpc_cnt;
//extern Bool_t g_temper_flag;
//extern void Console_Thread(void);
/** \} Console_Thread_Exported_Functions */

/** \} Rs232com_Thread */

/** \} Applications */

#endif /* __RS232COM_THREAD_H */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
