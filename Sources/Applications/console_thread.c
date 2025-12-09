/** ****************************************************************************
  * \file    console_thread.c
  * \author  Jang Ho Jong
  * \version V1.0.0
  * \date    2023.06.10
  * \brief   Console Thread Body
  * ***************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "console_thread.h"
#include "lib_command.h"
#include "lib_buffer.h"
#include "lib_string.h"


/*----------------------------------------------------------------------------
 *      'Console_Thread': Debugging thread through Serial COM
 *---------------------------------------------------------------------------*/

/** \addtogroup Applications
  * \{ */

/** \defgroup Console_Thread Console 쓰레드
  * \brief 디버깅용 시리얼 입력을 처리하는 쓰레드
  * \{ */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/** \defgroup Console_Thread_Private_Variables Console 쓰레드 비공개 변수
  * \{ */
static u08_t       _cmd_buf[MAX_SIZE_CMD_BUF];            /**< 명령어 버퍼 */
static u32_t       _cmd_cnt;                              /**< 명령어 버퍼의 입력 카운트 */
Bool_t g_monitor_flag;                                    /**< Console 모니터링용 플래그 */
Bool_t g_temper_flag;
/** \} Console_Thread_Private_Variables */

/* Exported variables --------------------------------------------------------*/
/** \addtogroup Console_Thread_Exported_Variables
  * \{ */
/** \} Console_Thread_Exported_Variables */

/* Private function prototypes -----------------------------------------------*/
/** \defgroup Console_Thread_Private_Functions Console 쓰레드 비공개 함수
  * \{ */

static void Console_Fill_input_to_buf(Rs232_t * rs);
static void Console_Process_Command(void);

static void Cmd_Help(char_t str[]);
static void Cmd_Reboot(char_t str[]);
static void Cmd_Version(char_t str[]);

#ifdef RTE_DEVICE_HAL_RTC
static void Cmd_Rtc(char_t str[]);
/* 시간문자열(스페이스로 구분)에서 단어를 파싱해서 주어진 변수 포인터에 대입
  하는 함수 선언부 */
static Bool_t RTC_FillUp_DateTime(char_t ** szDateTime, char_t ** szNext, uint8_t *ucDst);
#endif /* RTE_DEVICE_HAL_RTC */

#ifdef RTE_DEVICE_HAL_SRAM
static void Cmd_Read(char_t str[]);
static void Cmd_Write(char_t str[]);
static void Cmd_Erase(char_t str[]);
#endif /* RTE_DEVICE_HAL_SRAM */
static void Cmd_Led(char_t str[]);
static void Cmd_Relay(char_t str[]);
static void Cmd_RelayIn(char_t str[]);
static void Cmd_SensorIn(char_t str[]);
static void Cmd_SenTest(char_t str[]);
static void Cmd_DoFlt(char_t str[]);
static void Cmd_CurVlt(char_t str[]);
static void Cmd_CurTemp(char_t str[]);
static void Cmd_Mon_Con(char_t str[]);
static void Cmd_PowerIn(char_t str[]);
static void Cmd_Watchdog(char_t str[]);

/** \} Console_Thread_Private_Functions */

/* Exported functions --------------------------------------------------------*/
/** \addtogroup Console_Thread_Exported_Functions
  * \{ */

/** \brief  Console 쓰레드를 생성한다.
  * \note   Console 쓰레드를 생성하고 tid_console에 쓰레드 아이디를 할당받는다.
  * \retval 없음
  */
/** \} Console_Thread_Exported_Functions */

/* Private functions ---------------------------------------------------------*/
/** \addtogroup Console_Thread_Private_Functions
  * \{ */

/** \brief  Console 쓰레드 본체
  * \param  argument - 쓰레드로 전달된 인자
  * \note   시리얼 입력 수신 타이머를 초기화하고 시리얼 입력 인터럽트를 활성화한 후
            입력 발생시 명령어인지 파악하여 명령어를 처리한다.
  * \retval 없음
  */
void Console_Thread(void)
{
 
   /* 시리얼 입력이 발생하면 */
  if (serial3->rx_complete == True)
  {
  /* 명령어 버퍼를 수신된 데이터로 채운다. */
    Console_Fill_input_to_buf(serial3);
  /* RS-232 시리얼 포트의 수신을 복구시킨다. */
    Rs232_Restore_rx(serial3);
  /* 명령어 버퍼의 내용을 처리한다. */
    Console_Process_Command();
  }
}


/** \brief  전역변수 cmd_buf를 시리얼로 수신된 데이터로 채운다.
  * \param  rs - RS232 구조체 포인터
  * \note   cmd_buf의 여유공간과 수신된 길이가 cmd_buf 버퍼 크기를 초과한다면
            버퍼를 클리어하고 그렇지 않으면 수신된 길이만큼 버퍼에 복사한다.
  * \retval 없음
  */
static void Console_Fill_input_to_buf(Rs232_t * rs)
{
  /* 수신된 길이가 없다면 종료 */
  if (rs->rx_count !=0U)
  {
  /* 수신된 길이와 버퍼의 여유 길이의 합이 최대 크기를 초과했다면
     버퍼를 클리어한다. */
		if ((_cmd_cnt + rs->rx_count) > MAX_SIZE_CMD_BUF)
		{
			_cmd_cnt = 0U;
			Buffer08_Set(&_cmd_buf[0], 0U, MAX_SIZE_CMD_BUF);
		}
  /* 수신된 길이가 적당하다면 버퍼에 복사하고 버퍼 카운트를 수신된 길이만큼 증가시킨다. */
		else
		{
			Buffer08_Copy(&_cmd_buf[_cmd_cnt], rs->rx_buf, rs->rx_count);
			_cmd_cnt += rs->rx_count;
		}
	}
}

/** \brief  수신된 명령어를 처리한다.
  * \note   수신된 길이가 존재하고 수신된 길이가 버퍼 최대 크기를 초과하지 않았다면
            수신된 데이터가 명령어인지 분석하여 명령어를 실행하고 실행 후 버퍼를 클리어한다.
  * \retval 없음
  */
static void Console_Process_Command(void)
{
  Cmd_t cmd_list[] =
  {
    {"HELP",    &Cmd_Help},
    {"?",       &Cmd_Help},
    {"REBOOT",  &Cmd_Reboot},
    {"VER",     &Cmd_Version},
    {"LED",     &Cmd_Led},
    {"REY",     &Cmd_Relay},
    {"REYIN",   &Cmd_RelayIn},
    {"SENIN",   &Cmd_SensorIn},
	  {"SENTEST", &Cmd_SenTest},
	  {"DOFLT",   &Cmd_DoFlt},
    {"CURVLT",  &Cmd_CurVlt},
    {"CURTEMP", &Cmd_CurTemp},
    {"MONCON",  &Cmd_Mon_Con},  
    {"PWRIN", &Cmd_PowerIn},
    {"WATCHDOG",&Cmd_Watchdog},		
    {"CMDHIS",  &Cmd_History},
  };
  u32_t    nbrOfCmd = (sizeof(cmd_list) / sizeof(cmd_list[0]));
  CmdInput_t  input;

  /* 버퍼 카운트가 0이면 종료 */   /* 버퍼 카운트가 버퍼 최대 크기보다 크다면 종료 */
//  if ((_cmd_cnt != 0U) || (_cmd_cnt < MAX_SIZE_CMD_BUF ))---> 동적시험시 확인됨 23.11.28
  if ((_cmd_cnt != 0U) && (_cmd_cnt < MAX_SIZE_CMD_BUF ))	
  {

  /* 버퍼의 내용을 분석해서 명령어이면 */
		input = Cmd_Parse_Input(_cmd_buf, &_cmd_cnt);
		if (input == COMMAND)
		{
    /* 명령어 실행한다. */
			Cmd_Parse_Command(_cmd_buf, _cmd_cnt, cmd_list, nbrOfCmd);
    /* 버퍼 카운트와 버퍼를 클리어한다. */
			_cmd_cnt = 0U;
			Buffer08_Set(&_cmd_buf[0], 0U, MAX_SIZE_CMD_BUF);
		}
	}
}

/** \brief  "HELP" 명령어에 대해 명령어 도움말을 출력한다.
  * \param  str - 명령어 옵션 문자열
  * \note   명령어 리스트와 명령어당 설명을 출력한다.
  * \retval 없음
  */
static void Cmd_Help(char_t str[])
{
  static const char_t *start_line =
  "\n\n"
  "+ command --------------------+ function ----------------------------+\n";
  static const char_t *help =
  "| LED \"led\" [on/off]        | turn LED on or off                   |\n"
  "| REY \"relay\" [on/off]      | turn Relay ON or OFF                 |\n"
  "| REYIN \"relay\"             | show Relay Input state               |\n"
  "| SENIN                       | show Sensor State                    |\n"
  "| SENTEST \"TEST_x\" [on/off] | turn Sensor on or off                |\n"
  "| DOFLT \"FLTx\"              | show current FLT State               |\n"
  "| CURVLT                      | show Sensor Current[mA]              |\n"
  "| CURTEMP                     | show current Temperature             |\n"
  "| MONCON                      | show State Console On                |\n"	
	"| WATCHDOG                    | turn WATCHDOG on or off              |\n"
	"| PWRIN                       | show Power Ok CPU Input state        |\n"
  ;
  static const char_t *end_line =
  "+-----------------------------+--------------------------------------+\n";

  cprintf(start_line);
  cprintf(help);
  cprintf(end_line);
}

/** \brief  "REBOOT" 명령어에 대해 시스템을 리부트 시킨다.
  * \param  str - 명령어 옵션 문자열
  * \note   안내문을 출력하고 시스템 리셋을 수행한다.
  * \retval 없음
  */
static void Cmd_Reboot(char_t str[])
{
  cprintf("System Reboot!\n\n");
  cprintf("=================================================================\n");
  cprintf("========================RE BOOTING !!!!==========================\n");	
  cprintf("=================================================================\n");
  HAL_NVIC_SystemReset();
}

/** \brief  "VER" 명령어에 대해 시스템 버전을 출력한다.
  * \param  str - 명령어 옵션 문자열
  * \note   시스템 메인, 서브1, 서브2 버전을 출력한다.
  * \retval 없음
  */
static void Cmd_Version(char_t str[])
{
  cprintf("\n\n\t%s [V%d.%d.%d]\n\t(%s %s)\n",
         PROJECT_NAME, VERSION_MAIN, VERSION_SUB1, VERSION_SUB2,
         BUILD_DATE, BUILD_TIME);
}

/** \brief  "LED" 명령어에 대해 LED를 제어한다.
  * \param  str - 명령어 옵션 문자열
  * \note   "LED LED_2 ON"이면 2번 LED를 켠다.
  * \retval 없음
  */
static void Cmd_Led(char_t str[])
{
  u32_t led_num = 0;

  char_t    *szLED, *szNext, *szOnOff;
  Led_t     * led[kNbrOfLEDs];

  led[kLED_RUN] = run_led;
  led[kLED_HLT] = hlt_led;
  led[kLED_E_EB] = e_eb_led;
  led[kLED_OBR] = obr_led;
  led[kLED_DER] = der_led; 
  led[kLED_OBL] = obl_led;  
  led[kLED_DEL] = del_led;
  led[kLED_E_OBR] = e_obr_led;
  led[kLED_E_DER] = e_der_led;
  led[kLED_E_OBL] = e_obl_led;
  led[kLED_E_DEL] = e_del_led;
  
  szLED  = String_Extract_Word(str, &szNext, ' ');
  if (*szLED == '\0')
  {
    cprintf("Plz, append LED[RUN,HLT,E_EB,OBR,DER,OBL,DEL,E_OBR,E_DER,E_OBL,E_DEL] and ON or OFF");
  }
  else 
	{
      for (led_num = 0; led_num < kNbrOfLEDs; led_num++)
      {
          if (String_Compare_woCase_wLength(szLED, led[led_num]->name, String_Get_Length(led[led_num]->name)) == SAME)
          {
 //             ld = led[i];
              break;
          }
      }		
	}  // if (*szLED == '\0')   end
	/******************************************************************************/
  if (led_num >= kNbrOfLEDs) 
  {
	     cprintf("Plz, append LED[RUN,HLT,E_EB,OBR,DER,OBL,DEL,E_OBR,E_DER,E_OBL,E_DEL] and ON or OFF");
  }
  else 
	{
      szOnOff = szNext;
      if (*szOnOff == '\0')
      {
          cprintf("Plz, append ON or OFF\n");
//				  ctrl = 0;
      }
      else if (String_Compare_woCase_wLength(szOnOff, "ON", 2) == SAME) 
      {
//          ctrl = 1;
					Led_On(led[led_num]);
				  cprintf("%s On\n", led[led_num]->name);		
				
      }
      else if (String_Compare_woCase_wLength(szOnOff, "OFF", 3) == SAME)
      {
//          ctrl = 2;
			    Led_Off(led[led_num]);
          cprintf("%s Off\n", led[led_num]->name);				
      }
      else if (String_Compare_woCase_wLength(szOnOff, "TOGGLE", 6) == SAME)
      {
//          ctrl = 3;
				  Led_Toggle(led[led_num]);
          cprintf("%s Toggle\n", led[led_num]->name);
      }
      else
      {
          cprintf("Plz, append ON or OFF\n");
//          ctrl = 4;
     }
	}   //   if (i == kNbrOfLEDs) end		
}

/** \brief  "REY" 명령어에 대해 Relay를 제어한다.
  * \param  str - 명령어 옵션 문자열
  * \note   "REY EB1 ON"이면 REY EB1 접점을 연결한다.
  * \retval 없음
  */
static void Cmd_Relay(char_t str[])
{
  u32_t  relay_num = 0;
//  u32_t  ctrl = 0;    /* 2023년 10월 18 RTE(run time error)check시 변경  */
//  Relay_t     *ld;    /* 2023년 10월 18 RTE(run time error)check시 변경  */
  char_t    *szREY, *szNext, *szOnOff;
  Relay_t     * relayout[kNbrOfRYOs];
	
  relayout[kRY_EB1] = eb1_out ;
  relayout[kRY_EB2] = eb2_out ;
  relayout[kRY_DMPC] = dmpc_out ;
  relayout[kRY_HLT] = hlt_out ;
  relayout[kRY_OBR] = obr_out ;
  relayout[kRY_DER] = der_out ;
  relayout[kRY_OBL] = obl_out ;
  relayout[kRY_DEL] = del_out ;
  
  szREY  = String_Extract_Word(str, &szNext, ' ');
  if (*szREY == '\0')
  {
		cprintf("Plz, append REY[EB1,EB2,DMPC,HLT,OBR,DER,OBL,DEL] and ON or OFF");
  }
  else
	{
		for (relay_num = 0; relay_num < kNbrOfRYOs; relay_num++)
    {
      if (String_Compare_woCase_wLength(szREY, relayout[relay_num]->name, String_Get_Length(relayout[relay_num]->name)) == SAME)
      {
//        ld = relay[i];    /* 2023년 10월 18 RTE(run time error)check시 변경  */
        break;
      }
    }
	}
  if (relay_num >= kNbrOfRYOs)
  {
    cprintf("Plz, append REY[EB1,EB2,DMPC,HLT,OBR,DER,OBL,DEL] and ON or OFF");
  }
	else 
	{
    szOnOff = szNext;
    if (*szOnOff == '\0')
    {
      cprintf("Plz, append ON or OFF\n");
//      ctrl = 0;     /* 2023년 10월 18 RTE(run time error)check시 변경  */
    }

    else if (String_Compare_woCase_wLength(szOnOff, "ON", 2) == SAME)
    {
//     ctrl = 1;      /* 2023년 10월 18 RTE(run time error)check시 변경  */
			Relay_On(relayout[relay_num]);
      cprintf("%s On\n", relayout[relay_num]->name);		
    }
    else if (String_Compare_woCase_wLength(szOnOff, "OFF", 3) == SAME)
    {
//      ctrl = 2;    /* 2023년 10월 18 RTE(run time error)check시 변경  */
			Relay_Off(relayout[relay_num]);
      cprintf("%s Off\n", relayout[relay_num]->name);
    }
    else if (String_Compare_woCase_wLength(szOnOff, "TOGGLE", 6) == SAME)
    {
//      ctrl = 3;     /* 2023년 10월 18 RTE(run time error)check시 변경  */
			Relay_Toggle(relayout[relay_num]);
      cprintf("%s Toggle\n", relayout[relay_num]->name);
    }
    else
    {
      cprintf("Plz, append ON or OFF\n");
//      ctrl = 4;      /* 2023년 10월 18 RTE(run time error)check시 변경  */
    }
	}
}

/** \brief  "REYIN" 명령어에 대해 RELAY 상태를 모니터링 한다.
  * \param  str - 명령어 옵션 문자열
  * \note   "REYIN EB1"이면 EB1 Relay에 대한 Input State를 출력한다.
  * \retval 없음
  */
static void Cmd_RelayIn(char_t str[])
{
  u32_t  Number = 0;
//  Relay_t     *fr;    /* 2023년 10월 18 RTE(run time error)check시 변경  */
  char_t    *szIn, *szNext;
  Relay_t   * relayin[kNbrOfRYOs];
	
  relayin[kRY_EB1] = eb1_out ;
  relayin[kRY_EB2] = eb2_out ;
  relayin[kRY_DMPC] = dmpc_out ;
  relayin[kRY_HLT] = hlt_out ;
  relayin[kRY_OBR] = obr_out ;
  relayin[kRY_DER] = der_out ;
  relayin[kRY_OBL] = obl_out ;
  relayin[kRY_DEL] = del_out ;
  
  szIn  = String_Extract_Word(str, &szNext, ' ');
  if (*szIn == '\0')
  {
     for (Number = 0; Number < kNbrOfRYFBIs; Number++)
     {
			  cprintf("%s = %s\n",relayin[Number]->name,((relayin[Number]->state == Yes) ? "ON" : "OFF" ));
     }
  }
  else 
	{
    for (Number = 0; Number < kNbrOfRYFBIs; Number++)
    {
      if (String_Compare_woCase_wLength(szIn, relayin[Number]->name, String_Get_Length(relayin[Number]->name)) == SAME)
      {
//        fr = relay[i];     /* 2023년 10월 18 RTE(run time error)check시 변경  */
        break;
      }
    }
	}

  if (Number >= kNbrOfRYFBIs)
  {
		cprintf("Plz, append REY[EB1 ,EB2 ,DMPC ,HLT ,OBR ,DER ,OBL ,DEL ]\n\n");
  }
	else if(Number !=0) 
	{
    cprintf("%s state = %s\n", relayin[Number]->name, ((relayin[Number]->state == Yes) ? "ON" : "OFF"));
	}
}

/** \brief  "SENIN" 명령어에 대해 RELAY 상태를 모니터링 한다.
  * \param  str - 명령어 옵션 문자열
  * \note   "SENIN EB1"이면 EB1 Relay에 대한 Input State를 출력한다.
  * \retval 없음
  */
static void Cmd_SensorIn(char_t str[])
{
  u08_t  i = 0;

    for (i = 0; i < kNbrOfSIs; i++)
    {
			 cprintf("SENSOR_%d OSSD1 state = %s\n",(i+1U),(sdin[i]->Sensor_OSSD1  == True) ? "ON" : "OFF" );
       cprintf("SENSOR_%d OSSD2 state = %s\n\n",(i+1U),(sdin[i]->Sensor_OSSD2  == True) ? "ON" : "OFF" );
    }
}

/** \brief  "SENTEST" 명령어에 대해 Sensor를 테스트 한다.
  * \param  str - 명령어 옵션 문자열
  * \note   "SENTEST 1 ON"이면 SENSOR 1에 대한 TEST MODE로  접점을 연결한다.
  * \retval 없음
  */
static void Cmd_SenTest(char_t str[])
{
  u32_t  sentest_num = 0;
//  u32_t  ctrl = 0;
//  DO_t     *target;
  char_t    *szSenT, *szNext, *szOnOff;
  DO_t     * tsensor[kNbrOfSIs];
	
  tsensor[kSI_1] = mcu_test1;
  tsensor[kSI_2] = mcu_test2;
  tsensor[kSI_3] = mcu_test3;
  tsensor[kSI_4] = mcu_test4;
  
  szSenT  = String_Extract_Word(str, &szNext, ' ');
  if (*szSenT == '\0')
  {
    cprintf("Plz, append SENTEST[TEST_1,TEST_2,TEST_3,TEST_4]and ON or OFF\n");
  }
  else 
	{
		for (sentest_num = 0; sentest_num < kNbrOfSIs; sentest_num++)
		{
			if (String_Compare_woCase_wLength(szSenT, tsensor[sentest_num]->name, String_Get_Length(tsensor[sentest_num]->name)) == SAME)
			{
//				target = tsensor[i];
				break;
			}
		}
	}

  if (sentest_num >= kNbrOfSIs )
  {
    cprintf("Plz, append SENTEST[TEST_1,TEST_2,TEST_3,TEST_4]and ON or OFF\n");
  }
  else 
	{
    szOnOff = szNext;
    if (*szOnOff == '\0')
    {
      cprintf("Plz, append ON or OFF\n");
//      ctrl = 0;
    }
    if (String_Compare_woCase_wLength(szOnOff, "ON", 2) == SAME)
    {
//      ctrl = 1;
			DO_On(tsensor[sentest_num]);
      cprintf("%s On\n", tsensor[sentest_num]->name);	
    }
    else if (String_Compare_woCase_wLength(szOnOff, "OFF", 3) == SAME)
    {
//      ctrl = 2;
		  DO_Off(tsensor[sentest_num]);
      cprintf("%s Off\n", tsensor[sentest_num]);	
    }
  }
}

/** \brief  "DOFLT" 명령어에 대해 Digital Output Isolator FLT PIN 상태를 모니터링 한다.
  * \param  str - 명령어 옵션 문자열
  * \note   "DOFLT 1 "이면 Digital Output Isolator FLT #1 PIN에 대한 Input State를 출력한다.
  * \retval 없음
  */

static void Cmd_DoFlt(char_t str[])
{
  u32_t  flt_num = 0;
//  DI_t     *flt;     /* 2023년 10월 18 RTE(run time error)check시 변경  */
  char_t    *szFIn, *szNext;
  DI_t     * doflt[kNbrOfDISFs];

  doflt[kDISF_1]= fpin_1;
  doflt[kDISF_2] = fpin_2;

  
  szFIn  = String_Extract_Word(str, &szNext, ' ');
  if (*szFIn == '\0')
  {
			cprintf("%s state = %s\n", doflt[kDISF_1]->name,((doflt[kDISF_1]->curr_state == True) ? "ON" : "OFF"));
			cprintf("%s state = %s\n", doflt[kDISF_2]->name,((doflt[kDISF_2]->curr_state == True) ? "ON" : "OFF"));
  }
	else
	{
		for(flt_num = 0; flt_num < kNbrOfDISFs; flt_num++)
		{
			if (String_Compare_woCase_wLength(szFIn, doflt[flt_num]->name, String_Get_Length(doflt[flt_num]->name)) == SAME)
			{		
					break;
		  }
		}
	}
//	else if ((i == kNbrOfDOIS) && (*szFIn != '\0'))  
  if (flt_num >= kNbrOfDISFs)
  {
    cprintf("Plz, append DOFLT[FLT1,FLT2]\n");
  }
	else 
	{
    cprintf("%s state = %s\n", doflt[flt_num]->name, ((doflt[flt_num]->curr_state == True) ? "ON" : "OFF"));
	}
}

/** \brief  "CURAMP" 명령어에 대해 Safety Sensor에 대한 전류(Amp)를 모니터링 한다.
  * \param  str - 명령어 옵션 문자열
  * \note   "CURAMP 1 "이면 Safety Sensor #1 에 대한 현재의 전류 값을 출력한다.
  * \retval 없음
  */

static void Cmd_CurVlt(char_t str[])
{
	u08_t  i = 0;
  char_t    *szCuramp, *szNext;
  
  szCuramp  = String_Extract_Word(str, &szNext, ' ');

	if (*szCuramp == '\0')
  {		
    for(i = 0; i < kNbrOfSIs ; i ++)
	  {
			  cprintf(" Senor[%d] Currnet Volte =  %d[mV]\r\n",(i+1),sdin[i]->Sensor_Voltage);
        cprintf(" Senor[%d] Currnet Low Counter =  %d[mV]\r\n",(i+1),sdin[i]->Sensor_Low_Counter);
    }		
  }
	else 
	{
    
    cprintf("Plz, append CURVLT\n\n");
  }
}

/** \brief  "CURTEMP" 명령어에 대해 DEOD 시스템에 대한 온도를 모니터링 한다.
  * \param  str - 명령어 옵션 문자열
  * \note   "CURTEMP "이면 DEOD 시스템에 대한 온도를 출력한다.
  * \retval 없음
  */
static void Cmd_CurTemp(char_t str[])
{  

	 u32_t temper=0;	
	
	 temper = TMP117_get_Temperature(hi2c1);
	 if(temper > MINUS_TEMPER)
	 {  
		  g_device_temper=((65535-temper)*0.0078125);
	    if(g_monitor_flag == True)
      {		
		     cprintf("\n DEOD Temperature : -%f\n",g_device_temper);	
		  }
		  cprintf("\n DEOD Temperature : -%f\n",g_device_temper);
	 }
	 else 
	 {
		  g_device_temper=((temper)*0.0078125);
      if(g_monitor_flag == True)
      {			
		     cprintf("\n DEOD Temperature : %f\n",g_device_temper);	
		  }
		  cprintf("\n DEOD Temperature : %f\n",g_device_temper);
	 }
//   g_temper_flag = True;
}


/** \brief  "MONCON" 명령어에 대해 속도 모니터링을 제어한다.
  * \param  str - 명령어 옵션 문자열
  * \note   "ON"이면 모니터링을 켜고, "OFF"이면 모니터링을 끈다.
  * \retval 없음
  */
static void Cmd_Mon_Con(char_t str[])
{
  char_t  *szONOFF, *szNext;
  char_t  *text[2] = {"ON", "OFF"};

  szONOFF  = String_Extract_Word(str, &szNext, ' ');
  if (*szONOFF == '\0')
  {
     cprintf("Plz, append ON or OFF! Current:%s\n", (g_monitor_flag == True) ? "ON " : "OFF");
  }
  else 
	{
		if (String_Compare_woCase(szONOFF, text[0]) == 0U)
		{
			 cprintf("ON\n");
			 g_monitor_flag = True;
		}
		else if (String_Compare_woCase(szONOFF, text[1]) == 0U)
		{
			 cprintf("OFF\n");
			 g_monitor_flag = False;
		}
		else
		{
			 cprintf("Plz, append ON or OFF!\n");
		}
	}
}


/** \brief  "PWRIN" 명령어에 대해 Power Input 상태를 모니터링 한다.
  * \param  str - 명령어 옵션 문자열
  * \note   없음
  * \retval 없음
  */
static void Cmd_PowerIn(char_t str[])
{
		DI_Filter_State(pwrin,20);	
    cprintf("Power Input State :%s\n", (pwrin->curr_state == True) ? "ON " : "OFF");
}

/** \brief  "WATCHDOG" 명령어에 대해 Watchdog clock을 제어한다.
  * \param  str - 명령어 옵션 문자열
  * \note   "WATCHDOG OFF"이면 Watchdog toggle 기능을 OFF 한다.
  * \retval 없음
  */
static void Cmd_Watchdog(char_t str[])
{
  char_t  *szONOFF, *szNext;
  char_t  *text[2] = {"ON", "OFF"};

  szONOFF  = String_Extract_Word(str, &szNext, ' ');
  if (*szONOFF == '\0')
  { 
     cprintf("Plz, append ON or OFF!\n");
  }
  else 
	{
		if (String_Compare_woCase(szONOFF, text[0]) == 0U)
		{
			cprintf("ON\n");
			Watchdog_Unlock(wdog);
		}
		else if (String_Compare_woCase(szONOFF, text[1]) == 0U)
		{
			cprintf("OFF\n");
			Watchdog_Lock(wdog);
		}
		else
		{
			cprintf("Plz, append ON or OFF!\n");
		}
	}
}


/** \} Console_Thread_Private_Functions */

/** \} Console_Thread */

/** \} Applications */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
