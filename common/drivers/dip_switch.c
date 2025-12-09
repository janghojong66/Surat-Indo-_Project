/** ****************************************************************************
  * \file    dip_switch.c
  * \author  Park Keun Tae
  * \version V0.0.1
  * \date    2016.08.17
  * \brief   Dip Switch Device Driver Body
  * ***************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "dip_switch.h"
#include "device.h"

/** \addtogroup Drivers
  * \{ */

/** \defgroup DipSwitch_Driver DipSwitch 드라이버
  * \brief Dip Switch 디바이스를 제어하는 드라이버입니다.
  * \{ */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** \addtogroup DipSwitch_Driver_Exported_Functions
  * \{ */

/** \brief  Dip Switch 디바이스를 초기화한다.
  * \param  sw - Dip Switch 디바이스 구조체 포인터
  * \note   sw의 GPIO 초기화를 수행한다.
  * \retval 없음
  */
void DipSW_Init(DipSW_t * sw)
{
  uint32_t i;

  for (i = 0; i < sw->bits; i++)
  {
    Periph_Control_GPIO_CLK(sw->port[i], True);
    HAL_GPIO_Init(sw->port[i], &sw->init[i]);
  }
}

/** \brief  Dip Switch의 입력을 감지하여 필터링한 후 sw->value에 대입해 둔다.
  * \param  sw - Dip Switch 디바이스 구조체 포인터 
  * \param  tick_msec - msec 단위의 tick
  * \param  reverse - 입력값의 반전 여부
  * \note   입력 상태를 필터링한 뒤 sw->value에 대입하고 반환한다.
  * \retval 필터링된 입력값
  */
uint8_t DipSW_Detect_Value(DipSW_t * sw, uint32_t tick_msec, Bool_t reverse)
{
  uint8_t value = 0;
  
  value = DipSW_Get_value(sw, reverse);
  if (sw->prev_v != value)
  {
    sw->filtering -= tick_msec;
    if (sw->filtering == 0)
    {
      sw->value = value;
      sw->prev_v = value;
    }
  }
  else
  {
    sw->filtering = sw->max_filter;
  }
  
  return sw->value;
}

/** \brief  Dip Switch 디바이스의 입력값를 가져온다.
  * \param  sw - Dip Switch 디바이스 구조체 포인터
  * \param  reverse - True이면 입력값을 반전시킨다.
  * \note   sw 디바이스의 GPIO Pin을 읽어서 값으로 변환하여 반환한다..
  * \retval Dip Switch의 입력값
  */
uint8_t DipSW_Get_value(DipSW_t * sw, Bool_t reverse)
{
  uint32_t i;
  uint8_t level = 0;
  uint8_t value = 0;

  for (i = 0; i < sw->bits; i++)
  {
    if (HAL_GPIO_ReadPin(sw->port[i], sw->init[i].Pin) == sw->on[i])
    {
      level = 1;
    }
    else
    {
      level = 0;
    }
//    cprintf("%d %d\n", i, level);
    value |= (uint8_t)(level << i);
  }

  if (reverse == True)
  {
    value = (uint8_t)((uint32_t)((1 << sw->bits) - 1) - value);
  }

  return value;
}
/** \} DipSwitch_Driver_Exported_Functions */

/* Private functions ---------------------------------------------------------*/

/** \} DipSwitch_Driver */

/** \} Drivers */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
