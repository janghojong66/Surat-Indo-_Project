/** ****************************************************************************
  * \file    watchdog.c 
  * \author  Jang Ho Jong
  * \version V1.0.0
  * \date    2023.06.10
  * \brief   Watchdog Device Driver Body
  * ***************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "watchdog.h"
#include "device.h"
 

/** \addtogroup Drivers
  * \{ */

/** \defgroup Watchdog_Driver Watchdog 드라이버
  * \brief Watchdog 디바이스를 제어하는 드라이버입니다.
  * \{ */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** \addtogroup Watchdog_Driver_Exported_Functions
  * \{ */

/** \brief  Watchdog 디바이스를 초기화한다.
  * \param  wdog - Watchdog 디바이스 구조체 포인터
  * \note   wdog의 GPIO 초기화를 수행하고 초기 상태로 끈다.
  * \retval 없음
  */
void Watchdog_Init(Watchdog_t * wdog)
{
  Periph_Control_GPIO_CLK(wdog->port, True);
  HAL_GPIO_Init(wdog->port, &wdog->init);

  HAL_GPIO_WritePin(wdog->port, (u16_t)wdog->init.Pin, GPIO_PIN_RESET);
}

/** \brief  Watchdog 디바이스를 토글시킨다.
  * \param  wdog - Watchdog 디바이스 구조체 포인터
  * \note   wdog 디바이스의 GPIO Pin을 토글시킨다.
  * \retval 없음
  */
void Watchdog_Toggle(Watchdog_t * wdog)
{
  if (wdog->lock != True)
	{
		HAL_GPIO_TogglePin(wdog->port, (u16_t)wdog->init.Pin);
	}
}

/** \brief  Watchdog을 잠근다.
  * \param  wdog - Watchdog 디바이스 구조체 포인터
  * \note   wdog strobe가 출력되지 않도록 lock을 건다.
  * \retval 없음
*/
void Watchdog_Lock(Watchdog_t * wdog)
{
  wdog->lock = True;
}

/** \brief  Watchdog을 잠금해제한다.
  * \param  wdog - Watchdog 디바이스 구조체 포인터
  * \note   wdog strobe가 출력되도록 lock을 푼다.
  * \retval 없음
*/
void Watchdog_Unlock(Watchdog_t * wdog)
{
  wdog->lock = False;
}
/** \} Watchdog_Driver_Exported_Functions */

/* Private functions ---------------------------------------------------------*/

/** \} Watchdog_Driver */

/** \} Drivers */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
