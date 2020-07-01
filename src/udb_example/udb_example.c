/*******************************************************************************
  * @file           : udb_example.c
  * @project name	: udb
  * @version 		: udb-2.0
  * @brief          : Example for Microdatabase udb-2.0
  * @author         : Evgeny Voropaev
  *****************************************************************************/

#define __UDB_C

#include "udb.h"
#include "udb_example.h"
#include <stdlib.h>
#include <stdio.h>

#define MAXRECQTTY 2*1024
#define DATA_LE 0
#define DATA_BE 1

Sheet_t* pSh;

uint8_t udb_example_cachedb(const char* fname, Sheet_t* sh);
void PrintSheet(Sheet_t* sh);

void udb_example_init(void)
{
	uint32_t Rec_t_size;
	pSh = 0;
	udb_create(&pSh, MAXRECQTTY, DATA_BE, VAL_SZ, VAL_SZ);
	printf("udo_example_init");
}

void udb_example_deinit(void)
{
	printf("udb_example_deinit");
}

uint8_t udb_example_sort(const char* fname)
{
	if(!fname) return 0;
	udb_example_init();
	udb_example_cachedb(fname, pSh);
	udb_example_deinit();
	return 1;
}

uint8_t udb_example_cachedb(const char* fname, Sheet_t* sh)
{	uint8_t res = 0;
	FILE* f = NULL;
	uint32_t recs_qtty;
	if(!fname) return 0;

	do{	f = fopen(fname, "r");
		if (!f)
		{	printf("ERROR: udb_example_cachedb:"
					" Data file %s opening error!\n", fname);
			res = 0;
			break;
		}
		fscanf(f, "%d", &recs_qtty);
		if (recs_qtty > sh->max_qtty)
		{
			printf("ERROR: udb_example_cachedb: "
					"recs_qtty=%u > tag_sheet.max_qtty=%u",
					(unsigned int)recs_qtty,
						(unsigned int)sh->max_qtty);
			res=0;
			break;
		}
		printf("udb_example_cachedb: recs_qtty=%d!\n", recs_qtty);

		uint8_t* NewRec;
		NewRec = malloc( sh->rec_sz );

		for (uint32_t SS =0; SS < recs_qtty; SS++)
		{
			int scanned;
			uint8_t zpt;
			int sym;
			scanned = fscanf(f, "%c", &zpt);
			if(zpt!=',') { res = 0; break; }

			res = 1;
			uint32_t rec_inx_from_file;
			scanned = fscanf(f, "%x,", &rec_inx_from_file);
			if (!scanned) { res = 0; break; }

			for (uint32_t i=0; i<sh->rec_sz; i++)
			{
			  res = 1;
			  scanned = fscanf(f, "%02X", &sym);
			  NewRec[i] = (uint8_t) sym;
			  if (!scanned) { res = 0; break; }
			}
			if (!res) break;
			insrec(sh,NewRec);
		}
		if (NewRec) free(NewRec);
	  if(!res) break;
	}while(0);

	if(f)
	{	fclose(f);
	}

	if(res)
	{	printf("CacheDB func finish. Result-OK.\n");
		printf( "Cached from file %s.\n",fname);
		PrintSheet(sh);
	}
	else
	{	printf("CacheDB func finish. Result-NOK.\n");
	}

	return res;

}
void PrintSheet(Sheet_t* sh)
{
	printf( "Sheet params: \n"
			"              sh->val_be=%u,\n"
			"              sh->rec_sz=%u,\n"
			"              sh->max_qtty=%u,\n"
			"              sh->qtty=%u,\n"
			"              sh->recs=%lu,\n"
			"              sh->rii=%u,\n"
			"              sh->keys=%lu\n"
			"              sh->antikeys=%lu\n"
			"              sh->rec_exst=%lu\n",
			(unsigned int)sh->val_be,
			(unsigned int)sh->rec_sz,
			(unsigned int)sh->max_qtty,
			(unsigned int)sh->qtty,
			(long unsigned int)sh->recs,
			(unsigned int)sh->rii,
			(long unsigned int)sh->keys,
			(long unsigned int)sh->antikeys,
			(long unsigned int)sh->rec_exst
			);
	printf( "Sheet in fact order:\n"
			"       data_index(from recs) / key_index(from recs)"
			" | data_index(from keys) * data-existed /\\ data \n");
	for (uint32_t i = 0; i< sh->qtty; i++)
	{
		uint8_t* pRR = getrec(sh, i);
		printf(
			"       %u / %u | %u * %u /\\ 0x",
			(unsigned int) i,
			(unsigned int) sh->antikeys[i],
			(unsigned int) sh->keys[ sh->antikeys[i] ],
			(unsigned int) sh->rec_exst[i] );
		for (uint32_t DD = 0; DD < sh->rec_sz; DD++)
			printf("%02X", *(pRR+DD));
		printf("\n");
	}
	printf( "Sheet in key order(sorted order):\n"
			"       key_index(antikey)(from keys) / key_index(antikey)(from recs)"
			" | data_index(from keys) * data-existed /\\ data \n");
	for (uint32_t i = 0; i< sh->qtty; i++)
	{	uint8_t* pRR = getrec(sh, sh->keys[i]);
		printf(
			"       %u / %u | %u * %u /\\ 0x",
			(unsigned int) i,
			(unsigned int) sh->antikeys[sh->keys[i]],
			(unsigned int) sh->keys[i],
			(unsigned int) sh->rec_exst[sh->keys[i]] );
		for (uint32_t DD = 0; DD < sh->rec_sz; DD++)
			printf("%02X", *(pRR+DD) );
		printf("\n");
	}
}

uint8_t udb_example_search_group(const char* fname, uint8_t* search_val, uint8_t val_sz )
{
	if(!fname) return 0;
	udb_example_init();
	udb_example_cachedb(fname, pSh);
	index_t first_keyinx_of_grp, last_keyinx_of_grp;

	if ( sel(pSh,search_val,&first_keyinx_of_grp, &last_keyinx_of_grp) )
	{
		printf("Value 0x");
		for (uint8_t i = 0; i<val_sz; i++)
			printf("%02X", (unsigned int)search_val[i]);
		printf(" is found.");
		printf("first_keyinx_of_grp=%u, last_keyinx_of_grp=%u.\n",
				(unsigned int)first_keyinx_of_grp, (unsigned int)last_keyinx_of_grp);
	}
	else
	{
		printf("Value 0x");
		for (uint8_t i = 0; i<val_sz; i++)
			printf("%02X", (unsigned int)search_val[i]);
		printf(" is not found.\n");
	}

	udb_example_deinit();
	return 1;
}
