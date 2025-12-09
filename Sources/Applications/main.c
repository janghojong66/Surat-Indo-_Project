/** ****************************************************************************
  * \file    main.c 
  * \author  Jang Ho Jong
  * \version V1.0.0
  * \date    2024.10.10
  * \brief   Main Module Body
  * ***************************************************************************/
/** \remark  Version History
  * V1.0.0   - 최초 버전 - 2024.10.10
  *          - Power ON시 자기진단 수행시 Sensor 불량(Left,Right중 1개만)일 경우에는 HLT Signal
               Low(Relay, Led)처리후 Booting Processing 계속 진행
  *          - Power ON시 자기진단 수행시 Sensor 불량(Left,Right중 양쪽전부)일 경우에는 HLT Signal
               Low 및 System Shutdown 상태로 대기함.
  *          - HLT Signal은 고장발생시에만 Low 처리함.(EB 발생시에는 처리 안함)
  *          - EB1,EB2 출력시에는 HLT Signal High 상태 유지시킴
*/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "device.h"

/** \defgroup Applications Applications
  * \brief 시스템의 기능을 수행하는 부분입니다.
  * \{ */

/** \defgroup Main Main 모듈
  * \brief Board 초기화 및 부트로더 진입을 담당하는 모듈입니다.
  * \{ */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** \defgroup Main_Private_Functions Main 모듈 비공개 함수
  * \{ */
 
int32_t main(void); 
static void SystemClock_Config(void);
static void Error_Handler(void);


/** \} Main_Private_Functions */

/* Exported functions --------------------------------------------------------*/
/** \addtogroup Main_Exported_Functions
  * \{ */

/** @brief  Main program
  * @retval None
  */
int32_t main(void)
{
	
//	const u32_t tick =  msec(1000);
  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, Flash preread and Buffer caches
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Low Level Initialization      */
   HAL_Init();
   HAL_Delay(1000);	                                                 // usb to serial IC chip 초기화 완료까지 대기.....
                                                                     // selftest message 표시를 위해 Delay 추가

  /* Enable Flash prefetch */
   LL_FLASH_EnablePrefetch();
	
  /* Configure the system clock to 168 MHz */
   SystemClock_Config();
  /* Initialize serial ports on Shalom STM32F407VGT board */
   Device_Init();
  /* 프로젝트명,버젼,빌드일자,빌드 시간 표시 */
   cprintf("\n\n\t%s [V%d.%d.%d]\n\t(%s %s)\n", 
         PROJECT_NAME, VERSION_MAIN, VERSION_SUB1, VERSION_SUB2, 
         BUILD_DATE, BUILD_TIME);				 

  /* launch STL Tests */

	 STL_Self_Test();

/* Relay 및 Sensor 자기 진단 테스트를 수행한다.  */
	 Self_Diagnose_Perform();
//	HAL_Delay(500);	
	
   Rs232_Enable_Receiving(serial3);                                  // console port enable

	 while(1)
	 {
		/* System Monitoring 기능 수행  */
      Sysmon_Thread();

	  /* 충돌 및 탈선 감지 기능 수행 */
      Deod_Thread();
    
		/* 콘솔포트 명령어 처리 루틴  */
		  Console_Thread();
	 }	
}
/** \} Main_Exported_Functions */

/* Private functions ---------------------------------------------------------*/
/** \addtogroup Main_Private_Functions
  * \{ */

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 25
  *            PLL_N                          = 336
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  * @retval None
  */
static void SystemClock_Config(void)
{  
   RCC_ClkInitTypeDef RCC_ClkInitStruct;
   RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
   __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
   __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  

  /* Enable HSE Oscillator and activate PLL with HSE as source */
   RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
   RCC_OscInitStruct.HSEState = RCC_HSE_ON;
   RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
   RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
   RCC_OscInitStruct.PLL.PLLM = 25;
   RCC_OscInitStruct.PLL.PLLN = 336;
   RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
   RCC_OscInitStruct.PLL.PLLQ = 7;
   if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
   {
    /* Initialization Error */
      Error_Handler();
   }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
   RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
   RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
   RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
   RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
   RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
   if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
   {
    /* Initialization Error */
      Error_Handler();
   }

  /* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
   if (HAL_GetREVID() == 0x1001U)
   {
    /* Enable the Flash prefetch */
      __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
   }
}
/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
   while(1)
   {
//			Led_Toggle(error_led);	  
		  HAL_Delay(300U);
   }
}


/** \} Main_Private_Functions */

/** \} Main */ 

/** \} Applications */ 

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
