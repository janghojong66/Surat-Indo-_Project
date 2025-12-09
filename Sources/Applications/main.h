/** ****************************************************************************
  * \file    main.h
  * \author  Jang Ho Jong
  * \version V0.0.1
  * \date    2023.04.04
  * \brief   Main Module Header
  * ***************************************************************************/
/** \remark  Version History

*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "typedef.h"

/** \addtogroup Applications
  * \{ */

/** \addtogroup Main
  * \{ */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/** \defgroup Main_Exported_Constants Main 모듈 공개 상수
  * \{ */
/* #870 Multibyte warning 무시하라. */
//#pragma diag_suppress 870


#define PROJECT_NAME    "MAIN CONTROL DEVICE"      /**< 프로젝트명 */

#define VERSION_MAIN    1U              /**< 프로젝트의 메인 버전 */
#define VERSION_SUB1    0U              /**< 프로젝트의 서브1 버전 */
#define VERSION_SUB2    0U              /**< 프로젝트의 서브2 버전 */

#define BUILD_DATE      (__DATE__)      /**< 소프트웨어 빌드 날짜 */
#define BUILD_TIME      (__TIME__)      /**< 소프트웨어 빌드 시간 */
extern  void  STL_Self_Test(void);
/** \} Main_Exported_Constants */

/* Exported macro ------------------------------------------------------------*/
/** \defgroup Main_Exported_Macros Main 모듈 공개 매크로
  * \{ */

/** \} Main_Exported_Macros */

/* Exported functions ------------------------------------------------------- */

/** \} Main */

/** \} Applications */

/** \defgroup Libraries Libraries
  * \brief 시스템에서 공통적으로 사용되는 라이브러리입니다.
  * \{ */
/** \} Libraries */

/** \defgroup Drivers Device Drivers
  * \brief 시스템에서 사용되는 디바이스 드라이버입니다.
  * \{ */
/** \} Drivers */

#endif /* __MAIN_H */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
