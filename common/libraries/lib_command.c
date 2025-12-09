/** ****************************************************************************
  * \file    lib_command.c
  * \author  Jang Ho Jong
  * \version V1.0.0
  * \date    2023.06.10
  * \brief   Command for Debug Library Body
  * ***************************************************************************/
/** \remark  Version History
  * V0.0.1   - 최초 버전 - 2016.05.06
  * V0.0.2   - AsciiCtrl_t 타입 선언부를 .c에서 .h로 이동
*/

/* Includes ------------------------------------------------------------------*/
#include "lib_command.h"
#include "lib_string.h"
#include "lib_buffer.h"
#include "device.h"

/** \addtogroup Libraries
  * \{ */

/** \defgroup Command Command 라이브러리
  * \brief 명령어 처리 관련 라이브러리
  * \{ */

/* Private typedef -----------------------------------------------------------*/
/** \defgroup Command_Private_Types Command 라이브러리 비공개 타입
  * \{ */

/** \brief 방향키에 대한 타입 */
typedef enum
{
  DIRECTION_UP  = 0x41,   /**< 'A' Up Arrow Key */
  DIRECTION_DN  = 0x42,   /**< 'B' Down Arrow Key */
  DIRECTION_RT  = 0x43,   /**< 'C' Right Arrow Key */
  DIRECTION_LT  = 0x44,   /**< 'D' Left Arrow Key */
  NbrOfDIRECTION_KEY = 4
}DirectionKey_t;

/** \brief 입력 명령어 히스토리 구조체 타입 */
typedef struct
{
  u32_t  in;           /**< 히스토리에 삽입시 입력 인덱스 */
  u32_t  out;          /**< 방향키로 히스토리에서 꺼낼때 출력 인덱스 */
  u32_t  count;        /**< 현재 히스토리에 입력된 명령어 개수 */
  u08_t   buf[NbrOfCmdHistoryBuf][SizeOfCmdHistoryBuf]; /**< 히스토리 버퍼 */
  u08_t   tmp[SizeOfCmdHistoryBuf];       /**< 임시 버퍼 */
}CmdHistory_t;
/** \} Command_Private_Types */

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/** \defgroup Command_Private_Variables Command 라이브러리 비공개 변수
  * \{ */
static CmdHistory_t   _history;                 /**< 입력 명령어 히스토리 구조체 */
static char_t*        _szPrompt = "SHALOM>";    /**< 프롬프트 */
/** \} Command_Private_Variables */

/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** \defgroup Command_Private_Functions Command 라이브러리 비공개 함수
  * \{ */
/*void Send_CursorDirection(eDIRECTION_KEY eDirection);*/
static void Send_ClearLine(void);
static void Send_PrevCmdLine(void);
static CmdInput_t Cmd_Process_DirectionKey(u08_t buf[], u32_t *pCnt,
                                           u08_t bufEcho[], u32_t *psizeEcho);
static void Cmd_Process_UpKey(u08_t buf[], u32_t *pCnt, u08_t bufEcho[]);
static void Cmd_Process_DownKey(u08_t buf[], u32_t *pCnt, u08_t bufEcho[]);
static CmdInput_t Cmd_Process_LastChar(u08_t buf[], u32_t *pCnt, 
                                       u08_t bufEcho[], u32_t *psizeEcho);
/** \} Command_Private_Functions */

/* Exported functions --------------------------------------------------------*/
/** \addtogroup Command_Exported_Functions
  * \{ */

/** \brief  입력 데이터가 방향키인지 명령어인지 분석한다.
  * \param  buf - 입력 데이터가 들어있는 버퍼
  * \param  pCnt - 입력 데이터 카운트 포인터
  * \note   방향키 입력인지 문자인지 파악하여 에코하고 입력 데이터 타입을 반환한다.
  * \retval ret - 분석한 결과 (입력 데이터 타입)
  */
CmdInput_t Cmd_Parse_Input(u08_t buf[], u32_t *pCnt)
{
  u08_t     bufEcho[5] = {0U, 0U, 0U, 0U, 0U};
  u32_t    sizeEcho = 0U;
  CmdInput_t  ret;

  /* 방향키인 경우  */
  if( (*pCnt > 2U) && 
      (buf[*pCnt - 3U] == 0x1BU) && 
      (buf[*pCnt - 2U] == 0x5BU))
  {
    ret = Cmd_Process_DirectionKey(buf, pCnt, bufEcho, &sizeEcho);
  }
  /* 문자 입력인 경우 */
  else
  {
    bufEcho[4] = buf[*pCnt - 1U];
    ret = Cmd_Process_LastChar(buf, pCnt, bufEcho, &sizeEcho);
  }

  /* 입력 문자에 따라 에코한다. */
  if(sizeEcho > 0U)
  {
    cprint(bufEcho, sizeEcho);
  }

  return ret;
}

/** \brief  pBuf의 문자열에서 cmd에 들어 있는 명령어와 일치하는 문자열을 찾아서 해당 명령어를 수행한다.
  * \param  buf - 입력 문자열
  * \param  cnt - 입력 문자열의 길이
  * \param  list - 수행할 수 있는 명령어가 저장되어 있는 Command 구조체
  * \param  list_cnt - Command 구조체에 들어있는 명령어 개수
  * \note   입력 문장에서 첫번째 단어와 명령어 리스트에 있는 명령어와 비교하여
            일치하는 경우 해당 명령어를 수행한다.
  * \retval 없음
  */
void Cmd_Parse_Command(u08_t buf[], u32_t cnt, Cmd_t list[], u32_t list_cnt)
{
  u32_t    i=0U;
  char_t*     szNext = NULL;
  char_t*     szInp = NULL;
  u32_t    length_cmd = 0U;
  u32_t    length = 0U;
  
  Buffer08_Set(_history.tmp, 0U, SizeOfCmdHistoryBuf);
  Buffer08_Copy(_history.tmp, buf, cnt);

//  /* Rx 입력 문장의 소문자를 대문자로 변경 */
//  String_Cnvt_ToUpper((char_t*)&pBuf[0]);

  /* 입력문장중 첫번째 단어를 추출 */
  szInp = String_Extract_Word((char_t*)&_history.tmp[0], &szNext, ' ');
  length = String_Get_Length(szInp);
  
  if(*szInp != '\0')
	{
  /* 전체 명령어 리스트에서 */
		for(i = 0U; i < list_cnt; i++)
		{
    /* 명령어의 길이를 구해서 length에 대입 */
			length_cmd = String_Get_Length(list[i].cmd);
    
    /* 명령어의 길이와 입력 첫번째 단어의 길이가 같고 */
			if (length == length_cmd)
			{
      /* 첫번째 단어와 일치하는 Cmd 찾기 */
				if(String_Compare_woCase_wLength(list[i].cmd, szInp, length) == 0U)
				{
        /* Cmd일 경우만 _history.buf에 입력 문장 복사하고 _history.in, _history.out 증가 */
					Buffer08_Set(_history.buf[_history.in], 0U, SizeOfCmdHistoryBuf);
					Buffer08_Copy(_history.buf[_history.in], buf, cnt);
					_history.in++;
					if(_history.in == NbrOfCmdHistoryBuf)
					{
						_history.in = 0U;
					}
					_history.out = _history.in;

        /* Cmd일 경우만 저장된 Cmd 갯수 최대치까지 증가 */
					if(_history.count < NbrOfCmdHistoryBuf)
					{
						_history.count++;
					}

        /* Cmd 수행 */
					list[i].pFuncCmd(szNext);
					break;
				}
			}
		}
	}

  /* 일치하는 Cmd가 없을 경우 */
  if(i == list_cnt)
  {
    cprintf("%s is not Command\n", szInp);
  }
  cprintf("%s", _szPrompt);
}

/** \brief  "CMDHIS" 명령어에 따른 입력 명령어 히스토리를 출력한다.
  * \param  optn - 명령어 옵션 문자열
  * \note   입력 명령어 히스토리 버퍼의 입력 문장들을 순차적으로 출력한다.
  * \retval 없음
  */
void Cmd_History(char_t optn[])
{
  u32_t  i = 0U;
  
  for (i = 0U; i < NbrOfCmdHistoryBuf; i++)
  {
    cprintf("%d : ", i);
    cprintf("%s\n", &_history.buf[i][0]);
  }
  cprintf("InputIndex = %d, OutputIndex = %d\n", _history.in, _history.out);
}



/** \brief  현재 커서가 있는 줄을 지운다.
  * \note   시리얼 터미널로 현재 줄을 지우는 CSI code를 전송한다.
  * \retval 없음
  */
static void Send_ClearLine(void)
{
  u08_t     bufEcho[5];
  u32_t    sizeEcho = 0U;

  bufEcho[0] = 0x1BU;
  bufEcho[1] = 0x5BU;
  bufEcho[2] = 0x32U;
  bufEcho[3] = 0x4BU;   //(u08_t)'K';    /* CSI(Control Sequence Introducer) Erase in Line */
  sizeEcho = 4U;

  cprint(bufEcho, sizeEcho);
}

/** \brief  이전에 입력한 명령어를 출력한다.
  * \note   현재 커서를 줄의 맨앞으로 이동시키고 현재 줄을 모두 지운 후 
            프롬프트를 출력하고 이전에 입력된 명령어를 출력한다.
  * \retval 없음
  */
static void Send_PrevCmdLine(void)
{
  cprintf("\r");
  Send_ClearLine();
  cprintf("%s", _szPrompt);
  cprintf("%s", _history.buf[_history.out]);
}

/** \brief  방향키 입력에 대해서 처리한다.
  * \param  buf - 입력 데이터가 들어있는 버퍼
  * \param  pCnt - 입력 버퍼의 데이터 카운트 포인터
  * \param  bufEcho - 방향키 입력에 대해 출력할 데이터를 저장할 버퍼
  * \param  psizeEcho - 출력할 데이터 카운트 포인터
  * \note   좌, 우 방향키에 대해서는 커서를 이동시킨다.
            상, 하 방향키는 명령어 리스트에 저장된 명령어들을 출력한다.
  * \retval ret - 입력 데이터에 대한 타입을 반환한다.
  */
static CmdInput_t Cmd_Process_DirectionKey(u08_t buf[], u32_t *pCnt, u08_t bufEcho[], u32_t *psizeEcho)
{
  CmdInput_t  ret = KEY_CODE;
  
  switch(buf[*pCnt - 1U])
  {
    /* Up Key */
#ifndef __NOT_USE_ST__
    case (AsciiCtrl_t)DIRECTION_UP:
#else
  	case (u08_t)DIRECTION_UP:
#endif
      *pCnt -= 3U;
      Cmd_Process_UpKey(buf, pCnt, bufEcho);
      *psizeEcho = 0U;
      ret = ASCII_CHAR;
      break;
    /* Down Key */
#ifndef __NOT_USE_ST__
    case (AsciiCtrl_t)DIRECTION_DN:
#else
    case (u08_t)DIRECTION_DN:
#endif
      *pCnt -= 3U;
      Cmd_Process_DownKey(buf, pCnt, bufEcho);
      *psizeEcho = 0U;
      ret = ASCII_CHAR;
      break;
    /* 좌, 우 방향키가 입력된 경우 커서를 좌, 우로 이동시킨다. */
#ifndef __NOT_USE_ST__
    case (AsciiCtrl_t)DIRECTION_RT:
    case (AsciiCtrl_t)DIRECTION_LT:
#else
    case (u08_t)DIRECTION_RT:
    case (u08_t)DIRECTION_LT:
#endif
      bufEcho[0] = 0x1BU;
      bufEcho[1] = 0x5BU;
      bufEcho[2] = buf[*pCnt - 1U];
      *pCnt -= 3U;
      *psizeEcho = 3U;
      ret = KEY_CODE;
      break;
    default:
      break;
  }
  return ret;
}

/** \brief  상 방향키에 대해서 입력 명령어 히스토리 버퍼의 문장을 조회하여 출력한다.
  * \param  buf - 입력 데이터가 들어있는 버퍼
  * \param  pCnt - 입력 버퍼의 데이터 카운트 포인터
  * \param  bufEcho - 방향키 입력에 대해 출력할 데이터를 저장할 버퍼
  * \note   입력 히스토리의 out 인덱스를 감소시키고 out 인덱스에 해당하는 입력 문장을 출력한다.
  * \retval 없음
  */
static void Cmd_Process_UpKey(u08_t buf[], u32_t *pCnt, u08_t bufEcho[])
{
  /* 사용된 명령어가 하나도 없을 경우 */
  if(_history.count != 0U)
  {

  /* 현재 입력이 있는 상태에서 Up Arrow 키가 눌려졌을 경우 현재 입력을 버퍼에 임시 저장 */
		if(_history.out == _history.in)
		{
			if(*pCnt != 0U)
			{
				bufEcho[0] = 0U;
				buf[*pCnt] = bufEcho[0];
				(*pCnt)++;
      /* Rx 입력 문장을 그대로 사용자 명령어 버퍼에 복사 */
				Buffer08_Copy(&_history.buf[_history.out][0], &buf[0], *pCnt);
			}
		}

		if(_history.out == 0U)
		{
			_history.out = NbrOfCmdHistoryBuf;
		}
		_history.out--;

		if(_history.out >= _history.count)
		{
			_history.out++;
			if(_history.out == NbrOfCmdHistoryBuf)
			{
				_history.out = 0U;
			}
//			return;
		}

		else if(_history.out != _history.in)
		{
			Send_PrevCmdLine();
			*pCnt = (u32_t)Buffer08_Copy_TillZero(&buf[0], &_history.buf[_history.out][0]);
		}
		else
		{
			_history.out++;
			if(_history.out == NbrOfCmdHistoryBuf)
			{
				_history.out = 0U;
			}
		}
	}	
}

/** \brief  하 방향키에 대해서 입력 명령어 히스토리 버퍼의 문장을 조회하여 출력한다.
  * \param  buf - 입력 데이터가 들어있는 버퍼
  * \param  pCnt - 입력 버퍼의 데이터 카운트 포인터
  * \param  bufEcho - 방향키 입력에 대해 출력할 데이터를 저장할 버퍼
  * \note   입력 히스토리의 out 인덱스를 증가시키고 out 인덱스에 해당하는 입력 문장을 출력한다.
  * \retval 없음
  */
static void Cmd_Process_DownKey(u08_t buf[], u32_t *pCnt,u08_t bufEcho[])
{
  u32_t  i = 0U;

  /* 사용자 입력 리스트의 입력인덱스와 출력인덱스가 같지 않으면 */
  if(_history.out != _history.in)
  {
    _history.out++;
    if(_history.out == NbrOfCmdHistoryBuf)
    {
      _history.out = 0U;
    }
    Send_PrevCmdLine();
    while(_history.buf[_history.out][i] != 0U)
    {
      buf[i] = _history.buf[_history.out][i];
      i++;
    }
    *pCnt = i;
    if(_history.out == _history.in)
    {
      i = 0U;
      while(_history.buf[_history.out][i] != 0U)
      {
        _history.buf[_history.out][i] = 0U;
        i++;
      }
    }
  }
}

/** \brief  buf에 입력된 마지막 문자를 분석하여 입력 데이터 타입을 반환한다.
  * \param  buf - 입력 데이터가 들어있는 버퍼
  * \param  pCnt - 입력 버퍼의 데이터 카운트 포인터
  * \param  bufEcho - 입력에 대해 출력할 데이터를 저장할 버퍼
  * \param  psizeEcho - 출력할 데이터 카운트 포인터
  * \note   Backspace나 Delete, ESC, Enter 또는 그 외의 문자에 대해 처리한다.
  * \retval ret - 입력 데이터에 대한 타입을 반환한다.
  */
static CmdInput_t Cmd_Process_LastChar(u08_t buf[], u32_t *pCnt, 
                                       u08_t bufEcho[], u32_t *psizeEcho)
{
  CmdInput_t  ret = ASCII_CHAR;

  /* 입력된 마지막 문자를 확인한다. */
  switch(buf[*pCnt - 1U])
  {
#ifndef __NOT_USE_ST__
    case (AsciiCtrl_t)ASCII_ETX:
#else
    case (u08_t)ASCII_ETX:
#endif
      ret = ASCII_CTRL;
      break;
    /* Backspace나 Delete 키이면 */
#ifndef __NOT_USE_ST__
    case (AsciiCtrl_t)ASCII_BS:
    case (AsciiCtrl_t)ASCII_DEL:
#else
    case (u08_t)ASCII_BS:
    case (u08_t)ASCII_DEL:
#endif
      (*pCnt)--;
      if(*pCnt > 0U)
      {
        (*pCnt)--;
        bufEcho[0] = (u08_t)ASCII_BS;
        bufEcho[1] = (u08_t)ASCII_SPACE;
        bufEcho[2] = (u08_t)ASCII_BS;
        *psizeEcho = 3U;
      }
      ret = ASCII_CTRL;
      break;
    /* ESC 키이면 */
#ifndef __NOT_USE_ST__
    case (AsciiCtrl_t)ASCII_ESC:
#else
    case (u08_t)ASCII_ESC:
#endif
      *pCnt = 0U;
      bufEcho[0] = (u08_t)ASCII_CR;
      bufEcho[1] = (u08_t)ASCII_LF;
      *psizeEcho = 2U;
      cprint(bufEcho, *psizeEcho);
      *psizeEcho = 0U;
      cprintf("%s", _szPrompt);
      ret = ESCAPE_KEY;
      break;

    /* Enter 키이면 */
#ifndef __NOT_USE_ST__
    case (AsciiCtrl_t)ASCII_CR:
#else
    case (u08_t)ASCII_CR:
#endif
      buf[*pCnt - 1U] = 0U;
      bufEcho[0] = (u08_t)ASCII_CR;
      bufEcho[1] = (u08_t)ASCII_LF;
      *psizeEcho = 2U;
      ret = COMMAND;
      break;
    /* 그 외 문자이면 */
    default:
      bufEcho[0] = buf[*pCnt - 1U];
      *psizeEcho = 1U;
      ret = ASCII_CHAR;
      break;
  }
  return ret;
}


/** \} Command_Private_Functions */

/** \} Command */

/** \} Libraries */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
