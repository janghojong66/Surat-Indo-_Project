/** ****************************************************************************
  * \file    led.c 
  * \author  Jang Ho Jong
  * \version V1.0.0
  * \date    2023.06.10
  * \brief   LED Device Driver Body
  * ***************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "led.h"
#include "device.h"

/** \addtogroup Drivers
  * \{ */

/** \defgroup LED_Driver LED 드라이버
  * \brief LED 디바이스를 제어하는 드라이버입니다.
  * \{ */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** \addtogroup LED_Driver_Exported_Functions
  * \{ */

/** \brief  led 디바이스를 초기화한다.
  * \param  led - LED 디바이스 구조체 포인터
  * \note   led의 GPIO 초기화를 수행하고 초기 상태로 끈다.
  * \retval 없음
  */
void Led_Init(Led_t * led)
{
  Periph_Control_GPIO_CLK(led->port, True);
  HAL_GPIO_Init(led->port, &led->init);

  HAL_GPIO_WritePin(led->port, (u16_t)led->init.Pin, led->off);
}

/** \brief  led 디바이스를 켠다.
  * \param  led - LED 디바이스 구조체 포인터
  * \note   led 디바이스의 GPIO Pin을 조작하여 LED를 켠다.
  * \retval 없음
  */
void Led_On(Led_t * led)
{
  HAL_GPIO_WritePin(led->port, (u16_t)led->init.Pin, led->on);
}

/** \brief  led 디바이스를 끈다.
  * \param  led - LED 디바이스 구조체 포인터
  * \note   led 디바이스의 GPIO Pin을 조작하여 LED를 끈다.
  * \retval 없음
  */
void Led_Off(Led_t * led)
{
  HAL_GPIO_WritePin(led->port, (u16_t)led->init.Pin, led->off);
}

/** \brief  led 디바이스를 토글시킨다.
  * \param  led - LED 디바이스 구조체 포인터
  * \note   led 디바이스의 GPIO Pin을 조작하여 LED를 토글시킨다.
  * \retval 없음
  */
void Led_Toggle(Led_t * led)
{
  HAL_GPIO_TogglePin(led->port, (u16_t)led->init.Pin);
}
/** \} LED_Driver_Exported_Functions */

/* Private functions ---------------------------------------------------------*/

/** \} LED_Driver */

/** \} Drivers */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
