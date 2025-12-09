/** ****************************************************************************
  * \file    stm32f4xx_it.c
  * \author  Jang Ho Jong
  * \version V1.0.0
  * \date    2024.10.10
  * \brief   Main Interrupt Service Routines.
  *          This file provides all exceptions handler and
  *          peripherals interrupt service routine.
  * ***************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "device.h"


 
#ifdef _RTE_
#include "RTE_Components.h"             /* Component selection */
#endif

/** \addtogroup Drivers
  * \{ */

/** \defgroup IRQs Interrupt 서비스 루틴
  * \brief 인터럽트 서비스 루틴입니다.
  * \{ */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** \addtogroup IRQs_Exported_Functions
  * \{ */

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @retval None
  */
//void NMI_Handler(void)
//{
//}

/**
  * @brief  This function handles Hard Fault exception.
  * @retval None
  */
//void HardFault_Handler(void)
//{
  /* Go to infinite loop when Hard Fault exception occurs */
//  while (1)
//  {

//  }
//}

/**
  * @brief  This function handles Memory Manage exception.
  * @retval None
  */
//void MemManage_Handler(void)
//{
  /* Go to infinite loop when Memory Manage exception occurs */
//  while (1)
//  {
//  }
//}

/**
  * @brief  This function handles Bus Fault exception.
  * @retval None
  */
//void BusFault_Handler(void)
//{
  /* Go to infinite loop when Bus Fault exception occurs */
//  while (1)
//  {
//  }
//}

/**
  * @brief  This function handles Usage Fault exception.
  * @retval None
  */
//void UsageFault_Handler(void)
//{
  /* Go to infinite loop when Usage Fault exception occurs */
//  while (1)
//  {
//  }
//}

/**
  * @brief  This function handles SVCall exception.
  * @retval None
  */
 
//void SVC_Handler(void)
//{
//}
 

/**
  * @brief  This function handles Debug Monitor exception.
  * @retval None
  */
//void DebugMon_Handler(void)
//{
//}

/**
  * @brief  This function handles PendSVC exception.
  * @retval None
  */
 
//void PendSV_Handler(void)
//{
//}
 

/**
  * @brief  This function handles SysTick Handler.
  * @retval None
  */
 
void SysTick_Handler(void)
{
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();	
  Rs232_CountDown_Receive_Tick(serial3);
	g_Run_Led_timer++;									                               // RUN LED 동작 Timer   500msec
	g_Pwr_Ok_timer++;                                                  // 전원 입력 OK 신호 체크 timer
	g_Temp_check_timer++;								                               // Temperator Check Timer
	g_DEOD_Check_timer++;                                              // 장애물 센서 감지 Timer
	g_Flt_Signal_timer++;                                              // Safety Relay FLT 신호 감지 타이머
//  g_Relay_FB_check_timer++;	
//	g_Sensor_Check_timer++;
	/* Watch Dog Signal Toggel 15msec */
  /* 일정시간(15msec )마다 위치독 타이머를 갱신하여 WDT_OK 출력신호를 TOGGLE 한다. */
	g_Watch_Dog_timer++;								                               // Watch Dog Toggle Timer  15msec		
	g_Watchdog_Check_Pin_Timer++;                                      // Watch Dog Check_Pin 주기 타이머 
  if (g_Watch_Dog_timer >= WATCH_DOG_TOGGLE_TIME)
  {  
     g_Watch_Dog_timer = 0;
		 Watchdog_Toggle(wdog);		
	}
	g_Low_Voltage_Check_timer++;
}
 

/**
 * @brief  This function handles USART3 interrupt request.
 * @retval None
 */
void USART3_IRQHandler(void)
{
	
  /* Overrun 에러가 발생한 경우 */
//  if (__HAL_UART_GET_FLAG(serial3->handle, UART_FLAG_ORE) != 0U)
	  if((u08_t)__HAL_UART_GET_FLAG(serial3->handle, UART_FLAG_ORE) != (u08_t)RESET)
   {
//    Led_Toggle(error_led);
    /* Overrun 에러 플래그 클리어 */
    __HAL_UART_CLEAR_OREFLAG(serial3->handle);
   }
  
  /* 데이터 수신이 발생한 경우 */
//  if (__HAL_UART_GET_FLAG(serial3->handle, UART_FLAG_RXNE) != 0U)
	 if((u08_t) __HAL_UART_GET_FLAG(serial3->handle, UART_FLAG_RXNE)!= (u08_t)RESET)
  {
    /* RXNE 플래그 클리어 */
    __HAL_UART_CLEAR_FLAG(serial3->handle, UART_FLAG_RXNE);
    /* serial 포트 수신 인터럽트 처리 */
    Rs232_IRQHandler_RXNE(serial3);
    /* 수신 타이머를 설정한다. */
    Rs232_Set_Receive_Tick(serial3, msec(5));
  }

  /* 송신이 가능한 경우 */
//  if (__HAL_UART_GET_FLAG(serial3->handle, UART_FLAG_TXE) != 0U)
  if((u08_t) __HAL_UART_GET_FLAG(serial3->handle, UART_FLAG_TXE)!= (u08_t)RESET)
   {
    /* TXE 플래그 클리어 */
    __HAL_UART_CLEAR_FLAG(serial3->handle, UART_FLAG_TXE);
    /* serial 포트 송신 인터럽트 처리 */
    Rs232_IRQHandler_TXE(serial3);
   }
}

/** \} IRQs_Exported_Functions */

/* Private functions ---------------------------------------------------------*/

/** @} Drivers */

/** @} IRQs */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
