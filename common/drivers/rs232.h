/** ****************************************************************************
  * \file    rs232.h 
  * \author  Jang Ho Jong
  * \version V0.0.1
  * \date    2023.04.05
  * \brief   RS-232 Device Driver Header
  * ***************************************************************************/
/** \remark  Version History
  * V0.0.1   - 최초 버전 - 2016.05.06
  * V0.0.2   - Rs232_t 타입에 rx_tick 변수 추가
  *            Rs232_Disable_IRQ()
  *            Rs232_Set_Receive_Tick(),
  *            Rs232_CountDown_Receive_Tick() 함수 추가
  * V0.0.3   - Rs232_Printf() 함수 이름을 Printf()로 변경
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RS232_H
#define __RS232_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_uart.h"
#include "typedef.h"

/** \addtogroup Drivers
  * \{ */
  
/** \addtogroup RS232_Driver
  * \{ */

/* Exported types ------------------------------------------------------------*/
/** \defgroup RS232_Driver_Exported_Types RS232 드라이버 공개 타입
  * \{ */

/** \brief RS232 구조체 타입 */
typedef struct
{
  char_t             *name;
  GPIO_TypeDef*       port_tx;            /**< RS232 TX GPIO Port Pointer */
  u32_t            clk_tx;             /**< RS232 TX GPIO Port Clock Bit Definition */
  GPIO_InitTypeDef    init_tx;            /**< RS232 TX GPIO Init Structure */
  GPIO_TypeDef*       port_rx;            /**< RS232 RX GPIO Port Pointer */
  u32_t            clk_rx;             /**< RS232 RX GPIO Port Clock Bit Definition */
  GPIO_InitTypeDef    init_rx;            /**< RS232 RX GPIO Init Structure */
  u08_t            *tx_buf;             /**< RS232 Transmission Buffer Pointer */
  u32_t            tx_size;            /**< RS232 Transmission Buffer Size */
  u32_t            tx_count_in;        /**< RS232 Transmission Buffer Input Count */
  u32_t            tx_count_out;       /**< RS232 Transmission Buffer Output Count */
  Bool_t              tx_ing;             /**< RS232 Flag for Transmission Ongoing */
  u08_t            *rx_buf;             /**< RS232 Receiving Buffer Pointer */
  u32_t            rx_size;            /**< RS232 Receiving Buffer Size */
  u32_t            rx_count;           /**< RS232 Receiving Buffer Count */
  u32_t            rx_tick;            /**< RS232 Receiving Complete Tick */
  Bool_t              rx_complete;        /**< RS232 Flag for Receiving Complete */
  u08_t            *printf_buf;         /**< RS232 Buffer for Printf() */
  u32_t            printf_size;        /**< RS232 Printf-Buffer Size */
  Bool_t              usingIRQ;           /**< RS232 Flag of Using IRQ */
  IRQn_Type           irqn;               /**< RS232 IRQn */
  u32_t            irq_priority;       /**< RS232 IRQ Priority */
  UART_HandleTypeDef *handle;             /**< RS232 UART Handle Structure Pointer */
}Rs232_t;
/** \} RS232_Driver_Exported_Types */

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** \defgroup RS232_Driver_Exported_Functions RS232 드라이버 공개 함수
  * \{ */
void Rs232_Init(Rs232_t * rs);
void Rs232_Disable_IRQ(Rs232_t * rs);
void Rs232_Enable_Receiving(Rs232_t * rs);
void Rs232_IRQHandler_RXNE(Rs232_t * rs);
void Rs232_Set_Receive_Tick(Rs232_t * rs, u32_t tick);
void Rs232_CountDown_Receive_Tick(Rs232_t * rs);
void Rs232_Receive_TimeOut(Rs232_t * rs);
void Rs232_Restore_rx(Rs232_t * rs);
void Rs232_Transmit_Data(Rs232_t * rs, u08_t data[], u32_t count);
void Rs232_Send_Byte(Rs232_t * rs, u08_t byte);
void Rs232_IRQHandler_TXE(Rs232_t * rs);
void Printf(Rs232_t * rs, const char_t format[], ...);
/** \} RS232_Driver_Exported_Functions */

/** \} RS232_Driver */

/** \} Drivers */

#endif /* __RS232_H */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
