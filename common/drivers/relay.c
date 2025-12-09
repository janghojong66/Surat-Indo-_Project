/** ****************************************************************************
  * \file    relay.c 
  * \author  Jang Ho Jong
  * \version V1.0.0
  * \date    2023.06.10
  * \brief   Relay Device Driver Body
  * ***************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "relay.h"
#include "device.h"

/** \addtogroup Drivers
  * \{ */

/** \defgroup Relay_Driver Relay 드라이버
  * \brief Relay 디바이스를 제어하는 드라이버입니다.
  * \{ */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** \addtogroup Relay_Driver_Exported_Functions
  * \{ */

/** \brief  Relay 디바이스를 초기화한다.
  * \param  rly - Relay 디바이스 구조체 포인터
  * \note   rly의 GPIO 초기화를 수행하고 초기 상태로 끈다.
  * \retval 없음
  */
void Relay_Init(Relay_t * rly)
{
  Periph_Control_GPIO_CLK(rly->port, True);
  HAL_GPIO_Init(rly->port, &rly->init);

  HAL_GPIO_WritePin(rly->port, (u16_t)rly->init.Pin, rly->on);
  rly->state = False;
}

/** \brief  Relay 디바이스를 켠다.
  * \param  rly - Relay 디바이스 구조체 포인터
  * \note   rly 디바이스의 GPIO Pin을 조작하여 켠다.
  * \retval 없음
  */
void Relay_On(Relay_t * rly)
{
//  HAL_GPIO_WritePin(rly->port, rly->init.Pin, rly->on);
  HAL_GPIO_WritePin(rly->port, (u16_t)rly->init.Pin, rly->off);	
  rly->state = True;
}

/** \brief  Relay 디바이스를 끈다.
  * \param  rly - Relay 디바이스 구조체 포인터
  * \note   rly 디바이스의 GPIO Pin을 조작하여 끈다.
  * \retval 없음
  */
void Relay_Off(Relay_t * rly)
{
//  HAL_GPIO_WritePin(rly->port, rly->init.Pin, rly->off);
HAL_GPIO_WritePin(rly->port, (u16_t)rly->init.Pin, rly->on);	
  rly->state = False;
}

/** \brief  Relay 디바이스를 토글시킨다.
  * \param  rly - Relay 디바이스 구조체 포인터
  * \note   rly 디바이스의 GPIO Pin을 조작하여 토글시킨다.
  * \retval 없음
  */
void Relay_Toggle(Relay_t * rly)
{
  HAL_GPIO_TogglePin(rly->port, (u16_t)rly->init.Pin);
  if (rly->state == True)
  {
    rly->state = False;
  }
  else
  {
    rly->state = True;
  }
}

/** \brief  Relay 디바이스의 현재 상태를 가져온다.
  * \param  rly - Relay 디바이스 구조체 포인터
  * \note   현재 상태를 반환한다.
  * \retval True이면 ON상태, False이면 OFF상태
  */
Bool_t Relay_Get_state(Relay_t * rly)
{
  return rly->state;
}
/** \} Relay_Driver_Exported_Functions */

/* Private functions ---------------------------------------------------------*/

/** \} Relay_Driver */

/** \} Drivers */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
