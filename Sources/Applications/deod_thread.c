/** ****************************************************************************
  * \file    deod_thread.c
  * \author  Jang Ho Jong
  * \version V1.0.0
  * \date    2023.06.10
  * \brief   DEOD Thread Body
  * ***************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "deod_thread.h"
#include "device.h"


/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/** \defgroup Deod_Thread_Module_Private_Variables SpeedMeter 모듈 비공개 변수
  * \{ */

u32_t     g_DEOD_Check_timer=0;				/* 장애물 및 탈선 감지 주기 타이머 */
u32_t   	g_Low_Voltage_Check_timer=0;
/* Private function prototypes -----------------------------------------------*/
/** \defgroup Deod_Thread_Module_Private_Functions Key_Proc 모듈 비공개 함수
  * \{ */

/** \} Deod_Thread_Module_Private_Functions */

/* Exported functions --------------------------------------------------------*/
/** \addtogroup Deod_Thread_Module_Exported_Functions
  * \{ */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/** \defgroup Deod_Thread_Private_Variables 시스템 모니터링 쓰레드 비공개 변수
  * \{ */
static void Decide_DEOD_State(void);
void Read_Safety_Sensor_OSSD(void);
static void Read_Safety_Sensor_Voltage(void);
void Low_Voltage_Check(void);
/** \} Deod_Thread_Exported_Variables */

/* Private function prototypes -----------------------------------------------*/
/** \defgroup Deod_Thread_Private_Functions 시스템 모니터링 쓰레드 비공개 함수
  * \{ */

/* Private functions ---------------------------------------------------------*/
/** \addtogroup Deod_Thread_Private_Functions
  * \{ */

/** \brief  DEOD 쓰레드 본체
  * \param  argument - 쓰레드로 전달된 인자
  * \note
  * \retval 없음
  */
void   Deod_Thread(void)
{

	 /** 열차가 100Km/h로 달릴경우 1 Sec당 27.7M 전진. */
	 /** 100msec일 경우 이동 거리는 약 2.7M 이동함.  10mse 27cm 이동함. */
   if(g_DEOD_Check_timer  >= DEOD_SYS_CHECK_TIME)
   {  
	    g_DEOD_Check_timer = 0;
//    g_monitor_flag = True;
      Read_Safety_Sensor_Voltage();                                    /** 전류체크 */
      Read_Safety_Sensor_OSSD();
//		  Low_Voltage_Check();
		  Decide_DEOD_State();                                             /** 센서 동작 여부 최종 판단 */
	 }
}  
/*
void Low_Voltage_Check(void)
{
		u08_t i;
	
    if(g_Low_Voltage_Check_timer  >= 4000U)
   { 
      g_Low_Voltage_Check_timer = 0;		 
      for(i = 0; i < kNbrOfSIs ; i ++)
	    {  
	        if( sdin[i]->Sensor_Low_Counter > 200U)       // 전체 카운터값 변경
		      {
              sdin[i]->Sensor_Low_Counter = 5000U;		    // 센서카운터 변경				
          }	
          else 
          {
						sdin[i]->Sensor_Low_Counter = 0U;
          }	
				}
			}			
}
*/
/** \brief  센서의 전류값을 읽어서 장애물 및 탈선이 되었는지를 주기적으로 감시한다.
  * \note   센서의 전류값이 160mV  ~ 190mV 이면 정상 범위
  *         센서의 전류값이 2100mV ~ 2950mV 이면 장애물 및 탈선이 감지됨    
  * \retval 없음
  */

static void Read_Safety_Sensor_Voltage(void)
{
static u16_t adc1_value=0, adc2_value=0, adc3_value=0,adc4_value=0;
 
/*****************************************************************************
  **************************** OBR Sensor  ***********************************
******************************************************************************/
//    cprintf("\nSensor Voltage Check Interval Time\n");			       /** 모니터링 주기 테스트 코드 */
	  HAL_ADC_Start(&hadc1);
 		while(HAL_ADC_PollForConversion(&hadc1, 5) == HAL_OK)
 		{
			adc1_value = (u16_t)HAL_ADC_GetValue(&hadc1);
			adc1_value = ((3300U * adc1_value) / 4095U);	                 /** 검지시 200~ 499mV   평상시  51 ~ 199mV */
			sdin[kSI_1]->Sensor_Voltage = adc1_value;
	 
      if(adc1_value > cMIN_ACT_CUR) 
      { 
        sdin[kSI_1]->Sensor_flag = True;                                      /** 센서 이상 체크 */
      }
			else if(adc1_value < cLOW_CUR && sdin[kSI_1]->low_counter_start == 0)    /** Low Voltage */
			{
				sdin[kSI_1]->Sensor_Low_Counter++;
				sdin[kSI_1]->low_counter_start = 1;
				sdin[kSI_1]->Sensor_flag = True;                                       /** 센서 이상 체크 */
//				sdin[kSI_1]->Sensor_fail_flag = False;				
			}
			else if(adc1_value < cLOW_CUR && sdin[kSI_1]->low_counter_start == 1)
			{
					sdin[kSI_1]->Sensor_Low_Counter++;
			}
			else if(adc1_value > cLOW_CUR && sdin[kSI_1]->low_counter_start == 1)
			{
				   sdin[kSI_1]->Sensor_Low_Counter=0;
				   sdin[kSI_1]->low_counter_start = 0;
					 sdin[kSI_1]->Sensor_flag = False;
			}
	 	}
       

/*****************************************************************************
  **************************** DER Sensor  ***********************************
******************************************************************************/		
	  HAL_ADC_Start(&hadc1);
 		while(HAL_ADC_PollForConversion(&hadc1, 5) ==HAL_OK)
		{
			adc2_value = (u16_t)HAL_ADC_GetValue(&hadc1);
			adc2_value = ((3300U * adc2_value) / 4095U);
      sdin[kSI_2] ->Sensor_Voltage = adc2_value;
      if(adc2_value > cMIN_ACT_CUR) 
      { 
        sdin[kSI_2]->Sensor_flag = True;
      }
			else if(adc2_value < cLOW_CUR && sdin[kSI_2]->low_counter_start == 0)
			{
				sdin[kSI_2]->Sensor_Low_Counter++;
				sdin[kSI_2]->low_counter_start = 1;
				sdin[kSI_2]->Sensor_flag = True;
//				sdin[kSI_2]->Sensor_fail_flag = False;
			}
			else if(adc2_value < cLOW_CUR && sdin[kSI_2]->low_counter_start == 1)
			{
							sdin[kSI_2]->Sensor_Low_Counter++;
			}
			else if(adc2_value > cLOW_CUR && sdin[kSI_2]->low_counter_start == 1)
			{
				   sdin[kSI_2]->Sensor_Low_Counter=0;
				   sdin[kSI_2]->low_counter_start = 0;
					 sdin[kSI_2]->Sensor_flag = False;
			}
	  }

/*****************************************************************************
  **************************** OBL Sensor  ***********************************
******************************************************************************/		
	  HAL_ADC_Start(&hadc1);
 		while(HAL_ADC_PollForConversion(&hadc1, 5) ==HAL_OK)
		{
			adc3_value = (u16_t)HAL_ADC_GetValue(&hadc1);
			adc3_value = ((3300U * adc3_value) / 4095U);
      sdin[kSI_3] ->Sensor_Voltage = adc3_value;
      if(adc3_value > cMIN_ACT_CUR) 
      { 
        sdin[kSI_3]->Sensor_flag = True;
      }
			else if(adc3_value < cLOW_CUR && sdin[kSI_3]->low_counter_start == 0)
			{
				sdin[kSI_3]->Sensor_Low_Counter++;
				sdin[kSI_3]->low_counter_start = 1;
				sdin[kSI_3]->Sensor_flag = True;
//				sdin[kSI_3]->Sensor_fail_flag = False;				
				
			}
			else if(adc3_value < cLOW_CUR && sdin[kSI_3]->low_counter_start == 1)
			{
							sdin[kSI_3]->Sensor_Low_Counter++;
			}
			else if(adc3_value > cLOW_CUR && sdin[kSI_3]->low_counter_start == 1)
			{
				   sdin[kSI_3]->Sensor_Low_Counter=0;
				sdin[kSI_3]->low_counter_start = 0;
								sdin[kSI_3]->Sensor_flag = False;
			}	
	  }		

/*****************************************************************************
  **************************** DEL Sensor  ***********************************
******************************************************************************/
	  HAL_ADC_Start(&hadc1);
 		while(HAL_ADC_PollForConversion(&hadc1, 5) ==HAL_OK)
		{
			adc4_value = (u16_t)HAL_ADC_GetValue(&hadc1);
			adc4_value = ((3300U * adc4_value) / 4095U);
      sdin[kSI_4]->Sensor_Voltage = adc4_value;
      if(adc4_value > cMIN_ACT_CUR) 
      { 
        sdin[kSI_4]->Sensor_flag = True;
      }
			else if(adc4_value < cLOW_CUR && sdin[kSI_4]->low_counter_start == 0)
			{
				sdin[kSI_4]->Sensor_Low_Counter++;
				sdin[kSI_4]->low_counter_start = 1;
				sdin[kSI_4]->Sensor_flag = True;
//				sdin[kSI_4]->Sensor_fail_flag = False;				
			}
			else if(adc4_value < cLOW_CUR && sdin[kSI_4]->low_counter_start == 1)
			{
							sdin[kSI_4]->Sensor_Low_Counter++;
			}
			else if(adc1_value > cLOW_CUR && sdin[kSI_4]->low_counter_start == 1)
			{
				sdin[kSI_4]->Sensor_Low_Counter=0;
				sdin[kSI_4]->low_counter_start = 0;
				sdin[kSI_4]->Sensor_flag = False;
			}
	  }	
    if(g_monitor_flag == True)
    {			
		  cprintf("OBR  = %d\r\n", sdin[kSI_1]->Sensor_Voltage);
		  cprintf("DER  = %d\r\n", sdin[kSI_2]->Sensor_Voltage);
		  cprintf("OBL  = %d\r\n", sdin[kSI_3]->Sensor_Voltage);
		  cprintf("DEL  = %d\r\n", sdin[kSI_4]->Sensor_Voltage);
		}
}

void Read_Safety_Sensor_OSSD(void)
{
	u08_t sensor_num=0;
/*------------------------------------------------------------*/
// 센서 OSSD1(탈선) 설정값 일기 
/*------------------------------------------------------------*/	
//bool_t final_state = False;
//uint32_t tick_start = HAL_GetTick();
  
	for(sensor_num = 0; sensor_num < kNbrOfSIs; sensor_num++)
	{
		if(sdin[sensor_num]->Sensor_flag == True) 
		{	
      if(sensor_num == 0U)
			{
//        while((HAL_GetTick() - tick_start) < 3)
//        {
				  sdin[sensor_num]->Sensor_OSSD1 = (Bool_t)(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0));
				  sdin[sensor_num]->Sensor_OSSD2 = (Bool_t)(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1));
//        }
			}
			else if(sensor_num == 1U)
			{
				sdin[sensor_num]->Sensor_OSSD1 = (Bool_t)(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2));
				sdin[sensor_num]->Sensor_OSSD2 = (Bool_t)(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3));
			}
			else if(sensor_num == 2U)
			{
				sdin[sensor_num]->Sensor_OSSD1 = (Bool_t)(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4));
				sdin[sensor_num]->Sensor_OSSD2 = (Bool_t)(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5));
			}
			else if(sensor_num == 3U)
			{
    		sdin[sensor_num]->Sensor_OSSD1 = (Bool_t)(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6));
				sdin[sensor_num]->Sensor_OSSD2 = (Bool_t)(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7));
			}
      
			if((sdin[sensor_num]->Sensor_OSSD1 == True) && (sdin[sensor_num]->Sensor_OSSD2 == True	)) 
			{ 
				sdin[sensor_num]->Sensor_Counter++; 
			}
			else 
			{ 
				sdin[sensor_num]->Sensor_Counter = False; 
			}
		}

	  if(g_monitor_flag == True)
		{	
			cprintf("SENSOR%d state = %s\n",(sensor_num+1),(sdin[sensor_num] ->Sensor_OSSD1  == True) ? "ON" : "OFF" );
			cprintf("SENSOR%d state = %s\n",(sensor_num+1),(sdin[sensor_num] ->Sensor_OSSD2== True) ? "ON" : "OFF" );
			cprintf("SENSOR%d Counter = %d\n",(sensor_num+1),sdin[sensor_num] ->Sensor_Counter);
		}
	}
}


static void Decide_DEOD_State(void)
{
	u08_t i;
	Bool_t eb1_state;
	Bool_t eb2_state;	
	
  Led_t     * led[kNbrOfSIs];                                        // 센서의 수량 만큼 배열 확보
  Led_t     * e_led[kNbrOfSIs];                                        // 센서의 수량 만큼 배열 확보
  Relay_t   * relay[kNbrOfSIs];                                      // 센서의 수량 만큼 배열 확보

  led[kSI_1] = obr_led;
  led[kSI_2] = der_led;
  led[kSI_3] = obl_led;
  led[kSI_4] = del_led;
  
  e_led[kSI_1] = e_obr_led;
  e_led[kSI_2] = e_der_led;
  e_led[kSI_3] = e_obl_led;
  e_led[kSI_4] = e_del_led;
  
  relay[kSI_1] = obr_out ;
  relay[kSI_2] = der_out ;
  relay[kSI_3] = obl_out ;
  relay[kSI_4] = del_out ;
  
  for(i = 0; i < kNbrOfSIs ; i ++)
	{
	  if((sdin[i]->Sensor_Voltage < cMAX_ACT_CUR)  && (sdin[i]->Sensor_Voltage > cMIN_ACT_CUR ))  //장애물 감지 상태(2950 ~ 2100)
		{
		  if(((sdin[i]->Sensor_OSSD1== True) && (sdin[i]->Sensor_OSSD2== True) && (sdin[i]->Sensor_Counter > cMAX_COUNT)
				&& (sdin[i]->Sensor_fail_flag == False)))
			{
				cprintf("\n\n\nF_003.(Obstacle/Derailment Activ = %s)\n",led[i]->name);        /** Fault Code 출력	*/
			  Led_On(e_led[i]);	                                                               /** 감지된 해당 LED ON */						
        Led_On(e_eb_led);
				Relay_On(eb1_out);
				Relay_On(eb2_out);
        Relay_On(relay[i]);
//        Relay_Off(hlt_out);
//        Led_Off(hlt_led);        
//    		cprintf("\nEB1/EB2 Signal Output Command\n");			                            /** 퍼포먼스 테스트 코드 */	
				HAL_Delay(10);
//			  eb1_state = (u08_t)(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_8));
			  eb1_state = DI_Get_State(din[kRYFBI_EB1]);        
//			  eb2_state = (u08_t)(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_9));	
			  eb2_state = DI_Get_State(din[kRYFBI_EB2]); 				
//				cprintf("\n\eb1_state = %d\n",eb1_state);       	
//				Relay_On(eb2_out);
//				HAL_Delay(10);				
//			  eb2_state = (u08_t)(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_9));	
//				cprintf("\n\eb2_state = %d\n",eb2_state); 				
//				Relay_On(relay[i+5U]);                                       /** 감지된 해당 릴레이 Signal ON */
				if((eb1_state != True) || (eb2_state != True))
				{
					 Relay_Off(hlt_out);                                        /** HLT Signal OFF */
           Led_Off(hlt_led);					
					 Watchdog_Lock(wdog);
					 cprintf("Relay abnormal action then Health Relay On\n\n");
				}
				Led_On(run_led);	
    	  while(1)
        {}; 
		   }
    }	
    else if(sdin[i]->Sensor_Voltage < cLOW_CUR && sdin[i]->Sensor_flag == True && sdin[i]->Sensor_Low_Counter > 1000U )      /** 500mvolt 이하 전압 감지 */
    { 		 
			   cprintf("\n\n\nF_002.(Low Voltage or Disconnect Sernsor = %s)\n",led[i]->name);   /** Fault Code 출력	*/
//			   cprintf("\n\nLow Voltage  = %d\n",sdin[i]->Sensor_Voltage);                     /** Fault Code 출력	*/
//         if(sdin[i]->Sensor_Low_Counter  % 200 == 0)
 //        {g
             Led_On(led[i]);			
//             Led_Toggle(led[i]);
			       Relay_Off(hlt_out);	                                           /** HLT Signal OFF */
			       Led_Off(hlt_led);	                                             /** HLT Signal OFF */
             sdin[i]->Sensor_fail_flag = True;  
//         }           
     				 
    }
		else if(sdin[i]->Sensor_Voltage > (cMAX_ACT_CUR+1) && sdin[i]->Sensor_flag == True)     /** 2023년 12월 12일 추가   (2700mvolt 이상 검출시) */
    {    
			 cprintf("\n\n\nF_002.(Obstacle/Derailment Sensor High voltage = %s)\n",led[i]->name);      /** Fault Code 출력	*/	
       Led_On(led[i]);      
		   Relay_Off(hlt_out);	                                                                      /** HLT Signal OFF */
			 Led_Off(hlt_led);
       sdin[i]->Sensor_fail_flag = True;                                                          /** Fault 상황발생	*/	
    }
  }
 

	
	
  for(i = 0; i < 3 ; i += 2)                                              // OBR,DER,OBL,DEL   
  {
     if((sdin[i]->Sensor_fail_flag == True)  && (sdin[i+2]->Sensor_fail_flag == True)) // Left,Right 동시에 불량발생
     {  
        cprintf("\n\n\nF_003 Sensor Abnormality Both= %s,%s\r\n",led[i]->name,led[i+2]->name);
        Led_On(led[i]);
        Led_On(led[i+2]);        
			  Relay_Off(hlt_out);	
        Led_Off(hlt_led);
        Led_On(run_led);
			  Relay_On(eb1_out);
			  Relay_On(eb2_out);			 
//		    Watchdog_Lock(wdog);  	 
			  while(1)
        {};  
     }
   }  
}
/** \} Deod_Thread_Module_Private_Functions */

/** \} Deod_Thread */

/** \} Applications */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
