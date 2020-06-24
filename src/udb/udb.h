/*******************************************************************************
  * @file           : udb.h
  * @brief          : Header for Microdatabase library
  * @author         : Evgeny Voropaev, evoro@emmet.pro
  * @creation date  : 24.06.2020
  * @original proj. : torock.pro
  * @version 	    :1.0
  * @section License
  *
  * SPDX-License-Identifier: GPL-2.0-or-later
  *
  * Copyright (C) 2020 Emmet, LLC. All rights reserved.
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License
  * as published by the Free Software Foundation; either version 2
  * of the License, or (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.
  *
  *****************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UDB_H
#define __UDB_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

#define DATA_SZ 24

typedef struct
{	uint8_t Existed;
	uint16_t key_inx;
 	uint8_t dt[DATA_SZ];
} Rec_t;


typedef struct
{
	uint16_t qtty;
	uint16_t max_qtty;
	uint16_t rii; //record insert index;
	uint8_t data_sz;
	uint8_t data_be;//if data format is little-endian data_be =0
	uint16_t* keys;
	Rec_t* recs;
}Sheet_t;

uint8_t sheet_init(Sheet_t* sh, uint16_t max_qtty,
				uint8_t data_sz, uint8_t data_be,
				uint16_t* keys_array, Rec_t* recs_array);

uint8_t sheet_addrec(Sheet_t* sh, uint8_t data[]);

int8_t cmp_arr(uint8_t A[], uint8_t B[], uint16_t sz);

uint8_t sheet_search_logar(Sheet_t* sh, Rec_t* srch,
							int32_t* ins_key_inx);

uint8_t sheet_addrec(Sheet_t* sh, uint8_t data[]);
uint8_t sheet_delrec(Sheet_t* sh, uint16_t rec_inx);

#ifdef __cplusplus
 }
#endif

#endif /* __UDB_H */

/************************ (C) COPYRIGHT Emmet LLC for Torock Project *****END OF FILE****/
