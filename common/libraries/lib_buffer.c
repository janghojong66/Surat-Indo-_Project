/** ****************************************************************************
  * \file    lib_buffer.c
  * \author  Jang Ho Jong
  * \version V1.0.0
  * \date    2023.06.10
  * \brief   Buffer Library Body
  * ***************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "lib_buffer.h"
#include "device.h"

/** \addtogroup Libraries
  * \{ */

/** \defgroup Lib_Buffer Buffer 라이브러리
  * \brief Buffer 처리 관련 라이브러리입니다.
  * \{ */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** \addtogroup Lib_Buffer_Exported_Functions
  * \{ */

/** \brief  dst 버퍼에 src 버퍼를 length만큼 복사한다.
  * \param  dst - 타겟 버퍼
  * \param  src - 원본 버퍼
  * \param  length - 복사할 길이
  * \note   length만큼 src의 내용을 dst에 순차적으로 대입한다.
  * \retval void
  */
void Buffer08_Copy(u08_t dst[], u08_t src[], u32_t length)
{
  u32_t  i = 0U;

  for (i = 0U; i < length; ++i)
  {
    dst[i] = src[i];
  }
}

/** \brief  dst 버퍼에 src버퍼를 '\0'가 나올때까지 복사한다.
  * \param  dst - 타겟 버퍼
  * \param  src - 원본 버퍼
  * \note   src의 내용에 '\0'이 나타나기 전까지 src의 내용을 dst에 대입한다.
  * \retval void
  */
u32_t Buffer08_Copy_TillZero(u08_t dst[], u08_t src[])
{
  u32_t  i_src = 0U;
  u32_t  i_dst = 0U;

#ifdef __NOT_USE_ST__

  u08_t nCh = (u08_t)'\0';

  while (src[i_src] != nCh)
#else
  while ((src[i_src] != (u08_t)'\0') || ((i_src <= 30U) || (i_dst <= 30U)))
#endif
  {

			dst[i_dst] = src[i_src];
			i_src++;
			i_dst++;
  }

  return (u32_t)i_src;
}

/** \brief  dst 버퍼에 length만큼 value값으로 세트한다.
  * \param  dst - 타겟 버퍼
  * \param  value - 대입할 값
  * \param  length - 대입할 길이
  * \note   length만큼 value를 dst 버퍼에 순차적으로 대입한다.
  * \retval void
  */
void Buffer08_Set(u08_t dst[], u08_t value, u32_t length)
{
  u32_t  i = 0U;

  for (i = 0U; i < length; i++)
  {
    dst[i] = value;
  }
}

/** \} Lib_Buffer_Exported_Functions */

/* Private functions ---------------------------------------------------------*/

/** \} Lib_Buffer */

/** \} Libraries */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
