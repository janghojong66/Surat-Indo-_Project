/** ****************************************************************************
  * \file    gg712s_sensor.c
  * \author  Jang Ho Jong
  * \version V1.0.0
  * \date    2024.07.10
  * \brief   Safety Sensor Digital Input Device Driver Body
  * ***************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "gg712s_sensor.h"
#include "device.h"

/** \addtogroup Drivers
  * \{ */

/** \defgroup DI_Driver Digital Input 드라이버
  * \brief Digital Input 디바이스를 제어하는 드라이버입니다.
  * \{ */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** \addtogroup DI_Driver_Exported_Functions
  * \{ */

/** \brief  Sensor Digital Input 디바이스를 초기화한다.
  * \param  sin - Digital Input 디바이스 구조체 포인터
  * \note   sin의 GPIO 초기화를 수행한다.
  * \retval 없음
  */
void SDI_Init(SDI_t * sin)
{
  Periph_Control_GPIO_CLK(sin->port1, True);
  HAL_GPIO_Init(sin->port1, &sin->init1);
  Periph_Control_GPIO_CLK(sin->port2, True);
  HAL_GPIO_Init(sin->port2, &sin->init2);
}

/** \brief  Sensor Digital Input 디바이스의 상태를 필터링하여 현재 상태를 확정한다.
  * \param  sin - Sensor Digital Input 디바이스 구조체 포인터
  * \param  tick_msec - 필터링 틱 [msec]
  * \note   필터링 틱이 최대 필터링 시간보다 크다면 필터링할 수 없으므로 종료.
            sin의 실시간 상태가 현재 상태와 다른 경우 필터링하여
            필터링 시간동안 계속 다르다면 실시간 상태를 현재 상태로 갱신한다.
  * \retval 없음
*/
void SDI_Filter_State(SDI_t * sin, u32_t check_time)
{
  bool_t    state1,state2;
  
  if (check_time > sin->Max_filter)
  {
    cprintf("Filtering Tick is not valid (%dmsec) [ > %d]\n", check_time, sin->Max_filter);
    return;
  }
  else 
  {
    while(check_time)
    { 
      state1 = (bool_t)(HAL_GPIO_ReadPin(sin->port1, (u16_t)sin->init1.Pin) == sin->off1);
      state2 = (bool_t)(HAL_GPIO_ReadPin(sin->port2, (u16_t)sin->init2.Pin) == sin->off2);
//      cprintf("State1 = %s\n",(state1  == True) ? "1" : "0");
//      cprintf("State2 = %s\n",(state2  == True) ? "1" : "0");
      
      if((sin->Curr_state1 != state1) && (sin->Curr_state2 != state2))
      {
              cprintf("State1 = %s\n",(state1  == True) ? "1" : "0");
      cprintf("State2 = %s\n",(state2  == True) ? "1" : "0");
//        sin->Filtering -= check_time;
        check_time--;
        if (check_time == 0U)
        {
          sin->Sensor_OSSD1 = state1;
          sin->Curr_state1 = state1; 
          sin->Sensor_OSSD2 = state2;
          sin->Curr_state2 = state2;           
          sin->Filtering = sin->Max_filter;
        }
      }
      else
      {
        sin->Filtering = sin->Max_filter;
      }
    }
  }
}

/** \} SDI_Driver_Exported_Functions */

/* Private functions ---------------------------------------------------------*/

/** \} SDI_Driver */

/** \} Drivers */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
