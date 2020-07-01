/*******************************************************************************
  * @file           : udb_example.h
  * @brief          : Example for Microdatabase udb-2.0
  * @project name	: udb
  * @version 		: udb-2.0
  * @author         : Evgeny Voropaev
  *****************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UDB_EXAMPLE_H
#define __UDB_EXAMPLE_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

#define VAL_SZ 4U

void udb_example_init(void);
void udb_example_deinit(void);
uint8_t udb_example_sort(const char* fname);
uint8_t udb_example_search_group(const char* fname, uint8_t* search_val, uint8_t val_sz );

#ifdef __cplusplus
 }
#endif

#endif //__UDB_EXAMPLE_H

/************************ (C) COPYRIGHT Emmet for Torock *****END OF FILE****/
