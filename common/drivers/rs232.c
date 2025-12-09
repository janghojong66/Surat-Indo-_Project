/** ****************************************************************************
  * \file    rs232.c
  * \author  Jang Ho Jong
  * \version V1.0.0
  * \date    2023.06.10
  * \brief   RS-232 Device Driver Body
  * ***************************************************************************/
/** \remark  Version History
  * V0.0.1   - 최초 버전 - 2016.05.06
  * V0.0.2   - Rs232_t 타입에 rx_tick 변수 추가
  *            Rs232_Disable_IRQ()
  *            Rs232_Set_Receive_Tick(),
  *            Rs232_CountDown_Receive_Tick() 함수 추가
  * V0.0.3   - Rs232_Printf() 함수 이름을 Printf()로 변경
*/

/* Includes ------------------------------------------------------------------*/
#include "rs232.h"
#include "stm32f4xx_hal_cortex.h"
#include "device.h"

/** \addtogroup Drivers
  * \{ */

/** \defgroup RS232_Driver RS-232 디바이스 드라이버
  * \brief RS-232 디바이스를 제어하는 드라이버 입니다.
  * \{ */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** \addtogroup RS232_Driver_Exported_Functions
  * \{ */

/** \brief  RS232 Driver를 초기화한다.
  * \param  rs - RS232 구조체 포인터
  * \note   rs에 정의된 GPIO와 UART를 초기화하고, 인터럽트를 활성화한다.
  * \retval None
  */
	
#ifndef __NOT_USE_ST__
void Rs232_Init(Rs232_t * rs)
{
  Periph_Control_GPIO_CLK(rs->port_tx,True);
  HAL_GPIO_Init(rs->port_tx, &rs->init_tx);
  Periph_Control_GPIO_CLK(rs->port_rx,True);
  HAL_GPIO_Init(rs->port_rx, &rs->init_rx);
  
  Periph_Control_UART_CLK(rs->handle->Instance,True);
  HAL_UART_Init(rs->handle);
  
  if (rs->usingIRQ != False)
  {
		HAL_NVIC_SetPriority(rs->irqn, rs->irq_priority, 0U);
		HAL_NVIC_EnableIRQ(rs->irqn);
	}
}
#else

void Rs232_Init(Rs232_t * rs)
{
  HAL_ENABLE_GPIO_CLK((rs->port_tx));
  HAL_GPIO_Init(rs->port_tx, &rs->init_tx);
  HAL_ENABLE_GPIO_CLK((rs->port_rx));
  HAL_GPIO_Init(rs->port_rx, &rs->init_rx);

  HAL_ENABLE_UART_CLK((rs->handle->Instance));
  HAL_UART_Init(rs->handle);

  if (rs->usingIRQ != False)
  {
	  HAL_NVIC_SetPriority(rs->irqn, rs->irq_priority, 0U);
	  HAL_NVIC_EnableIRQ(rs->irqn);
  }
}
#endif

/** \brief  RS232 Driver의 인터럽트를 비활성화한다.
  * \param  rs - RS232 구조체 포인터
  * \note   rs의 인터럽트를 비활성화한다.
  * \retval 없음
  */
#ifndef __NOT_USE_ST__
void Rs232_Disable_IRQ(Rs232_t * rs)
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
#endif
/** \brief  RS232 Driver의 수신을 활성화한다.
  * \param  rs - RS232 구조체 포인터
  * \note   rs의 수신 인터럽트와 에러 인터럽트를 활성화한다.
  * \retval None
  */
#ifndef __NOT_USE_ST__
void Rs232_Enable_Receiving(Rs232_t * rs)
{
  /* Enable the UART Parity Error Interrupt */
  __HAL_UART_ENABLE_IT(rs->handle, UART_IT_PE);
  
  /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
  __HAL_UART_ENABLE_IT(rs->handle, UART_IT_ERR);
  
  /* Enable the UART Data Register not empty Interrupt */
  __HAL_UART_ENABLE_IT(rs->handle, UART_IT_RXNE);
}
#endif
/** \brief  RS232 Driver의 수신 인터럽트를 처리한다.
  * \param  rs - RS232 구조체 포인터
  * \note   수신된 데이터를 rs의 수신 버퍼에 저장하고 수신 카운트를 조절한다.
  * \retval 없음
  */
void Rs232_IRQHandler_RXNE(Rs232_t * rs)
{
  u32_t  dr = rs->handle->Instance->DR;
  
  rs->rx_complete = False;
  rs->rx_buf[rs->rx_count] = (u08_t)(dr & (u08_t)0x00FF);
  rs->rx_count++;
  if (rs->rx_count >= rs->rx_size)
  {
    rs->rx_count = 0;
  }
}

/** \brief  RS232의 수신 타이머를 tick으로 설정한다.
  * \param  rs - RS232 구조체 포인터
  * \param  tick - 수신 타이머를 설정할 값 [millisec]
  * \note   수신 인터럽트에서 수신될 때마다 수신 타이머를 설정(갱신)하여
            타이머 만기시 수신 완료로 처리한다.
  * \retval 없음
  */
void Rs232_Set_Receive_Tick(Rs232_t * rs, u32_t tick)
{
  rs->rx_tick = tick;
}

/** \brief  RS232의 수신 타이머를 감소시킨다. 타이머 만기시 수신 완료를 설정한다.
  * \param  rs - RS232 구조체 포인터
  * \note   타이머 인터럽트에서 매 타이머 tick마다 호출된다.
            수신 타이머가 설정되어 있으면 감소시키며 수신 타이머가 0이 되면
            수신 완료를 설정한다.
  * \retval 없음
  */
void Rs232_CountDown_Receive_Tick(Rs232_t * rs)
{
  if (rs->rx_tick != 0U)
  {
    rs->rx_tick--;
    if (rs->rx_tick == 0U)
    {
      rs->rx_complete = True;
    }
  }
}

/** \brief  RS232 Driver의 수신 타임아웃을 처리한다.
  * \param  rs: RS232 구조체 포인터
  * \note   rs의 수신 완료 플래그를 True로 셋한다.
  * \retval None
  */
void Rs232_Receive_TimeOut(Rs232_t * rs)
{
  rs->rx_complete = True;
}

/** \brief  RS232 Driver의 수신 기능을 복원시킨다.
  * \param  rs - RS232 구조체 포인터
  * \note   수신된 데이터가 처리된 후 다시 수신할 수 있도록
            수신 버퍼와 수신 카운트, 수신 완료 플래그를 클리어한다.
  * \retval None
  */
void Rs232_Restore_rx(Rs232_t * rs)
{
  u32_t i = 0;
  
  for (i = 0; i < MAX_SIZE_CMD_BUF; i++)
  {
    rs->rx_buf[i] = 0;
  }
  rs->rx_count = 0;
  rs->rx_complete = False;
}

/** \brief  RS232 Driver를 통해 data를 그 count 만큼 rs로 전송한다.
  * \param  rs - RS232 구조체 포인터
  * \param  data - 송신할 데이터가 저장된 배열
  * \param  count - 송신할 데이터의 수
  * \note   data를 송신 버퍼에 복사하면서 송신 인터럽트를 활성화하여 
  *         송신 버퍼의 데이터가 전송되게끔한다.
  * \retval 없음
  */
void Rs232_Transmit_Data(Rs232_t * rs, u08_t data[], u32_t count)
{
  u32_t    i = 0;
  
  for (i = 0; i < count; i++)
  {
    Rs232_Send_Byte(rs, data[i]);
  }
}

/** \brief  RS232 Driver를 통해 byte를 rs로 전송한다.
  * \param  rs - RS232 구조체 포인터
  * \param  byte - 송신할 1byte의 데이터
  * \note   송신할 데이터를 송신 버퍼에 복사하면서 송신 카운트를 조절하며,
            송신 중이 아니면 송신 인터럽트를 활성화시킨다.
  * \retval None
  */
#ifndef __NOT_USE_ST__
void Rs232_Send_Byte(Rs232_t * rs, u08_t byte)
{
  u32_t  tmp;
  
  tmp = rs->tx_count_in + 1U;
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
    rs->tx_ing = True;
    __HAL_UART_ENABLE_IT(rs->handle, UART_IT_TXE);
  }
}
#endif
/** \brief  RS232 Driver의 송신 인터럽트를 처리한다.
  * \param  rs - RS232 구조체 포인터
  * \note   송신 버퍼에 들어있는 데이터를 1byte씩 전송하고 송신 카운트를 조절하며,
            모두 송신된 경우 송신 인터럽트를 비활성화시킨다.
  * \retval None
  */
#ifndef __NOT_USE_ST__
void Rs232_IRQHandler_TXE(Rs232_t * rs)
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
#endif

/** \} RS232_Driver_Exported_Functions */

/* Private functions ---------------------------------------------------------*/

/** \} RS232_Driver */

/** \} Drivers */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
