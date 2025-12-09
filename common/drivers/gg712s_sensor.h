/** ****************************************************************************
  * \file    sensor digital_input.h
  * \author  Jang Ho Jong
  * \version V0.0.1
  * \date    2024.07.15
  * \brief   Sensor Digital Input Device Driver Header
  * ***************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GG712S_SENSOR_H
#define __GG712S_SENSOR_H

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
char_t	*name;
GPIO_TypeDef       *port1;                                     
GPIO_InitTypeDef    init1;
GPIO_PinState       on1;     /**< Digital Input ON상태에 대한 GPIO 핀 상태
                                   GPIO_PIN_RESET 혹은 GPIO_PIN_SET 값을 가질 수 있다. */
GPIO_PinState       off1;    /**< Digital Input OFF상태에 대한 GPIO 핀 상태
                                   GPIO_PIN_RESET 혹은 GPIO_PIN_SET 값을 가질 수 있다. */  
GPIO_TypeDef       *port2;                                     
GPIO_InitTypeDef    init2;
GPIO_PinState       on2;     /**< Digital Input ON상태에 대한 GPIO 핀 상태
                                   GPIO_PIN_RESET 혹은 GPIO_PIN_SET 값을 가질 수 있다. */
GPIO_PinState       off2;    /**< Digital Input OFF상태에 대한 GPIO 핀 상태
                                   GPIO_PIN_RESET 혹은 GPIO_PIN_SET 값을 가질 수 있다. */    
u08_t   Sensor_flag; 	
u08_t   Sensor_Counter;  
u16_t	  Sensor_Voltage;
Bool_t  Sensor_OSSD1;
Bool_t  Sensor_OSSD2;
Bool_t  Curr_state1;
Bool_t  Prev_state1;
Bool_t  Curr_state2;
Bool_t  Prev_state2;
u32_t   Max_filter;
u32_t   Filtering;  
}SDI_t;
/** \} DI_Driver_Exported_Types */

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** \defgroup DI_Driver_Exported_Functions Digital Input 드라이버 공개 함수
  * \{ */
void SDI_Init(SDI_t * sin);
void SDI_Filter_State(SDI_t * sin, u32_t tick_msec);
//void SDI_Filter_State2(SDI_t * sin, u32_t tick_msec);
/** \} DI_Driver_Exported_Functions */

/** \} DI_Driver */

/** \} Drivers */

#endif /* __DIGITAL_INPUT_H */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/