/** ****************************************************************************
  * \file    periph_stm32f4xx.c
  * \author  Jang Ho Jong
  * \version V1.0.0
  * \date    2023.06.10
  * \brief   STM32F4xx Peripheral Control Driver Body
  * ***************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "periph_stm32f4xx.h"
#include "device.h"
#ifndef __NOT_USE_ST__
#include <stdarg.h>
#endif
#include <stdio.h>
/** \addtogroup Applications
  * \{ */

/** \defgroup PeriphCtrl 페리페럴 제어
  * \brief STM32F4xx 시리즈 MCU의 페리페럴 제어 기능을 제공합니다.
  * \{ */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** \addtogroup PeriphCtrl_Exported_Functions
  * \{ */

/** \brief  GPIO 클럭을 활성화 또는 비활성화 시킨다.
  * \param  port - 제어할 GPIO port
  * \param  ctrl - True이면 활성화, False이면 비활성화
  * \note   주어진 port에 해당하는 RCC 레지스터를 설정한다.
  * \retval 없음
*/
void Periph_Control_GPIO_CLK(GPIO_TypeDef * port, Bool_t ctrl)
{
  u32_t        i = 0;
  const GPIO_TypeDef * addr[9] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF,
                            GPIOG, GPIOH, GPIOI};
  const u32_t bit[9] = {RCC_AHB1ENR_GPIOAEN, RCC_AHB1ENR_GPIOBEN, RCC_AHB1ENR_GPIOCEN,
                     RCC_AHB1ENR_GPIODEN, RCC_AHB1ENR_GPIOEEN, RCC_AHB1ENR_GPIOFEN,
                     RCC_AHB1ENR_GPIOGEN, RCC_AHB1ENR_GPIOHEN, RCC_AHB1ENR_GPIOIEN};
//  volatile u32_t tmpreg = 0;

  for (i = 0U; i < 9U; i++)
  {
    if (addr[i] == port)
    {
      break;
    }
  }

	if( i < 9U) 
	{
		if (ctrl == True)
		{
			SET_BIT(RCC->AHB1ENR, bit[i]);
		}
		else
		{
			CLEAR_BIT(RCC->AHB1ENR, bit[i]);
		}
	}
//  tmpreg = READ_BIT(RCC->AHB1ENR, bit[i]);
}

/** \brief  UART 클럭을 활성화 또는 비활성화 시킨다.
  * \param  uart - 제어할 UART
  * \param  ctrl - True이면 활성화, False이면 비활성화
  * \note   주어진 uart에 해당하는 RCC 레지스터를 설정한다.
  * \retval 없음
*/
void Periph_Control_UART_CLK(USART_TypeDef * uart, Bool_t ctrl)
{
  uint32_t            i = 0;
  const USART_TypeDef * addr[6] = {USART1, USART2, USART3, UART4, UART5, USART6};
  volatile uint32_t * reg[6] = {&RCC->APB2ENR, &RCC->APB1ENR, &RCC->APB1ENR,
                                &RCC->APB1ENR, &RCC->APB1ENR, &RCC->APB2ENR};
  uint32_t bit[6] = {RCC_APB2ENR_USART1EN, RCC_APB1ENR_USART2EN, RCC_APB1ENR_USART3EN,
                     RCC_APB1ENR_UART4EN, RCC_APB1ENR_UART5EN, RCC_APB2ENR_USART6EN};
  volatile uint32_t tmpreg = 0;

  for (i = 0; i < 6; i++)
  {
    if (uart == addr[i])
    {
      break;
    }
  }

  if (ctrl == True)
  {
    SET_BIT(*(reg[i]), bit[i]);
  }
  else
  {
    CLEAR_BIT(*(reg[i]), bit[i]);
  }
  tmpreg = READ_BIT(*(reg[i]), bit[i]);
}

#ifdef RTE_DEVICE_HAL_UART
/** \brief  Console printf().
  * \param  format - 출력할 데이터 포맷형식
  * \param  ... - 데이터 포맷형식에 들어갈 데이터(가변인자)
  * \retval None
  */
#ifndef __NOT_USE_ST__
void cprintf(const char_t format[], ...)
{
  u32_t  i = 0U;
  u08_t nCh = 0;
  __va_list arglist = {(void *)0, };

  va_start(arglist, format);
  vsprintf((char_t *)serial3->printf_buf, format, arglist);
  va_end(arglist);

  while(serial3->printf_buf[i] != 0U)
  {
    if (i < serial3->tx_size)
    {
      nCh = (u08_t)'\n';
      if (serial3->printf_buf[i] == nCh)
      {
        Rs232_Send_Byte(serial3, (u08_t)'\r');
      }
      nCh = (u08_t)'\r';
      if (serial3->printf_buf[i] == nCh)
      {
        Rs232_Send_Byte(serial3, (u08_t)'\n');
      }
      Rs232_Send_Byte(serial3, serial3->printf_buf[i]);
      i++;
    }
    else
    {
      break;
    }
  }
}
#endif


/** \brief  buf를 그 size만큼 Console로 전송한다.
  * \param  buf - 송신할 데이터가 저장된 배열
  * \param  size - 송신할 데이터의 수
  * \note   data를 송신 버퍼에 복사하면서 송신 인터럽트를 활성화하여
  *         송신 버퍼의 데이터가 전송되게끔한다.
  * \retval 없음
  */
void cprint(u08_t buf[], u32_t size)
{
  Rs232_Transmit_Data(serial3, buf, size);
}


#endif /* RTE_DEVICE_HAL_UART */

/** \} PeriphCtrl_Exported_Functions */

/* Private functions ---------------------------------------------------------*/
/** \addtogroup PeriphCtrl_Private_Functions
  * \{ */
/** \} PeriphCtrl_Private_Functions */

/** \} PeriphCtrl */

/** \} Drivers */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
