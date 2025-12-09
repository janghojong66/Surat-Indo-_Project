/** ****************************************************************************
  * \file    Self_Diagnose_perform.c(Performing pre-self-diagnosis)
  * \author  Jang Ho Jong
  * \version V1.0.0
  * \date    2023.06.10
  * \brief   Self_Diagnose_perform Thread Body 
  * ***************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "self_diagnose_perform.h"
/*----------------------------------------------------------------------------
 *      'Self_Diagnose_perform': Performing pre-self-diagnosis
 *---------------------------------------------------------------------------*/
/** \addtogroup Applications
  * \{ */

/** \defgroup Self_Diagnose_perform
  * \brief Self_Diagnose_perform
  * \{ */


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/** \defgroup Self_Diagnose_perform_Variables Self_Diagnose_perform 비공개 변수
  * \{ */
static void Read_Sensor_Current_Value(void);
static void Read_Sensor_OSSD_Input(void);
void Read_Relay_FeedBack_State(void);


/* Private function prototypes -----------------------------------------------*/


/** \brief Self_Diagnose_perform 본체
  * \note  Sensor에 전원을 공급하고 자가진단을 수행하여 센서, 릴레이등 이상을 검사하고
  * \      이상 발생시 HLT_SIGNAL를 송출한다.
  * \param  argument - 쓰레드로 전달된 인자
  * \retval 없음
  */
void Self_Diagnose_Perform(void)
{
	
		/* Sensor Power Control PIN을 ON 시켜서 장애물 센서쪽에 전원을 공급한다. */
//	DO_On(snp_out);
	
	/* 감지 센서의 연결 상태 점검  */
	/* 센서의 연결 상태는 ADC1의 전류가 xxxmVolt ~ xxxxmVolt 상태를 유지 해야 함. */
	HAL_Delay(50);
	/* 콘솔 표시용 Flag "ON"하여 상태가 표시되도록 함.  */
  g_monitor_flag = True;

  Relay_On(hlt_out);                                                 // SYSTEM HLT Signal On 
  Led_On(hlt_led);                                                   // SYSTEM HLT LED On

  Read_Relay_FeedBack_State();   
	
  Read_Sensor_Current_Value();     

//  g_monitor_flag = False;
	
	/* 감지 센서의 초기화 상태 점검  */
	/* 센서의 초기화 상태는 각 센서의 OSSD1, OSSD2의 입력상태가 Low 이어야 함. */
  
  Read_Sensor_OSSD_Input();                                          // PC로 동작할 경우에는 PB15 PIN를 읽어서 2CH or 4CH를 선택한다.  
	/* 콘솔 표시용 Flag "OFF"하여 상태가 표시 안 되도록 함.  */  
  g_monitor_flag = False;
  
	/* 릴레이 피드백 상태 점검  */
	/* 릴레이 피드백 상태를 점검하기 위해서는 릴레이 제어포트를 "ON"시켜야함  */
	/* 릴레이 피드백 상태는 각각의 릴레이 FB 값이 HIGH 이어야 함. */
//  Read_Relay_FeedBack_State();   

	Check_Temperature();
}

/** \brief  ADUM4190으로부터 현재의 전압값을 읽어온다.
  * \param  ADC-Sensor 구조체 포인터
  * \note   센서의 연결상태를 체크하여 연결되지 않았으면 계속 Rebooting 한다
  * \retval 없음
  */

static void Read_Sensor_Current_Value(void)
{
  static  u16_t cur_val1=0, cur_val2=0, cur_val3=0,cur_val4=0;
  u08_t  i;
//  Relay_t     * relayout[5];
  Led_t       * ledout[4];
  
  ledout[0] = obr_led ;
  ledout[1] = der_led ;
  ledout[2] = obl_led ;
  ledout[3] = del_led ;  
/*    
  relayout[0] = obr_out ;
  relayout[1] = der_out ;
  relayout[2] = obl_out ;
  relayout[3] = del_out ;
  relayout[4] = hlt_out ;
*/  
/*****************************************************************************
  **************************** OBR Sensor  ***********************************
******************************************************************************/
  HAL_ADC_Start(&hadc1);
	while(HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK)
  {  
     cur_val1 = (u16_t)HAL_ADC_GetValue(&hadc1);
		 cur_val1 = ((3300U * cur_val1) / 4095U);
		 sdin[kSI_1]->Sensor_Voltage = (cur_val1);	
//      if(cur_val1 > cMIN_ACT_CUR || cur_val1 < cLOW_CUR)                       /** 측정볼트가 500 < ||  > 2700  */ 
//      { 
//        sdin[kSI_1]->Sensor_fail_flag = True;                                  /** 센서 이상 상태 이고 */
//      }
//     	else
//      {
//        sdin[kSI_1]->Sensor_fail_flag = False;  // 센서는 정상 상태
//      }		
	}
  if(g_monitor_flag == True) cprintf("OBR SENSOR Voltage = %d\r\n", cur_val1);
		
/*****************************************************************************
  **************************** DER Sensor  ***********************************
******************************************************************************/
	HAL_ADC_Start(&hadc1);
	while(HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK)
  {  
		 cur_val2 = (u16_t)HAL_ADC_GetValue(&hadc1);
		 cur_val2 = ((3300U * cur_val2) / 4095U);
		 sdin[kSI_2]->Sensor_Voltage = (cur_val2);
//      if(cur_val2 > cMIN_ACT_CUR || cur_val2 < cLOW_CUR ) 
//      { 
//        sdin[kSI_2]->Sensor_fail_flag = True;        // 센서가 동작된 상태 이고 (250908)
//      }
//     	else
//      {
//        sdin[kSI_2]->Sensor_fail_flag = False;
//      }				
	}
  if(g_monitor_flag == True) cprintf("DER SENSOR Voltage = %d\r\n", cur_val2);

/*****************************************************************************
  **************************** OBL Sensor  ***********************************
******************************************************************************/
	 HAL_ADC_Start(&hadc1);
	 while(HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK)
	 {
		  cur_val3 = (u16_t)HAL_ADC_GetValue(&hadc1);
			cur_val3 = ((3300U * cur_val3) / 4095U);
			sdin[kSI_3]->Sensor_Voltage = (cur_val3);
//      if(cur_val3 > cMIN_ACT_CUR || cur_val3 < cLOW_CUR ) 
//      { 
//        sdin[kSI_3]->Sensor_flag = True;
//      }
//     	else
//      {
//        sdin[kSI_3]->Sensor_fail_flag = False;
//      }				 
	 }
   if(g_monitor_flag == True) cprintf("OBL SENSOR Voltage = %d\r\n", cur_val3);

/*****************************************************************************
  **************************** DEL Sensor  ***********************************
******************************************************************************/
	 HAL_ADC_Start(&hadc1);
	 while(HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK)
	 {    
		  cur_val4 = (u16_t)HAL_ADC_GetValue(&hadc1);
			cur_val4 = ((3300U * cur_val4) / 4095U);
			sdin[kSI_4]->Sensor_Voltage = (cur_val4);
//		  if(cur_val4 > cMIN_ACT_CUR || cur_val4 < cLOW_CUR ) 
//      { 
//        sdin[kSI_4]->Sensor_flag = True;
//      }
//     	else
//      {
//        sdin[kSI_4]->Sensor_fail_flag = False;
//      }		
	 }  	 
   if(g_monitor_flag == True) cprintf("DEL SENSOR Voltage = %d\r\n", cur_val4);	

/***************************************************/
/* Sensor Voltage 이상 유무 Check하여 이상있으면 이상 상태 표시 */
/***************************************************/
	
   HAL_Delay(1000);
  	 
   for(i = 0; i < kNbrOfSIs ; i ++)    
	 {
	    if((sdin[i]->Sensor_Voltage < cMAX_ACT_CUR)  && (sdin[i]->Sensor_Voltage > cMIN_ACT_CUR ))  //장애물 감지 상태(2700 ~ 500)
		  {
//		     if((sdin[i]->Sensor_OSSD1== True) && (sdin[i]->Sensor_OSSD2== True))   OSSD1,OSSD2는 현상태에서는 체크 안됨..
//				 {
	          cprintf("(Self Diagnostic)Active Sensor = %s\r\n",ledout[i]->name);
		        Relay_Off(hlt_out);                                                /** HLT Signal Low 처리후 계속 프로세스 진행 */
            Led_Off(hlt_led);
            Led_On(ledout[i]);
            sdin[i]->Sensor_fail_flag = True;
//		      cprintf("System While Loop!(Disconnect Sernsor)\n\n");
//				  while(1)
//          {};				
//         }
			}
  //    else if((sdin[i]->Sensor_Voltage < cLOW_CUR ) && (sdin[i]->Sensor_fail_flag == False))      /** 500mvolt 이하 전압 감지 */
      else if((sdin[i]->Sensor_Voltage < cLOW_CUR ))                           /** 500mvolt 이하 전압 감지 */
      { 			
			   cprintf("(Self Diagnostic)Disconnect Sernsor = %s)\n",ledout[i]->name);               /** Fault Code 출력	*/
				 cprintf("\n\nLow Voltage  = %d\n",sdin[i]->Sensor_Voltage); 
         Led_On(ledout[i]);			
			   Relay_Off(hlt_out);	                                                 /** HLT Signal OFF */
//				 Relay_Off(relayout[4]);
			   Led_Off(hlt_led);	                                                   /** HLT Signal OFF */
         sdin[i]->Sensor_fail_flag = True;                                     /**Falut Code 발생 **/ 
      }
      else if(sdin[i]->Sensor_Voltage > cMAX_CUR)	
      {
			   cprintf("(Self Diagnostic)High Voltage = %s\n",ledout[i]->name);               /** Fault Code 출력	*/				
				 cprintf(" Sernsor High Voltage = %d\n",sdin[i]->Sensor_Voltage);             /** Fault Code 출력	*/
         Led_On(ledout[i]);			
			   Relay_Off(hlt_out);	                                                 /** HLT Signal OFF */
//				 Relay_Off(relayout[4]);	
			   Led_Off(hlt_led);	                                                   /** HLT Signal OFF */
         sdin[i]->Sensor_fail_flag = True;                                     /**Falut Code 발생 **/ 
      }	
	 } 
	 
   for(i = 0; i < 3 ; i++)                                          // OBR,DER,OBL,DEL   
   {
      if((sdin[i]->Sensor_fail_flag == True)  && (sdin[i+2]->Sensor_fail_flag == True)) /** Left,Right 동시에  Falut Code 발생 **/ 
      {  
         cprintf("(Self Diagnostic)Sensor Abnormality Both= %s,%s\r\n",ledout[i]->name,ledout[i+1]->name);
         Led_On(ledout[i]);
         Led_On(ledout[i+2]);        
				 Relay_Off(hlt_out);	
//				 Relay_Off(relayout[4]);		
         Led_Off(hlt_led);
         Led_On(run_led);
				 Led_On(e_eb_led);			
  			 Relay_On(eb1_out);
				 Relay_On(eb2_out);
//			   Watchdog_Lock(wdog);        
				 while(1)
         {};  
      }
   }
}  

/** \brief  TEST SN#_ON Port를 구동하여 센서의 OSSD1,OSSD2의 값을 읽어온다.
  * \note   OSSD1,OSSD2의 값이 Low 상태이어야 정상임.
  * \retval 없음
  */

static void Read_Sensor_OSSD_Input(void)
{
	u08_t	i;
  Relay_t     * relayout[4];
  Led_t       * ledout[4];
  
  ledout[0] = obr_led ;
  ledout[1] = der_led ;
  ledout[2] = obl_led ;
  ledout[3] = del_led ;  
/*    
  relayout[0] = obr_out ;
  relayout[1] = der_out ;
  relayout[2] = obl_out ;
  relayout[3] = del_out ;
 */ 
  static Bool_t    Sensor_Ossd[kNbrOfSOs];
  
	DO_On(mcu_test1);                                                  // 20231212 추가
  HAL_Delay(20);                                                     // 릴레이 동작 시간 필요함.
 	Sensor_Ossd[kSO_1] = (Bool_t)(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0));
	Sensor_Ossd[kSO_2] = (Bool_t)(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1));  
  if(g_monitor_flag == True)
  {
    cprintf("SENSOR1_1(OBR) state = %s\n",(Sensor_Ossd[kSO_1]  == True) ? "1" : "0");
    cprintf("SENSOR1_2(OBR) state = %s\n",(Sensor_Ossd[kSO_2]  == True) ? "1" : "0");
  }
	DO_Off(mcu_test1);                                                  // 20231212 추가

	DO_On(mcu_test2);                                                   // 20231212 추가
	HAL_Delay(20);
	Sensor_Ossd[kSO_3] = (Bool_t)(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2));
	Sensor_Ossd[kSO_4] = (Bool_t)(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3));
  if(g_monitor_flag == True)
  {
    cprintf("SENSOR2_1(DER) state = %s\n",(Sensor_Ossd[kSO_3] == True) ? "1" : "0");
    cprintf("SENSOR2_2(DER) state = %s\n",(Sensor_Ossd[kSO_4] == True) ? "1" : "0");
  }
	DO_Off(mcu_test2);                                                 // 20231212 추가

	DO_On(mcu_test3);                                                  // 20231212 추가
	HAL_Delay(20);
	Sensor_Ossd[kSO_5] = (Bool_t)(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4));
	Sensor_Ossd[kSO_6] = (Bool_t)(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5));
  if(g_monitor_flag == True)
  {
    cprintf("SENSOR3_1(OBL) state = %s\n",(Sensor_Ossd[kSO_5] == True) ? "1" : "0");
    cprintf("SENSOR3_2(OBL) state = %s\n",(Sensor_Ossd[kSO_6] == True) ? "1" : "0");
  }	
	DO_Off(mcu_test3);                                                 // 20231212 추가

	DO_On(mcu_test4);                                                  // 20231212 추가
	HAL_Delay(20);
//	Sensor_Ossd[kSO_7] = (Bool_t)(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6));
	Sensor_Ossd[kSO_8] = (Bool_t)(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7));
  if(g_monitor_flag == True)
  {
    cprintf("SENSOR4_1(DEL) state = %s\n",(Sensor_Ossd[kSO_7] == True) ? "1" : "0");
    cprintf("SENSOR4_2(DEL) state = %s\n",(Sensor_Ossd[kSO_8] == True) ? "1" : "0");
  }
	DO_Off(mcu_test4);                                                 // 20231212 추가
	
  for(i = 0; i < kNbrOfSOs; i+=2)
	{
		if((Sensor_Ossd[i] == True) || (Sensor_Ossd[i+1] == True))
		{
			 cprintf("==============Derailment or OBstacle Logic Value Error==================\r\n");
       sdin[i/2]->Sensor_fail_flag = True;      
		   Relay_Off(hlt_out);                                         // HLT Signal Low 처리후 계속 프로세스 진행
       Led_Off(hlt_led);
			 Led_On(ledout[i/2]);
		}
	}
   for(i = 0; i < 3 ; i += 2)   
  {
				if(((Sensor_Ossd[i] == True) && (Sensor_Ossd[i+1] == True)) && ((Sensor_Ossd[i+4] == True) && (Sensor_Ossd[i+5] == True)))
 //    if((sdin[i]->Sensor_fail_flag == True)  && (sdin[i+1]->Sensor_fail_flag == True)) // Left,Right 동시에 불량발생
     {  
        cprintf(" Sensor Abnormality Both= %s,%s\r\n",ledout[i]->name,ledout[i+2]->name);
					Led_On(ledout[i]);
          Led_On(ledout[i/2]);			
			  Relay_Off(hlt_out);	
        Led_Off(hlt_led);
//        Led_On(run_led);
//			  Led_On(e_led[i]);	                                                               /** 감지된 해당 LED ON */						
        Led_On(e_eb_led);
				Relay_On(eb1_out);
				Relay_On(eb2_out);
	//	    Watchdog_Lock(wdog);        
			  while(1)
        {};  
      }
   }
}
/** \brief  Relay를 제어하여 릴레이 FeedBack 값을 읽어온다.
  * \note   Relay FeedBack값이 HIGH 상태이어야 정상임.
  * \retval 없음
  */

//static void Read_Relay_FeedBack_State(void)
void Read_Relay_FeedBack_State(void)
{
u08_t 	i;
Bool_t   fb_state[kNbrOfRYFBIs];
	
	
/* RY_EB1 릴레이 high로 설정   */
/* RY_EB1_FB Port State Read */
/* HIGH(True) = 정상, LOW(False) = 이상 */

		Relay_On(eb1_out);
		HAL_Delay(10);
//    fb_state[kRYFBI_EB1] = (Bool_t)(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_8));
    fb_state[kRYFBI_EB1] = DI_Get_State(din[kRYFBI_EB1]);
		Relay_Off(eb1_out);
	
		Relay_On(eb2_out);
		HAL_Delay(10);
//		fb_state[kRYFBI_EB2] = (Bool_t)(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_9));
		fb_state[kRYFBI_EB2] = DI_Get_State(din[kRYFBI_EB2]);  
		Relay_Off(eb2_out);
	
		Relay_On(dmpc_out);
		HAL_Delay(10);
//		fb_state[kRYFBI_DMPC] = (Bool_t)(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_10));
		fb_state[kRYFBI_DMPC] = DI_Get_State(din[kRYFBI_DMPC]);     
		Relay_Off(dmpc_out);
	
		Relay_On(hlt_out);
		HAL_Delay(10);
//		fb_state[kRYFBI_HLT] = (Bool_t)(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_11));
		fb_state[kRYFBI_HLT] = DI_Get_State(din[kRYFBI_HLT]);   
//		Relay_Off(hlt_out);     // HLT Signal OFF 되면 TCMS에 통보됨에 따라 셀프테스트 진행시에는 OFF 시키면 안됨

		
		Relay_On(obr_out);
		HAL_Delay(10);
//		fb_state[kRYFBI_OBR] = (Bool_t)(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_12));
		fb_state[kRYFBI_OBR] = DI_Get_State(din[kRYFBI_OBR]);   
		Relay_Off(obr_out);

		Relay_On(der_out);
		HAL_Delay(10);
//		fb_state[kRYFBI_DER] = (Bool_t)(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_13));
    fb_state[kRYFBI_DER] = DI_Get_State(din[kRYFBI_DER]);
		Relay_Off(der_out);

		Relay_On(obl_out);
		HAL_Delay(10);   
//		fb_state[kRYFBI_OBL] = (Bool_t)(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_14));
		fb_state[kRYFBI_OBL] = DI_Get_State(din[kRYFBI_OBL]);   
		Relay_Off(obl_out);

    Relay_On(del_out);
		HAL_Delay(10);
//		fb_state[kRYFBI_DEL] = (Bool_t)(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_15));
		fb_state[kRYFBI_DEL] = DI_Get_State(din[kRYFBI_DEL]);    
    Relay_Off(del_out);

  	for(i=2; i < kNbrOfRYFBIs; i++)
  	{  
		   if(fb_state[i] == False)
			 {  
					cprintf("\n\n\nF_003.%d\n",i);                              // Fault Code 출력
					cprintf("========== System Halt(Relay Fail) ==========\r\n");
		      Relay_Off(hlt_out);                                         // HLT Signal Low 처리후 계속 프로세스 진행
          Led_Off(hlt_led);
				}
		}
}

/** \} Self_Diagnose_perform_Private_Functions */

/** \} Applications */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
