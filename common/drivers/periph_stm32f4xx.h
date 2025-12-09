/** ****************************************************************************
  * \file    periph_stm32f4xx.h
  * \author  Park Keun Tae
  * \version V0.0.1
  * \date    2017.12.03
  * \brief   STM32F4xx Peripheral Control Driver Header
  * ***************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PERIPH_STM32F4xx_H
#define __PERIPH_STM32F4xx_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** \addtogroup Drivers
  * \{ */

/** \addtogroup PeriphCtrl
  * \{ */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/** \defgroup PeriphCtrl_Exported_Constants 페리페럴제어 공개 상수
  * \{ */

/** \defgroup FSMC_BUS_정의 FSMC BUS 정의
  * \{ */
#define FSMC_CLK_Port           (GPIOD)
#define FSMC_CLK                (GPIO_PIN_3)            /**< PD3 */
#define FSMC_NOE_NWE_Port       (GPIOD)
#define FSMC_NOE                (GPIO_PIN_4)            /**< PD4 */
#define FSMC_NWE                (GPIO_PIN_5)            /**< PD5 */
#define FSMC_NWAIT_Port         (GPIOD)
#define FSMC_NWAIT              (GPIO_PIN_6)            /**< PD6 */
#define FSMC_NADV_Port          (GPIOB)
#define FSMC_NADV               (GPIO_PIN_7)            /**< PB7 */
#define FSMC_NBL_Port           (GPIOE)
#define FSMC_NBL0               (GPIO_PIN_0)            /**< PE0 */
#define FSMC_NBL1               (GPIO_PIN_1)            /**< PE1 */
#define FSMC_CLE_ALE_Port       (GPIOD)
#define FSMC_CLE                (GPIO_PIN_11)           /**< PD11 */
#define FSMC_ALE                (GPIO_PIN_12)           /**< PD12 */
#define FSMC_INT_Port           (GPIOG)
#define FSMC_INT2               (GPIO_PIN_6)            /**< PG6 */
#define FSMC_INT3               (GPIO_PIN_7)            /**< PG7 */

#define FSMC_NE1_Port           (GPIOD)
#define FSMC_NE1                (GPIO_PIN_7)            /**< PD7 */
#define FSMC_NE2_Port           (GPIOG)
#define FSMC_NE2                (GPIO_PIN_9)            /**< PG9 */
#define FSMC_NE3_Port           (GPIOG)
#define FSMC_NE3                (GPIO_PIN_10)           /**< PG10 */
#define FSMC_NE4_Port           (GPIOG)
#define FSMC_NE4                (GPIO_PIN_12)           /**< PG12 */

#define FSMC_NCE2_Port          (GPIOD)
#define FSMC_NCE2               (GPIO_PIN_7)            /**< PD7 */
#define FSMC_NCE3_Port          (GPIOG)
#define FSMC_NCE3               (GPIO_PIN_9)            /**< PG9 */
#define FSMC_NCE4_Port          (GPIOG)
#define FSMC_NCE4_1             (GPIO_PIN_10)           /**< PG10 */
#define FSMC_NCE4_2             (GPIO_PIN_11)           /**< PG11 */

#if defined(RTE_DEVICE_HAL_SDRAM)
#define FMC_SDCKE0_Port         (GPIOC)
#define FMC_SDCKE0              (GPIO_PIN_3)            /**< PC3 */
#define FMC_SDNE0_Port          (GPIOC)
#define FMC_SDNE0               (GPIO_PIN_2)            /**< PC2 */
#define FMC_SDCKE1_Port         (GPIOB)
#define FMC_SDCKE1              (GPIO_PIN_5)            /**< PB5 */
#define FMC_SDNE1_Port          (GPIOB)
#define FMC_SDNE1               (GPIO_PIN_6)            /**< PB6 */
#define FMC_BA0_Port            (GPIOG)
#define FMC_BA0                 (GPIO_PIN_4)            /**< PG4 */
#define FMC_BA1_Port            (GPIOG)
#define FMC_BA1                 (GPIO_PIN_5)            /**< PG5 */
#define FMC_SDNWE_Port          (GPIOC)
#define FMC_SDNWE               (GPIO_PIN_0)            /**< PC0 */
#define FMC_SDCLK_Port          (GPIOG)
#define FMC_SDCLK               (GPIO_PIN_8)            /**< PG8 */
#define FMC_NRAS_Port           (GPIOF)
#define FMC_NRAS                (GPIO_PIN_11)           /**< PF11 */
#define FMC_NCAS_Port           (GPIOG)
#define FMC_NCAS                (GPIO_PIN_15)           /**< PG15 */
#endif 
/** \} FSMC_BUS_정의 */

/** \} PeriphCtrl_Exported_Constants */

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** \defgroup PeriphCtrl_Exported_Functions 페리페럴제어 공개 함수
  * \{ */
void Periph_Control_GPIO_CLK(GPIO_TypeDef * port, Bool_t ctrl);
void Periph_Control_UART_CLK(USART_TypeDef * uart, Bool_t ctrl);
void Periph_Control_TIM_CLK(TIM_TypeDef * tim, Bool_t ctrl);
void Periph_Control_CAN_CLK(CAN_TypeDef * can, Bool_t ctrl);
void Periph_Control_I2C_CLK(I2C_TypeDef * i2c, Bool_t ctrl);
void Periph_Control_ADC_CLK(ADC_TypeDef * adc, Bool_t ctrl);
void Periph_Control_DMA_CLK(DMA_TypeDef * dma, Bool_t ctrl);
void Periph_Control_DAC_CLK(DAC_TypeDef * dac, Bool_t ctrl);
void Periph_Control_SPI_CLK(SPI_TypeDef * spi, Bool_t ctrl);

#ifdef RTE_DEVICE_HAL_UART
void cprintf(const char_t format[], ...);
void tprintf(const char_t format[], ...);
void cprint(u08_t buf[], u32_t size);
#ifdef ARM_MATH_CM4
void   cprint_float32(const char_t form[], float32_t value, u32_t digit_f);
#endif /* ARM_MATH_CM4 */
#endif /* RTE_DEVICE_HAL_UART */

#if (defined(RTE_DEVICE_HAL_NAND) || defined(RTE_DEVICE_HAL_NOR) || defined(RTE_DEVICE_HAL_SDRAM) || defined(RTE_DEVICE_HAL_SRAM))
void Periph_DataBus_Init(u32_t width);
void Periph_AddrBus_Init(u32_t length);
#endif /* #if (defined(RTE_DEVICE_HAL_NAND) || defined(RTE_DEVICE_HAL_NOR) || defined(RTE_DEVICE_HAL_SDRAM) || defined(RTE_DEVICE_HAL_SRAM)) */

/** \} PeriphCtrl_Exported_Functions */

/** \} PeriphCtrl */

/** \} Drivers */

#endif /* __PERIPH_STM32F4xx_H */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
