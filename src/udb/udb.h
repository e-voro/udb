/*******************************************************************************
  * @file           : udb.h
  * @project name	: udb
  * @version 		: udb-2.3
  * @brief          : Header for Microdatabase library
  * @author         : Evgeny Voropaev, evoro@emmet.pro
  * @edition date  	: 02.07.2020
  * @creation date  : 24.06.2020
  * @original proj. : torock.pro
  * @section License
  * SPDX-License-Identifier: GPL-2.0-or-later
  * Copyright (C) 2020 Emmet, LLC. All rights reserved.
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License
  * as published by the Free Software Foundation; either version 2
  * of the License, or (at your option) any later version.
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  * You should have received a copy of the GNU General Public License
  * along with this program.
  *****************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UDB_H
#define __UDB_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include "ev-port.h"
#include "udb_conf.h"
/*
 * keys - Array of keys - an array of sequence numbers of records
 * which saved into the table. Keys sorted in ascending order of
 * values of data saved into the table.
 *
 * Antikeys - array of antikeys. Sequence number of member of
 * Antikeys-array is equal to sequence number of data record inside
 *  table. Value of Antikey - is sequence number of key inside
 *  Keys-array linked with data record.
 *
 * Existed - array of flags about data record existence status. Deleted and
 * inexisted records are marked by Existed=0.  Sequence number of member of
 * EXISTED array is equal to sequence number of data record of the table.
 * That means, Existed[i] is meshed and matched to rec[i].
 *
 * VALUE - is a field of the record which used for data comparison during records sorting
 * value is always first bytes of the record. value have size val_sz
 *
 * RECORD - data record inside table. First bytes of RECORD is VALUE.
 * Record have size rec_sz.
 */

typedef uint16_t index_t;
typedef int32_t  windex_t;	//Widened index for exclude possibly index_t overflow
							//during service operations
typedef uint8_t  rec_t;

typedef struct
{
	evMutex mu;
	uint16_t qtty;
	uint16_t max_qtty;
	index_t rii; //next new record insert index;
	uint8_t val_be;//if data format is little-endian data_be =0
	uint8_t val_sz;
	uint8_t rec_sz;
	index_t* keys;
	index_t* antikeys;
	uint8_t* rec_exst;
	rec_t* recs;
}Sheet_t;

#ifdef UDB_DINAMICALLY_ALLOCATION_ENABLE
uint8_t udb_create( Sheet_t** ppsh,
						uint16_t max_qtty,
							uint8_t val_be,
								uint8_t val_sz,
									uint8_t rec_sz);
void udb_destroy( Sheet_t** ppsh);
#endif

uint8_t udb_init(Sheet_t* sh,
					uint16_t max_qtty,
						uint8_t val_be,
							uint8_t val_sz,
								uint8_t rec_sz,
									index_t* keys,
										index_t* antikeys,
											uint8_t* rec_exst,
												rec_t* recs);

void 	udb_deinit(Sheet_t* sh);

int8_t 	cmp_arr(uint8_t A[], uint8_t B[], uint16_t sz);

bool_et udb_search_logar(Sheet_t* sh,
							uint8_t* srch_val,
								windex_t* found_key_inx);

bool_et udb_select(Sheet_t* sh,
						uint8_t* srch_val,
							index_t* first_key_inx,
								index_t* last_key_inx);

uint8_t udb_insert_record(Sheet_t* sh, rec_t* newrec);
uint8_t udb_delete_record(Sheet_t* sh, index_t rec_inx);
rec_t* 	udb_get_record(Sheet_t* sh, index_t rec_inx);

//Short function names
#define getrec(pSheet, Record_index)	udb_get_record(pSheet, Record_index)
#define insrec(pSheet, pNewRecord)		udb_insert_record(pSheet, pNewRecord)
#define delrec(pSheet, Record_index) 	udb_delete_record(pSheet, Record_index)
#define search	udb_search_logar
#define sel( pSheet, pSearch_value, pFirst_key_inx, pLast_key_inx ) 	udb_select( pSheet, pSearch_value, pFirst_key_inx, pLast_key_inx )

//Endian type of values
#define DATA_LE 0
#define DATA_BE 1

#ifdef __cplusplus
 }
#endif

#endif /* __UDB_H */

/************************ (C) COPYRIGHT Emmet LLC for Torock Project *****END OF FILE****/
