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
#include "udb_conf.h"
#include <stdlib.h>
#include <string.h>

uint8_t sheet_addkey(Sheet_t* sh, uint16_t rec_inx,
						uint16_t* new_key_inx);

uint8_t sheet_delkey(Sheet_t* sh, uint16_t key_inx);

#ifdef UDB_DINAMICALLY_ALLOCATION_ENABLE
uint8_t udb_create( Sheet_t** ppsh,
					uint16_t max_qtty,
						uint8_t val_be,
							uint8_t val_sz,
								uint8_t rec_sz)
{	uint8_t res;
	uint16_t* keys;
	uint16_t* antikeys;
	uint8_t* rec_exst;
	uint8_t* recs;

	do
	{
	  *ppsh = malloc( sizeof(Sheet_t) );
	  if (!*ppsh) { res=0; break; }
	  keys 	   = malloc( sizeof( (**ppsh).keys)	   * max_qtty);
	  antikeys = malloc( sizeof( (**ppsh).antikeys ) * max_qtty);
	  rec_exst = malloc( sizeof( (**ppsh).rec_exst)   * max_qtty);
	  recs	   = malloc( sizeof( (**ppsh).recs) * rec_sz * max_qtty);
	  if( 	!keys 		||	!antikeys 	||
	  		!rec_exst	||	!recs
	  	)
	  {	res=0; break; }

	  udb_init(*ppsh, max_qtty, val_be, val_sz,	rec_sz,
	  					keys, antikeys, rec_exst, recs);
	  res = 1;
	}while(0);

	if (!res) { udb_destroy(ppsh); }
	return res;
}

void udb_destroy( Sheet_t** ppsh)
{
	if ( *ppsh )
	{	osAcquireMutex( &(**ppsh).mu);
		if ( (**ppsh).keys) 	free( (**ppsh).keys );
		if ( (**ppsh).antikeys) free( (**ppsh).antikeys );
		if ( (**ppsh).recs) 	free( (**ppsh).recs );
		if ( (**ppsh).rec_exst)  free( (**ppsh).rec_exst );
		osReleaseMutex(&(**ppsh).mu);

		udb_deinit(*ppsh);
		free(*ppsh);
		*ppsh = 0;
	}
}

#endif

uint8_t udb_init(Sheet_t* sh,
					uint16_t max_qtty,
						uint8_t val_be,
							uint8_t val_sz,
								uint8_t rec_sz,
									uint16_t* keys,
										uint16_t* antikeys,
											uint8_t* rec_exst,
												uint8_t* recs)
{	uint8_t res=0;
	do
	{ if (!sh) { res=0; break; }
	  memset( sh, 0, sizeof(Sheet_t) );
	  if ( !osCreateMutex(&sh->mu) ) {res = 0; break;}
	  sh->qtty = 0;
	  sh->max_qtty = max_qtty;
	  sh->rii = 0;
	  sh->val_be = val_be;
	  sh->val_sz = val_sz;
	  sh->rec_sz = rec_sz;
	  sh->keys = keys;
	  sh->antikeys = antikeys;
	  sh->rec_exst= rec_exst;
	  sh->recs = recs;
	  if( 	!sh->keys 		||
	  		!sh->antikeys 	||
	  		!sh->rec_exst	||
	  		!sh->recs
	  	)
	  	{	res=0; break; }
	  memset( sh->keys, 0, max_qtty * sizeof( sh->keys));
	  memset( sh->antikeys, 0, max_qtty * sizeof( sh->antikeys));
	  memset( sh->rec_exst, 0, max_qtty * sizeof( sh->rec_exst));
	  memset( sh->recs,0, sizeof( sh->rec_exst) * rec_sz * max_qtty );
	  res = 1;
	}while(0);
	if (!res) { udb_deinit(sh); }
	return res;
}

void udb_deinit(Sheet_t* sh)
{
	if(!sh) return;
	osDeleteMutex(&sh->mu);
}

uint8_t sheet_addrec(Sheet_t* sh, uint8_t* newrec)
{
	uint8_t rslt=0;
	uint16_t new_key_inx=0;
	if(!sh) {	return 0;	}
	osAcquireMutex(&sh->mu);
	do{//body
		if( sh->rec_exst[sh->rii] !=0 )
		{
			sheet_delrec(sh, sh->rii);
		}

		uint8_t* pRR = getrec(sh, sh->rii);
		for(uint16_t i=0; i < sh->rec_sz; i++)
			*( pRR + i) = newrec[i];
		if (sheet_addkey(sh, sh->rii, &new_key_inx) )
		{	sh->rec_exst[sh->rii] = 1;//sh->recs[sh->rii].Existed = 1;
			sh->antikeys[sh->rii] = new_key_inx;//sh->recs[sh->rii].key_inx = new_key_inx;
			sh->qtty++;
			rslt = 1;
			if (sh->rii >= sh->max_qtty - 1)
			{	sh->rii = 0;	}
			else { 	sh->rii++;	}
		}
		else
		{	sh->rec_exst[sh->rii] = 0;//sh->recs[sh->rii].Existed = 0;
			rslt = 0;
		}
	}while(0);//body
	osReleaseMutex(&sh->mu);
	return rslt;
}

uint8_t sheet_delrec(Sheet_t* sh, uint16_t rec_inx)
{
	sheet_delkey(sh, sh->antikeys[rec_inx]);
	sh->rec_exst[rec_inx] = 0;//sh->recs[rec_inx].Existed = 0;
	if (sh->qtty) sh->qtty--;
	return 1;
}

uint8_t* getrec(Sheet_t* sh, uint16_t rec_inx)
{
	if (!sh) return 0;
	return (uint8_t*) ( (unsigned int)sh->recs +
						(unsigned int)rec_inx * (unsigned int)sh->rec_sz) ;
}

uint8_t sheet_delkey(Sheet_t* sh, uint16_t key_inx)
{	for (uint16_t i = key_inx; i < sh->qtty; i++)
	{	sh->keys[i] = sh->keys[i+1];
		sh->antikeys[sh->keys[i]] = i;//sh->recs[sh->keys[i]].key_inx = i;
	}
	return 1;
}

uint8_t sheet_addkey(Sheet_t* sh, uint16_t rec_inx,
						uint16_t* new_key_inx)
{
	int32_t insert_key_index;
	sheet_search_logar(sh, getrec(sh, rec_inx), &insert_key_index);

	for (uint16_t i = sh->qtty; i > insert_key_index; i--)
	{
		sh->keys[i] = sh->keys[i-1];
		sh->antikeys[sh->keys[i]] = i;
	}

	sh->keys[insert_key_index] = rec_inx;

	*new_key_inx = (uint16_t)insert_key_index;
	return 1;
}

uint8_t sheet_search_logar(Sheet_t* sh, uint8_t* srch_val,
							int32_t* found_key_inx)
{
   int32_t first, last, middle, n;

   n = sh->qtty;
   if (n<1)
   {
	   *found_key_inx = 0;
	   return 0;
   }

   first = 0;
   last = n - 1;
   middle = (first+last)/2;
   int8_t cmp_res;
   while ( first<=last)
   {
	   cmp_res =
		cmp_arr( getrec(sh, sh->keys[middle]), srch_val, sh->val_sz);

	   //if (array[middle] < search)
	   if ( cmp_res < 0 )
	   {
         first = middle + 1;
         *found_key_inx = middle + 1;
	   }
	   //else if (array[middle] == search)
	   else if (cmp_res == 0)
	   {
         printf("search is present at index %d.\n", middle);
         *found_key_inx = middle;
         break;
	   }
	   else
	   { //if (array[middle] > search)
         last = middle - 1;
         *found_key_inx = middle;
	   }

	   middle = (first + last)/2;
   }

   printf("founded_key_inx=%d\n", *found_key_inx);

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

