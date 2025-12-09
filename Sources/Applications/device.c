/** ****************************************************************************
  * \file    device.c
  * \author  Jang Ho Jong
  * \version V1.0.0
  * \date    2024.10.10
  * \brief   Board Device Definition Body
  * ***************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "device.h"

/** \addtogroup Applications
  * \{ */
ADC_HandleTypeDef    hadc1;
I2C_HandleTypeDef    hi2c1;
/** \defgroup Device 하드웨어 장치
  * \brief 시스템의 하드웨어 장치를 구성하고 디바이스 객체를 제공합니다.
  * \{ */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/** \addtogroup Device_Exported_Variables
  * \{ */  
Rs232_t  * serial3;               /* RS-232 serial 디바이스 객체 */

Led_t  * run_led;                 /* RUN LED 디바이스 객체 */
Led_t  * der_led;                /* DMPC LED 디바이스 객체 */
Led_t  * del_led;              /* PC_ERR LED 디바이스 객체 */
Led_t  * e_obr_led;                 /* OBR LED 디바이스 객체 */
Led_t  * e_der_led;                 /* DER LED 디바이스 객체 */
Led_t  * e_obl_led;                 /* OBL LED 디바이스 객체 */
Led_t  * e_del_led;                 /* DEL LED 디바이스 객체 */
Led_t  * hlt_led;                 /* HLT LED 디바이스 객체 */
Led_t  * obr_led;                 /* HCR LED 디바이스 객체 */
Led_t  * e_eb_led;             /* COM_ERR LED 디바이스 객체 */
Led_t  * obl_led;                  /* EB LED 디바이스 객체 */
Led_t  * comtx_led;               /* COMTX LED 디바이스 객체 */
Led_t  * comrx_led;               /* COMRX LED 디바이스 객체 */

Relay_t  * eb1_out;               /* EB1 Relay Output 디바이스 객체 */
Relay_t  * eb2_out;               /* EB2 Relay Output 디바이스 객체 */
Relay_t  * hlt_out;               /* HLT Relay Output 디바이스 객체 */
Relay_t  * dmpc_out;              /* DEPC Relay Output 디바이스 객체 */
Relay_t  * obr_out;               /* OBR Relay Output 디바이스 객체 */
Relay_t  * der_out;               /* DER Relay Output 디바이스 객체 */
Relay_t  * obl_out;               /* OBL Relay Output 디바이스 객체 */
Relay_t  * del_out;               /* DEL Relay Output 디바이스 객체 */

DO_t     * mcu_test1;             /* Sensor Test1 Output 디바이스 객체 */
DO_t     * mcu_test2;             /* Sensor Test2 Output 디바이스 객체 */
DO_t     * mcu_test3;             /* Sensor Test3 Output 디바이스 객체 */
DO_t     * mcu_test4;             /* Sensor Test4 Output 디바이스 객체 */
DO_t     * snp_out;               /* Sensor Power Output Control 디바이스 객체 */

DI_t     * din[kNbrOfRYFBIs];     /* DIGITAL INPUT 디바이스 객체 */
DEOD_t   * sdin[kNbrOfSIs];       /* Sensor Input(OSSD1/OSSD2) 디바이스 객체 */
DI_t     * fpin_1;                /* Digital Output Isolator State Input 디바이스 객체 */
DI_t     * fpin_2;                /* Digital Output Isolator State Input 디바이스 객체 */
DI_t     * pwrin;                 /* POWER INPUT State 디바이스 객체 */
DI_t     * pc_ch_num;             /* PC Board Sensor 채널 수(2CH/4CH) 선택 스위치 입력 디바이스 객체 */
DI_t     * hcr_input;             /* HCR(선두부/후두부) 선택 스위치 입력 디바이스 객체 */
DI_t     * wdt_ok;                /* CU or PC Board 선택 스위치 입력 디바이스 객체 */
DipSW_t  * cfg;                   /* PC BOARD 연결 갯수 선택 스위치 입력 디바이스 객체 */
Watchdog_t *wdog;                 /* WATCHDOG SIGNAL 입력 디바이스 객체 */

/** \} Device_Exported_Variables */

/* Private function prototypes -----------------------------------------------*/
/** \defgroup Device_Private_Functions Board 디바이스 비공개 함수
  * \{ */

static void Device_Init_LED(void);
static void Device_Init_Console(void);
static void Device_Init_Watchdog(void);
static void Device_Init_Relay_Output(void);
static void Device_Init_Relay_Feed_Back_Input(void);
static void Device_Init_Sensor_Input(void);
static void Device_Init_Sensor_Test_Out(void);
static void Device_Init_DOIS(void);
static void Device_Init_I2C(void);
static void Pwr_OK_CPU_Init(void);	
static void Device_Init_ADC(void);

/** \} Device_Private_Functions */

/* Exported functions --------------------------------------------------------*/
/** \addtogroup Device_Exported_Functions
  * \{ */

/** \brief  디바이스의 초기화를 수행한다.
  * \note   각 디바이스를 초기화한다.
  * \retval 없음
  */
void Device_Init(void)
{

  /* LED 디바이스 초기화 */
  Device_Init_LED();
  /* RS232 디바이스 초기화 */
  Device_Init_Console();
  
  /*Relay Output Port 디바이스 초기화 */
	//Relay 제어 출력 Port에 대한 초기화
  Device_Init_Relay_Output();

	/* Sensor Test Output 디바이스 초기화 */
	// GG712 센서 Test를 위한 제어 신호 PORT 초기화
  Device_Init_Sensor_Test_Out();

  /* 디지털 INPUT 디바이스 초기화 */
	//  Relay Feed Back Port에 대한 초기화
  Device_Init_Relay_Feed_Back_Input();	

  /* Sensor INPUT 디바이스 초기화 */
	// GG712 SIL2 센서에 대한 디바이스 초기화 
  Device_Init_Sensor_Input();
  
  /* Digital Output Isolator State 디바이스 초기화 */
	// Si83404AAA Device 초기화
  Device_Init_DOIS();

  /* Watchdog 디바이스 초기화 */	
  Device_Init_Watchdog();
	
	/* Power Ok CPU 디바이스 초기화 */	
	Pwr_OK_CPU_Init();
  /* I2C 디바이스 초기화 */
	//TMP117 CHIP에 대한 디바이스 초기화
	//I2C통신방식 사용
  Device_Init_I2C();
/* ADC Chanel 디바이스 초기화 */
//ADC Port에 대한 디바이스 초기화
//GG712 센서의 전류를 감시하여 동작여부를 체크
  Device_Init_ADC();	
}

/** \} Device_Exported_Functions */

/* Private functions ---------------------------------------------------------*/
/** \addtogroup Device_Private_Functions
  * \{ */


/** \brief  LED 디바이스를 초기화한다.
  * \note   각 LED의 정의를 Led_t 타입의 구조체에 대입하여 디바이스 전역변수를
            설정해두며 초기화를 수행한다.
  * \retval 없음
  */
static void Device_Init_LED(void)
{
	
  static Led_t  _led[kNbrOfLEDs];

  _led[kLED_RUN].name       = LED_RUN_NAME ;
  _led[kLED_RUN].port       = LED_RUN_Port;
  _led[kLED_RUN].init.Pin   = LED_RUN_Pin;
  _led[kLED_RUN].init.Mode  = GPIO_MODE_OUTPUT_PP;
  _led[kLED_RUN].init.Pull  = GPIO_NOPULL;
  _led[kLED_RUN].init.Speed = GPIO_SPEED_HIGH;
  _led[kLED_RUN].on         = LED_RUN_ON;
  _led[kLED_RUN].off        = LED_RUN_OFF;
  run_led             = &_led[kLED_RUN];
  Led_Init(run_led);

  _led[kLED_DER].name       = LED_DMPC_NAME ;
  _led[kLED_DER].port       = LED_DMPC_Port;
  _led[kLED_DER].init.Pin   = LED_DMPC_Pin;
  _led[kLED_DER].init.Mode  = GPIO_MODE_OUTPUT_PP;
  _led[kLED_DER].init.Pull  = GPIO_NOPULL;
  _led[kLED_DER].init.Speed = GPIO_SPEED_HIGH;
  _led[kLED_DER].on         = LED_DMPC_ON;
  _led[kLED_DER].off        = LED_DMPC_OFF;
  der_led             = &_led[kLED_DER];
  Led_Init(der_led);

  _led[kLED_E_EB].name       = LED_485_ERR_NAME ;
  _led[kLED_E_EB].port       = LED_485_ERR_Port;
  _led[kLED_E_EB].init.Pin   = LED_485_ERR_Pin;
  _led[kLED_E_EB].init.Mode  = GPIO_MODE_OUTPUT_PP;
  _led[kLED_E_EB].init.Pull  = GPIO_NOPULL;
  _led[kLED_E_EB].init.Speed = GPIO_SPEED_HIGH;
  _led[kLED_E_EB].on         = LED_485_ERR_ON;
  _led[kLED_E_EB].off        = LED_485_ERR_OFF;
  e_eb_led             = &_led[kLED_E_EB];
  Led_Init(e_eb_led);

  _led[kLED_HLT].name       = LED_HLT_NAME ;
  _led[kLED_HLT].port       = LED_HLT_Port;
  _led[kLED_HLT].init.Pin   = LED_HLT_Pin;
  _led[kLED_HLT].init.Mode  = GPIO_MODE_OUTPUT_PP;
  _led[kLED_HLT].init.Pull  = GPIO_NOPULL;
  _led[kLED_HLT].init.Speed = GPIO_SPEED_HIGH;
  _led[kLED_HLT].on         = LED_HLT_ON;
  _led[kLED_HLT].off        = LED_HLT_OFF;
  hlt_led             = &_led[kLED_HLT];
  Led_Init(hlt_led);
  
  _led[kLED_OBR].name       = LED_HCR_NAME ;
  _led[kLED_OBR].port       = LED_HCR_Port;
  _led[kLED_OBR].init.Pin   = LED_HCR_Pin;
  _led[kLED_OBR].init.Mode  = GPIO_MODE_OUTPUT_PP;
  _led[kLED_OBR].init.Pull  = GPIO_NOPULL;
  _led[kLED_OBR].init.Speed = GPIO_SPEED_HIGH;
  _led[kLED_OBR].on         = LED_HCR_ON;
  _led[kLED_OBR].off        = LED_HCR_OFF;
  obr_led             = &_led[kLED_OBR];
  Led_Init(obr_led);

  _led[kLED_DEL].name       = LED_PC_ERR_NAME ;
  _led[kLED_DEL].port       = LED_PC_ERR_Port;
  _led[kLED_DEL].init.Pin   = LED_PC_ERR_Pin;
  _led[kLED_DEL].init.Mode  = GPIO_MODE_OUTPUT_PP;
  _led[kLED_DEL].init.Pull  = GPIO_NOPULL;
  _led[kLED_DEL].init.Speed = GPIO_SPEED_HIGH;
  _led[kLED_DEL].on         = LED_PC_ERR_ON;
  _led[kLED_DEL].off        = LED_PC_ERR_OFF;
  del_led             = &_led[kLED_DEL];
  Led_Init(del_led);
  
  _led[kLED_E_OBR].name       = LED_OBR_NAME ;
  _led[kLED_E_OBR].port       = LED_OBR_Port;
  _led[kLED_E_OBR].init.Pin   = LED_OBR_Pin;
  _led[kLED_E_OBR].init.Mode  = GPIO_MODE_OUTPUT_PP;
  _led[kLED_E_OBR].init.Pull  = GPIO_NOPULL;
  _led[kLED_E_OBR].init.Speed = GPIO_SPEED_HIGH;
  _led[kLED_E_OBR].on         = LED_OBR_ON;
  _led[kLED_E_OBR].off        = LED_OBR_OFF;
  e_obr_led             = &_led[kLED_E_OBR];
  Led_Init(e_obr_led);

  _led[kLED_E_DER].name       = LED_DER_NAME ;
  _led[kLED_E_DER].port       = LED_DER_Port;
  _led[kLED_E_DER].init.Pin   = LED_DER_Pin;
  _led[kLED_E_DER].init.Mode  = GPIO_MODE_OUTPUT_PP;
  _led[kLED_E_DER].init.Pull  = GPIO_NOPULL;
  _led[kLED_E_DER].init.Speed = GPIO_SPEED_HIGH;
  _led[kLED_E_DER].on         = LED_DER_ON;
  _led[kLED_E_DER].off        = LED_DER_OFF;
  e_der_led             = &_led[kLED_E_DER];
  Led_Init(e_der_led);

  _led[kLED_E_OBL].name       = LED_OBL_NAME ;
  _led[kLED_E_OBL].port       = LED_OBL_Port;
  _led[kLED_E_OBL].init.Pin   = LED_OBL_Pin;
  _led[kLED_E_OBL].init.Mode  = GPIO_MODE_OUTPUT_PP;
  _led[kLED_E_OBL].init.Pull  = GPIO_NOPULL;
  _led[kLED_E_OBL].init.Speed = GPIO_SPEED_HIGH;
  _led[kLED_E_OBL].on         = LED_OBL_ON;
  _led[kLED_E_OBL].off        = LED_OBL_OFF;
  e_obl_led             = &_led[kLED_E_OBL];
  Led_Init(e_obl_led);

  _led[kLED_E_DEL].name       = LED_DEL_NAME ;
  _led[kLED_E_DEL].port       = LED_DEL_Port;
  _led[kLED_E_DEL].init.Pin   = LED_DEL_Pin;
  _led[kLED_E_DEL].init.Mode  = GPIO_MODE_OUTPUT_PP;
  _led[kLED_E_DEL].init.Pull  = GPIO_NOPULL;
  _led[kLED_E_DEL].init.Speed = GPIO_SPEED_HIGH;
  _led[kLED_E_DEL].on         = LED_DEL_ON;
  _led[kLED_E_DEL].off        = LED_DEL_OFF;
  e_del_led             = &_led[kLED_E_DEL];
  Led_Init(e_del_led);


  _led[kLED_OBL].name       = LED_EB_NAME ;
  _led[kLED_OBL].port       = LED_EB_Port;
  _led[kLED_OBL].init.Pin   = LED_EB_Pin;
  _led[kLED_OBL].init.Mode  = GPIO_MODE_OUTPUT_PP;
  _led[kLED_OBL].init.Pull  = GPIO_NOPULL;
  _led[kLED_OBL].init.Speed = GPIO_SPEED_HIGH;
  _led[kLED_OBL].on         = LED_EB_ON;
  _led[kLED_OBL].off        = LED_EB_OFF;
  obl_led             = &_led[kLED_OBL];
  Led_Init(obl_led);
}

/** \brief  RS232(Console Port)디바이스를 초기화한다.
  * \note   디바이스 구조체에 디바이스 정의 대입하여 디바이스  전역변수를
            설정해두며 초기화를 수행한다.
  * \retval 없음
  */
static void Device_Init_Console(void)
{
  static Rs232_t _serial;
  static UART_HandleTypeDef _hserial;
  static u08_t _serial_buf_tx[SERIAL_BUF_TX_SIZE];
  static u08_t _serial_buf_rx[SERIAL_BUF_RX_SIZE];
  static u08_t _serial_buf_printf[SERIAL_BUF_PRINTF_SIZE];


  _serial.port_tx            = SERIAL_TX_Port;
  _serial.init_tx.Pin        = SERIAL_TX_Pin;
  _serial.init_tx.Mode       = GPIO_MODE_AF_PP;
  _serial.init_tx.Pull       = GPIO_PULLUP;
  _serial.init_tx.Speed      = GPIO_SPEED_FREQ_VERY_HIGH;
  _serial.init_tx.Alternate  = SERIAL_TX_Alternate;

  _serial.port_rx            = SERIAL_RX_Port;
  _serial.init_rx.Pin        = SERIAL_RX_Pin;
  _serial.init_rx.Mode       = GPIO_MODE_AF_PP;
  _serial.init_rx.Pull       = GPIO_PULLUP;
  _serial.init_rx.Speed      = GPIO_SPEED_FREQ_VERY_HIGH;
  _serial.init_rx.Alternate  = SERIAL_RX_Alternate;

  _serial.tx_buf             = _serial_buf_tx;
  _serial.tx_size            = SERIAL_BUF_TX_SIZE;
  _serial.tx_count_in        = 0U;
  _serial.tx_count_out       = 0U;
  _serial.tx_ing             = False;

  _serial.rx_buf             = _serial_buf_rx;
  _serial.rx_size            = SERIAL_BUF_RX_SIZE;
  _serial.rx_count           = 0U;
  _serial.rx_tick            = 0U;
  _serial.rx_complete        = False;

  _serial.printf_buf         = _serial_buf_printf;
  _serial.printf_size        = SERIAL_BUF_PRINTF_SIZE;

  _serial.usingIRQ           = SERIAL_IRQ_Use;
  _serial.irqn               = SERIAL_IRQn;
  _serial.irq_priority       = SERIAL_IRQ_Priority;

  _hserial.Instance          = SERIAL_UART;
  _hserial.Init.BaudRate     = SERIAL_BaudRate;
  _hserial.Init.WordLength   = SERIAL_WordLength;
  _hserial.Init.StopBits     = SERIAL_StopBit;
  _hserial.Init.Parity       = SERIAL_Parity;
  _hserial.Init.Mode         = SERIAL_Mode;
  _hserial.Init.HwFlowCtl    = SERIAL_HwFlowCtl;
  _hserial.Init.OverSampling = SERIAL_OverSampling;
  _serial.handle             = &_hserial;

  serial3                      = &_serial;
  Rs232_Init(serial3);
}


/** \brief  Watch-Dog Timer 디바이스를 초기화한다.
  * \note   각 Watch-dog의 정의를 Watchdog_t 타입의 구조체에 대입하여 디바이스 전역변수를
            설정해두며 초기화를 수행한다.
  * \retval 없음
  */

static void Device_Init_Watchdog(void)
{
  static Watchdog_t  _wdog;

  _wdog.name       = WATCHDOG_NAME;
  _wdog.port       = WATCHDOG_Port;
  _wdog.init.Pin   = WATCHDOG_Pin;
  _wdog.init.Mode  = GPIO_MODE_OUTPUT_PP;
  _wdog.init.Pull  = GPIO_NOPULL;
  _wdog.init.Speed = GPIO_SPEED_HIGH;
  wdog             = &_wdog; 
  Watchdog_Init(wdog);
}


/** \brief  PWR_OK_CPU 디바이스를 초기화한다.
  * \note   각 DI의 정의를 DI_t 타입의 구조체에 대입하여 디바이스 전역변수를
            설정해두며 초기화를 수행한다.
  * \retval 없음
  */

static void Pwr_OK_CPU_Init(void)
{
  static DI_t  _pwrin;

  _pwrin.name       = PWR_OK_CPU_NAME;
  _pwrin.max_filter = FILTER_msec_DipSW;
  _pwrin.filtering  = FILTER_msec_DipSW;
  _pwrin.port       = PWR_OK_CPU_Port;
  _pwrin.init.Pin   = PWR_OK_CPU_Pin;
  _pwrin.init.Mode  = GPIO_MODE_INPUT;
  _pwrin.init.Pull  = GPIO_PULLUP;
  _pwrin.init.Speed = GPIO_SPEED_HIGH;
  _pwrin.on         = PWR_OK_CPU_ON;
  _pwrin.off        = PWR_OK_CPU_OFF;
  pwrin             = &_pwrin;
  DI_Init(pwrin);
	pwrin->curr_state = False;
}


/** \brief  RELAY OUTPUT 디바이스를 초기화한다.
  * \note   각 RELAY OUTPUT의 정의를 DO_t 타입의 구조체에 대입하여 디바이스 전역변수를
            설정해두며 초기화를 수행한다.
  * \retval 없음
  */
static void Device_Init_Relay_Output(void)
{
  static Relay_t  _rly[kNbrOfRYOs];

  _rly[kRY_EB1].name       = DO_RY_EB1_NAME;
  _rly[kRY_EB1].port       = DO_RY_EB1_Port;
  _rly[kRY_EB1].init.Pin   = DO_RY_EB1_Pin;
  _rly[kRY_EB1].init.Mode  = GPIO_MODE_OUTPUT_PP;
  _rly[kRY_EB1].init.Pull  = GPIO_PULLUP;
  _rly[kRY_EB1].init.Speed = GPIO_SPEED_HIGH;
  _rly[kRY_EB1].on         = DO_RY_EB1_ON;
  _rly[kRY_EB1].off        = DO_RY_EB1_OFF;
  eb1_out                  = &_rly[kRY_EB1];
  Relay_Init(eb1_out);

  _rly[kRY_EB2].name       = DO_RY_EB2_NAME;
  _rly[kRY_EB2].port       = DO_RY_EB2_Port;
  _rly[kRY_EB2].init.Pin   = DO_RY_EB2_Pin;
  _rly[kRY_EB2].init.Mode  = GPIO_MODE_OUTPUT_PP;
  _rly[kRY_EB2].init.Pull  = GPIO_PULLUP;
  _rly[kRY_EB2].init.Speed = GPIO_SPEED_HIGH;
  _rly[kRY_EB2].on         = DO_RY_EB2_ON;
  _rly[kRY_EB2].off        = DO_RY_EB2_OFF;
  eb2_out                  = &_rly[kRY_EB2];
  Relay_Init(eb2_out);

  _rly[kRY_DMPC].name       = DO_RY_DEPC_NAME;
  _rly[kRY_DMPC].port       = DO_RY_DEPC_Port;
  _rly[kRY_DMPC].init.Pin   = DO_RY_DEPC_Pin;
  _rly[kRY_DMPC].init.Mode  = GPIO_MODE_OUTPUT_PP;
  _rly[kRY_DMPC].init.Pull  = GPIO_PULLUP;
  _rly[kRY_DMPC].init.Speed = GPIO_SPEED_HIGH;
  _rly[kRY_DMPC].on         = DO_RY_DEPC_ON;
  _rly[kRY_DMPC].off        = DO_RY_DEPC_OFF;
  dmpc_out                  = &_rly[kRY_DMPC];
  Relay_Init(dmpc_out);

  _rly[kRY_HLT].name       = DO_RY_HLT_NAME;
  _rly[kRY_HLT].port       = DO_RY_HLT_Port;
  _rly[kRY_HLT].init.Pin   = DO_RY_HLT_Pin;
  _rly[kRY_HLT].init.Mode  = GPIO_MODE_OUTPUT_PP;
  _rly[kRY_HLT].init.Pull  = GPIO_PULLUP;
  _rly[kRY_HLT].init.Speed = GPIO_SPEED_HIGH;
  _rly[kRY_HLT].on         = DO_RY_HLT_ON;
  _rly[kRY_HLT].off        = DO_RY_HLT_OFF;
  hlt_out                  = &_rly[kRY_HLT];
  Relay_Init(hlt_out);
 
  _rly[kRY_OBR].name       = DO_RY_OBR_NAME;
  _rly[kRY_OBR].port       = DO_RY_OBR_Port;
  _rly[kRY_OBR].init.Pin   = DO_RY_OBR_Pin;
  _rly[kRY_OBR].init.Mode  = GPIO_MODE_OUTPUT_PP;
  _rly[kRY_OBR].init.Pull  = GPIO_PULLUP;
  _rly[kRY_OBR].init.Speed = GPIO_SPEED_HIGH;
  _rly[kRY_OBR].on         = DO_RY_OBR_ON;
  _rly[kRY_OBR].off        = DO_RY_OBR_OFF;
  obr_out                  = &_rly[kRY_OBR];
  Relay_Init(obr_out);

  _rly[kRY_DER].name       = DO_RY_DER_NAME;
  _rly[kRY_DER].port       = DO_RY_DER_Port;
  _rly[kRY_DER].init.Pin   = DO_RY_DER_Pin;
  _rly[kRY_DER].init.Mode  = GPIO_MODE_OUTPUT_PP;
  _rly[kRY_DER].init.Pull  = GPIO_PULLUP;
  _rly[kRY_DER].init.Speed = GPIO_SPEED_HIGH;
  _rly[kRY_DER].on         = DO_RY_DER_ON;
  _rly[kRY_DER].off        = DO_RY_DER_OFF;
  der_out              = &_rly[kRY_DER];
  Relay_Init(der_out);

  _rly[kRY_OBL].name       = DO_RY_OBL_NAME;
  _rly[kRY_OBL].port       = DO_RY_OBL_Port;
  _rly[kRY_OBL].init.Pin   = DO_RY_OBL_Pin;
  _rly[kRY_OBL].init.Mode  = GPIO_MODE_OUTPUT_PP;
  _rly[kRY_OBL].init.Pull  = GPIO_PULLUP;
  _rly[kRY_OBL].init.Speed = GPIO_SPEED_HIGH;
  _rly[kRY_OBL].on         = DO_RY_OBL_ON;
  _rly[kRY_OBL].off        = DO_RY_OBL_OFF;
  obl_out                  = &_rly[kRY_OBL];
  Relay_Init(obl_out);

  _rly[kRY_DEL].name       = DO_RY_DEL_NAME;
  _rly[kRY_DEL].port       = DO_RY_DEL_Port;
  _rly[kRY_DEL].init.Pin   = DO_RY_DEL_Pin;
  _rly[kRY_DEL].init.Mode  = GPIO_MODE_OUTPUT_PP;
  _rly[kRY_DEL].init.Pull  = GPIO_PULLUP;
  _rly[kRY_DEL].init.Speed = GPIO_SPEED_HIGH;
  _rly[kRY_DEL].on         = DO_RY_DEL_ON;
  _rly[kRY_DEL].off        = DO_RY_DEL_OFF;
  del_out                  = &_rly[kRY_DEL];
  Relay_Init(del_out);
}



/** \brief  MCU TEST#_ON Output 디바이스를 초기화한다.
  * \note   각 MCU TEST#_ON의 정의를 DO_t 타입의 구조체에 대입하여 디바이스 전역변수를
            설정해두며 초기화를 수행한다.
  * \retval 없음
  */
static void Device_Init_Sensor_Test_Out(void)
{
static DO_t  _sout[kNbrOfTest];
static DO_t  _snp_out;;
	
  _sout[TEST_1].name       = MCU_TEST1_NAME;
  _sout[TEST_1].port       = MCU_TEST1_Port;
  _sout[TEST_1].init.Pin   = MCU_TEST1_Pin;
  _sout[TEST_1].init.Mode  = GPIO_MODE_OUTPUT_PP;
  _sout[TEST_1].init.Pull  = GPIO_PULLUP;
  _sout[TEST_1].init.Speed = GPIO_SPEED_HIGH;
  _sout[TEST_1].on         = MCU_TEST1_ON;
  _sout[TEST_1].off        = MCU_TEST1_OFF;
  mcu_test1                = &_sout[TEST_1];
  DO_Init(mcu_test1);

  _sout[TEST_2].name       = MCU_TEST2_NAME;
  _sout[TEST_2].port       = MCU_TEST2_Port;
  _sout[TEST_2].init.Pin   = MCU_TEST2_Pin;
  _sout[TEST_2].init.Mode  = GPIO_MODE_OUTPUT_PP;
  _sout[TEST_2].init.Pull  = GPIO_PULLUP;
  _sout[TEST_2].init.Speed = GPIO_SPEED_HIGH;
  _sout[TEST_2].on         = MCU_TEST2_ON;
  _sout[TEST_2].off        = MCU_TEST2_OFF;
  mcu_test2                = &_sout[TEST_2];
  DO_Init(mcu_test2);

  _sout[TEST_3].name       = MCU_TEST3_NAME;
  _sout[TEST_3].port       = MCU_TEST3_Port;
  _sout[TEST_3].init.Pin   = MCU_TEST3_Pin;
  _sout[TEST_3].init.Mode  = GPIO_MODE_OUTPUT_PP;
  _sout[TEST_3].init.Pull  = GPIO_PULLUP;
  _sout[TEST_3].init.Speed = GPIO_SPEED_HIGH;
  _sout[TEST_3].on         = MCU_TEST3_ON;
  _sout[TEST_3].off        = MCU_TEST3_OFF;
  mcu_test3                = &_sout[TEST_3];
  DO_Init(mcu_test3);

  _sout[TEST_4].name       = "TEST_4";
  _sout[TEST_4].port       = MCU_TEST4_Port;
  _sout[TEST_4].init.Pin   = MCU_TEST4_Pin;
  _sout[TEST_4].init.Mode  = GPIO_MODE_OUTPUT_PP;
  _sout[TEST_4].init.Pull  = GPIO_PULLUP;
  _sout[TEST_4].init.Speed = GPIO_SPEED_HIGH;
  _sout[TEST_4].on         = MCU_TEST4_ON;
  _sout[TEST_4].off        = MCU_TEST4_OFF;
  mcu_test4             = &_sout[TEST_4];
  DO_Init(mcu_test4);

  _snp_out.name            = SNP_POWER_NAME;
  _snp_out.port            = SNP_POWER_Port;
  _snp_out.init.Pin        = SNP_POWER_Pin;
  _snp_out.init.Mode       = GPIO_MODE_OUTPUT_PP;
  _snp_out.init.Pull       = GPIO_PULLUP;
  _snp_out.init.Speed      = GPIO_SPEED_HIGH;
  _snp_out.on              = SNP_POWER_ON;
  _snp_out.off             = SNP_POWER_OFF;
  snp_out                  = &_snp_out;
  DO_Init(snp_out);
}

/** \brief  Relay Loop Back Input 디바이스를 초기화한다.
  * \note   각 DI의 정의를 DI_t 타입의 구조체에 대입하여 디바이스 전역변수를
            설정해두며 초기화를 수행한다.
  * \retval 없음
  */
static void Device_Init_Relay_Feed_Back_Input(void)
{
  static DI_t  _din[kNbrOfRYFBIs];

  _din[kRYFBI_EB1].name       = DI_RY_EB1_FB_NAME;
  _din[kRYFBI_EB1].max_filter = FILTER_msec_DipSW;
  _din[kRYFBI_EB1].filtering  = FILTER_msec_DipSW;
  _din[kRYFBI_EB1].port       = DI_RY_EB1_FB_Port;
  _din[kRYFBI_EB1].init.Pin   = DI_RY_EB1_FB_Pin;
  _din[kRYFBI_EB1].init.Mode  = GPIO_MODE_INPUT;
  _din[kRYFBI_EB1].init.Pull  = GPIO_PULLUP;
  _din[kRYFBI_EB1].init.Speed = GPIO_SPEED_HIGH;
  _din[kRYFBI_EB1].on         = DI_RY_EB1_FB_ON;
  _din[kRYFBI_EB1].off        = DI_RY_EB1_FB_OFF;
  din[kRYFBI_EB1]             = &_din[kRYFBI_EB1];
  DI_Init(din[kRYFBI_EB1]);

  _din[kRYFBI_EB2].name       = DI_RY_EB2_FB_NAME;
  _din[kRYFBI_EB2].max_filter = FILTER_msec_DipSW;
  _din[kRYFBI_EB2].filtering  = FILTER_msec_DipSW;
  _din[kRYFBI_EB2].port       = DI_RY_EB2_FB_Port;
  _din[kRYFBI_EB2].init.Pin   = DI_RY_EB2_FB_Pin;
  _din[kRYFBI_EB2].init.Mode  = GPIO_MODE_INPUT;
  _din[kRYFBI_EB2].init.Pull  = GPIO_PULLUP;
  _din[kRYFBI_EB2].init.Speed = GPIO_SPEED_HIGH;
  _din[kRYFBI_EB2].on         = DI_RY_EB2_FB_ON;
  _din[kRYFBI_EB2].off        = DI_RY_EB2_FB_OFF;
  din[kRYFBI_EB2]             = &_din[kRYFBI_EB2];
  DI_Init(din[kRYFBI_EB2]);

  _din[kRYFBI_DMPC].name       = DI_RY_DEPC_FB_NAME;
  _din[kRYFBI_DMPC].max_filter = FILTER_msec_DipSW;
  _din[kRYFBI_DMPC].filtering  = FILTER_msec_DipSW;
  _din[kRYFBI_DMPC].port       = DI_RY_DEPC_FB_Port;
  _din[kRYFBI_DMPC].init.Pin   = DI_RY_DEPC_FB_Pin;
  _din[kRYFBI_DMPC].init.Mode  = GPIO_MODE_INPUT;
  _din[kRYFBI_DMPC].init.Pull  = GPIO_PULLUP;
  _din[kRYFBI_DMPC].init.Speed = GPIO_SPEED_HIGH;
  _din[kRYFBI_DMPC].on         = DI_RY_DEPC_FB_ON;
  _din[kRYFBI_DMPC].off        = DI_RY_DEPC_FB_OFF;
  din[kRYFBI_DMPC]             = &_din[kRYFBI_DMPC];
  DI_Init(din[kRYFBI_DMPC]);

  _din[kRYFBI_HLT].name       = DI_RY_HLT_FB_NAME;
  _din[kRYFBI_HLT].max_filter = FILTER_msec_DipSW;
  _din[kRYFBI_HLT].filtering  = FILTER_msec_DipSW;
  _din[kRYFBI_HLT].port       = DI_RY_HLT_FB_Port;
  _din[kRYFBI_HLT].init.Pin   = DI_RY_HLT_FB_Pin;
  _din[kRYFBI_HLT].init.Mode  = GPIO_MODE_INPUT;
  _din[kRYFBI_HLT].init.Pull  = GPIO_PULLUP;
  _din[kRYFBI_HLT].init.Speed = GPIO_SPEED_HIGH;
  _din[kRYFBI_HLT].on         = DI_RY_HLT_FB_ON;
  _din[kRYFBI_HLT].off        = DI_RY_HLT_FB_OFF;
  din[kRYFBI_HLT]             = &_din[kRYFBI_HLT];
  DI_Init(din[kRYFBI_HLT]);
  
  _din[kRYFBI_OBR].name       = DI_RY_OBR_FB_NAME;
  _din[kRYFBI_OBR].max_filter = FILTER_msec_DipSW;
  _din[kRYFBI_OBR].filtering  = FILTER_msec_DipSW;
  _din[kRYFBI_OBR].port       = DI_RY_OBR_FB_Port;
  _din[kRYFBI_OBR].init.Pin   = DI_RY_OBR_FB_Pin;
  _din[kRYFBI_OBR].init.Mode  = GPIO_MODE_INPUT;
  _din[kRYFBI_OBR].init.Pull  = GPIO_PULLUP;
  _din[kRYFBI_OBR].init.Speed = GPIO_SPEED_HIGH;
  _din[kRYFBI_OBR].on         = DI_RY_OBR_FB_ON;
  _din[kRYFBI_OBR].off        = DI_RY_OBR_FB_OFF;
  din[kRYFBI_OBR]             = &_din[kRYFBI_OBR];
  DI_Init(din[kRYFBI_OBR]);

  _din[kRYFBI_DER].name       = DI_RY_DER_FB_NAME;
  _din[kRYFBI_DER].max_filter = FILTER_msec_DipSW;
  _din[kRYFBI_DER].filtering  = FILTER_msec_DipSW;
  _din[kRYFBI_DER].port       = DI_RY_DER_FB_Port;
  _din[kRYFBI_DER].init.Pin   = DI_RY_DER_FB_Pin;
  _din[kRYFBI_DER].init.Mode  = GPIO_MODE_INPUT;
  _din[kRYFBI_DER].init.Pull  = GPIO_PULLUP;
  _din[kRYFBI_DER].init.Speed = GPIO_SPEED_HIGH;
  _din[kRYFBI_DER].on         = DI_RY_DER_FB_ON;
  _din[kRYFBI_DER].off        = DI_RY_DER_FB_OFF;
  din[kRYFBI_DER]             = &_din[kRYFBI_DER];
  DI_Init(din[kRYFBI_DER]);

  _din[kRYFBI_OBL].name       = DI_RY_OBL_FB_NAME;
  _din[kRYFBI_OBL].max_filter = FILTER_msec_DipSW;
  _din[kRYFBI_OBL].filtering  = FILTER_msec_DipSW;
  _din[kRYFBI_OBL].port       = DI_RY_OBL_FB_Port;
  _din[kRYFBI_OBL].init.Pin   = DI_RY_OBL_FB_Pin;
  _din[kRYFBI_OBL].init.Mode  = GPIO_MODE_INPUT;
  _din[kRYFBI_OBL].init.Pull  = GPIO_PULLUP;
  _din[kRYFBI_OBL].init.Speed = GPIO_SPEED_HIGH;
  _din[kRYFBI_OBL].on         = DI_RY_OBL_FB_ON;
  _din[kRYFBI_OBL].off        = DI_RY_OBL_FB_OFF;
  din[kRYFBI_OBL]             = &_din[kRYFBI_OBL];
  DI_Init(din[kRYFBI_OBL]);

  _din[kRYFBI_DEL].name       = DI_RY_DEL_FB_NAME;
  _din[kRYFBI_DEL].max_filter = FILTER_msec_DipSW;
  _din[kRYFBI_DEL].filtering  = FILTER_msec_DipSW;
  _din[kRYFBI_DEL].port       = DI_RY_DEL_FB_Port;
  _din[kRYFBI_DEL].init.Pin   = DI_RY_DEL_FB_Pin;
  _din[kRYFBI_DEL].init.Mode  = GPIO_MODE_INPUT;
  _din[kRYFBI_DEL].init.Pull  = GPIO_PULLUP;
  _din[kRYFBI_DEL].init.Speed = GPIO_SPEED_HIGH;
  _din[kRYFBI_DEL].on         = DI_RY_DEL_FB_ON;
  _din[kRYFBI_DEL].off        = DI_RY_DEL_FB_OFF;
  din[kRYFBI_DEL]             = &_din[kRYFBI_DEL];
  DI_Init(din[kRYFBI_DEL]);
}

/** \brief  Sensor Input 디바이스를 초기화한다.
  * \note   각 DI의 정의를 DI_t 타입의 구조체에 대입하여 디바이스 전역변수를
            설정해두며 초기화를 수행한다.
  * \retval 없음
  */
static void Device_Init_Sensor_Input(void)
{
  static DEOD_t  _sdin[kNbrOfSIs];

  
		/* 센서 1번째(OBR) 관련 PORT 초기화 */
	_sdin[kSI_1].name           = SI1_OSSD_NAME;	
  _sdin[kSI_1].port1          = SI1_OSSD1_Port;
  _sdin[kSI_1].init1.Pin      = SI1_OSSD1_Pin;
  _sdin[kSI_1].init1.Mode     = GPIO_MODE_INPUT;
  _sdin[kSI_1].init1.Pull     = GPIO_PULLUP;
  _sdin[kSI_1].init1.Speed    = GPIO_SPEED_HIGH;
  Periph_Control_GPIO_CLK(_sdin->port1, True);
  HAL_GPIO_Init(_sdin->port1, &_sdin->init1);
/*************************************************************************/	
  _sdin[kSI_1].port2          = SI1_OSSD2_Port;
  _sdin[kSI_1].init2.Pin      = SI1_OSSD2_Pin;
  _sdin[kSI_1].init2.Mode     = GPIO_MODE_INPUT;
  _sdin[kSI_1].init2.Pull     = GPIO_PULLUP;
  _sdin[kSI_1].init2.Speed    = GPIO_SPEED_HIGH;
  Periph_Control_GPIO_CLK(_sdin->port2, True);
  HAL_GPIO_Init(_sdin->port2, &_sdin->init2);	
/**************************************************************************/	
  _sdin[kSI_1].Sensor_flag    = False;
  _sdin[kSI_1].Sensor_fail_flag = False;
  _sdin[kSI_1].Sensor_Counter = 0;
  _sdin[kSI_1].Sensor_Voltage = 0;
  _sdin[kSI_1].Sensor_OSSD1   = False;
  _sdin[kSI_1].Sensor_OSSD2   = False;	
  sdin[kSI_1]                 = &_sdin[kSI_1];
	
		/* 센서 2번째(DER) 관련 PORT 초기화 */
	_sdin[kSI_2].name           = SI2_OSSD_NAME;		
  _sdin[kSI_2].port1          = SI2_OSSD1_Port;
  _sdin[kSI_2].init1.Pin      = SI2_OSSD1_Pin;
  _sdin[kSI_2].init1.Mode     = GPIO_MODE_INPUT;
  _sdin[kSI_2].init1.Pull     = GPIO_PULLUP;
  _sdin[kSI_2].init1.Speed    = GPIO_SPEED_HIGH;
  Periph_Control_GPIO_CLK(_sdin->port1, True);
  HAL_GPIO_Init(_sdin->port1, &_sdin->init1);
/**************************************************************************/	
  _sdin[kSI_2].port2          = SI2_OSSD2_Port;
  _sdin[kSI_2].init2.Pin      = SI2_OSSD2_Pin;
  _sdin[kSI_2].init2.Mode     = GPIO_MODE_INPUT;
  _sdin[kSI_2].init2.Pull     = GPIO_PULLUP;
  _sdin[kSI_2].init2.Speed    = GPIO_SPEED_HIGH;
  Periph_Control_GPIO_CLK(_sdin->port2, True);
  HAL_GPIO_Init(_sdin->port2, &_sdin->init2);	
/**************************************************************************/	
  _sdin[kSI_2].Sensor_flag    = False;
  _sdin[kSI_2].Sensor_fail_flag = False;  
  _sdin[kSI_2].Sensor_Counter = 0;
  _sdin[kSI_2].Sensor_Voltage = 0;
  _sdin[kSI_2].Sensor_OSSD1   = False;
  _sdin[kSI_2].Sensor_OSSD2   = False;
  sdin[kSI_2]                 = &_sdin[kSI_2];

		/* 센서 3번째(OBL) 관련 PORT 초기화 */
	_sdin[kSI_3].name           = SI3_OSSD_NAME;		
  _sdin[kSI_3].port1          = SI3_OSSD1_Port;
  _sdin[kSI_3].init1.Pin      = SI3_OSSD1_Pin;
  _sdin[kSI_3].init1.Mode     = GPIO_MODE_INPUT;
  _sdin[kSI_3].init1.Pull     = GPIO_PULLUP;
  _sdin[kSI_3].init1.Speed    = GPIO_SPEED_HIGH;
  Periph_Control_GPIO_CLK(_sdin->port1, True);
  HAL_GPIO_Init(_sdin->port1, &_sdin->init1);
/**************************************************************************/	
  _sdin[kSI_3].port2          = SI3_OSSD2_Port;
  _sdin[kSI_3].init2.Pin      = SI3_OSSD2_Pin;
  _sdin[kSI_3].init2.Mode     = GPIO_MODE_INPUT;
  _sdin[kSI_3].init2.Pull     = GPIO_PULLUP;
  _sdin[kSI_3].init2.Speed    = GPIO_SPEED_HIGH;
  Periph_Control_GPIO_CLK(_sdin->port2, True);
  HAL_GPIO_Init(_sdin->port2, &_sdin->init2);	
/**************************************************************************/	
  _sdin[kSI_3].Sensor_flag    = False;
  _sdin[kSI_3].Sensor_fail_flag = False;  
  _sdin[kSI_3].Sensor_Counter = 0;
  _sdin[kSI_3].Sensor_Voltage = 0;
  _sdin[kSI_3].Sensor_OSSD1   = False;
  _sdin[kSI_3].Sensor_OSSD2   = False;
  sdin[kSI_3]                 = &_sdin[kSI_3];

		/* 센서 4번째(DEL) 관련 PORT 초기화 */
	_sdin[kSI_4].name           = SI4_OSSD_NAME;		
  _sdin[kSI_4].port1          = SI4_OSSD1_Port;
  _sdin[kSI_4].init1.Pin      = SI4_OSSD1_Pin;
  _sdin[kSI_4].init1.Mode     = GPIO_MODE_INPUT;
  _sdin[kSI_4].init1.Pull     = GPIO_PULLUP;
  _sdin[kSI_4].init1.Speed    = GPIO_SPEED_HIGH;
  Periph_Control_GPIO_CLK(_sdin->port1, True);
  HAL_GPIO_Init(_sdin->port1, &_sdin->init1);
/**************************************************************************/	
  _sdin[kSI_4].port2          = SI4_OSSD2_Port;
  _sdin[kSI_4].init2.Pin      = SI4_OSSD2_Pin;
  _sdin[kSI_4].init2.Mode     = GPIO_MODE_INPUT;
  _sdin[kSI_4].init2.Pull     = GPIO_PULLUP;
  _sdin[kSI_4].init2.Speed    = GPIO_SPEED_HIGH;
  Periph_Control_GPIO_CLK(_sdin->port2, True);
  HAL_GPIO_Init(_sdin->port2, &_sdin->init2);	
/**************************************************************************/	
  _sdin[kSI_4].Sensor_flag    = False;
  _sdin[kSI_4].Sensor_fail_flag = False;  
  _sdin[kSI_4].Sensor_Counter = 0;
  _sdin[kSI_4].Sensor_Voltage = 0;
  _sdin[kSI_4].Sensor_OSSD1   = False;
  _sdin[kSI_4].Sensor_OSSD2   = False;
  sdin[kSI_4]                 = &_sdin[kSI_4];
}


/** \brief  Digital Output Isolator State 디바이스를 초기화한다.
  * \note   각 DOIS의 정의를 DI_t 타입의 구조체에 대입하여 디바이스 전역변수를
            설정해두며 초기화를 수행한다.
  * \retval 없음
  */

static void Device_Init_DOIS(void)
{
  static DI_t   _fpin_1;
  static DI_t   _fpin_2;
  static DI_t   _wdt_ok;	
	
  _fpin_1.name       = DOIS_1_NAME;
  _fpin_1.max_filter = FILTER_msec_DipSW;
  _fpin_1.filtering  = FILTER_msec_DipSW;
  _fpin_1.port       = DOIS_1_Port;
  _fpin_1.init.Pin   = DOIS_1_Pin;
  _fpin_1.init.Mode  = GPIO_MODE_INPUT;
  _fpin_1.init.Pull  = GPIO_PULLUP;
  _fpin_1.init.Speed = GPIO_SPEED_HIGH;
  _fpin_1.on         = DOIS_1_ON;
  _fpin_1.off        = DOIS_1_OFF;
  _fpin_1.curr_state = False;
  fpin_1             = &_fpin_1;
  DI_Init(fpin_1);

  _fpin_2.name       = DOIS_2_NAME;
  _fpin_2.max_filter = FILTER_msec_DipSW;
  _fpin_2.filtering  = FILTER_msec_DipSW;
  _fpin_2.port       = DOIS_2_Port;
  _fpin_2.init.Pin   = DOIS_2_Pin;
  _fpin_2.init.Mode  = GPIO_MODE_INPUT;
  _fpin_2.init.Pull  = GPIO_PULLUP;
  _fpin_2.init.Speed = GPIO_SPEED_HIGH;
  _fpin_2.on         = DOIS_2_ON;
  _fpin_2.off        = DOIS_2_OFF;
    _fpin_2.curr_state = False;
  fpin_2             = &_fpin_2;
  DI_Init(fpin_2);
	
  _wdt_ok.name       =  WDT_OK_NAME;
  _wdt_ok.max_filter = FILTER_msec_DipSW;
  _wdt_ok.filtering  = FILTER_msec_DipSW;
  _wdt_ok.port       =  WDT_OK_Port;
  _wdt_ok.init.Pin   =  WDT_OK_Pin;
  _wdt_ok.init.Mode  = GPIO_MODE_INPUT;
  _wdt_ok.init.Pull  = GPIO_PULLUP;
  _wdt_ok.init.Speed = GPIO_SPEED_HIGH;
  _wdt_ok.on         =  WDT_OK_ON;
  _wdt_ok.off        =  WDT_OK_OFF;
  wdt_ok            = &_wdt_ok;
  DI_Init(wdt_ok);
}

/** \brief  TMP117 디바이스를 초기화한다.
  * \note   각 TMP117의 정의를 TMP117_t 타입의 구조체에 대입하여 디바이스 전역변수를
            설정해두며 초기화를 수행한다.
  * \retval 없음
  */

static void Device_Init_I2C(void)
{
	
  hi2c1.Instance             = I2C1;
  hi2c1.Init.ClockSpeed      = 100000;					//100KH
  hi2c1.Init.DutyCycle       = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1     = 0;   				 
  hi2c1.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;     
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;	
  hi2c1.Init.OwnAddress2     = 0;               			        
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;     
  hi2c1.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;       
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	{
//		Led_On(error_led);
	}
}  
/**
* @brief I2C MSP Initialization
* @param hi2c: I2C handle pointer
* @retval None
*/
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c1)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hi2c1->Instance==I2C1)
  {
     __HAL_RCC_GPIOB_CLK_ENABLE();
/*     I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */	
//      Peripheral clock enable  
		
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    __HAL_RCC_I2C1_CLK_ENABLE();

  }

}

// 
static void Device_Init_ADC(void)
{
	 ADC_ChannelConfTypeDef sConfig;

 /*##-1- Configure the ADC peripheral #######################################*/

   hadc1.Instance                   = ADC1;
   hadc1.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV2;
   hadc1.Init.Resolution            = ADC_RESOLUTION_12B;
   hadc1.Init.ScanConvMode          = ENABLE;                      //DISABLE;   Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
   hadc1.Init.ContinuousConvMode    = DISABLE;                     //ENABLE;    Continuous mode disabled to have only 1 conversion at each conversion trig */
   hadc1.Init.DiscontinuousConvMode = ENABLE;                      //DISABLE;   Parameter discarded because sequencer is disabled */
   hadc1.Init.NbrOfDiscConversion   = 1;
   hadc1.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;        /* Conversion start trigged at each external event */
   hadc1.Init.ExternalTrigConv      = ADC_SOFTWARE_START;
   hadc1.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
   hadc1.Init.NbrOfConversion       = 4;
   hadc1.Init.DMAContinuousRequests = DISABLE;
   hadc1.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;

	if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
  //    ADC initialization Error */
	}
  
  /*##-2- Configure ADC regular channel ######################################*/
 
	sConfig.Channel      = ADC_CHANNEL_0;
	sConfig.Rank         = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES ;
	if (HAL_ADC_ConfigChannel(&hadc1,&sConfig) != HAL_OK)
	{
	//	Channel Configuration Error 
	}

	sConfig.Channel      = ADC_CHANNEL_1;
	sConfig.Rank         = 2;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES ;
	sConfig.Offset       = 0;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
//     Channel Configuration Error 
	}

	sConfig.Channel      = ADC_CHANNEL_2;
	sConfig.Rank         = 3;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES ;
	sConfig.Offset       = 0;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
 //    Channel Configuration Error 
	}
	
	sConfig.Channel      = ADC_CHANNEL_3;
	sConfig.Rank         = 4;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES ;
	sConfig.Offset       = 0;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
 //    Channel Configuration Error  
	}  	
}

/**
* @brief ADC MSP Initialization
* This function configures the hardware resources used in this example
* @param hadc: ADC handle pointer
* @retval None
*/


void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc1)
{

  GPIO_InitTypeDef GPIO_InitStruct;
 if(hadc1->Instance==ADC1)

  {	
    __HAL_RCC_ADC1_CLK_ENABLE();		// ADC Clk Enable
    __HAL_RCC_GPIOA_CLK_ENABLE();		// Port Clk Enable
/*    ADC1 GPIO Configuration     
    PA0     ------> ADC1_IN10 ------> SN1_ADC
    PA1     ------> ADC1_IN11 ------> SN2_ADC
	  PA2     ------> ADC1_IN12 ------> SN3_ADC	
    PA3     ------> ADC1_IN13 ------> SN4_ADC
*/
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	}

}

/** \} Device_Device_Private_Functions */

/** \} Device */

/** \} Drivers */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
