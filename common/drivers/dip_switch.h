/** ****************************************************************************
  * \file    dip_switch.h
  * \author  Park Keun Tae
  * \version V0.0.1
  * \date    2016.08.17
  * \brief   Dip Switch Device Driver Header
  * ***************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DIP_SWITCH_H
#define __DIP_SWITCH_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "typedef.h"

/** \addtogroup Drivers
  * \{ */

/** \addtogroup DipSwitch_Driver
  * \{ */

/* Exported types ------------------------------------------------------------*/
/** \defgroup DipSwitch_Driver_Exported_Types Dip Switch 드라이버 공개 타입
  * \{ */

/** \brief Dip Switch 구조체 타입 */
typedef struct
{
  char_t             *name;        /**< Dip Switch 이름 문자열 */
  uint32_t            bits;        /**< Dip Switch의 bit 개수. 4이면 4bit DipSwitch, 8이면 8bit */
  GPIO_TypeDef       *port[3];     /**< Dip Switch의 Bit0 ~ Bit7까지 연결된 GPIO 포트.
                                        GPIOx [x = A ~ I] 값을 가질 수 있다. */
  GPIO_InitTypeDef    init[3];     /**< Dip Switch의 Bit0 ~ Bit7까지 연결된 GPIO 핀
                                        GPIO_PIN_x [x = 0 ~ 15, All] 값을 가질 수 있다. */
  GPIO_PinState       on[3];       /**< Dip Switch의 Bit0 ~ Bit7까지 ON상태에 대한 GPIO 핀 상태
                                        GPIO_PIN_RESET 혹은 GPIO_PIN_SET 값을 가질 수 있다. */
  GPIO_PinState       off[3];      /**< Dip Switch의 Bit0 ~ Bit7까지 OFF상태에 대한 GPIO 핀 상태
                                        GPIO_PIN_RESET 혹은 GPIO_PIN_SET 값을 가질 수 있다. */
  uint8_t             value;       /**< Dip Switch에서 설정되어진 값 */
  uint8_t             prev_v;      /**< 이전 설정값 */
  uint32_t            max_filter;  /**< 최대 입력 필터링 [msec] */
  uint32_t            filtering;   /**< 필터링 */
}DipSW_t;
/** \} DipSwitch_Driver_Exported_Types */

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** \defgroup DipSwitch_Driver_Exported_Functions Dip Switch 드라이버 공개 함수
  * \{ */
void DipSW_Init(DipSW_t * sw);
uint8_t DipSW_Detect_Value(DipSW_t * sw, uint32_t tick_msec, Bool_t reverse);
uint8_t DipSW_Get_value(DipSW_t * sw, Bool_t reverse);
/** \} DipSwitch_Driver_Exported_Functions */

/** \} DipSwitch_Driver */

/** \} Drivers */

#endif /* __DIP_SWITCH_H */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
