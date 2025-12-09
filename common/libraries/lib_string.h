/** ****************************************************************************
  * \file    lib_string.h
  * \author  Jang Ho Jong
  * \version V0.0.1
  * \date    2023.04.05
  * \brief   String Library Header
  * ***************************************************************************/
/** \remark Version History
  * V0.0.1  - 최초 버전 - 2016.05.06
  * V0.0.2  - String_Convert_IntegerToString() 함수 추가 - 2016.05.30
  * V0.0.3  - String_Convert_IntegerToString() 함수에 digit, firstzero 인자 추가
  * V0.0.4  - 2019.02.18 
              Char_Compare_woCase() 함수에서 알파벳만 대소 구별없이 비교하도록 보완
*/
/*
  - String 라이브러리

  - 개념:

  - 동작:

  - 고장:

  - 모니터링:
    없음.

  - String 접두어를 사용한다.

  - 사용법
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LIB_STRING_H
#define LIB_STRING_H

/* Includes ------------------------------------------------------------------*/
#include "typedef.h"

/** \addtogroup Libraries
  * \{ */

/** \addtogroup Lib_String
  * \{ */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/** \defgroup Lib_String_Exported_Constants String 라이브러리 공개 상수
  * \{ */
/** \brief 문자열 비교 후 결과 */
//enum E_SAME_DIFF
//{
//  SAME = 0U,             /**< 비교 후 일치 */
//  DIFF = ~(SAME),       /**< 비교 후 불일치 */
//};
#define		SAME		0U
#define   DIFF    1U
/** \} Lib_String_Exported_Constants */

/* Exported macro ------------------------------------------------------------*/
/** \defgroup Lib_String_Exported_Macros String 라이브러리 공개 매크로
  * \{ */

/** \} Lib_String_Exported_Macros */
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** \defgroup Lib_String_Exported_Functions String 라이브러리 공개 함수
  * \{ */
void String_Convert_toDecimalFormat(uint64_t num, char_t str[]);
void String_Convert_toUpperCase(char_t str[]);
int32_t String_Convert_toInteger(char_t str[]);
void String_Convert_IntegerToString(char_t str[], uint32_t integer, uint32_t digit, Bool_t firstzero);
uint8_t Char_Convert_toHex(uint8_t ch);
Bool_t String_Convert_StringToHex(char_t str[], uint32_t *hex);
char_t* String_Extract_Word(char_t str[], char_t* next[], char_t separator);
uint32_t String_Compare(char_t str1[], char_t str2[]);
uint32_t Char_Compare_woCase(char_t ch1, char_t ch2);
uint32_t String_Compare_woCase(char_t str1[], char_t str2[]);
uint32_t String_Compare_woCase_wLength(char_t str1[], const char_t str2[], uint32_t length);
uint32_t String_Get_Length(const char_t str[]);

/** \} Lib_String_Exported_Functions */

/** \} Lib_String */

/** \} Libraries */

#endif  /* LIB_STRING_H */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
