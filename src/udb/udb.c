/*******************************************************************************
  * @file           : udb.c
  * @brief          : Microdatabase library
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

#define __UDB_C
#include <stdio.h>
#include "udb.h"
#include "os_port.h"

static OsMutex udb_mut;

uint8_t sheet_addkey(Sheet_t* sh, uint16_t rec_inx,
						uint16_t* new_key_inx);

uint8_t sheet_delkey(Sheet_t* sh, uint16_t rec_inx, uint16_t key_inx);

uint8_t sheet_init(Sheet_t* sh, uint16_t max_qtty,
				uint8_t data_sz, uint8_t data_be,
				uint16_t* keys_array, Rec_t* recs_array)
{
	uint8_t rslt;
	osAcquireMutex(&udb_mut);
	do{//body
		if (!sh || !max_qtty || !data_sz || !keys_array || !recs_array)
		{	rslt = 0; break;	}
		sh->qtty = 0;
		sh->max_qtty = max_qtty;
		sh->rii = 0;
		sh->data_sz = data_sz;
		sh->data_be = data_be;
		sh->keys = keys_array;
		sh->recs = recs_array;
		for (uint16_t i=0; i<max_qtty; i++)
			sh->recs[i].Existed = 0;

		rslt=1;
	}while(0);
	osReleaseMutex(&udb_mut);
	return rslt;
}

uint8_t sheet_addrec(Sheet_t* sh, uint8_t data[])
{
	uint8_t rslt=0;
	uint16_t new_key_inx=0;
	osAcquireMutex(&udb_mut);
	do{//body
		if(!sh) {	rslt = 0; break;	}


		if (sh->recs[sh->rii].Existed != 0)
		{
			sheet_delrec(sh, sh->rii);
		}

		for(uint16_t i=0; i < sh->data_sz; i++)
				sh->recs[sh->rii].dt[i] = data[i];

		if (sheet_addkey(sh, sh->rii, &new_key_inx) )
		{	sh->recs[sh->rii].Existed = 1;
			sh->recs[sh->rii].key_inx = new_key_inx;
			sh->qtty++;
			rslt = 1;
			if (sh->rii >= sh->max_qtty - 1)
			{	sh->rii = 0;	}
			else { 	sh->rii++;	}
		}
		else
		{	sh->recs[sh->rii].Existed = 0;
			rslt = 0;
		}
	}while(0);//body
	osReleaseMutex(&udb_mut);
	return rslt;
}

uint8_t sheet_delrec(Sheet_t* sh, uint16_t rec_inx)
{
	sheet_delkey(sh, rec_inx, sh->recs[rec_inx].key_inx);
	sh->recs[rec_inx].Existed = 0;
	if (sh->qtty) sh->qtty--;
	return 1;
}

uint8_t sheet_delkey(Sheet_t* sh, uint16_t rec_inx, uint16_t key_inx)
{
	if(sh->keys[key_inx] != rec_inx) return 0;
	else
		for (uint16_t i = key_inx; i < sh->qtty; i++)
		{
			sh->keys[i] = sh->keys[i+1];
			sh->recs[sh->keys[i]].key_inx = i;
		}

	return 1;
}

uint8_t sheet_addkey(Sheet_t* sh, uint16_t rec_inx,
						uint16_t* new_key_inx)
{
	//uint16_t new_key;
	//найти куда вставить эти данные,
	//т.е. выяснить, больше какого элемента из отсортированного списка
	//эти данные и меньше какого элемента
	int32_t insert_key_index;
	sheet_search_logar(sh,
						&sh->recs[rec_inx],
							&insert_key_index);

	for (uint16_t i = sh->qtty; i > insert_key_index; i--)
	{
		sh->keys[i] = sh->keys[i-1];
		sh->recs[sh->keys[i]].key_inx = i;
	}

	sh->keys[insert_key_index] = rec_inx;

	*new_key_inx = (uint16_t)insert_key_index;
	//if (sh->qtty == 1) new_key =
	return 1;
}

uint8_t sheet_search_logar(Sheet_t* sh, Rec_t* srch,
							int32_t* ins_key_inx)
{
   int32_t first, last, middle, n;

   n = sh->qtty;
   if (n<1)
   {
	   *ins_key_inx = 0;
	   return 0;
   }

   first = 0;
   last = n - 1;
   middle = (first+last)/2;
   int8_t cmp_res;
   while ( first<=last)
   {
	   cmp_res =
		  cmp_arr( sh->recs[sh->keys[middle]].dt,
				  	  srch->dt,
					  	  sh->data_sz);
	   //if (array[middle] < search)
	   if ( cmp_res < 0 )
	   {
         first = middle + 1;
         *ins_key_inx = middle + 1;
	   }
	   //else if (array[middle] == search)
	   else if (cmp_res == 0)
	   {
         printf("search is present at index %d.\n", middle);
         *ins_key_inx = middle;
         break;
	   }
	   else
	   { //if (array[middle] > search)
         last = middle - 1;
         *ins_key_inx = middle;
	   }

	   middle = (first + last)/2;
   }

   printf("ins_key_inx=%d", *ins_key_inx);

   if (first > last)
   {  printf("Not found! SEARCH is not present in the list.\n");
      return 0;
   }
   else return 1;
}

int8_t cmp_arr(uint8_t A[], uint8_t B[], uint16_t sz)
{	uint16_t i = 0;
	for (; i<sz; i++)
	{	if (A[i]==B[i]) continue;
		else break;
	}

	if (i == sz) return 0;//A==B
	if (A[i] < B[i]) return -1; //A<B
	else return	1;//A>B
}

