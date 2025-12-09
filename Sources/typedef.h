/** ****************************************************************************
  * \file    typedef.h
  * \author  Jang Ho Jong
  * \version V0.0.1
  * \date    2023.04.05
  * \brief   Common Type Definition Header
  * ***************************************************************************/
/** \remark  Version History
  * V0.0.1   - ?? ?? - 2016.05.20

*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TYPEDEF_H
#define __TYPEDEF_H

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
typedef   signed          char i08_t;
typedef   signed short     int i16_t;
typedef   signed           int i32_t;

typedef unsigned          char u08_t;
typedef unsigned short     int u16_t;
typedef unsigned           int u32_t;

typedef char        char_t;

typedef enum
{
  False,
  True = !False
}Bool_t;
typedef Bool_t      bool_t;

#ifndef float32_t
typedef float float32_t;
#endif

/* Exported macro ------------------------------------------------------------*/
#define msec(ms)          (ms)            /**< ???? ???? ??, ms = milisec */
#define sec(ms)           (ms * 1000)     /**< ?? ???? ?? */
#define _sec(ms)          (ms / 1000)     /**< ???? ?? ?? */

#define byte(B)           (B)               /**< ???? ???? ??, B = byte */
#define Kbyte(B)          (B * 1024)        /**< ?????? ?? */
#define Mbyte(B)          (Kbyte(B) * 1024) /**< ?????? ?? */

#define Hz(herz)          (herz)                /**< ???? ???? ??, herz = herz */
#define KHz(herz)         (herz * 1000)         /**< ?????? ?? */
#define MHz(herz)         (herz * 1000 * 1000)  /**< ?????? ?? */

#define Yes             True
#define No              False

#define On              True
#define Off             False

#define RUN             0x00
#define STOP            0x01
#define EXPIRED         0x02

#define MASTER          0x01
#define SLAVE           0x02

#define OPMODE          0x01
#define IDCNT            0x02

/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __TYPEDEF_H */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
