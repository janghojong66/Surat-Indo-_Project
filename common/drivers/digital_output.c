/** ****************************************************************************
  * \file    digital_output.c 
  * \author  Jang Ho Jong
  * \version V1.0.0
  * \date    2023.06.10
  * \brief   Digital Output Device Driver Body
  * ***************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "digital_output.h"
#include "device.h"

/** \addtogroup Drivers
  * \{ */

/** \defgroup DO_Driver DO 드라이버
  * \brief DO 디바이스를 제어하는 드라이버입니다.
  * \{ */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** \addtogroup DO_Driver_Exported_Functions
  * \{ */

/** \brief  DO 디바이스를 초기화한다.
  * \param  dout - DO 디바이스 구조체 포인터
  * \note   dout의 GPIO 초기화를 수행하고 초기 상태로 끈다.
  * \retval 없음
  */
void DO_Init(DO_t * dout)
{
  Periph_Control_GPIO_CLK(dout->port, True);
  HAL_GPIO_Init(dout->port, &dout->init);

	  HAL_GPIO_WritePin(dout->port, (u16_t)dout->init.Pin, dout->on);
}

/** \brief  DO 디바이스를 켠다.
  * \param  dout - DO 디바이스 구조체 포인터

  * \note   dout 디바이스의 GPIO Pin을 조작하여 켠다.
  * \retval 없음
  */
void DO_On(DO_t * dout)
{

	  HAL_GPIO_WritePin(dout->port, (u16_t)dout->init.Pin, dout->on);
}

/** \brief  DO 디바이스를 끈다.
  * \param  dout - DO 디바이스 구조체 포인터
  * \note   dout 디바이스의 GPIO Pin을 조작하여 끈다.
  * \retval 없음
  */
void DO_Off(DO_t * dout)
{
		  HAL_GPIO_WritePin(dout->port, (u16_t)dout->init.Pin, dout->off);

}

/** \brief  DO 디바이스를 토글시킨다.
  * \param  dout - DO 디바이스 구조체 포인터
  * \note   dout 디바이스의 GPIO Pin을 조작하여 토글시킨다.
  * \retval 없음
  */
void DO_Toggle(DO_t * dout)
{
  HAL_GPIO_TogglePin(dout->port, (u16_t)dout->init.Pin);
}
/** \} DO_Driver_Exported_Functions */

/* Private functions ---------------------------------------------------------*/

/** \} DO_Driver */

/** \} Drivers */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
