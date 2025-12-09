/** ****************************************************************************
  * \file    digital_input.h
  * \author  Jang Ho Jong
  * \version V0.0.1
  * \date    2023.04.05
  * \brief   Digital Input Device Driver Header
  * ***************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DIGITAL_INPUT_H
#define __DIGITAL_INPUT_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "typedef.h"

/** \addtogroup Drivers
  * \{ */

/** \addtogroup DI_Driver
  * \{ */

/* Exported types ------------------------------------------------------------*/
/** \defgroup DI_Driver_Exported_Types Digital Input 드라이버 공개 타입
  * \{ */

/** \brief Digital Input 구조체 타입 */
typedef struct
{
  char_t             *name;   /**< Digital Input 이름 문자열 */
  GPIO_TypeDef       *port;   /**< Digital Input가 연결된 GPIO 포트.
                                   GPIOx [x = A ~ I] 값을 가질 수 있다. */
  GPIO_InitTypeDef    init;   /**< Digital Input가 연결된 GPIO 핀
                                   GPIO_PIN_x [x = 0 ~ 15, All] 값을 가질 수 있다. */
  GPIO_PinState       on;     /**< Digital Input ON상태에 대한 GPIO 핀 상태
                                   GPIO_PIN_RESET 혹은 GPIO_PIN_SET 값을 가질 수 있다. */
  GPIO_PinState       off;    /**< Digital Input OFF상태에 대한 GPIO 핀 상태
                                   GPIO_PIN_RESET 혹은 GPIO_PIN_SET 값을 가질 수 있다. */
	Bool_t              curr_state;
  Bool_t              prev_state;
  u32_t              max_filter;
  u32_t              filtering;
  Bool_t             Sensor_OSSD;
}DI_t;
/** \} DI_Driver_Exported_Types */

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** \defgroup DI_Driver_Exported_Functions Digital Input 드라이버 공개 함수
  * \{ */
void DI_Init(DI_t * in);
u08_t DI_Get_State(DI_t * in);
void DI_Filter_State(DI_t * in, u32_t tick_msec);
/** \} DI_Driver_Exported_Functions */

/** \} DI_Driver */

/** \} Drivers */

#endif /* __DIGITAL_INPUT_H */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
