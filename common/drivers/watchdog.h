/** ****************************************************************************
  * \file    watchdog.h 
  * \author  Jang Ho Jong
  * \version V0.0.1
  * \date    2023.04.05
  * \brief   Watchdog Device Driver Header
  * ***************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WATCHDOG_H
#define __WATCHDOG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "typedef.h"

/** \addtogroup Drivers
  * \{ */
  
/** \addtogroup Watchdog_Driver
  * \{ */

/* Exported types ------------------------------------------------------------*/
/** \defgroup Watchdog_Driver_Exported_Types Watchdog 드라이버 공개 타입
  * \{ */

/** \brief Watchdog 구조체 타입 */
typedef struct
{
  char_t             *name;   /**< Watchdog 이름 문자열 */
  GPIO_TypeDef       *port;   /**< Watchdog가 연결된 GPIO 포트. 
                                   GPIOx [x = A ~ I] 값을 가질 수 있다. */
  GPIO_InitTypeDef    init;   /**< Watchdog가 연결된 GPIO 핀 
                                   GPIO_PIN_x [x = 0 ~ 15, All] 값을 가질 수 있다. */
  Bool_t              lock;   /**< Watchdog이 동작하지 않도록 잠금 */
}Watchdog_t;
/** \} Watchdog_Driver_Exported_Types */

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** \defgroup Watchdog_Driver_Exported_Functions Watchdog 드라이버 공개 함수
  * \{ */
void Watchdog_Init(Watchdog_t * wdog);
void Watchdog_Toggle(Watchdog_t * wdog);
void Watchdog_Lock(Watchdog_t * wdog);
void Watchdog_Unlock(Watchdog_t * wdog);
/** \} Watchdog_Driver_Exported_Functions */

/** \} Watchdog_Driver */

/** \} Drivers */

#endif /* __WATCHDOG_H */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
