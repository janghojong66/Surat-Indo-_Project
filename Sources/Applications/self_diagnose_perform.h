/** ****************************************************************************
  * \file    self_diagnose_perform.h
  * \author  Jang Ho Jong
  * \version V0.0.1
  * \date    2023.04.05
  * \brief   self_diagnose_perform module Header
  * ***************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SELF_DIAGNOSE_PERFORM_H
#define __SELF_DIAGNOSE_PERFORM_H

/* Includes ------------------------------------------------------------------*/
#include "self_diagnose_perform.h"
#include "device.h"

/** \addtogroup Applications
  * \{ */

#define	cMIN_ACT_CUR		  2100U    
#define	cLOW_CUR			     500U      
#define cMAX_CUR          2950U
#define cMAX_ACT_CUR			2700U
#define cMAX_COUNT           5U    // 10msec 10? 100msec
#define	PASS						True
#define FAIL						False
/** \addtogroup self_diagnose_perform
  * \{ */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/** \defgroup self_diagnose_perform_Variables ??? ???? ??? ?? ??
  * \{ */
extern void Self_Diagnose_Perform(void);
extern void Read_Relay_FeedBack_State(void);

//extern void Read_Sensor_Current_Value(void);
//extern void Read_Sensor_OSSD_Input(void);
#endif /* __SELF_DIAGNOSE_PERFORM_H */
/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
