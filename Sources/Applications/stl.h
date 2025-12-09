/** ****************************************************************************
  * \file    stl_self_test.h
  * \author  Jang Ho Jong
  * \version V0.0.1
  * \date    2023.03.19
  * \brief   Board Device Definition Header
  * ***************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STL_H
#define __STL_H

/* Includes ------------------------------------------------------------------*/

#define FLASH_SECTION_SIZE 1024U
#define RAM_SECTION_SIZE 128U
#ifdef STL_DISABLE_RAM_BCKUP_BUF
#define RAM_BACKUP_BUFFER_SIZE 0   
#else
#define RAM_BACKUP_BUFFER_SIZE 32U
#endif
#define TEST_RAM_START_ADDR (0x2001F000U + RAM_BACKUP_BUFFER_SIZE)
#define TEST_ROM_START_ADDR 0x08000000U
#define TEST_FLASH_SECTION_NB 4U /* 4 Flash sections are tested */
#define TEST_RAM_SECTION_NB 8U /* 8 RAM sections are tested */
extern void STL_Self_test(void);

/** \} Device_Exported_Functions */

/** \} Device */

/** \} Applications */

#endif /* __STL_H */

/*************** (C) COPYRIGHT SHALOM ENGINEERING CO., LTD. ****END OF FILE****/
