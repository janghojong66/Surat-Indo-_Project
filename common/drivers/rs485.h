/** ****************************************************************************
  * \file    rs485.h 
  * \author  Park Keun Tae
  * \version V0.0.2
  * \date    2017.12.15
  * \brief   RS-485 Device Driver Header
  * ***************************************************************************/
/** \remark  Version History
  * V0.0.1   - 최초 버전 - 2016.10.17
  * V0.0.2   - 2017.12.15
               초기화시 TXE핀을 Low로 세팅하도록 변경.
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RS485_H
#define __RS485_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_uart.h"
#include "typedef.h"

/** \addtogroup Drivers
  * \{ */
  
/** \addtogroup RS485_Driver
  * \{ */

/* Exported types ------------------------------------------------------------*/
/** \defgroup RS485_Driver_Exported_Types RS485 드라이버 공개 타입
  * \{ */

/** \brief RS485 구조체 타입 */
typedef struct
{
  char_t             *name;
  GPIO_TypeDef*       port_tx;            /**< RS485 TX GPIO Port Pointer */
  uint32_t            clk_tx;             /**< RS485 TX GPIO Port Clock Bit Definition */
  GPIO_InitTypeDef    init_tx;            /**< RS485 TX GPIO Init Structure */
  GPIO_TypeDef*       port_rx;            /**< RS485 RX GPIO Port Pointer */
  uint32_t            clk_rx;             /**< RS485 RX GPIO Port Clock Bit Definition */
  GPIO_InitTypeDef    init_rx;            /**< RS485 RX GPIO Init Structure */
  GPIO_TypeDef*       port_txe;           /**< RS485 TXE GPIO Port Pointer */
  uint32_t            clk_txe;            /**< RS485 TXE GPIO Port Clock Bit Definition */
  GPIO_InitTypeDef    init_txe;           /**< RS485 TXE GPIO Init Structure */
  GPIO_PinState       on_txe;             /**< RS485 TXE를 ON 하는 GPIO 핀 상태 
                                               GPIO_PIN_RESET 혹은 GPIO_PIN_SET 값을 가질 수 있다. */
  GPIO_PinState       off_txe;            /**< RS485 TXE를 OFF 하는 GPIO 핀 상태 
                                               GPIO_PIN_RESET 혹은 GPIO_PIN_SET 값을 가질 수 있다. */
  uint8_t            *tx_buf;             /**< RS485 Transmission Buffer Pointer */
  uint32_t            tx_size;            /**< RS485 Transmission Buffer Size */
  uint32_t            tx_count_in;        /**< RS485 Transmission Buffer Input Count */
  uint32_t            tx_count_out;       /**< RS485 Transmission Buffer Output Count */
  Bool_t              tx_ing;             /**< RS485 Flag for Transmission Ongoing */
  uint8_t            *rx_buf;             /**< RS485 Receiving Buffer Pointer */
  uint32_t            rx_size;            /**< RS485 Receiving Buffer Size */
  uint32_t            rx_count;           /**< RS485 Receiving Buffer Count */
  uint32_t            rx_tick;            /**< RS485 Receiving Complete Tick */
  Bool_t              rx_complete;        /**< RS485 Flag for Receiving Complete */
  uint8_t            *printf_buf;         /**< RS485 Buffer for Printf() */
  uint32_t            printf_size;        /**< RS485 Printf-Buffer Size */
  Bool_t              usingIRQ;           /**< RS485 Flag of Using IRQ */
  IRQn_Type           irqn;               /**< RS485 IRQn */
  uint32_t            irq_priority;       /**< RS485 IRQ Priority */
  UART_HandleTypeDef *handle;             /**< RS485 UART Handle Structure Pointer */
}Rs485_t;
/** \} RS485_Driver_Exported_Types */

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** \defgroup RS485_Driver_Exported_Functions RS485 드라이버 공개 함수
  * \{ */
void Rs485_Init(Rs485_t * rs);
void Rs485_Disable_IRQ(Rs485_t * rs);
void Rs485_Enable_Receiving(Rs485_t * rs);
void Rs485_IRQHandler_RXNE(Rs485_t * rs);
void Rs485_Set_Receive_Tick(Rs485_t * rs, uint32_t tick);
void Rs485_CountDown_Receive_Tick(Rs485_t * rs);
void Rs485_Receive_TimeOut(Rs485_t * rs);
void Rs485_Restore_rx(Rs485_t * rs);
void Rs485_Transmit_Data(Rs485_t * rs, uint8_t data[], uint32_t count);
void Rs485_Send_Byte(Rs485_t * rs, uint8_t byte);
void Rs485_IRQHandler_TXE(Rs485_t * rs);
void Rs485_IRQHandler_TC(Rs485_t * rs);
/** \} RS485_Driver_Exported_Functions */

/** \} RS485_Driver */

/** \} Drivers */

#endif /* __RS485_H */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
