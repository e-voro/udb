/*******************************************************************************
  * @file           : udb.c
  * @project name	: udb
  * @version 		: udb-2.0
  * @brief          : Microdatabase library
  * @author         : Evgeny Voropaev, evoro@emmet.pro
  * @creation date  : 24.06.2020
  * @original proj. : torock.pro
  * @date			: 01.07.2020
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

//short function names
#define addkey	udb_addkey
#define delkey	udb_delkey

uint8_t udb_addkey(Sheet_t* sh,
						index_t rec_inx,
							index_t* new_key_inx);

uint8_t udb_delkey(Sheet_t* sh, index_t key_inx);

#ifdef UDB_DINAMICALLY_ALLOCATION_ENABLE

uint8_t udb_create( Sheet_t** ppsh,
						uint16_t max_qtty,
							uint8_t val_be,
								uint8_t val_sz,
									uint8_t rec_sz)
{	uint8_t res;
	index_t* keys;
	index_t* antikeys;
	uint8_t* rec_exst;
	rec_t* recs;

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
	{	evTakeMutex( &(**ppsh).mu, evMAXDELAY);
		if ( (**ppsh).keys) 	free( (**ppsh).keys );
		if ( (**ppsh).antikeys) free( (**ppsh).antikeys );
		if ( (**ppsh).recs) 	free( (**ppsh).recs );
		if ( (**ppsh).rec_exst)  free( (**ppsh).rec_exst );
		evReleaseMutex(&(**ppsh).mu);

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
									index_t* keys,
										index_t* antikeys,
											uint8_t* rec_exst,
												rec_t* recs)
{	uint8_t res=0;
	do
	{ if (!sh) { res=0; break; }
	  memset( sh, 0, sizeof(Sheet_t) );
	  if ( !evCreateMutex(&sh->mu) ) {res = 0; break;}
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
	evDeleteMutex(&sh->mu);
}

uint8_t udb_insert_record(Sheet_t* sh, rec_t* newrec)
{
	uint8_t rslt=0;
	uint16_t new_key_inx=0;
	if(!sh) {	return 0;	}
	evTakeMutex(&sh->mu, evMAXDELAY);

	do{//body
		if( sh->rec_exst[sh->rii] !=0 )
		{
			delrec(sh, sh->rii);
		}

		uint8_t* pRR = getrec(sh, sh->rii);
		for(uint16_t i=0; i < sh->rec_sz; i++)
			*( pRR + i) = newrec[i];
		if (addkey(sh, sh->rii, &new_key_inx) )
		{	sh->rec_exst[sh->rii] = 1;
			sh->antikeys[sh->rii] = new_key_inx;
			sh->qtty++;
			rslt = 1;
			if (sh->rii >= sh->max_qtty - 1)
			{	sh->rii = 0;	}
			else { 	sh->rii++;	}
		}
		else
		{	sh->rec_exst[sh->rii] = 0;
			rslt = 0;
		}
	}while(0);//body
	evReleaseMutex(&sh->mu);
	return rslt;
}

uint8_t udb_delete_record(Sheet_t* sh, index_t rec_inx)
{
	delkey(sh, sh->antikeys[rec_inx]);
	sh->rec_exst[rec_inx] = 0;
	if (sh->qtty) sh->qtty--;
	return 1;
}

rec_t* udb_get_record(Sheet_t* sh, index_t rec_inx)
{
	if (!sh) return 0;
	return (rec_t*) ( (unsigned int)sh->recs +
						(unsigned int)rec_inx * (unsigned int)sh->rec_sz) ;
}

uint8_t udb_delkey(Sheet_t* sh, index_t key_inx)
{	for (uint16_t i = key_inx; i < sh->qtty; i++)
	{	sh->keys[i] = sh->keys[i+1];
		sh->antikeys[sh->keys[i]] = i;
	}
	return 1;
}

uint8_t udb_addkey(Sheet_t* sh,
						index_t rec_inx,
							index_t* new_key_inx)
{
	windex_t insert_key_index;
	search(sh, getrec(sh, rec_inx), &insert_key_index);

	for (uint16_t i = sh->qtty; i > insert_key_index; i--)
	{
		sh->keys[i] = sh->keys[i-1];
		sh->antikeys[sh->keys[i]] = i;
	}

	if (insert_key_index < sh->max_qtty-1)
	{
		sh->keys[insert_key_index] = rec_inx;
		*new_key_inx = (index_t)insert_key_index;
		return 1;
	}
	else //insert_key_index >= sh->max_qtty-1
	{
		return 0;
	}
}

bool_et udb_search_logar(Sheet_t* sh,
							uint8_t* srch_val,
								windex_t* found_key_inx)
{
   windex_t first, last, middle, n;

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

bool_et udb_select(Sheet_t* sh,
						uint8_t* srch_val,
							index_t* first_key_inx,
								index_t* last_key_inx)
{
	bool_et res;
	windex_t found_key_inx;
	int8_t cmp_res;
	index_t FF, LL;
	windex_t tmp;
	evTakeMutex(&sh->mu, evMAXDELAY);
	do {
		if ( search(sh, srch_val, &found_key_inx) )
		{
			if ( found_key_inx >= sh->max_qtty  || found_key_inx < 0)
			{	res = 0;  break;
			}
			//Search for group beginning
			FF = found_key_inx;
			cmp_res = 0;//found value equal to srch_val
			tmp = FF;
			while( !cmp_res && tmp>=0  )
			{ 	cmp_res = cmp_arr( getrec(sh, sh->keys[tmp]), srch_val, sh->val_sz );
				if (!cmp_res)
					FF = (index_t)tmp;
				tmp--;
			}
			*first_key_inx = FF;

			//Search for group ending
			cmp_res = 0;
			LL = found_key_inx;
			tmp = LL;
			while( !cmp_res && tmp<sh->qtty  )
			{ 	cmp_res = cmp_arr( getrec(sh, sh->keys[tmp]), srch_val, sh->val_sz );
				if (!cmp_res)
					LL = (index_t)tmp;
				tmp++;
			}
			*last_key_inx = LL;
			res = 1;
		}
		else
		{
			res = 0;
		}
	}while(0);
	evReleaseMutex(&sh->mu);

	return res;
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

