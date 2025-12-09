/** ****************************************************************************
  * \file    device.h
  * \author  Jang Ho Jong
  * \version V0.0.1
  * \date    2024.10.19
  * \brief   Board Device Definition Header
  * ***************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DEVICES_H
#define __DEVICES_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "led.h"
#include "dip_switch.h"
#include "rs232.h"
#include "relay.h"
#include "digital_output.h"
#include "digital_input.h"
#include "stm32f407xx.h"
#include "periph_stm32f4xx.h"
#include "watchdog.h"
#include "stm32f4xx_hal_i2c.h"
#include "deod_thread.h"
#include "self_diagnose_perform.h"
#include "sysmon_thread.h"
#include "console_thread.h"
#include "stl_user_api.h"
#include "stm32f4xx_ll_system.h"
#include "tpm117.h"


typedef struct
{
char_t	*name;
GPIO_TypeDef       *port1;                                     
GPIO_InitTypeDef    init1;  
GPIO_TypeDef       *port2;                                     
GPIO_InitTypeDef    init2;
Bool_t  Sensor_flag; 	
u08_t   Sensor_Counter;
u32_t   Sensor_Low_Counter; 
u08_t   low_counter_start;	
u16_t	  Sensor_Voltage;
Bool_t  Sensor_OSSD1;
Bool_t  Sensor_OSSD2;
Bool_t  Sensor_fail_flag;
}DEOD_t;

/** \addtogroup Drivers
  * \{ */
  
/** \addtogroup Board_Device
  * \{ */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/** \defgroup Board_Device_Exported_Constants Board 디바이스 공개 상수
  * \{ */

/** \defgroup UART_디바이스_정의 UART 디바이스 정의
  * \{ */
/** \defgroup RS-232_Serial3_Port_Device_Definitions RS-232 Serial3 포트 디바이스 정의
  * \brief RS-232 Serial3 port 관련 pin map과 UART 속성 정의부
  * \{ */
#define SERIAL_TX_Port          (GPIOB)                         /**< TX GPIO Port */
#define SERIAL_TX_Pin           (GPIO_PIN_10)                   /**< TX GPIO Pin */
#define SERIAL_TX_Mode          (GPIO_MODE_AF_PP)               /**< TX GPIO Mode */
#define SERIAL_TX_Pull          (GPIO_PULLUP)                   /**< TX GPIO Pull-Up/Down */
#define SERIAL_TX_Speed         (GPIO_SPEED_FREQ_VERY_HIGH)     /**< TX GPIO Speed */
#define SERIAL_TX_Alternate     (GPIO_AF7_USART3)               /**< TX GPIO Alternate */

#define SERIAL_RX_Port          (GPIOB)                         /**< RX GPIO Port */
#define SERIAL_RX_Pin           (GPIO_PIN_11)                   /**< RX GPIO Pin */
#define SERIAL_RX_Mode          (GPIO_MODE_AF_PP)               /**< RX GPIO Mode */
#define SERIAL_RX_Pull          (GPIO_PULLUP)                   /**< RX GPIO Pull-Up/Down */
#define SERIAL_RX_Speed         (GPIO_SPEED_FREQ_VERY_HIGH)     /**< RX GPIO Speed */
#define SERIAL_RX_Alternate     (GPIO_AF7_USART3)               /**< RX GPIO Alternate */

#define SERIAL_BUF_TX_SIZE      (2048U)                         /**< 송신 버퍼 크기 */
#define SERIAL_BUF_RX_SIZE      (2048U)                         /**< 수신 버퍼 크기 */
#define SERIAL_BUF_PRINTF_SIZE  (512U)                          /**< Printf용 버퍼 크기 */

#define SERIAL_IRQ_Use          (True)                          /**< 인터럽트 사용 유무 */
#define SERIAL_IRQn             (USART3_IRQn)                   /**< 인터럽트 식별 번호 */
#define SERIAL_IRQ_Priority     (4U)                            /**< 인터럽트 우선 순위 */

#define SERIAL_UART             (USART3)                        /**< USART */
#define SERIAL_BaudRate         (115200U)                       /**< Baud-Rate */
#define SERIAL_WordLength       (UART_WORDLENGTH_8B)            /**< Word Length */
#define SERIAL_StopBit          (UART_STOPBITS_1)               /**< Stop Bit */
#define SERIAL_Parity           (UART_PARITY_NONE)              /**< Parity Bit */
#define SERIAL_Mode             (UART_MODE_TX_RX)               /**< 송수신 모드  */
#define SERIAL_HwFlowCtl        (UART_HWCONTROL_NONE)           /**< Hardware Control */
#define SERIAL_OverSampling     (UART_OVERSAMPLING_16)          /**< Over Sampling */
//#define SERIAL_IRQHandler       USART3_IRQHandler

/** \} RS-232_Serial3_Port_Device_Definitions */


/** \defgroup RS485Ch1_Port_정의 RS-485 ATC Repeater 포트 디바이스 정의
  * \brief RS-485 ATC Repeater port 관련 pin map과 UART 속성 정의부
  * \{ */
#define Rs485Ch1_NAME            "Rs485Ch1"

#define Rs485Ch1_TX_Port         (GPIOA)                /**< TX GPIO Port */
#define Rs485Ch1_TX_Pin          (GPIO_PIN_9)           /**< TX GPIO Pin */
#define Rs485Ch1_TX_Alternate    (GPIO_AF7_USART1)      /**< TX GPIO Alternate */

#define Rs485Ch1_RX_Port         (GPIOA)                /**< RX GPIO Port */
#define Rs485Ch1_RX_Pin          (GPIO_PIN_10)           /**< RX GPIO Pin */
#define Rs485Ch1_RX_Alternate    (GPIO_AF7_USART1)      /**< RX GPIO Alternate */

#define Rs485Ch1_TXE_Port        (GPIOA)                /**< TXE GPIO Port */
#define Rs485Ch1_TXE_Pin         (GPIO_PIN_8)           /**< TXE GPIO Pin */
#define Rs485Ch1_TXE_ON          (GPIO_PIN_SET)         /**< ON 했을 때 Pin의 값 */
#define Rs485Ch1_TXE_OFF         (GPIO_PIN_RESET)       /**< OFF 했을 때 Pin의 값*/

#define Rs485Ch1_BUF_TX_SIZE     (512)                 /**< 송신 버퍼 크기 */
#define Rs485Ch1_BUF_RX_SIZE     (512)                 /**< 수신 버퍼 크기 */
#define Rs485Ch1_BUF_PRINTF_SIZE (512)                 /**< Printf용 버퍼 크기 */

#define Rs485Ch1_IRQ_Use         (True)                 /**< 인터럽트 사용 유무 */
#define Rs485Ch1_IRQn            (USART1_IRQn)          /**< 인터럽트 식별 번호 */
#define Rs485Ch1_IRQ_Priority    (7)                    /**< 인터럽트 우선 순위 */

#define Rs485Ch1_UART            (USART1)               /**< USART */
#define Rs485Ch1_BaudRate        (38400)                /**< Baud-Rate */
#define Rs485Ch1_WordLength      (UART_WORDLENGTH_8B)   /**< Word Length */
#define Rs485Ch1_StopBit         (UART_STOPBITS_1)      /**< Stop Bit */
#define Rs485Ch1_Parity          (UART_PARITY_NONE)     /**< Parity Bit */
#define Rs485Ch1_Mode            (UART_MODE_TX_RX)      /**< 송수신 모드  */
#define Rs485Ch1_HwFlowCtl       (UART_HWCONTROL_NONE)  /**< Hardware Control */
#define Rs485Ch1_OverSampling    (UART_OVERSAMPLING_16) /**< Over Sampling */

#define Rs485Ch1_IRQHandler      USART1_IRQHandler
/** \} Rs485Ch1_Port_정의 */
/** \} RS485_디바이스_정의 */


/** \defgroup LED_디바이스_정의 LED 디바이스 정의
  * \{ */
/** \brief LED 디바이스 식별 및 총 개수 */

enum
{
  kLED_RUN,           /**< RUN LED 1번 */
  kLED_HLT,         /**< ON : DM으로 동작, OFF : PC로 동작 LED 2번 */
  kLED_E_EB,         /**< RS485통신 ERROR발생시 ON LED 3번 */
  kLED_OBR,           /**< HLT LED 4번 */
  kLED_DER,           /**< HCR INPUT시 ON/OFF됨 LED 5번 */
  kLED_OBL,          /**<PC에서 ERROR이 발생했다는 표시 LED 6번 */   
  kLED_DEL,           /**< OBR LED 7번 */
  kLED_E_OBR,           /**< DER LED 8번 */
  kLED_E_DER,           /**< OBL LED 9번 */
  kLED_E_OBL,           /**< DEL LED 10번 */
  kLED_E_DEL,            /**< EB LED  11번 */
//  kLED_COMTX,        /**< RS484통신 TX LED */
//  kLED_COMRX,        /**< RS484통신 RX LED */
  kNbrOfLEDs          /**< LED 총 개수 */
};

/** \defgroup RUN_LED  정의
  * \brief RUN LED pin map과 속성
  * \{ */
#define LED_RUN_NAME            "RUN"
#define LED_RUN_Port           (GPIOE)                         /**< GPIO Port */
#define LED_RUN_Pin            (GPIO_PIN_0)                    /**< GPIO Pin */
#define LED_RUN_ON             (GPIO_PIN_RESET)                /**< ON 했을 때 Pin의 값 */
#define LED_RUN_OFF            (GPIO_PIN_SET)                  /**< OFF 했을 때 Pin의 값 */
/** \} RUN_LED */

/** \defgroup DMPC_LED  정의
  * \brief DMPC LED pin map과 속성
  * \{ */
#define LED_DMPC_NAME           "DER"
#define LED_DMPC_Port           (GPIOE)                         /**< GPIO Port */
#define LED_DMPC_Pin            (GPIO_PIN_6)                    /**< GPIO Pin */
#define LED_DMPC_ON             (GPIO_PIN_RESET)                /**< ON 했을 때 Pin의 값 */
#define LED_DMPC_OFF            (GPIO_PIN_SET)                  /**< OFF 했을 때 Pin의 값 */
/** \} DMPC_LED */

/** \defgroup OBR_LED  정의
  * \brief OBR LED pin map과 속성
  * \{ */
#define LED_OBR_NAME           "E_OBR"
#define LED_OBR_Port           (GPIOA)                         /**< GPIO Port */
#define LED_OBR_Pin            (GPIO_PIN_4)                    /**< GPIO Pin */
#define LED_OBR_ON             (GPIO_PIN_RESET)                /**< ON 했을 때 Pin의 값 */
#define LED_OBR_OFF            (GPIO_PIN_SET)                  /**< OFF 했을 때 Pin의 값 */
/** \} OBR_LED */

/** \defgroup DER_LED  정의
  * \brief DER LED pin map과 속성
  * \{ */
#define LED_DER_NAME           "E_DER"
#define LED_DER_Port           (GPIOA)                         /**< GPIO Port */
#define LED_DER_Pin            (GPIO_PIN_6)                    /**< GPIO Pin */
#define LED_DER_ON             (GPIO_PIN_RESET)                /**< ON 했을 때 Pin의 값 */
#define LED_DER_OFF            (GPIO_PIN_SET)                  /**< OFF 했을 때 Pin의 값 */
/** \} OBR_LED */

/** \defgroup OBL_LED  정의
  * \brief OBL LED pin map과 속성
  * \{ */
#define LED_OBL_NAME           "E_OBL"
#define LED_OBL_Port           (GPIOA)                         /**< GPIO Port */
#define LED_OBL_Pin            (GPIO_PIN_5)                    /**< GPIO Pin */
#define LED_OBL_ON             (GPIO_PIN_RESET)                /**< ON 했을 때 Pin의 값 */
#define LED_OBL_OFF            (GPIO_PIN_SET)                  /**< OFF 했을 때 Pin의 값 */
/** \} OBL_LED */

/** \defgroup DEL_LED  정의
  * \brief DEL LED pin map과 속성
  * \{ */
#define LED_DEL_NAME           "E_DEL"
#define LED_DEL_Port           (GPIOA)                         /**< GPIO Port */
#define LED_DEL_Pin            (GPIO_PIN_7)                    /**< GPIO Pin */
#define LED_DEL_ON             (GPIO_PIN_RESET)                /**< ON 했을 때 Pin의 값 */
#define LED_DEL_OFF            (GPIO_PIN_SET)                  /**< OFF 했을 때 Pin의 값 */
/** \} OBL_LED */

/** \defgroup HLT_LED  정의
  * \brief HLT LED pin map과 속성
  * \{ */
#define LED_HLT_NAME           "HLT"
#define LED_HLT_Port           (GPIOE)                         /**< GPIO Port */
#define LED_HLT_Pin            (GPIO_PIN_1)                    /**< GPIO Pin */
#define LED_HLT_ON             (GPIO_PIN_RESET)                /**< ON 했을 때 Pin의 값 */
#define LED_HLT_OFF            (GPIO_PIN_SET)                  /**< OFF 했을 때 Pin의 값 */
/** \} HLT_LED */

/** \defgroup HCR_LED  정의
  * \brief HCR LED pin map과 속성
  * \{ */
#define LED_HCR_NAME           "OBR"
#define LED_HCR_Port           (GPIOE)                         /**< GPIO Port */
#define LED_HCR_Pin            (GPIO_PIN_4)                    /**< GPIO Pin */
#define LED_HCR_ON             (GPIO_PIN_RESET)                /**< ON 했을 때 Pin의 값 */
#define LED_HCR_OFF            (GPIO_PIN_SET)                  /**< OFF 했을 때 Pin의 값 */
/** \} HCR_LED */

/** \defgroup 485_ERR_LED  정의
  * \brief 485_ERR LED pin map과 속성
  * \{ */
#define LED_485_ERR_NAME           "E_EB"
#define LED_485_ERR_Port           (GPIOE)                         /**< GPIO Port */
#define LED_485_ERR_Pin            (GPIO_PIN_2)                    /**< GPIO Pin */
#define LED_485_ERR_ON             (GPIO_PIN_RESET)                /**< ON 했을 때 Pin의 값 */
#define LED_485_ERR_OFF            (GPIO_PIN_SET)                  /**< OFF 했을 때 Pin의 값 */
/** \} 485_ERR_LED */

/** \defgroup PC_ERR_LED  정의
  * \brief PC_ERR LED pin map과 속성
  * \{ */
#define LED_PC_ERR_NAME           "DEL"
#define LED_PC_ERR_Port           (GPIOE)                         /**< GPIO Port */
#define LED_PC_ERR_Pin            (GPIO_PIN_5)                    /**< GPIO Pin */
#define LED_PC_ERR_ON             (GPIO_PIN_RESET)                /**< ON 했을 때 Pin의 값 */
#define LED_PC_ERR_OFF            (GPIO_PIN_SET)                  /**< OFF 했을 때 Pin의 값 */
/** \} PC_ERR_LED */

/** \defgroup EB_LED  정의
  * \brief EB LED pin map과 속성
  * \{ */
#define LED_EB_NAME           "OBL"
#define LED_EB_Port           (GPIOE)                         /**< GPIO Port */
#define LED_EB_Pin            (GPIO_PIN_3)                    /**< GPIO Pin */
#define LED_EB_ON             (GPIO_PIN_RESET)                /**< ON 했을 때 Pin의 값 */
#define LED_EB_OFF            (GPIO_PIN_SET)                  /**< OFF 했을 때 Pin의 값 */
/** \} EB_LED */

/** \defgroup COM_TX_LED 정의
  * \brief COM_TX LED pin map과 속성
  * \{ */
#define LED_COMTX_NAME          "COMTX"
#define LED_COMTX_Port          (GPIOB)                /**< GPIO Port */
#define LED_COMTX_Pin           (GPIO_PIN_0)           /**< GPIO Pin */
#define LED_COMTX_ON            (GPIO_PIN_RESET)       /**< ON 했을 때 Pin의 값 */
#define LED_COMTX_OFF           (GPIO_PIN_SET)         /**< OFF 했을 때 Pin의 값 */
/** \} COM_TX_LED */

/** \defgroup COM_RX_LED  정의
  * \brief COM_RX LED pin map과 속성
  * \{ */
#define LED_COMRX_NAME          "COMRX"
#define LED_COMRX_Port          (GPIOB)                /**< GPIO Port */
#define LED_COMRX_Pin           (GPIO_PIN_1)           /**< GPIO Pin */
#define LED_COMRX_ON            (GPIO_PIN_RESET)       /**< ON 했을 때 Pin의 값 */
#define LED_COMRX_OFF           (GPIO_PIN_SET)         /**< OFF 했을 때 Pin의 값 */
/** \} COM_RX_LED */	

/** \defgroup Watchdog_디바이스_정의 Watchdog 디바이스 정의
  * \{ */
/** \brief Watchdog 디바이스 식별 및 총 개수 */

#define WATCHDOG_NAME            "WDT_DATA"
#define WATCHDOG_Port            (GPIOD)                /**< GPIO Port */
#define WATCHDOG_Pin             (GPIO_PIN_0)           /**< GPIO Pin */
/** \} Watchdog */
/** \} Watchdog_디바이스_정의 */

#define PWR_OK_CPU_NAME            "P_OK_CPU"
#define PWR_OK_CPU_Port            (GPIOD)                /**< GPIO Port */
#define PWR_OK_CPU_Pin             (GPIO_PIN_2)           /**< GPIO Pin */
#define PWR_OK_CPU_ON              (GPIO_PIN_RESET)       /**< ON 했을 때 Pin의 값 */
#define PWR_OK_CPU_OFF             (GPIO_PIN_SET)         /**< OFF 했을 때 Pin의 값 */
/** \} Power Ok CPU */
/** \} Power Ok CPU_디바이스_정의 */

#define FILTER_msec_DipSW         30    /**< Dip Switch 입력에 대한 최대 필터링 msec */


/** \defgroup Relay_Output 디바이스_정의 
  * \{ */
/** \brief DO 디바이스 식별 및 총 개수 */
enum
{
  kRY_EB1,
  kRY_EB2,
  kRY_DMPC,
  kRY_HLT,
  kRY_OBR,
  kRY_DER,
  kRY_OBL,
  kRY_DEL,
  kNbrOfRYOs  
};

/** \defgroup DO_1 출력 1번 정의
 * \brief 출력 1번 pin map과 속성
 * \{ */
#define DO_RY_EB1_NAME          "EB1"
#define DO_RY_EB1_Port          (GPIOE)                /**< GPIO Port */
#define DO_RY_EB1_Pin           (GPIO_PIN_7)           /**< GPIO Pin */
#define DO_RY_EB1_ON            (GPIO_PIN_RESET)       /**< ON 했을 때 Pin의 값 */
#define DO_RY_EB1_OFF           (GPIO_PIN_SET)         /**< OFF 했을 때 Pin의 값 */
/** \} DO_RY_EB1 */

/** \defgroup DO_2 출력 2번 정의
 * \brief 출력 2번 pin map과 속성
 * \{ */
#define DO_RY_EB2_NAME          "EB2"
#define DO_RY_EB2_Port          (GPIOE)                /**< GPIO Port */
#define DO_RY_EB2_Pin           (GPIO_PIN_8)           /**< GPIO Pin */
#define DO_RY_EB2_ON            (GPIO_PIN_RESET)       /**< ON 했을 때 Pin의 값 */
#define DO_RY_EB2_OFF           (GPIO_PIN_SET)         /**< OFF 했을 때 Pin의 값 */
/** \} DO_RY_EB2 */

/** \defgroup DO_3 출력 3번 정의
 * \brief 출력 3번 pin map과 속성
 * \{ */
#define DO_RY_DEPC_NAME          "DMPC"
#define DO_RY_DEPC_Port          (GPIOE)                /**< GPIO Port */
#define DO_RY_DEPC_Pin           (GPIO_PIN_9)          /**< GPIO Pin */
#define DO_RY_DEPC_ON            (GPIO_PIN_RESET)       /**< ON 했을 때 Pin의 값 */
#define DO_RY_DEPC_OFF           (GPIO_PIN_SET)         /**< OFF 했을 때 Pin의 값 */
/** \} DO_RY_SNP */

/** \defgroup DO_4 출력 4번 정의
 * \brief 출력 4번 pin map과 속성
 * \{ */
#define DO_RY_HLT_NAME          "HLT"
#define DO_RY_HLT_Port          (GPIOE)                /**< GPIO Port */
#define DO_RY_HLT_Pin           (GPIO_PIN_10)          /**< GPIO Pin */
#define DO_RY_HLT_ON            (GPIO_PIN_RESET)       /**< ON 했을 때 Pin의 값 */
#define DO_RY_HLT_OFF           (GPIO_PIN_SET)         /**< OFF 했을 때 Pin의 값 */
/** \} DO_RY_HLT */

/** \defgroup DO_5 출력 5번 정의
 * \brief 출력 5번 pin map과 속성
 * \{ */
#define DO_RY_OBR_NAME          "OBR"
#define DO_RY_OBR_Port          (GPIOE)                /**< GPIO Port */
#define DO_RY_OBR_Pin           (GPIO_PIN_11)          /**< GPIO Pin */
#define DO_RY_OBR_ON            (GPIO_PIN_RESET)       /**< ON 했을 때 Pin의 값 */
#define DO_RY_OBR_OFF           (GPIO_PIN_SET)         /**< OFF 했을 때 Pin의 값 */
/** \} DO_RY_DEPC */

/** \defgroup DO_6 출력 6번 정의
 * \brief 출력 6번 pin map과 속성
 * \{ */
#define DO_RY_DER_NAME          "DER"
#define DO_RY_DER_Port          (GPIOE)                /**< GPIO Port */
#define DO_RY_DER_Pin           (GPIO_PIN_12)          /**< GPIO Pin */
#define DO_RY_DER_ON            (GPIO_PIN_RESET)       /**< ON 했을 때 Pin의 값 */
#define DO_RY_DER_OFF           (GPIO_PIN_SET)         /**< OFF 했을 때 Pin의 값 */
/** \} DO_RY_DER */

/** \defgroup DO_7 출력 7번 정의
 * \brief 출력 7번 pin map과 속성
 * \{ */
#define DO_RY_OBL_NAME          "OBL"
#define DO_RY_OBL_Port          (GPIOE)                /**< GPIO Port */
#define DO_RY_OBL_Pin           (GPIO_PIN_13)          /**< GPIO Pin */
#define DO_RY_OBL_ON            (GPIO_PIN_RESET)       /**< ON 했을 때 Pin의 값 */
#define DO_RY_OBL_OFF           (GPIO_PIN_SET)         /**< OFF 했을 때 Pin의 값 */
/** \} DO_RY_OBL */

/** \defgroup DO_8 출력 8번 정의
 * \brief 출력 8번 pin map과 속성
 * \{ */
#define DO_RY_DEL_NAME          "DEL"
#define DO_RY_DEL_Port          (GPIOE)                /**< GPIO Port */
#define DO_RY_DEL_Pin           (GPIO_PIN_14)          /**< GPIO Pin */
#define DO_RY_DEL_ON            (GPIO_PIN_RESET)       /**< ON 했을 때 Pin의 값 */
#define DO_RY_DEL_OFF           (GPIO_PIN_SET)         /**< OFF 했을 때 Pin의 값 */
/** \} DO_RY_DEL */

/** \defgroup Relay_LoopBack_디바이스_정의 DI 디바이스 정의
  * \{ */
/** \brief Relay_LoopBack 디바이스 식별 및 총 개수 */

  
enum
{
  kRYFBI_EB1,
  kRYFBI_EB2,
  kRYFBI_DMPC,
  kRYFBI_HLT,
  kRYFBI_OBR,
  kRYFBI_DER,
  kRYFBI_OBL,
  kRYFBI_DEL,
  kNbrOfRYFBIs
};

/** \defgroup DI_1 입력 1번 정의
 * \brief 입력 1번 pin map과 속성
 * \{ */
#define DI_RY_EB1_FB_NAME          "EB1_FB"
#define DI_RY_EB1_FB_Port          (GPIOD)                /**< GPIO Port */
#define DI_RY_EB1_FB_Pin           (GPIO_PIN_8)           /**< GPIO Pin */
#define DI_RY_EB1_FB_ON            (GPIO_PIN_RESET)       /**< ON 했을 때 Pin의 값 */
#define DI_RY_EB1_FB_OFF           (GPIO_PIN_SET)         /**< OFF 했을 때 Pin의 값 */
/** \} DI_RY_EB1_FB */

/** \defgroup DI_2 입력 2번 정의
 * \brief 입력 2번 pin map과 속성
 * \{ */
#define DI_RY_EB2_FB_NAME          "EB2_FB"
#define DI_RY_EB2_FB_Port          (GPIOD)                /**< GPIO Port */
#define DI_RY_EB2_FB_Pin           (GPIO_PIN_9)           /**< GPIO Pin */
#define DI_RY_EB2_FB_ON            (GPIO_PIN_RESET)       /**< ON 했을 때 Pin의 값 */
#define DI_RY_EB2_FB_OFF           (GPIO_PIN_SET)         /**< OFF 했을 때 Pin의 값 */
/** \} DI_RY_EB2_FB */

/** \defgroup DI_3 입력 3번 정의
 * \brief 입력 3번 pin map과 속성
 * \{ */
#define DI_RY_DEPC_FB_NAME          "DMPC_FB"
#define DI_RY_DEPC_FB_Port          (GPIOD)                /**< GPIO Port */
#define DI_RY_DEPC_FB_Pin           (GPIO_PIN_10)          /**< GPIO Pin */
#define DI_RY_DEPC_FB_ON            (GPIO_PIN_RESET)       /**< ON 했을 때 Pin의 값 */
#define DI_RY_DEPC_FB_OFF           (GPIO_PIN_SET)         /**< OFF 했을 때 Pin의 값 */
/** \} DI_RY_DEPC_FB */

/** \defgroup DI_4 입력 4번 정의
 * \brief 입력 4번 pin map과 속성
 * \{ */
#define DI_RY_HLT_FB_NAME          "HLT_FB"
#define DI_RY_HLT_FB_Port          (GPIOD)                /**< GPIO Port */
#define DI_RY_HLT_FB_Pin           (GPIO_PIN_11)          /**< GPIO Pin */
#define DI_RY_HLT_FB_ON            (GPIO_PIN_RESET)       /**< ON 했을 때 Pin의 값 */
#define DI_RY_HLT_FB_OFF           (GPIO_PIN_SET)         /**< OFF 했을 때 Pin의 값 */
/** \} DI_RY_HLT_FB */


/** \defgroup DI_5 입력 5번 정의
 * \brief 입력 5번 pin map과 속성
 * \{ */
#define DI_RY_OBR_FB_NAME          "OBR_FB"
#define DI_RY_OBR_FB_Port          (GPIOD)                /**< GPIO Port */
#define DI_RY_OBR_FB_Pin           (GPIO_PIN_12)          /**< GPIO Pin */
#define DI_RY_OBR_FB_ON            (GPIO_PIN_RESET)       /**< ON 했을 때 Pin의 값 */
#define DI_RY_OBR_FB_OFF           (GPIO_PIN_SET)         /**< OFF 했을 때 Pin의 값 */
/** \} DI_RY_OBR_FB */

/** \defgroup DI_6 입력 6번 정의
 * \brief 입력 6번 pin map과 속성
 * \{ */
#define DI_RY_DER_FB_NAME          "DER_FB"
#define DI_RY_DER_FB_Port          (GPIOD)                /**< GPIO Port */
#define DI_RY_DER_FB_Pin           (GPIO_PIN_13)          /**< GPIO Pin */
#define DI_RY_DER_FB_ON            (GPIO_PIN_RESET)       /**< ON 했을 때 Pin의 값 */
#define DI_RY_DER_FB_OFF           (GPIO_PIN_SET)         /**< OFF 했을 때 Pin의 값 */
/** \} DI_RY_DER_FB */

/** \defgroup DI_7 입력 7번 정의
 * \brief 입력 7번 pin map과 속성
 * \{ */
#define DI_RY_OBL_FB_NAME          "OBL_FB"
#define DI_RY_OBL_FB_Port          (GPIOD)                /**< GPIO Port */
#define DI_RY_OBL_FB_Pin           (GPIO_PIN_14)          /**< GPIO Pin */
#define DI_RY_OBL_FB_ON            (GPIO_PIN_RESET)       /**< ON 했을 때 Pin의 값 */
#define DI_RY_OBL_FB_OFF           (GPIO_PIN_SET)         /**< OFF 했을 때 Pin의 값 */
/** \} DI_RY_OBL_FB */

/** \defgroup DI_8 입력 8번 정의
 * \brief 입력 8번 pin map과 속성
 * \{ */
#define DI_RY_DEL_FB_NAME          "DEL_FB"
#define DI_RY_DEL_FB_Port          (GPIOD)                /**< GPIO Port */
#define DI_RY_DEL_FB_Pin           (GPIO_PIN_15)          /**< GPIO Pin */
#define DI_RY_DEL_FB_ON            (GPIO_PIN_RESET)       /**< ON 했을 때 Pin의 값 */
#define DI_RY_DEL_FB_OFF           (GPIO_PIN_SET)         /**< OFF 했을 때 Pin의 값 */
/** \} DI_8 */
/** \} DI_디바이스_정의 */

/** \defgroup Sensor Input_디바이스_정의 SI 디바이스 정의
  * \{ */
/** \brief SI 디바이스 식별 및 총 개수 */
enum
{
  kSI_1,
  kSI_2,
  kSI_3,
  kSI_4,
  kNbrOfSIs
};
//#define   kNbrOfSI 8U
#define SI1_OSSD_NAME          "SEN1"
#define SI1_OSSD1_Port          (GPIOC)                /**< GPIO Port */
#define SI1_OSSD1_Pin           (GPIO_PIN_0)           /**< GPIO Pin */
/** \} SI1_OSSD1 */
/** \defgroup 센서1 입력 2번1번(OSSD2)
 * \brief 입력 2번 pin map 
 * \{ */
#define SI1_OSSD2_Port          (GPIOC)                /**< GPIO Port */
#define SI1_OSSD2_Pin           (GPIO_PIN_1)           /**< GPIO Pin */
/** \} SI1_OSSD2 */

/** \defgroup 센서2 입력 1번(OSSD1)
 * \brief 입력 1번 pin map 
 * \{ */
#define SI2_OSSD_NAME          "SEN2"
#define SI2_OSSD1_Port          (GPIOC)                /**< GPIO Port */
#define SI2_OSSD1_Pin           (GPIO_PIN_2)          /**< GPIO Pin */
/** \} SI2_OSSD1 */
/** \defgroup 센서2 입력 2번(OSSD2)
 * \brief 입력 2번 pin map 
 * \{ */
#define SI2_OSSD2_Port          (GPIOC)                /**< GPIO Port */
#define SI2_OSSD2_Pin           (GPIO_PIN_3)          /**< GPIO Pin */
/** \} SI2_OSSD2 */

/** \defgroup 센서3 입력 1번(OSSD1)
 * \brief 입력 1번 pin map 
 * \{ */
#define SI3_OSSD_NAME          "SEN3"
#define SI3_OSSD1_Port          (GPIOC)                /**< GPIO Port */
#define SI3_OSSD1_Pin           (GPIO_PIN_4)           /**< GPIO Pin */
/** \} SI3_OSSD1 */

/** \defgroup 센서3 입력 2번(OSSD2)
 * \brief 입력 2번 pin map
 * \{ */
#define SI3_OSSD2_Port          (GPIOC)                /**< GPIO Port */
#define SI3_OSSD2_Pin           (GPIO_PIN_5)           /**< GPIO Pin */
/** \} SI3_OSSD2 */

/** \defgroup 센서4 입력 1번(OSSD1)
 * \brief 입력 1번 pin map
 * \{ */
#define SI4_OSSD_NAME          "SEN4"
#define SI4_OSSD1_Port          (GPIOC)                /**< GPIO Port */
#define SI4_OSSD1_Pin           (GPIO_PIN_6)           /**< GPIO Pin */
/** \} SI4_OSSD1 */
/** \defgroup  센서4 입력 2번(OSSD1)
 * \brief 입력 2번 pin map
 * \{ */
#define SI4_OSSD2_Port          (GPIOC)                /**< GPIO Port */
#define SI4_OSSD2_Pin           (GPIO_PIN_7)           /**< GPIO Pin */
/** \} SI4_OSSD2 */


/** \defgroup Digital Output Isolator State Inptu_정의 
  * \{ */
/** \brief DOIS 디바이스 식별 및 총 개수 */
enum
{
  kDISF_1,
  kDISF_2,
  kNbrOfDISFs
};

/** \defgroup Digital Output Isolator State Input입력 1번 정의
 * \brief 입력 1번 pin map과 속성
 * \{ */
#define DOIS_1_NAME          "FLT1"
#define DOIS_1_Port          (GPIOA)                /**< GPIO Port */
#define DOIS_1_Pin           (GPIO_PIN_11)           /**< GPIO Pin */
#define DOIS_1_ON            (GPIO_PIN_RESET)       /**< ON 했을 때 Pin의 값 */
#define DOIS_1_OFF           (GPIO_PIN_SET)         /**< OFF 했을 때 Pin의 값 */
/** \} DOIS_1 */

/** \defgroup DOIS_2 입력 2번 정의
 * \brief 입력 2번 pin map과 속성
 * \{ */
#define DOIS_2_NAME          "FLT2"
#define DOIS_2_Port          (GPIOA)                /**< GPIO Port */
#define DOIS_2_Pin           (GPIO_PIN_12)           /**< GPIO Pin */
#define DOIS_2_ON            (GPIO_PIN_RESET)       /**< ON 했을 때 Pin의 값 */
#define DOIS_2_OFF           (GPIO_PIN_SET)         /**< OFF 했을 때 Pin의 값 */
/** \} DOIS_1 */

/** \defgroup Watchdog_check_pin 입력  정의
 * \brief 입력  pin map과 속성
 * \{ */
#define WDT_OK_NAME          "WDT_OK"
#define WDT_OK_Port          (GPIOD)                /**< GPIO Port */
#define WDT_OK_Pin           (GPIO_PIN_1)           /**< GPIO Pin */
#define WDT_OK_ON            (GPIO_PIN_RESET)       /**< ON 했을 때 Pin의 값 */
#define WDT_OK_OFF           (GPIO_PIN_SET)         /**< OFF 했을 때 Pin의 값 */
/** \} DOIS_1 */



/** \defgroup MCU_TEST#_ON OUTPUT_디바이스_정의  
  * \{ */
/** \brief MCU_TEST#_ON OUTPUT 디바이스 식별 및 총 개수 */
enum
{
  TEST_1,
  TEST_2,
  TEST_3,
  TEST_4,  
  kNbrOfTest
};
	 
/** \defgroup Sensor Test Output 입력 1번 정의
 * \brief 입력 1번 pin map과 속성
 * \{ */
#define MCU_TEST1_NAME          "TEST_1"
#define MCU_TEST1_Port          (GPIOC)                /**< GPIO Port */
#define MCU_TEST1_Pin           (GPIO_PIN_8)           /**< GPIO Pin */
#define MCU_TEST1_ON            (GPIO_PIN_RESET)       /**< ON 했을 때 Pin의 값 */
#define MCU_TEST1_OFF           (GPIO_PIN_SET)         /**< OFF 했을 때 Pin의 값 */
/** \} MCU_TEST1 */

/** \defgroup Sensor Test Input 입력 2번 정의
 * \brief 입력 2번 pin map과 속성
 * \{ */
#define MCU_TEST2_NAME          "TEST_2"
#define MCU_TEST2_Port          (GPIOC)                /**< GPIO Port */
#define MCU_TEST2_Pin           (GPIO_PIN_9)           /**< GPIO Pin */
#define MCU_TEST2_ON            (GPIO_PIN_RESET)       /**< ON 했을 때 Pin의 값 */
#define MCU_TEST2_OFF           (GPIO_PIN_SET)         /**< OFF 했을 때 Pin의 값 */
/** \} MCU_TEST1 */

/** \defgroup Sensor Test Input 입력 3번 정의
 * \brief 입력 3번 pin map과 속성
 * \{ */
#define MCU_TEST3_NAME          "TEST_3"
#define MCU_TEST3_Port          (GPIOC)                /**< GPIO Port */
#define MCU_TEST3_Pin           (GPIO_PIN_10)          /**< GPIO Pin */
#define MCU_TEST3_ON            (GPIO_PIN_RESET)       /**< ON 했을 때 Pin의 값 */
#define MCU_TEST3_OFF           (GPIO_PIN_SET)         /**< OFF 했을 때 Pin의 값 */
/** \} MCU_TEST3 */

/** \defgroup Sensor Test Input 입력 4번 정의
 * \brief 입력 4번 pin map과 속성
 * \{ */
#define MCU_TEST4_NAME          "TEST_4"
#define MCU_TEST4_Port          (GPIOC)                /**< GPIO Port */
#define MCU_TEST4_Pin           (GPIO_PIN_11)          /**< GPIO Pin */
#define MCU_TEST4_ON            (GPIO_PIN_RESET)       /**< ON 했을 때 Pin의 값 */
#define MCU_TEST4_OFF           (GPIO_PIN_SET)         /**< OFF 했을 때 Pin의 값 */
/** \} MCU_TEST4 */
/** \} MCU_TEST#_ON OUTPUT_디바이스_정의 */

/** \defgroup Sensor Power Control  정의
 * \brief  
 * \{ */
#define SNP_POWER_NAME          "SEN_POWER"
#define SNP_POWER_Port          (GPIOE)                /**< GPIO Port */
#define SNP_POWER_Pin           (GPIO_PIN_15)          /**< GPIO Pin */
#define SNP_POWER_ON            (GPIO_PIN_RESET)       /**< ON 했을 때 Pin의 값 */
#define SNP_POWER_OFF           (GPIO_PIN_SET)         /**< OFF 했을 때 Pin의 값 */
/** \} SNP_POWER */
/** \} SNP_POWER OUTPUT_디바이스_정의 */


/** \defgroup HCR Input_디바이스_정의  
 * \brief 입력 1번 pin map과 속성
 * \{ */
#define HCR_INPUT_NAME          "HCR"
#define HCR_INPUT_Port          (GPIOB)                /**< GPIO Port */
#define HCR_INPUT_Pin           (GPIO_PIN_5)           /**< GPIO Pin */
#define HCR_INPUT_ON            (GPIO_PIN_RESET)       /**< ON 했을 때 Pin의 값 */
#define HCR_INPUT_OFF           (GPIO_PIN_SET)         /**< OFF 했을 때 Pin의 값 */
/** \} HCR INPUT */

/** \defgroup Sensor Chanel 구분 Input_디바이스_정의  
 * \brief 입력 1번 pin map과 속성
 * \{ */
#define SEN_CH_INPUT_NAME          "CHANEL_NUM"
#define SEN_CH_INPUT_Port          (GPIOB)                /**< GPIO Port */
#define SEN_CH_INPUT_Pin           (GPIO_PIN_15)           /**< GPIO Pin */
#define SEN_CH_INPUT_ON            (GPIO_PIN_RESET)       /**< ON 했을 때 Pin의 값 */
#define SEN_CH_INPUT_OFF           (GPIO_PIN_SET)         /**< OFF 했을 때 Pin의 값 */
/** \} SEN_CH INPUT */
enum
{
  kSO_1,
  kSO_2,
  kSO_3,
  kSO_4, 
  kSO_5, 
  kSO_6, 
  kSO_7, 
  kSO_8, 
  kNbrOfSOs
};

/** \defgroup DipSW_디바이스_정의 Dip-Switch 디바이스 정의
 * \{ ************************************************************************/

/** \brief DipSW 디바이스 식별 및 총 개수 */
/*---------------------------------------------------------------------------*/
enum DipSWDevice {
  kCFG_DipSW,     /**< 시스템 설정용 Dip-Switch */
  kNbrOfDipSWs    /**< Dip-Switch 총 개수 */
};

/** \defgroup CFG_DipSW 시스템 설정용 Dip-Switch 정의
 * \brief 시스템 설정용 Dip-Switch의 pin map과 속성
 * \{ ************************************************************************/
#define CFG_DIPSW_NAME          "CFG_DIPSW"
#define FILTER_msec_DipSW       30    /**< Dip Switch 입력에 대한 최대 필터링 msec */

#define CFG_DIPSW_BITS          (3)
#define CFG_DIPSW_BIT0_ON       (GPIO_PIN_RESET)  /**< ON  상태일 때 Pin의 값 */
#define CFG_DIPSW_BIT0_OFF      (GPIO_PIN_SET)    /**< OFF 상태일 때 Pin의 값 */
#define CFG_DIPSW_BIT1_ON       (GPIO_PIN_RESET)  /**< ON  상태일 때 Pin의 값 */
#define CFG_DIPSW_BIT1_OFF      (GPIO_PIN_SET)    /**< OFF 상태일 때 Pin의 값 */
#define CFG_DIPSW_BIT2_ON       (GPIO_PIN_RESET)  /**< ON  상태일 때 Pin의 값 */
#define CFG_DIPSW_BIT2_OFF      (GPIO_PIN_SET)    /**< OFF 상태일 때 Pin의 값 */
#define CFG_DIPSW_BIT3_ON       (GPIO_PIN_RESET)  /**< ON  상태일 때 Pin의 값 */
#define CFG_DIPSW_BIT3_OFF      (GPIO_PIN_SET)    /**< OFF 상태일 때 Pin의 값 */

#define CFG_SW1_Pin             (GPIO_PIN_12)
#define CFG_SW1_GPIO_Port       (GPIOB)
#define CFG_SW2_Pin             (GPIO_PIN_13)
#define CFG_SW2_GPIO_Port       (GPIOB)
#define CFG_SW3_Pin             (GPIO_PIN_14)
#define CFG_SW3_GPIO_Port       (GPIOB)

/* CFG_DipSW */
/** \} ***********************************************************************/


/* DipSW_디바이스_정의 */
/** \} ***********************************************************************/


/** \} Board_Device_Exported_Constants */

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/** \defgroup Board_Device_Exported_Variables Board 디바이스 공개 변수
  * \{ */
extern Rs232_t * serial3;            /**< RS-232 serial3 디바이스 객체 */

extern Led_t  * run_led;                 /* RUN LED 디바이스 객체 */
extern Led_t  * der_led;                /* DMPC LED 디바이스 객체 */
extern Led_t  * del_led;              /* PC_ERR LED 디바이스 객체 */
extern Led_t  * e_obr_led;                 /* OBR LED 디바이스 객체 */
extern Led_t  * e_der_led;                 /* DER LED 디바이스 객체 */
extern Led_t  * e_obl_led;                 /* OBL LED 디바이스 객체 */
extern Led_t  * e_del_led;                 /* DEL LED 디바이스 객체 */
extern Led_t  * hlt_led;                 /* HLT LED 디바이스 객체 */
extern Led_t  * obr_led;                 /* HCR LED 디바이스 객체 */
extern Led_t  * e_eb_led;             /* COM_ERR LED 디바이스 객체 */
extern Led_t  * obl_led;                  /* EB LED 디바이스 객체 */

extern Watchdog_t * wdog;
extern Relay_t      * eb1_out;           /* Relay Output 디바이스 객체 */
extern Relay_t      * eb2_out;           /* Relay Output 디바이스 객체 */
extern Relay_t      * dmpc_out;           /* Relay Output 디바이스 객체 */
extern Relay_t      * hlt_out;           /* Relay Output 디바이스 객체 */
extern Relay_t      * obr_out;           /* Relay Output 디바이스 객체 */
extern Relay_t      * der_out;           /* Relay Output 디바이스 객체 */
extern Relay_t      * obl_out;           /* Relay Output 디바이스 객체 */
extern Relay_t      * del_out;           /* Relay Output 디바이스 객체 */

extern DO_t       * mcu_test1;
extern DO_t       * mcu_test2;
extern DO_t       * mcu_test3;
extern DO_t       * mcu_test4;
extern DO_t       * snp_out;

extern DI_t       * din[kNbrOfRYFBIs];      /**< Relay Loop Back Input 디바이스 객체 */
extern DEOD_t     * sdin[kNbrOfSIs];          /**< Sensor Input 디바이스 객체 */
extern DI_t       * fpin_1;
extern DI_t       * fpin_2;
extern DI_t	      * pwrin; 
extern DI_t       * wdt_ok;
extern DI_t       * hcr_input;
//extern DI_t       *pc_ch_num;
extern DipSW_t    * cfg; 
extern I2C_HandleTypeDef    hi2c1;
extern ADC_HandleTypeDef    hadc1;
//extern DEOD_t * sensort[kNbrOfMO];
/** \} Board_Device_Exported_Constants */

/** \} Device_Exported_Variables */

/* Exported functions --------------------------------------------------------*/
/** \defgroup Device_Exported_Functions 디바이스 공개 함수
  * \{ */
void Device_Init(void);
//extern PulseCount_t  * pulsecnt;            /**< 타코미터 입력 디바이스 객체 */
void Periph_Control_GPIO_CLK(GPIO_TypeDef * port, Bool_t ctrl);
void Periph_Control_UART_CLK(USART_TypeDef * uart, Bool_t ctrl);
void Periph_Control_TIM_CLK(TIM_TypeDef * tim, Bool_t ctrl);
#ifdef RTE_DEVICE_HAL_UART
void cprintf(const char_t format[], ...);
void cprint(u08_t buf[], u32_t size);
#endif /* RTE_DEVICE_HAL_UART */

#if (defined(RTE_DEVICE_HAL_NAND) || defined(RTE_DEVICE_HAL_NOR) || defined(RTE_DEVICE_HAL_SDRAM) || defined(RTE_DEVICE_HAL_SRAM))
void Periph_DataBus_Init(u32_t width);
void Periph_AddrBus_Init(u32_t length);
#endif /* #if (defined(RTE_DEVICE_HAL_NAND) || defined(RTE_DEVICE_HAL_NOR) || defined(RTE_DEVICE_HAL_SDRAM) || defined(RTE_DEVICE_HAL_SRAM)) */
#ifdef RTE_DEVICE_HAL_RTC
void tprintf(const char_t format[], ...);
#endif /* RTE_DEVICE_HAL_RTC */

/** \} Device_Exported_Functions */

/** \} Device */

/** \} Applications */

#endif /* __DEVICES_H */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
