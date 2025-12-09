/** ****************************************************************************
  * \file    led.h 
  * \author  Jang Ho Jong
  * \version V0.0.1
  * \date    2023.04.05
  * \brief   LED Device Driver Header
  * ***************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LED_H
#define __LED_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "typedef.h"

/** \addtogroup Drivers
  * \{ */
  
/** \addtogroup LED_Driver
  * \{ */

/* Exported types ------------------------------------------------------------*/
/** \defgroup LED_Driver_Exported_Types LED 드라이버 공개 타입
  * \{ */

/** \brief LED 구조체 타입 */
typedef struct
{
  char_t             *name;   /**< LED 이름 문자열 */
  GPIO_TypeDef       *port;   /**< LED가 연결된 GPIO 포트. 
                                   GPIOx [x = A ~ I] 값을 가질 수 있다. */
  GPIO_InitTypeDef    init;   /**< LED가 연결된 GPIO 핀 
                                   GPIO_PIN_x [x = 0 ~ 15, All] 값을 가질 수 있다. */
  GPIO_PinState       on;     /**< LED를 켜지게 하는 GPIO 핀 상태 
                                   GPIO_PIN_RESET 혹은 GPIO_PIN_SET 값을 가질 수 있다. */
  GPIO_PinState       off;    /**< LED를 꺼지게 하는 GPIO 핀 상태 
                                   GPIO_PIN_RESET 혹은 GPIO_PIN_SET 값을 가질 수 있다. */
}Led_t;
/** \} LED_Driver_Exported_Types */

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** \defgroup LED_Driver_Exported_Functions LED 드라이버 공개 함수
  * \{ */
void Led_Init(Led_t * led);
void Led_On(Led_t * led);
void Led_Off(Led_t * led);
void Led_Toggle(Led_t * led);
/** \} LED_Driver_Exported_Functions */

/** \} LED_Driver */

/** \} Drivers */

#endif /* __LED_H */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
