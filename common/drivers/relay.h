/** ****************************************************************************
  * \file    relay.h 
  * \author  Jang Ho Jong
  * \version V0.0.1
  * \date    2023.04.05
  * \brief   Relay Device Driver Header
  * ***************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RELAY_H
#define __RELAY_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "typedef.h"

/** \addtogroup Drivers
  * \{ */
  
/** \addtogroup Relay_Driver
  * \{ */

/* Exported types ------------------------------------------------------------*/
/** \defgroup Relay_Driver_Exported_Types Relay 드라이버 공개 타입
  * \{ */

/** \brief Relay 구조체 타입 */
typedef struct
{
  char_t             *name;   /**< Relay 이름 문자열 */
  GPIO_TypeDef       *port;   /**< Relay가 연결된 GPIO 포트. 
                                   GPIOx [x = A ~ I] 값을 가질 수 있다. */
  GPIO_InitTypeDef    init;   /**< Relay가 연결된 GPIO 핀 
                                   GPIO_PIN_x [x = 0 ~ 15, All] 값을 가질 수 있다. */
  GPIO_PinState       on;     /**< Relay를 켜지게 하는 GPIO 핀 상태 
                                   GPIO_PIN_RESET 혹은 GPIO_PIN_SET 값을 가질 수 있다. */
  GPIO_PinState       off;    /**< Relay를 꺼지게 하는 GPIO 핀 상태 
                                   GPIO_PIN_RESET 혹은 GPIO_PIN_SET 값을 가질 수 있다. */
  Bool_t              state;  /**< 현재 Relay 상태 True면 ON, False면 OFF */
}Relay_t;
/** \} Relay_Driver_Exported_Types */

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** \defgroup Relay_Driver_Exported_Functions Relay 드라이버 공개 함수
  * \{ */
void Relay_Init(Relay_t * rly);
void Relay_On(Relay_t * rly);
void Relay_Off(Relay_t * rly);
void Relay_Toggle(Relay_t * rly);
Bool_t Relay_Get_state(Relay_t * rly);
/** \} Relay_Driver_Exported_Functions */

/** \} Relay_Driver */

/** \} Drivers */

#endif /* __RELAY_H */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
