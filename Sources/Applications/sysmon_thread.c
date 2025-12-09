/** ****************************************************************************
  * \file    sysmon_thread.c
  * \author  Jang Ho Jong
  * \version V1.0.0
  * \date    2023.06.10
  * \brief   System Monitoring Thread Body
  * ***************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "sysmon_thread.h"
#include "self_diagnose_perform.h"
#include "device.h"

/*----------------------------------------------------------------------------
 *      'Sysmon_Thread': System 모니터링
 *---------------------------------------------------------------------------*/


/** \addtogroup Applications
  * \{ */

/** \defgroup Sysmon_Thread System Monitoring 쓰레드
  * \brief 시스템 모니터링하는 쓰레드
  * \{ */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/** \defgroup Sysmon_Thread_Private_Variables 시스템 모니터링 쓰레드 비공개 변수
  * \{ */
u32_t  g_Run_Led_timer=0;                /* RUN LED 점멸 주기 타이머 */
u16_t  g_Watch_Dog_timer=0;              /* Watch Dog Toggle 주기 타이머 */
u32_t  g_Pwr_Ok_timer=0;                 /* POWER OK CPU Signal 체크 주기 타이머 */
u32_t  g_Flt_Signal_timer = 0;           /* FLT 신호 감지 타이머 */
u32_t  g_Temp_check_timer=0;             /* DEOD System 온도 체크 주기 타이머 */
u08_t  FLT1_Status,FLT2_Status;          /* Digital Isolator FLt Pin 체크 주기 타이머 */
//u32_t  g_Relay_FB_check_timer=0;       /* Relay FB check주기 타이머 */
//u32_t  g_Sensor_Check_timer=0;         /* Sensor Logic Check 주기 타이머 */
float  g_device_temper = 0;
I2C_HandleTypeDef i2c1;
u16_t  g_Watchdog_Check_Pin_Timer=0;      /* Watch Dog Check_Pin 주기 타이머 */
/** \} Sysmon_Thread_Private_Variables */

/* Exported variables --------------------------------------------------------*/
/** \addtogroup Sysmon_Thread_Exported_Variables
  * \{ */

/** \} Sysmon_Thread_Exported_Variables */

/* Private function prototypes -----------------------------------------------*/
/** \defgroup Sysmon_Thread_Private_Functions 시스템 모니터링 쓰레드 비공개 함수
  * \{ */
static void Check_Digital_Output_Isol(void);
void Check_Temperature(void);
static void Watchdog_Check_Pin(void);
static void Check_HCR_Signal_Input(void);
/** \} Sysmon_Thread_Private_Functions */

/* Exported functions --------------------------------------------------------*/
/** \addtogroup Sysmon_Thread_Exported_Functions
  * \{ */

/** \brief  Sysmon 쓰레드 본체
  * \param  argument - 쓰레드로 전달된 인자
  * \note
  * \retval 없음
  */
void  Sysmon_Thread(void)
{
  /* RUN LED 점멸 */
	/* 일정시간(500msec )마다 RUN LED를 TOGGLE 시킨다. */
  if (g_Run_Led_timer >= RUN_LED_TOGGLE_TIME)
  {  
	   g_Run_Led_timer = 0U;
		 Led_Toggle(run_led);
	}

 
  /* Digital Output Isolator FLT PIN 상태 체크  */
	/* 일정시간(10sec )마다 FLT를 체크하여 일정온도를 벗어나면 ERR를 띄운다. */
	if(g_Flt_Signal_timer >= FLT_SIGNAL_CHECK_TIME)
	{
		 g_Flt_Signal_timer = 0;
     Check_Digital_Output_Isol();
	}

  /* Relay FeedBack Check  */
	/* 일정시간(10sec )마다 Relay FeedBackd을 체크하여 Relay에 이상이 발생하였을 경우 ERR를 띄운다. */
//	if(g_Relay_FB_check_timer >= RLY_FB_CHECK_TIME)
//	{
//		g_Relay_FB_check_timer = 0;
//    Check_Realy_FeedBack();
//	}
	
	/* 일정시간(1sec )마다 Watchdog input pin을 체크하여  이상이 발생하였을 경우 ERR를 띄운다. */
	if(g_Watchdog_Check_Pin_Timer >= WATCHDOG_CHECK_PIN_TIME)
	{
		 g_Watchdog_Check_Pin_Timer = 0;
     Watchdog_Check_Pin();
	}	
	
  /* Sensor OSSD1,OSSD2 LOGIC PIN 상태 체크  */
	/* 일정시간(10sec )마다 OSSD1 OSSD2 Logic을 체크하여  ERR를 띄운다. */
//	if(g_Sensor_Check_timer >= SENSOR_A12_CHECK_TIME)
//	{
//		g_Sensor_Check_timer = 0;
//    Check_Sensor_Logic();
//	}	
		/* 온도센서의 온도 상태 점검  */
		/* 일정시간(10sec )마다 온도 체크를 체크하여 일정온도를 벗어나면 ERR를 띄운다. */
  if (g_Temp_check_timer >= TEMPERATOR_CHECK_TIME)	 
  {
	   g_Temp_check_timer = 0;
     g_temper_flag = True;
		 Check_Temperature();
//    g_temper_flag = False;
  }
	  /* 일정시간(1sec )마다 PWR_OK_CPU PORT를 체크하여 파워 입력 여부를 확인한다. */

  if (g_Pwr_Ok_timer >= POWER_OK_CHECK_TIME)			
  {  
//		cprintf("\nPower OK Check Time\n");			// 모니터링 주기 테스트 코드
     g_Pwr_Ok_timer = 0U;	
     pwrin->curr_state = DI_Get_State(pwrin);	  // power ok input pin read
//		cprintf("pwrin->curr_state = %d\n",pwrin->curr_state);
  	 if(pwrin->curr_state == False)                                   
     { 
 		    cprintf("\n\n\nF_006.\n");      // Fault Code 출력			/* 주제어 보드로 동작  */
				Relay_Off(hlt_out);	
        Led_Off(hlt_led);      
//		    Watchdog_Lock(wdog);
//		    while (1) {};
		 }
	 }
}

/** \brief  DIGITAL OUTPUT ISOLATOR의 Fault PIN 상태를 확인한다.
  * \param   
  * \note    
  * \retval 없음
  */

static void Check_Digital_Output_Isol(void)
{

//  cprintf("\nCheck_Digital_Output_Isol\n");			                   // 모니터링 주기 테스트 코드
//  fpin_1->curr_state = (Bool_t)(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_3));  
  fpin_1->curr_state = DI_Get_State(fpin_1);
	HAL_Delay(50);
//	fpin_2->curr_state = (Bool_t)(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4));
  fpin_2->curr_state = DI_Get_State(fpin_2);
	HAL_Delay(50);
  if((fpin_1->curr_state == False) || (fpin_2->curr_state == False)) /**< activ low */
  {
		  cprintf("\n\n\nF_007.\n");                                     /**< Fault Code 출력	*/
//      DO_Off(snp_out);                                               /**< 릴레이 전원 공급 off */
      Relay_Off(hlt_out);                                            /**< hlt signal off */
      Led_Off(hlt_led);                                              /**< hlt led on */ 
//			Watchdog_Lock(wdog);
//			while (1) {}; 
  }
}

/** \brief  DEOD 장비의 온도를 주기적으로 체크한다.
  * \param   
  * \note    
  * \retval 없음
  */

void Check_Temperature(void)
{
	u32_t i2c_temper=0;
	
  i2c_temper = (u32_t)TMP117_get_Temperature(hi2c1);
/************************************************************************************
* 영하의 온도 처리 ROUTINE
*************************************************************************************/	
//  cprintf("\nTemperature Check\n");			                           // 모니터링 주기 테스트 코드	
	if(i2c_temper > MINUS_TEMPER)
	{
		g_device_temper=(((float)65535U-(float)i2c_temper)*(float)0.0078125);;
	  if(g_temper_flag == True)
    {		
		  cprintf("\n DEOD Temperature : -%f\n",g_device_temper);	
		}
		if(g_device_temper >= (float)LOW_TEMPER)                         /**< 영하의 온도 측정 : -40도 이하상태 */
	  {
//    cprintf("\n DEOD Temperature : -%f\n",device_temper);	
			 cprintf("\n\n\nF_004.\n");                                     /**< Fault Code 출력 */
//	     Relay_Off(hlt_out);
//       Led_Off(hlt_led);      
	  }
		else if(g_device_temper >= (float)LOW_WARNING_TEMPER)            /**< 영하의 온도 측정 -30도 이하상태 */
		{
		   cprintf("\n DEOD Temperature : -%f\n",g_device_temper);
			 cprintf("\n\n\nF_005.\n");                                    /**< Fault Code 출력 */
		}
	}
/************************************************************************************
* 영상의 온도 처리 ROUTINE
*************************************************************************************/		
	else 
	{
		g_device_temper=((float)(i2c_temper)*(float)0.0078125);
    if(g_temper_flag == True)
    {			
		  cprintf("\n DEOD Temperature : %f\n",g_device_temper);	
		}
		if(g_device_temper >= (float)HIGH_TEMPER )                       /**< 영하의 온도 측정 70~79도 이상상태 */
		{
			cprintf("\n DEOD Temperature : %f\n",g_device_temper);
			cprintf("\n\n\nF_004.\n");                                     /**< Fault Code 출력 */

//			   Relay_Off(hlt_out);
//         Led_Off(hlt_led);      
		}	
		else if(g_device_temper >= (float)HIGH_WARNING_TEMPER)           /**< 영하의 온도 측정 80도 이상상태  */
		{
			cprintf("\n DEOD Temperature : %f\n",g_device_temper);
			cprintf("\n\n\nF_005.\n");                                     /**< Fault Code 출력 */ 
		}	
	}
//	cprintf("\n DEOD Temperature : %f\n",g_device_temper);	         /**< 모니터링 주기 테스트 코드 */
}


/** \brief  Relay FeedBack Check를 하여 릴레이 상태를 확인한다.
  * \param   
  * \note    
  * \retval 없음
  */

//static void Check_Realy_FeedBack(void)
//{
//  cprintf("\nRelay Feedback Check\n");			                       /**< 모니터링 주기 테스트 코드	*/	
//  Read_Relay_FeedBack_State();  
//}

/** \brief  WDT_OK PIN를 Check하여 Watchdog 상태를 확인한다.   
  * \retval 없음
  */

static void Watchdog_Check_Pin(void)
{
Bool_t watchdog_state;
//		cprintf("\nWatchdog_Check_Pin\n");                             /**< 모니터링 주기 테스트 코드 */ 	
  g_Watchdog_Check_Pin_Timer = 0U;	
	watchdog_state = DI_Get_State(wdt_ok);	    
  if(watchdog_state == False)		
	{ 
	  cprintf("\n\n\nF_008.\n");                                       /**< Fault Code 출력  */    
		Watchdog_Lock(wdog);
    while(1)
    {};
  }
}


/** \brief  Sensor OSSD1 OSSD2 Logic Check를 하여 센서의 동작 상태를 확인한다.
  * \param   
  * \note    
  * \retval 없음
  */

//static void Check_Sensor_Logic(void)
//{
//  static Bool_t    Sensor_Ossd[kNbrOfSOs];
//	u08_t i;


//  cprintf("\nCheck_Sensor_A1_A1\n");			                         // 모니터링 주기 테스트 코드
/*------------------------------------------------------------*/
// Sensor #1 Logic Test 1, 2
/*------------------------------------------------------------*/
//	DO_On(mcu_test1);   //20231212 추가
 // HAL_Delay(20); 

// 	Sensor_Ossd[kSO_1] = (Bool_t)(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0));
//	Sensor_Ossd[kSO_2] = (Bool_t)(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1));  

//  if(g_monitor_flag == True)
//  {
//    cprintf("SENSOR1_1 state = %s\n",(Sensor_Ossd[kSO_1]  == True) ? "1" : "0");
//    cprintf("SENSOR1_2 state = %s\n",(Sensor_Ossd[kSO_2]  == True) ? "1" : "0");
//  }
//	DO_Off(mcu_test1);   //20231212 추가
 
	/*------------------------------------------------------------*/
// Sensor #2 Logic Test 1, 2
/*------------------------------------------------------------*/
//	DO_On(mcu_test2);                                                  //20231212 추가
//	HAL_Delay(20);
//	Sensor_Ossd[kSO_3] = (Bool_t)(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2));
//	Sensor_Ossd[kSO_4] = (Bool_t)(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3));
//  if(g_monitor_flag == True)
//  {
//    cprintf("SENSOR2_1 state = %s\n",(Sensor_Ossd[kSO_3] == True) ? "1" : "0");
//    cprintf("SENSOR2_2 state = %s\n",(Sensor_Ossd[kSO_4] == True) ? "1" : "0");
//  }
//	DO_Off(mcu_test2);                                                 //20231212 추가
	/*------------------------------------------------------------*/
// Sensor #3 Logic Test 1, 2
/*------------------------------------------------------------*/
//	DO_On(mcu_test3);                                                  //20231212 추가
//	HAL_Delay(20);
//	Sensor_Ossd[kSO_5] = (Bool_t)(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4));
//	Sensor_Ossd[kSO_6] = (Bool_t)(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5));
//  if(g_monitor_flag == True)
//  {
//    cprintf("SENSOR3_1 state = %s\n",(Sensor_Ossd[kSO_5] == True) ? "1" : "0");
//    cprintf("SENSOR3_2 state = %s\n",(Sensor_Ossd[kSO_6] == True) ? "1" : "0");
//  }		
//	DO_Off(mcu_test3);                                                 //20231212 추가
/*------------------------------------------------------------*/
// Sensor #4 Logic Test 1, 2
/*------------------------------------------------------------*/
//	DO_On(mcu_test4);                                                  //20231212 추가
//	HAL_Delay(20);
//	Sensor_Ossd[kSO_7] = (Bool_t)(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6));
//	Sensor_Ossd[kSO_8] = (Bool_t)(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7));
//  if(g_monitor_flag == True)
//  {
//    cprintf("SENSOR4_1 state = %s\n",(Sensor_Ossd[kSO_7] == True) ? "1" : "0");
//    cprintf("SENSOR4_2 state = %s\n",(Sensor_Ossd[kSO_8] == True) ? "1" : "0");
//  }
//	DO_Off(mcu_test4);                                                 //20231212 추가  

//  for(i = 0; i < kNbrOfSOs ; i ++)
//	{  
//	   if(Sensor_Ossd[i] == False)
//	   {
//		    cprintf("\n\nF_009.\n");	                                   // Fault Code 출력										
//  		  Watchdog_Lock(wdog);
//		    cprintf("System While Loop!\n\n");
//        while(1)
//        {};
//     }
//  }		
//}	
	

/** \} Sysmon_Thread_Private_Functions */

/** \} Sysmon_Thread */

/** \} Applications */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
