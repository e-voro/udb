/*******************************************************************************
  * @file           : udb_example.h
  * @brief          : Example for Microdatabase
  * @author         : Evgeny Voropaev
  *****************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UDB_EXAMPLE_H
#define __UDB_EXAMPLE_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

#define VAL_SZ 2U

void udb_example_init(void);
void udb_example_deinit(void);
uint8_t udb_example_sort(const char* fname);


#ifdef __cplusplus
 }
#endif

#endif //__UDB_EXAMPLE_H

/************************ (C) COPYRIGHT Emmet for Torock *****END OF FILE****/
