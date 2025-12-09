/** ****************************************************************************
  * \file    stl.c
  * \author  Jang Ho Jong
  * \version V1.0.0
  * \date    2023.06.10
  * \brief   stl self test Module Body
  * ***************************************************************************/
#include "stl.h"
#include "device.h"
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/

static void STL_SCH_Test(void);
static void STL_CPU_TM1_Test(void);
static void STL_CPU_TM2_Test(void);
static void STL_CPU_TM3_Test(void);
static void STL_CPU_TM4_Test(void);
static void STL_CPU_TM5_Test(void);
static void STL_CPU_TM6_Test(void);
static void STL_CPU_TM7_Test(void);
static void STL_CPU_TM8_Test(void);
static void STL_CPU_TM9_Test(void);
static void STL_CPU_TM10_Test(void);
static void STL_CPU_TM11_Test(void);
static void STL_FLASH_TM_Test(void);
static void STL_RAM_TM_Test(void);

void STL_Self_Test(void)
{ 
 
	STL_SCH_Test();
	STL_CPU_TM1_Test();
	STL_CPU_TM2_Test();
	STL_CPU_TM3_Test();
	STL_CPU_TM4_Test();
	STL_CPU_TM5_Test();
	STL_CPU_TM6_Test();
	STL_CPU_TM7_Test();
	STL_CPU_TM8_Test();
	STL_CPU_TM9_Test();
	STL_CPU_TM10_Test();
	STL_CPU_TM11_Test();	
	STL_FLASH_TM_Test();
	STL_RAM_TM_Test();	
}
/**
  * @brief  This function is executed in case of STL error occurrence.
  * @param  LedToggleSpeed
  * @retval None
  */
static void StlErrorHandler(u32_t LedToggleSpeed)
{   

   while (1)
   {  
		  HAL_Delay(LedToggleSpeed);
	 	  cprintf("\n\n\nF_001.\n");
   }
}

/**
  * @brief  This function is executed in case of STL SCH Test 
  * @param  None
  * @retval None
  */
static void STL_SCH_Test(void)
{
  /************/
  /* Init STL */
  /************/

  cprintf(
	"\n======================================================================"
  "\n=              STM32F407VGT CPU Arm core Test                        ="
  "\n======================================================================"
  "\n\n"
	);
  if (STL_SCH_Init() != STL_OK)
  {
     /* STL Defense Programming if LED1 is slowly blinking (1 sec. period) */
		 StlErrorHandler(1000);
	   cprintf("\n\n\nSTL initializes the scheduler error.\n");
  }  
  else
  {  
			cprintf("\nSTL initializes the scheduler PASS.\n");	  	  
  }
}

/*********************************************************************************
CPU Arm?core tests
The STL includes the following CPU test modules, listed below, together with a generic description of the test capability:
?TM1: implements the test of general purpose register
?TM1L: implements a simplified version for the test of general purpose register
?TM2: implements the test of arithmetic functions
?TM3: implements the test of multiplier functions
?TM4: implements the test of control flow instructions
?TM5: implements the test of load/store unit(s)
?TM6: implements the test of shift and rotate functions
?TM7: implements the test of stack pointer
?TM8: implements the test of pipeline functions
?TM9: implements the test of the Arm Cortex 4 DSP functions
?TM10: implements the test of the FPU register bank. This TM shall not be used by the application unless the FPU is activated.
?TM11: implements the test of the FPU functions. This TM shall not be used by the application unless the FPU is activated.
The tests listed above can be run either in single or multiple test mode  
***************************************************************************************************/
  
  /***********/
  /* CPU TM1 */
  /* implements the test of general purpose register */
  /***********/
static void STL_CPU_TM1_Test(void)
{
  STL_TmStatus_t StlCpuTm1Status = STL_ERROR;	

	
  if (STL_SCH_RunCpuTM1(&StlCpuTm1Status) != STL_OK)
  {
    /* STL Defense Programming if LED1 is slowly blinking (1 sec. period) */ 
		  cprintf("\n(TM1)");	 		
			StlErrorHandler(3000); 
  }
  else if(StlCpuTm1Status != STL_PASSED)
  {  
     /* STL Tests FAILED if LED1 is quickly blinking (0.2 sec. period) */
//	 cprintf("\n(TM1)implements the test of general purpose register. FAIL");	
		 cprintf("\n(TM1_1)");			
		 StlErrorHandler(200);
  }  
	else
	{
		 cprintf("\n(TM1)implements the test of general purpose register. PASS");	  
	}
}

  /***********/
  /* CPU TM2 */
  /* implements the test of arithmetic functions */
  /***********/
static void STL_CPU_TM2_Test(void)
{
  STL_TmStatus_t StlCpuTm2Status = STL_ERROR;
 
  if (STL_SCH_RunCpuTM2(&StlCpuTm2Status) != STL_OK)
  {
     /* STL Defense Programming if LED1 is slowly blinking (1 sec. period) */
		 cprintf("\n(TM2)");	
     StlErrorHandler(1000);
  }
  else if (StlCpuTm2Status != STL_PASSED)
  {
     /* STL Tests FAILED if LED1 is quickly blinking (0.2 sec. period) */
		 cprintf("\n(TM2_2)");	
     StlErrorHandler(200);
  }
	else 
	{  
		 cprintf("\n(TM2)implements the test of arithmetic functions. PASS");	 
  }		
}

  /***********/
  /* CPU TM3 */
  /* implements the test of multiplier functions */
  /***********/

static void STL_CPU_TM3_Test(void)
{
  STL_TmStatus_t StlCpuTm3Status = STL_ERROR;

  if (STL_SCH_RunCpuTM3(&StlCpuTm3Status) != STL_OK)
  {
    /* STL Defense Programming if LED1 is slowly blinking (1 sec. period) */
						  cprintf("\n(TM3)");	
    StlErrorHandler(1000);
  }
  else if (StlCpuTm3Status != STL_PASSED)
  {
    /* STL Tests FAILED if LED1 is quickly blinking (0.2 sec. period) */
		  cprintf("\n(TM3)_1");	
    StlErrorHandler(200);
  }
	else
	{
		cprintf("\n(TM3)implements the test of multiplier functions.PASS");	 
  }		
}

  /***********/
  /* CPU TM4 */
  /* implements the test of control flow instructions */
  /***********/
static void STL_CPU_TM4_Test(void)
{
  STL_TmStatus_t StlCpuTm4Status = STL_ERROR;

  if (STL_SCH_RunCpuTM4(&StlCpuTm4Status) != STL_OK)
  {
    /* STL Defense Programming if LED1 is slowly blinking (1 sec. period) */	
    cprintf("\n(TM4)");		 
    StlErrorHandler(1000);
  }
  else if (StlCpuTm4Status != STL_PASSED)
  {
    /* STL Tests FAILED if LED1 is quickly blinking (0.2 sec. period) */
// Rs232_Printf(serial3, "\n(TM4)implements the test of control flow functions.PASS");		 
     cprintf("\n(TM4)_1");		 		
     StlErrorHandler(200);
  }
	else
	{
		 cprintf("\n(TM4)implements the test of control flow functions.PASS");	
	}
}


  /***********/
  /* CPU TM5 */
  /* implements the test of load/store unit(s) */
  /***********/
static void STL_CPU_TM5_Test(void)
{
  STL_TmStatus_t StlCpuTm5Status = STL_ERROR;

  if (STL_SCH_RunCpuTM5(&StlCpuTm5Status) != STL_OK)
  {
    /* STL Defense Programming if LED1 is slowly blinking (1 sec. period) */	 
     cprintf("\n(TM5)");		 		
     StlErrorHandler(1000);
  }
  else if (StlCpuTm5Status != STL_PASSED)
  {
    /* STL Tests FAILED if LED1 is quickly blinking (0.2 sec. period) */
		 cprintf("\n(TM5)_1");		 
     StlErrorHandler(200);
  }
	else
	{
		 cprintf("\n(TM5)implements the test of load/store unit(s).PASS");
	}
}


  /***********/
  /* CPU TM6 */
  /* implements the test of shift and rotate functions */
  /***********/
static void STL_CPU_TM6_Test(void)
{
  STL_TmStatus_t StlCpuTm6Status = STL_ERROR;

  if(STL_SCH_RunCpuTM6(&StlCpuTm6Status) != STL_OK)
  {
    /* STL Defense Programming if LED1 is slowly blinking (1 sec. period) */	  
     StlErrorHandler(1000);
  }
  else if(StlCpuTm6Status != STL_PASSED)
  {
     /* STL Tests FAILED if LED1 is quickly blinking (0.2 sec. period) */\
     StlErrorHandler(200);
  }
  else
	{
		 cprintf("\n(TM6)implements the test of shift and rotate functions.PASS");
	}
}

  /***********/
  /* CPU TM7 */
  /* implements the test of stack pointer */
  /***********/

static void STL_CPU_TM7_Test(void)
{
  STL_TmStatus_t StlCpuTm7Status = STL_ERROR;

  if(STL_SCH_RunCpuTM7(&StlCpuTm7Status) != STL_OK)
  {
    /* STL Defense Programming if LED1 is slowly blinking (1 sec. period) */	  
     StlErrorHandler(1000);
  }
  else if(StlCpuTm7Status != STL_PASSED)
  {
    /* STL Tests FAILED if LED1 is quickly blinking (0.2 sec. period) */\
     StlErrorHandler(200);
  }
	else
	{
		 cprintf("\n(TM7)implements the test of stack pointer.PASS");
	}
}

  /***********/
  /* CPU TM8 */
  /* implements the test of pipeline functions */
  /***********/

static void STL_CPU_TM8_Test(void)
{
  STL_TmStatus_t StlCpuTm8Status = STL_ERROR;

  if (STL_SCH_RunCpuTM8(&StlCpuTm8Status) != STL_OK)
  {
    /* STL Defense Programming if LED1 is slowly blinking (1 sec. period) */	  
     StlErrorHandler(1000);
  }
  else if (StlCpuTm8Status != STL_PASSED)
  {
    /* STL Tests FAILED if LED1 is quickly blinking (0.2 sec. period) */\
     StlErrorHandler(200);
  }
	else
	{  
		 cprintf( "\n(TM8)implements the test of pipeline functions.PASS");
  }
}


  /***********/
  /* CPU TM9 */
  /* implements the test of the Arm Cortex 4 DSP functions */
  /***********/
static void STL_CPU_TM9_Test(void)
{
  STL_TmStatus_t StlCpuTm9Status = STL_ERROR;

  if (STL_SCH_RunCpuTM9(&StlCpuTm9Status) != STL_OK)
 {
    /* STL Defense Programming if LED1 is slowly blinking (1 sec. period) */	  
    StlErrorHandler(1000);
  }
  else if (StlCpuTm9Status != STL_PASSED)
  {
    /* STL Tests FAILED if LED1 is quickly blinking (0.2 sec. period) */\
    StlErrorHandler(200);
  }
	else
	{
		cprintf("\n(TM9)implements the test of the Arm Cortex 4 DSP functions.PASS");
	}
}

  /***********/
  /* CPU TM10 */
  /* implements the test of the FPU register bank */
  /***********/
static void STL_CPU_TM10_Test(void)
{
  STL_TmStatus_t StlCpuTm10Status = STL_ERROR;

  if (STL_SCH_RunCpuTM10(&StlCpuTm10Status) != STL_OK)
 {
    /* STL Defense Programming if LED1 is slowly blinking (1 sec. period) */	  
    StlErrorHandler(1000);
  }
  else if (StlCpuTm10Status != STL_PASSED)
  {
    /* STL Tests FAILED if LED1 is quickly blinking (0.2 sec. period) */\
    StlErrorHandler(200);
  }
	else
	{
		cprintf("\n(TM10)implements the test of the FPU register bank.PASS");
	}
}

  /***********/
  /* CPU TM11 */
  /* implements the test of the FPU functions */
  /***********/
static void STL_CPU_TM11_Test(void)
{
  STL_TmStatus_t StlCpuTm11Status = STL_ERROR;
	
  if (STL_SCH_RunCpuTM11(&StlCpuTm11Status) != STL_OK)
 {
    /* STL Defense Programming if LED1 is slowly blinking (1 sec. period) */	  
    StlErrorHandler(1000);
  }
  else if(StlCpuTm11Status != STL_PASSED)
  {
    /* STL Tests FAILED if LED1 is quickly blinking (0.2 sec. period) */\
    StlErrorHandler(200);
  }
	else 
	{
    cprintf("\n(TM11)implements the test of the FPU functions.PASS");
	}
}
  
  /************/
  /* Flash TM */
  /************/
// cprintf(flash_test_msg1);
static void STL_FLASH_TM_Test(void)
{
	STL_MemSubset_t FlashSubset; 
  STL_MemConfig_t FlashConfig1; 
  STL_TmStatus_t StlFlashStatus = STL_ERROR;
#ifndef __NOT_USE_ST__	
  /* FLASH Config */
  FlashSubset.StartAddr = TEST_ROM_START_ADDR;
  FlashSubset.EndAddr = (uint32_t)(TEST_ROM_START_ADDR + (TEST_FLASH_SECTION_NB * FLASH_SECTION_SIZE) - 1U);
  FlashSubset.pNext = NULL;
  FlashConfig1.pSubset = &FlashSubset;
  FlashConfig1.NumSectionsAtomic = TEST_FLASH_SECTION_NB; /* 1 shot */
#else
  /* FLASH Config */
  FlashSubset.StartAddr = TEST_ROM_START_ADDR;
  FlashSubset.EndAddr = (uint32_t)(TEST_ROM_START_ADDR + (TEST_FLASH_SECTION_NB * FLASH_SECTION_SIZE) - 1U);
  FlashSubset.pNext = NULL;
  FlashConfig1.pSubset = &FlashSubset;
  FlashConfig1.NumSectionsAtomic = TEST_FLASH_SECTION_NB; /* 1 shot */
#endif
	cprintf(
  "\n======================================================================"
  "\n=              STM32F407VGT FLASH Memory Test                        ="
  "\n======================================================================"
  "\n\n"
	); 
	
	  if (STL_SCH_Init() != STL_OK)
  {
    /* STL Defense Programming if LED1 is slowly blinking (1 sec. period) */
				StlErrorHandler(1000);
				cprintf("\n\n\nSTL initializes the scheduler error.\n");
  }
  else {
				cprintf("\nSTL initializes the scheduler PASS.\n");	  	  
  }
  /* Init Flash TM */
  if (STL_SCH_InitFlash(&StlFlashStatus) != STL_OK)
  {
    /* STL Defense Programming if LED1 is slowly blinking (1 sec. period) */ 
    StlErrorHandler(1000);
  }
  /* check Flash TM status is reset to STL_NOT_TESTED */
  else if (StlFlashStatus != STL_NOT_TESTED)
  {
    /* STL Defense Programming if LED1 is slowly blinking (1 sec. period) */	
    StlErrorHandler(1000);
  }
  
 //  Rs232_Printf(serial3, "\nFLASH Memory Initial.PASS");
  
  /* configure Flash TM */
  if (STL_SCH_ConfigureFlash(&StlFlashStatus, &FlashConfig1) == STL_KO)
  {
    /* STL Defense Programming if LED1 is slowly blinking (1 sec. period) */		    
    StlErrorHandler(1000);
  }
  /* check Flash TM status is reset to STL_NOT_TESTED */
  else if (StlFlashStatus != STL_NOT_TESTED)
  {
    /* STL Defense Programming if LED1 is slowly blinking (1 sec. period) */
    StlErrorHandler(1000);
  }
	else 
	{
		cprintf("\nFLASH Memory Configure. PASS");
	}
  
  /* Run Flash TM */
  if (STL_SCH_RunFlashTM(&StlFlashStatus) != STL_OK)
  {
    /* STL Defense Programming if LED1 is slowly blinking (1 sec. period) */
    StlErrorHandler(1000);
  }
  else
  {
		cprintf("\nFLASH Memory Test. PASS\n\n");	  
  }
}


  /**********/
  /* RAM TM */
  /**********/

static void STL_RAM_TM_Test(void)
{
  STL_TmStatus_t StlRamStatus = STL_ERROR; 
  STL_MemSubset_t RamSubset;
  STL_MemConfig_t RamConfig1;
#ifndef __NOT_USE_ST__	 
  /* RAM Config */
  RamSubset.StartAddr = TEST_RAM_START_ADDR;
  RamSubset.EndAddr = (uint32_t)(TEST_RAM_START_ADDR + TEST_RAM_SECTION_NB * RAM_SECTION_SIZE - 1U);
  RamSubset.pNext = NULL;
  RamConfig1.pSubset = &RamSubset;
  RamConfig1.NumSectionsAtomic = TEST_RAM_SECTION_NB; /* 1 shot */	
#else
  RamSubset.StartAddr = TEST_RAM_START_ADDR;
  RamSubset.EndAddr = (uint32_t)(TEST_RAM_START_ADDR + TEST_RAM_SECTION_NB * RAM_SECTION_SIZE - 1U);
  RamSubset.pNext = NULL;
  RamConfig1.pSubset = &RamSubset;
  RamConfig1.NumSectionsAtomic = TEST_RAM_SECTION_NB; /* 1 shot */		
#endif
 cprintf(
	"\n======================================================================"
  "\n=                     STM32F407VGT RAM Test                          ="
  "\n======================================================================"
  "\n\n"
	);
	 if (STL_SCH_Init() != STL_OK)
   {
    /* STL Defense Programming if LED1 is slowly blinking (1 sec. period) */
			StlErrorHandler(1000);
			cprintf("\n\n\nSTL initializes the scheduler error.\n");
   }
   else
   {
			 cprintf("\nSTL initializes the scheduler PASS.\n");	  	  
   }
	
  /* Init RAM TM */
  if (STL_SCH_InitRam(&StlRamStatus) != STL_OK)
  {
	/* STL Defense Programming if LED1 is slowly blinking (1 sec. period) */
     StlErrorHandler(1000);
  }
  /* check RAM TM status is reset to STL_NOT_TESTED */
  else if (StlRamStatus != STL_NOT_TESTED)
  {
	/* STL Defense Programming if LED1 is slowly blinking (1 sec. period) */
     StlErrorHandler(1000);
  }
	else 
	{
		 cprintf("\nRAM Initial. PASS");	  
	}
  
  /* Configure RAM TM */
  if (STL_SCH_ConfigureRam(&StlRamStatus, &RamConfig1) != STL_OK)
  {
    /* STL Defense Programming if LED1 is slowly blinking (1 sec. period) */
     StlErrorHandler(1000);
  }
  /* check RAM TM status is reset to STL_NOT_TESTED */
  else if (StlRamStatus != STL_NOT_TESTED)
  {
	/* STL Defense Programming if LED1 is slowly blinking (1 sec. period) */
     StlErrorHandler(1000);
  }
//    Rs232_Printf(serial3, "\nRAM Configure. PASS");	  
  
  /* run RAM TM */
  if (STL_SCH_RunRamTM(&StlRamStatus) != STL_OK)
  {
    /* STL Defense Programming if LED1 is slowly blinking (1 sec. period) */
     StlErrorHandler(1000);
  }
  /* check RAM TM status */
  else if (StlRamStatus != STL_PASSED)
  {
    /* STL Tests FAILED if LED1 is quickly blinking (0.2 sec. period) */
     StlErrorHandler(200);
  }
  else 
  {
    /* STL Tests PASSED */
 //   Led_Toggle(run_led);
    cprintf("\nRAM Test. PASS\n\n");		  
  }
}
/** \} Main_Private_Functions */

/** \} STL_Self_Test */ 

/** \} Applications */ 

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/