/** ****************************************************************************
  * \file    lib_buffer.h
  * \author  Jang Ho Jong
  * \version V0.0.1
  * \date    2023.04.05
  * \brief   Buffer Library Header
  * ***************************************************************************/
/*
  - Buffer 라이브러리

  - 개념:

  - 동작:

  - 고장:

  - 모니터링:
    없음.

  - Buffer 접두어를 사용한다.

  - 사용법
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LIB_BUFFER_H
#define __LIB_BUFFER_H

/* Includes ------------------------------------------------------------------*/
#include "typedef.h"


/** \addtogroup Libraries
  * \{ */

/** \addtogroup Lib_Buffer
  * \{ */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** \defgroup Lib_Buffer_Exported_Functions Buffer 라이브러리 공개 함수
  * \{ */
extern void Buffer08_Copy(u08_t dst[], u08_t src[], u32_t length);
extern uint32_t Buffer08_Copy_TillZero(u08_t dst[], u08_t src[]);
extern void Buffer08_Set(u08_t dst[], u08_t value, u32_t length);
extern int32_t Buffer08_Compare(u08_t dst[], u08_t src[], u32_t length);
extern Bool_t Buffer08_Verify(u08_t dst[], u08_t value, u32_t length);
extern uint32_t BIT_Reverse32(u32_t data);
extern void   View_Buf32(u32_t buf[], u32_t size, u32_t lineFeed, char_t * form);
extern void   View_Buf16(u16_t buf[], u32_t size, u32_t lineFeed, char_t * form);
extern void   View_Buf08(u08_t buf[], u32_t size, u32_t lineFeed, char_t * form);
/** \} Lib_Buffer_Exported_Functions */

/** \} Lib_Buffer */

/** \} Libraries */

#endif /* __LIB_BUFFER_H */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
