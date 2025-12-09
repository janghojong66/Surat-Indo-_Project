/** ****************************************************************************
  * \file    lib_string.c
  * \author  Jang Ho Jong
  * \version V1.0.0
  * \date    2023.06.10
  * \brief   String Library Body
  * ***************************************************************************/
/** \remark Version History
  * V0.0.1  - 최초 버전 - 2016.05.06

*/
/* Includes ------------------------------------------------------------------*/
#include "lib_string.h"
#include <stdio.h> 

/** \addtogroup Libraries
  * \{ */

/** \defgroup Lib_String String 라이브러리
  * \brief 문자열 처리 관련 라이브러리입니다.
  * \{ */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if 0
static char_t _numberSystem[16] = "0123456789ABCDEF";
#endif

/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
#if 0
static void safestrrev(char_t buffer[], const u32_t bufferSize, const u32_t strlen);
static u32_t negateBuffer(char_t buf[], const u32_t buf_size, const u32_t strlen, const u32_t radix);
static int32_t twosNegation(const int32_t x, const u32_t radix);
#endif

/* Exported functions --------------------------------------------------------*/
/** \addtogroup Lib_String_Exported_Functions
  * \{ */

/** \brief  num값을 십진수 문자열(, 구분자 사용) 형식으로 변환하여 str에 저장한다.
  * \param  num - 변환할 숫자값
  * \param  str - 변환한 십진수 문자열이 저장될 문자열 버퍼
  * \note   num값의 자릿수에 따라 문자열로 변환하여 str에 저장한다.
  * \retval 없음
  */
void String_Convert_toDecimalFormat(uint64_t num, char_t str[])
{
  uint32_t  v = 0U;
  char_t*   format[5] = {"%d,", "%03d,", "%03d,%03d", "%d,%03d", "%d"};

//  if (value >= (uint64_t)1e12) 
  /* 1,000,000,000,000 보다 클 경우 */
  if (num >= (uint64_t)1000000000000) 
  {
    v = (uint32_t)(num / (uint64_t)1000000000000);
    str += sprintf (str, format[0], v);
    num %= (uint64_t)1000000000000;
    v = (uint32_t)(num / (uint64_t)1000000000);
    str += sprintf (str, format[1], v);
    num %= (uint64_t)1000000000;
    v = (uint32_t)(num / (uint64_t)1000000);
    str += sprintf (str, format[1], v);
    num %= (uint64_t)1000000;
    sprintf (str, format[2], (uint32_t)(num / 1000U), (uint32_t)(num % 1000U));
    return;
  }
  /* 1,000,000,000 보다 클 경우 */
  if(num >= (uint64_t)1000000000)
  {
    str += sprintf (str, format[0], num / (uint64_t)1000000000);
    num %= (uint64_t)1000000000;
    str += sprintf (str, format[1], num /(uint64_t)1000000);
    num %= (uint64_t)1000000;
    sprintf (str, format[2], (uint32_t)(num / 1000U), (uint32_t)(num % 1000U));
    return;
  }
  /* 1,000,000 보다 클 경우 */
  if (num >= (uint64_t)1000000) 
  {
    str += sprintf (str, format[0], (uint32_t)(num / (uint64_t)1000000));
    num %= (uint64_t)1000000;
    sprintf (str, format[2], (uint32_t)(num / 1000U), (uint32_t)(num % 1000U));
    return;
  }
  /* 1,000 보다 클 경우 */
  if (num >= 1000U) 
  {
    sprintf (str, format[3], (uint32_t)(num / 1000U), (uint32_t)(num % 1000U));
    return;
  }
  sprintf (str, format[4], (uint32_t)num);
}

/** \brief  str 정수문자열을 부호있는 정수로 변환하여 반환한다.
  * \param  str - 변환할 정수문자열
  * \note   str의 길이만큼 순차적으로 숫자와 자릿수를 알아내어 정수를 조합한 뒤 반환한다.
            '-' 문자가 있는 경우 음수로 변환하여 반환한다.
  * \retval ret - 변환된 정수
  */
int32_t String_Convert_toInteger(char_t str[])
{
  uint32_t  i = 0U;
  uint32_t  length = 0U;
  int32_t   number = 0;
  int32_t   ret = 0;
  uint32_t  digitLength = 0U;
  int32_t   digit = 1;
  Bool_t    signMinus = False;

  /* 문자열의 길이를 구하여 length에 대입한다. */
  length = String_Get_Length((const char_t*)str);

  /* 문자열의 길이가 0이 될 때까지 반복 */
  while(length != 0U)
  {
    /* 문자열의 길이를 1 감소시킨다. */
    length--;
    /* 인덱스의 문자가 '-'이면 음수부호 플래그 signMinus를 켠다. */
    if (str[i] == '-')
    {
      signMinus = True;
    }
    /* 인덱스의 문자가 '+'이면 음수부호 플래그 signMinus를 끈다. */
    else if (str[i] == '+')
    {
      signMinus = False;
    }
    /* 문자가 '0'부터 '9'사이라면 정수 값으로 바꾸고
       현재 문자열 길이에 해당하는 자릿수로 정수 값을 환산한다. */
    else if((str[i] >= '0') && (str[i] <= '9'))
    {
      number = ((int32_t)str[i] - (int32_t)'0');
      digitLength = length;
      while(digitLength != 0U)
      {
        digitLength--;
        digit *= 10;
      }
      ret += (number * digit);
      digit = 1;
    }
    /* 문자가 부호나 정수문자가 아니면 반복 종료 */
    else
    {
      break;
    }
    /* 인덱스를 증가시켜 다음 문자를 가리키게 한다. */
    i++;
  }

  /* 음수부호 플래그가 켜져 있으면 변환된 정수를 음수로 반환한다. */
  if (signMinus == (Bool_t)True)
  {
    ret = 0 - ret;
  }

  return ret;
}

/** \brief  부호없는 정수를 숫자열로 변환한다.
  * \param  str - 변환 후 숫자열을 저장할 문자열 포인터
  * \param  integer - 변환할 정수
  * \param  digit - 변환할 자릿수
  * \param  firstzero - True이면 최초의 0을 제거하고 False면 포함시킨다.
  * \note   십억부터 10단위로 정수를 나누어서 각 자릿수를 숫자열로 변환하여 str에 저장한다.
            나눈 값이 최초의 '0'인 경우는 제거한다.
  * \retval 없음
  */
void String_Convert_IntegerToString(char_t str[], uint32_t integer, uint32_t digit, Bool_t firstzero)
{
  uint32_t i = 0;
  uint32_t pos = 0;
  uint32_t status = 0;
  uint32_t divider = 1; //1000000000;
  
  for (i = 1; i < digit; i++)
  {
    divider *= 10;
  }
  for (i = 0; i < digit; i++)
  {
    str[pos] = (integer / divider) + '0';
    pos++;
    integer = integer % divider;
    divider /= 10;
    
    if (firstzero == True)
    {
      if ((str[pos - 1] == '0') && (status == 0))
      {
        pos = 0;
      }
      else
      {
        status++;
      }
    }
  }
}

/** \brief  ch 16진수 문자를 Hex값으로 변환하여 반환한다.
  * \param  ch - 변환할 16진수 문자값
  * \note   ch의 문자가 16진수 문자에 해당하면 16진수 값으로 변환하여 반환한다.
            ch의 문자가 16진수 문자가 아니라면 'z'를 반환한다.
  * \retval hex - 변환된 16진수값
  */
uint8_t Char_Convert_toHex(uint8_t ch)
{
  uint8_t hex = 0U;

  if((ch >= (uint8_t)'0') && (ch <= (uint8_t)'9')) 
  {
    hex = (uint8_t)((uint8_t)ch - (uint8_t)'0');
  }
  else if((ch >= (uint8_t)'a') && (ch <= (uint8_t)'f')) 
  {
    hex = (uint8_t)(10U + ((uint8_t)ch - (uint8_t)'a'));
  }
  else if((ch >= (uint8_t)'A') && (ch <= (uint8_t)'F')) 
  {
    hex = (uint8_t)(10U + ((uint8_t)ch - (uint8_t)'A'));
  }
  else 
  {
    return (uint8_t)'z';
  }
  return hex;
}

/** \brief  str 16진수 문자열을 Hex값으로 변환하여 반환한다.
  * \param  str - 변환할 16진수 문자열
  * \param  hex - 변환된 16진수값이 저장될 포인터
  * \note   
  * \retval 변환 성공이면 True, 그렇지 않으면 False
  */
Bool_t String_Convert_StringToHex(char_t str[], uint32_t *hex)
{
  uint32_t i = 0;
  uint32_t length = 0;
  Bool_t   evenOrOdd = No;
  
  if ((str[0] == '0') && (str[1] == 'x'))
  {
    str = &str[2];
    length = String_Get_Length((const char_t*)str);
    evenOrOdd = (Bool_t)(length & 0x01);
    if (evenOrOdd != 0)
    {
//      cprintf("Plz, Hex value length must be even!!\n");
      return False;
    }
    *hex = 0;
    for (i = 0; i < length; i++)
    {
      *hex |= (uint32_t)(Char_Convert_toHex((uint8_t)str[i]) << ((length - 1 - i) * 4));
    }
  }
  else
  {
//    cprintf("Plz, append prefix \"0x\" \n");
    return False;
  }
  return True;
}

/** \brief  str 문자열에서 separator로 구분되는 단어를 추출하고 두번째 단어를 next가 가리키게 한다.
  * \param  str - separator로 구분된 단어들이 들어 있는 문자열
  * \param  next - str 문자열에서 두 번째 단어를 가리킬 문자열 포인터
  * \param  separator - 문자열에서 단어와 단어를 구별할 문자(space, dot, ...)
  * \note   separator로 구분된는 첫번째 단어의 포인터를 반환하고 두번째 단어를 next가 가리키게 한다.
  * \retval word - 첫 번째 단어를 가리키는 문자열 포인터
  */

#ifndef __NOT_USE_ST__
char_t* String_Extract_Word(char_t str[], char_t* next[], char_t separator)
{
  u32_t  i = 0U;
  char_t*   word;
  Bool_t    loop = False;

  /* 문자열이 비어있다면 next가 str을 가리키게 하고 str을 반환한다. */
  if(str[i] == '\0') 
  {
    *next = str;
    return (char_t*)(str);
  }

  /* str 문자열에서 separator로 시작되는 부분은 '\0'로 대치시키면서 인덱스를 증가시킨다. */
  for(i = 0U; str[i] == ' '; i++)
  {
    str[i] = '\0';
  }

  /* separator가 끝난 다음 문자를 word 문자열 포인터가 가리키게 한다. */
  word = &str[i];

  /* separator가 끝난 다음 문자부터 separator나 '\0'이 나올때까지 인덱스를 증가시킨다. */
  do
  {
    if((str[i] == '\0') || (str[i] == separator))
    {
      break;
    }
    else
    {
      i++;
			if(i >= 30U)
			{
				      loop = False;
			}
			else
			{
				      loop = True;
			}
//      loop = True;
    }
  }while(loop == (Bool_t)True);

  /* 위 do-while에서 separator가 나와서 루프가 종료된 경우 
     separator부분을 '\0'으로 대치시키면서 인덱스를 증가시킨다. */
  if(str[i] != '\0')
  {
    while(str[i] == separator)
    {
      str[i] = '\0';
      i++;
    }
  }

  /* 증가된 인덱스에서 시작하는 문자열을 next 문자열 포인터가 가리키게 한다. */
  *next = &str[i];                /* &next entry */

  /* 첫번째 찾은 단어의 문자열 포인터를 반환한다. */
  return (word);
}

#else

char_t* String_Extract_Word(char_t str[], char_t* next[], char_t separator)
{
  u32_t  i = 0U;
  char_t*   word;
  Bool_t    loop = False;


  /* 문자열이 비어있다면 next가 str을 가리키게 하고 str을 반환한다. */
  if(str[i] != '\0')
  {
	  /* str 문자열에서 separator로 시작되는 부분은 '\0'로 대치시키면서 인덱스를 증가시킨다. */
	  /*
	  for(i = 0U; str[i] == ' '; i++)
	  {
		str[i] = '\0';
	  }
	  */

	  i = 0U;
	  while(str[i] == ' ')
	  {
		  str[i] = '\0';
		  i++;
	  }

	  /* separator가 끝난 다음 문자를 word 문자열 포인터가 가리키게 한다. */
	  word = &str[i];

	  /* separator가 끝난 다음 문자부터 separator나 '\0'이 나올때까지 인덱스를 증가시킨다. */
	  do
	  {
		if((str[i] != '\0') || (str[i] != separator))
		{
      i++;
			if(i >= 30U)
			{
				      loop = False;
			}
			else
			{
				      loop = True;
			}
//      loop = True;
		}
	  }while(loop == (Bool_t)True);

	  /* 위 do-while에서 separator가 나와서 루프가 종료된 경우
		 separator부분을 '\0'으로 대치시키면서 인덱스를 증가시킨다. */
	  if(str[i] != '\0')
	  {
		while(str[i] == separator)
		{
		  str[i] = '\0';
		  i++;
		}
	  }

	  /* 증가된 인덱스에서 시작하는 문자열을 next 문자열 포인터가 가리키게 한다. */
	  *next = &str[i];                /* &next entry */

  }
  else
  {
	  *next = str;
	  word = (char_t*)(str);
  }
  /* 첫번째 찾은 단어의 문자열 포인터를 반환한다. */
  return (word);
}

#endif

/** \brief  str1과 str2 문자열의 내용을 비교한다.
  * \param  str1 - 첫번째 문자열
  * \param  str2 - 두번째 문자열
  * \note   str1 문자열이 끝날 때까지 두 문자열의 내용을 순차적으로 비교하여 
            값이 다를 경우 두 문자값의 차이만큼을 반환하고 모든 문자가 일치하면
            0을 반환한다.
  * \retval ret - 모든 문자열이 일치하면 0, 불일치하면 0이 아닌 값을 반환한다.
  */
uint32_t String_Compare(char_t str1[], char_t str2[])
{
  u32_t  i = 0U;
  u32_t   ret = 0;
	
  while(str1[i] != '\0')
  {
    if(str1[i] != str2[i])
    {	
      ret = (u32_t)((u32_t)str1[i] - (u32_t)str2[i]);
      break;
    }
    i++;
  }

  return ret;
}

/** \brief  ch1과 ch2 문자를 대소문자 구별없이 비교한다.
  * \param  ch1 - 첫번째 문자
  * \param  ch2 - 두번째 문자
  * \note   두 문자가 일치하면 SAME을 반환하고 불일치하는 경우
            큰 값의 문자(소문자)를 대문자로 변환 후 비교하여 불일치하면 
            두 문자값의 차이만큼을 반환한다.
  * \retval ret - 두 문자가 일치하거나 대소문자 구별없이 일치하면 SAME, 
            불일치하면 두 문자값의 차이를 반환한다.
  */

uint32_t Char_Compare_woCase(char_t ch1, char_t ch2)
{
  char_t    c;
  char_t    gap = (char_t)('a' - 'A');
  Bool_t    isCh1Alphabet = No;
  Bool_t    isCh2Alphabet = No;
  u32_t   ret = SAME;
  

	if(ch1 != ch2)
	{
  /* ch1, ch2가 알파벳인지 검사한다. */
		if(((ch1 >= 'a') && (ch1 <= 'z')) || ((ch1 >= 'A') && (ch1 <= 'Z')))
		{
			isCh1Alphabet = Yes;
		}

		if(((ch2 >= 'a') && (ch2 <= 'z')) || ((ch2 >= 'A') && (ch2 <= 'Z')))
		{
			isCh2Alphabet = Yes;
		}
  
  /* ch1, ch2가 모두 알파벳이면 대소비교 */
		if ((isCh1Alphabet == Yes) && (isCh2Alphabet == Yes))
		{
			if (ch1 > ch2)
			{
				c = (char_t)(ch1 - gap);
				if (c != ch2)
				{
					ret = (u32_t)((u32_t)ch1 - (u32_t)ch2);
				}
			}
			else
			{
				c = (char_t)(ch2 - gap);
				if (c != ch1)
				{
					ret = (u32_t)((u32_t)ch1 - (u32_t)ch2);

				}
			}
		}
  /* 하나라도 알파벳이 아니면 DIFF 반환 */
		else
		{
			ret = DIFF;
		}  
	}
	else
	{
    ret = SAME;
  }
	return ret;
}


/** \brief  str1과 str2 문자열의 내용을 대소문자 구별없이 비교한다.
  * \param  str1 - 첫번째 문자열
  * \param  str2 - 두번째 문자열
  * \note   str1 문자열이 끝날 때까지 두 문자열의 내용을 순차적으로 비교하여
            값이 다를 경우 두 문자값의 차이만큼을 반환하고 모든 문자가 일치하면 
            SAME을 반환한다.
  * \retval ret - 모든 문자열이 대소문자 구별없이 일치하면 SAME, 
            불일치하면 0이 아닌 값을 반환한다.
  */
uint32_t String_Compare_woCase(char_t str1[], char_t str2[])
{
  u32_t  i = 0U;
  uint32_t   ret = 0U;

  while(str1[i] != '\0')
  {
    ret = Char_Compare_woCase(str1[i], str2[i]);
    if((ret == 0U) && (i <= 30U))   // 문자열 최대 30개까지만 카운팅
    {
				i++;
		}
		else 
		{
			break;
		}
  }
  return ret;
}

/** \brief  str1과 str2 문자열의 내용을 대소문자 구별없이 length만큼 비교한다.
  * \param  str1 - 첫번째 문자열
  * \param  str2 - 두번째 문자열
  * \param  length - 비교할 길이
  * \note   length만큼 두 문자열의 내용을 순차적으로 비교하여
            값이 다를 경우 두 문자값의 차이만큼을 반환하고 모든 문자가 일치하면 
            SAME을 반환한다.
  * \retval ret - 모든 문자열이 대소문자 구별없이 일치하면 SAME, 
            불일치하면 0이 아닌 값을 반환한다.
  */
uint32_t String_Compare_woCase_wLength(char_t str1[], const char_t str2[], u32_t length)
{
  u32_t  i = 0U;
  u32_t   ret = 0U;
  u32_t  length1 = 0U;
  u32_t  length2 = 0U;
  
  length1 = String_Get_Length(str1);
  length2 = String_Get_Length(str2);
  
  if (length != length1)
  {
    ret =  1U;
  }
  
  if (length != length2)
  {
    ret = 1U;
  }

  while(length != 0U)
  {
    length--;
    ret = Char_Compare_woCase(str1[i], str2[i]);
    if((ret == 0U) && (i <= 30U))   // 문자열 최대 50개까지만 카운팅
    {
				i++;
		}
		else 
		{
			break;
		}
  }

  return ret;
}


/** \brief  str 문자열의 길이를 구하여 반환한다.
  * \param  str - 길이를 구할 문자열
  * \note   str 문자열의 문자가 '\0'이 나올 때까지 순차적으로 검사하며 
            길이를 증가시킨 후 길이를 반환한다.
  * \retval i - 문자열의 길이
  */
u32_t String_Get_Length(const char_t str[])
{
  u32_t  i = 0U;

  while(str[i] != '\0')
  {
		if( i <= 50U)			// 문자열 최대 50개까지만 카운팅
		{
			i++;
		}
		else
		{
			break;
		}
  }
  return (u32_t)i;
}



/** \} Lib_String */

/** \} Libraries */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
