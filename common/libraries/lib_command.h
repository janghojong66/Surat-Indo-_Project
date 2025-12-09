/** ****************************************************************************
  * \file    lib_command.h
  * \author  Park Keun Tae
  * \version V0.0.2
  * \date    2016.06.15
  * \brief   Command for Debug Library Header
  * ***************************************************************************/
/** \remark  Version History
  * V0.0.1   - 최초 버전 - 2016.05.06
  * V0.0.2   - AsciiCtrl_t 타입 선언부를 .c에서 .h로 이동
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LIB_COMMAND_H
#define __LIB_COMMAND_H

/* Includes ------------------------------------------------------------------*/
#include "device.h"
#include "main.h"

/** \addtogroup Libraries
  * \{ */

/** \addtogroup Command
  * \{ */

/* Exported types ------------------------------------------------------------*/
/** \defgroup Command_Exported_Types Command 라이브러리 공개 타입
  * \{ */

/** \brief Command 입력 데이터 타입 */
typedef enum
{
  ASCII_CHAR = 0U,      /**< Printable character */
  ASCII_CTRL,           /**< Control character */
  COMMAND,              /**< Command */
  KEY_CODE,             /**< Direction Arrows, F1 ~ F12, HOME, END, INSERT, DELETE, ...*/
  ESCAPE_KEY,           /**< Esc character */
  NbrOfCmdInput
}CmdInput_t;

/** \brief ASCII 제어 문자에 대한 타입 */
typedef enum
{
  ASCII_SOH   = 0x01,   /**< [Ctrl+A] */
  ASCII_STX   = 0x02,   /**< [Ctrl+B] */
  ASCII_ETX   = 0x03,   /**< [Ctrl+C] */
  ASCII_EOT   = 0x04,   /**< [Ctrl+D] */
  ASCII_ENQ   = 0x05,   /**< [Ctrl+E] */
  ASCII_ACK   = 0x06,   /**< [Ctrl+F] */
  ASCII_CEL   = 0x07,   /**< [Ctrl+G] */
  ASCII_BS    = 0x08,   /**< [Ctrl+H, BackSapce] */
  ASCII_HT    = 0x09,   /**< [Ctrl+I, Tab] */
  ASCII_LF    = 0x0A,   /**< [Ctrl+J, Enter] */
  ASCII_VT    = 0x0B,   /**< [Ctrl+K] */
  ASCII_FF    = 0x0C,   /**< [Ctrl+L] */
  ASCII_CR    = 0x0D,   /**< [Ctrl+M, Enter] */
  ASCII_SO    = 0x0E,   /**< [Ctrl+N] */
  ASCII_SI    = 0x0F,   /**< [Ctrl+O] */
  ASCII_DLE   = 0x10,   /**< [Ctrl+P] */
  ASCII_DC1   = 0x11,   /**< [Ctrl+Q] */
  ASCII_DC2   = 0x12,   /**< [Ctrl+R] */
  ASCII_DC3   = 0x13,   /**< [Ctrl+S] */
  ASCII_DC4   = 0x14,   /**< [Ctrl+T] */
  ASCII_NAK   = 0x15,   /**< [Ctrl+U] */
  ASCII_SYN   = 0x16,   /**< [Ctrl+V] */
  ASCII_ETB   = 0x17,   /**< [Ctrl+W] */
  ASCII_CAN   = 0x18,   /**< [Ctrl+X] */
  ASCII_EM    = 0x19,   /**< [Ctrl+Y] */
  ASCII_SUB   = 0x1A,   /**< [Ctrl+Z] */
  ASCII_ESC   = 0x1B,   /**< [ESC] */
  ASCII_FS    = 0x1C,   /**< [] */
  ASCII_GS    = 0x1D,   /**< [] */
  ASCII_RS    = 0x1E,   /**< [] */
  ASCII_US    = 0x1F,   /**< [] */
  ASCII_SPACE = 0x20,   /**< [SpaceBar] */
  ASCII_DEL   = 0x7F,   /**< [Delete] */
  NbrOfAsciiCtrl = 33
}AsciiCtrl_t;

/** \brief Command 구조체 타입 */
typedef struct
{
  char_t  *cmd;         /**< 명령어 문자열 */
  void    (*pFuncCmd)(char_t * szArg); /**< 명령어에 대한 수행 함수 포인터 */
}Cmd_t;

typedef struct
{
  char_t    *option;      /**< 명령어 옵션 문자열 */
  u32_t  length;       /**< 옵션 문자열 길이 */
  char_t    *help;        /**< 옵션 도움말 */
  char_t*   (*pFuncOption)(char_t szValue[]);   /**< 옵션에 대한 수행 함수 포인터 */
} CmdOption_t;
/** \} Command_Exported_Types*/

/* Exported constants --------------------------------------------------------*/
/** \defgroup Command_Exported_Constants Command 라이브러리 공개 상수
  * \{ */
#define SizeOfCmdHistoryBuf     128U     /**< 입력 명령어 히스토리에 저장할 명령어 한 개의 버퍼 크기 */
#define NbrOfCmdHistoryBuf      8U       /**< 입력한 명령어 히스토리 버퍼의 최대 개수 */
/** \} Command_Exported_Constantsb */

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** \defgroup Command_Exported_Functions Command 라이브러리 공개 함수
  * \{ */
extern CmdInput_t Cmd_Parse_Input(u08_t buf[], uint32_t *pCnt);
extern void Cmd_Parse_Command(u08_t buf[], u32_t cnt, Cmd_t list[], u32_t list_cnt);
extern void Cmd_History(char_t optn[]);
extern void HEXA_View(u08_t buf[], u32_t size, u32_t lineFeed);
/** \} Command_Exported_Functions*/

/** \} Command */

/** \} Libraries */

#endif /* __COMMAND_H */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
