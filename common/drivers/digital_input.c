/** ****************************************************************************
  * \file    digital_input.c
  * \author  Jang Ho Jong
  * \version V1.0.0
  * \date    2023.06.10
  * \brief   Digital Input Device Driver Body
  * ***************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "digital_input.h"
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

/** \brief  Digital Input 디바이스를 초기화한다.
  * \param  in - Digital Input 디바이스 구조체 포인터
  * \note   in의 GPIO 초기화를 수행한다.
  * \retval 없음
  */
void DI_Init(DI_t * in)
{
  Periph_Control_GPIO_CLK(in->port, True);
  HAL_GPIO_Init(in->port, &in->init);
}

/** \brief  Digital Input 디바이스의 상태를 가져온다.
  * \param  in - Digital Input 디바이스 구조체 포인터
  * \note   in 디바이스의 GPIO Pin을 읽어서 ON, OFF 상태를 반환한다.
  * \retval True: ON 상태, False: OFF 상태
  */
u08_t DI_Get_State(DI_t * in)
{
	Bool_t ret_val;

  if (HAL_GPIO_ReadPin(in->port, (u16_t)in->init.Pin) == in->off)
  {
	  ret_val = True;
  }
  else
  {
	  ret_val = False;
  }  
	return ret_val;
}

/** \brief  Digital Input 디바이스의 상태를 필터링하여 현재 상태를 확정한다.
  * \param  in - Digital Input 디바이스 구조체 포인터
  * \param  tick_msec - 필터링 틱 [msec]
  * \note   필터링 틱이 최대 필터링 시간보다 크다면 필터링할 수 없으므로 종료.
            in의 실시간 상태가 현재 상태와 다른 경우 필터링하여
            필터링 시간동안 계속 다르다면 실시간 상태를 현재 상태로 갱신한다.
  * \retval 없음
*/
void DI_Filter_State(DI_t * in, uint32_t tick_msec)
{
  Bool_t    state;
  
  if (tick_msec > in->max_filter)
  {
    cprintf("Filtering Tick is not valid (%dmsec) [ > %d]\n",
                    tick_msec, in->max_filter);
    return;
  }
  
  state = DI_Get_State(in);
  if (in->curr_state != state)
  {
    in->filtering -= tick_msec;
    if (in->filtering == 0)
    {
      in->curr_state = state;
      in->filtering = in->max_filter;
    }
  }
  else
  {
    in->filtering = in->max_filter;
  }
}
/** \} DI_Driver_Exported_Functions */

/* Private functions ---------------------------------------------------------*/

/** \} DI_Driver */

/** \} Drivers */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
