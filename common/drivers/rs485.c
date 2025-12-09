/** ****************************************************************************
  * \file    rs485.c
  * \author  Park Keun Tae
  * \version V0.0.2
  * \date    2017.12.15
  * \brief   RS-485 Device Driver Body
  * ***************************************************************************/
/** \remark  Version History
  * V0.0.1   - 최초 버전 - 2016.10.17
  * V0.0.2   - 2017.12.15
               초기화시 TXE핀을 Low로 세팅하도록 변경.
*/

/* Includes ------------------------------------------------------------------*/
#include "rs485.h"
#include "device.h"

/** \addtogroup Drivers
  * \{ */

/** \defgroup RS485_Driver RS-485 디바이스 드라이버
  * \brief RS-485 디바이스를 제어하는 드라이버 입니다.
  * \{ */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** \addtogroup RS485_Driver_Exported_Functions
  * \{ */

/** \brief  RS485 Driver를 초기화한다.
  * \param  rs - RS485 구조체 포인터
  * \note   rs에 정의된 GPIO와 UART를 초기화하고, 인터럽트를 활성화한다.
  * \retval None
  */
void Rs485_Init(Rs485_t * rs)
{
  Periph_Control_GPIO_CLK(rs->port_tx, True);
  HAL_GPIO_Init(rs->port_tx, &rs->init_tx);
  Periph_Control_GPIO_CLK(rs->port_rx, True);
  HAL_GPIO_Init(rs->port_rx, &rs->init_rx);
  Periph_Control_GPIO_CLK(rs->port_txe, True);
  HAL_GPIO_Init(rs->port_txe, &rs->init_txe);
  
  HAL_GPIO_WritePin(rs->port_txe, rs->init_txe.Pin, rs->off_txe);

  Periph_Control_UART_CLK(rs->handle->Instance, True);
  HAL_UART_Init(rs->handle);
  
  if (rs->usingIRQ == False)
  {
    return;
  }
  HAL_NVIC_SetPriority(rs->irqn, rs->irq_priority, 0);
  HAL_NVIC_EnableIRQ(rs->irqn);
}

/** \brief  RS485 Driver의 인터럽트를 비활성화한다.
  * \param  rs - RS485 구조체 포인터
  * \note   rs의 인터럽트를 비활성화한다.
  * \retval 없음
  */
void Rs485_Disable_IRQ(Rs485_t * rs)
{
  /* Disable the UART Parity Error Interrupt */
  __HAL_UART_DISABLE_IT(rs->handle, UART_IT_PE);
  
  /* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
  __HAL_UART_DISABLE_IT(rs->handle, UART_IT_ERR);
  
  /* Disable the UART Data Register not empty Interrupt */
  __HAL_UART_DISABLE_IT(rs->handle, UART_IT_RXNE);
  
  /* Disable the UART Transmission Complete Interrupt */
  __HAL_UART_DISABLE_IT(rs->handle, UART_IT_TC);

  /* Disable the UART Transmit data register empty Interrupt */
  __HAL_UART_DISABLE_IT(rs->handle, UART_IT_TXE);

  /* rs의 인터럽트를 비활성화한다. */
  HAL_NVIC_DisableIRQ(rs->irqn);
}

/** \brief  RS485 Driver의 수신을 활성화한다.
  * \param  rs - RS485 구조체 포인터
  * \note   rs의 수신 인터럽트와 에러 인터럽트를 활성화한다.
  * \retval None
  */
void Rs485_Enable_Receiving(Rs485_t * rs)
{
  /* Enable the UART Parity Error Interrupt */
  __HAL_UART_ENABLE_IT(rs->handle, UART_IT_PE);
  
  /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
  __HAL_UART_ENABLE_IT(rs->handle, UART_IT_ERR);
  
  /* Enable the UART Data Register not empty Interrupt */
  __HAL_UART_ENABLE_IT(rs->handle, UART_IT_RXNE);
}

/** \brief  RS485 Driver의 수신 인터럽트를 처리한다.
  * \param  rs - RS485 구조체 포인터
  * \note   수신된 데이터를 rs의 수신 버퍼에 저장하고 수신 카운트를 조절한다.
  * \retval 없음
  */
void Rs485_IRQHandler_RXNE(Rs485_t * rs)
{
  uint32_t  dr = rs->handle->Instance->DR;
  
  rs->rx_complete = False;
  rs->rx_buf[rs->rx_count] = (uint8_t)(dr & (uint8_t)0x00FF);
  rs->rx_count++;
  if (rs->rx_count >= rs->rx_size)
  {
    rs->rx_count = 0;
  }
}

/** \brief  RS485의 수신 타이머를 tick으로 설정한다.
  * \param  rs - RS485 구조체 포인터
  * \param  tick - 수신 타이머를 설정할 값 [millisec]
  * \note   수신 인터럽트에서 수신될 때마다 수신 타이머를 설정(갱신)하여
            타이머 만기시 수신 완료로 처리한다.
  * \retval 없음
  */
void Rs485_Set_Receive_Tick(Rs485_t * rs, uint32_t tick)
{
  rs->rx_tick = tick;
}

/** \brief  RS485의 수신 타이머를 감소시킨다. 타이머 만기시 수신 완료를 설정한다.
  * \param  rs - RS485 구조체 포인터
  * \note   타이머 인터럽트에서 매 타이머 tick마다 호출된다.
            수신 타이머가 설정되어 있으면 감소시키며 수신 타이머가 0이 되면
            수신 완료를 설정한다.
  * \retval 없음
  */
void Rs485_CountDown_Receive_Tick(Rs485_t * rs)
{
  if (rs->rx_tick != 0)
  {
    rs->rx_tick--;
    if (rs->rx_tick == 0)
    {
      rs->rx_complete = True;
    }
  }
}

/** \brief  RS485 Driver의 수신 타임아웃을 처리한다.
  * \param  rs: RS485 구조체 포인터
  * \note   rs의 수신 완료 플래그를 True로 셋한다.
  * \retval None
  */
void Rs485_Receive_TimeOut(Rs485_t * rs)
{
  rs->rx_complete = True;
}

/** \brief  RS485 Driver의 수신 기능을 복원시킨다.
  * \param  rs - RS485 구조체 포인터
  * \note   수신된 데이터가 처리된 후 다시 수신할 수 있도록
            수신 버퍼와 수신 카운트, 수신 완료 플래그를 클리어한다.
  * \retval None
  */
void Rs485_Restore_rx(Rs485_t * rs)
{
  uint32_t i = 0;
  
  for (i = 0; i < rs->rx_count; i++)
  {
    rs->rx_buf[i] = 0;
  }
  rs->rx_count = 0;
  rs->rx_complete = False;
}

/** \brief  RS485 Driver를 통해 data를 그 count 만큼 rs로 전송한다.
  * \param  rs - RS485 구조체 포인터
  * \param  data - 송신할 데이터가 저장된 배열
  * \param  count - 송신할 데이터의 수
  * \note   data를 송신 버퍼에 복사하면서 송신 인터럽트를 활성화하여 
  *         송신 버퍼의 데이터가 전송되게끔한다.
  * \retval 없음
  */
void Rs485_Transmit_Data(Rs485_t * rs, uint8_t data[], uint32_t count)
{
  uint32_t    i = 0;
  
  for (i = 0; i < count; i++)
  {
    Rs485_Send_Byte(rs, data[i]);
  }
}

/** \brief  RS485 Driver를 통해 byte를 rs로 전송한다.
  * \param  rs - RS485 구조체 포인터
  * \param  byte - 송신할 1byte의 데이터
  * \note   송신할 데이터를 송신 버퍼에 복사하면서 송신 카운트를 조절하며,
            송신 중이 아니면 송신 인터럽트를 활성화시킨다.
  * \retval None
  */
void Rs485_Send_Byte(Rs485_t * rs, uint8_t byte)
{
  uint32_t  tmp;
  
  tmp = rs->tx_count_in + 1;
  if (tmp >= rs->tx_size)
  {
    tmp = 0;
  }
  
  while (tmp == rs->tx_count_out)
  {
    HAL_Delay(msec(20));
  }
  
  rs->tx_buf[rs->tx_count_in] = byte;
  rs->tx_count_in = tmp;
  
  if (rs->tx_ing == False)
  {
//    Led_On(dbg02_led);
    HAL_GPIO_WritePin(rs->port_txe, rs->init_txe.Pin, rs->on_txe);
    rs->tx_ing = True;
    __HAL_UART_ENABLE_IT(rs->handle, UART_IT_TXE);
    __HAL_UART_ENABLE_IT(rs->handle, UART_IT_TC);
  }
}

/** \brief  RS485 Driver의 송신 인터럽트를 처리한다.
  * \param  rs - RS485 구조체 포인터
  * \note   송신 버퍼에 들어있는 데이터를 1byte씩 전송하고 송신 카운트를 조절하며,
            모두 송신된 경우 송신 인터럽트를 비활성화시킨다.
  * \retval None
  */
void Rs485_IRQHandler_TXE(Rs485_t * rs)
{
  if (rs->tx_count_out != rs->tx_count_in)
  {
    rs->handle->Instance->DR = rs->tx_buf[rs->tx_count_out];
    rs->tx_count_out++;
    if (rs->tx_count_out >= rs->tx_size)
    {
      rs->tx_count_out = 0;
    }
    rs->tx_ing = True;
  }
  else
  {
    rs->tx_ing = False;
    __HAL_UART_DISABLE_IT(rs->handle, UART_IT_TXE);
  }
}

/** \brief  RS485 Driver의 송신 완료 인터럽트를 처리한다.
  * \param  rs - RS485 구조체 포인터
  * \note   
  * \retval None
*/
void Rs485_IRQHandler_TC(Rs485_t * rs)
{
  if (rs->tx_count_out == rs->tx_count_in)
  {
    HAL_GPIO_WritePin(rs->port_txe, rs->init_txe.Pin, rs->off_txe);
//    Led_Off(dbg02_led);
  }
}
/** \} RS485_Driver_Exported_Functions */

/* Private functions ---------------------------------------------------------*/

/** \} RS485_Driver */

/** \} Drivers */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
