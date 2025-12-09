/** ****************************************************************************
  * \file    digital_output.h 
  * \author  Jang Ho Jong
  * \version V0.0.1
  * \date    2023.04.05
  * \brief   Digital Output Device Driver Header
  * ***************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DIGITAL_OUTPUT_H
#define __DIGITAL_OUTPUT_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "typedef.h"

/** \addtogroup Drivers
  * \{ */
  
/** \addtogroup DO_Driver
  * \{ */

/* Exported types ------------------------------------------------------------*/
/** \defgroup DO_Driver_Exported_Types DO 드라이버 공개 타입
  * \{ */

/** \brief DO 구조체 타입 */
typedef struct
{
  char_t             *name;   /**< DO 이름 문자열 */
  GPIO_TypeDef       *port;   /**< DO가 연결된 GPIO 포트. 
                                   GPIOx [x = A ~ I] 값을 가질 수 있다. */
  GPIO_InitTypeDef    init;   /**< DO가 연결된 GPIO 핀 
                                   GPIO_PIN_x [x = 0 ~ 15, All] 값을 가질 수 있다. */
  GPIO_PinState       on;     /**< DO를 켜지게 하는 GPIO 핀 상태 
                                   GPIO_PIN_RESET 혹은 GPIO_PIN_SET 값을 가질 수 있다. */
  GPIO_PinState       off;    /**< DO를 꺼지게 하는 GPIO 핀 상태 
                                   GPIO_PIN_RESET 혹은 GPIO_PIN_SET 값을 가질 수 있다. */
}DO_t;
/** \} DO_Driver_Exported_Types */

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** \defgroup DO_Driver_Exported_Functions DO 드라이버 공개 함수
  * \{ */
void DO_Init(DO_t * dout);
void DO_On(DO_t * dout);
void DO_Off(DO_t * dout);
void DO_Toggle(DO_t * dout);
/** \} DO_Driver_Exported_Functions */

/** \} DO_Driver */

/** \} Drivers */

#endif /* __LED_H */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
