#define __UDB_C

#include "udb.h"
#include "udb_example.h"
#include <stdlib.h>
#include <stdio.h>

#define MAXRECQTTY 2*1024
#define DATA_LE 0
#define DATA_BE 1

Sheet_t tag_sheet;
uint16_t K[MAXRECQTTY];
Rec_t* R;

uint8_t udb_example_cachedb(const char* fname, Sheet_t* sh);
void PrintSheet(Sheet_t* sh);

void udb_example_init(void)
{
	uint32_t Rec_t_size;
	R = malloc( sizeof(Rec_t) * MAXRECQTTY);
	sheet_init(&tag_sheet, MAXRECQTTY, DATA_SZ, DATA_BE, K, R);
	printf("udo_example_init");
}

void udb_example_deinit(void)
{
	if(R) free(R);
	printf("udo_example_deinit");
}

uint8_t udb_example_sort(const char* fname)
{
	if(!fname) return 0;
	udb_example_init();
	udb_example_cachedb(fname, &tag_sheet);
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
		if (recs_qtty > tag_sheet.max_qtty)
		{
			printf("ERROR: udb_example_cachedb: "
					"recs_qtty=%u > tag_sheet.max_qtty=%u",
					(unsigned int)recs_qtty,
						(unsigned int)tag_sheet.max_qtty);
			res=0;
			break;
		}
		printf("udb_example_cachedb: recs_qtty=%d!\n", recs_qtty);

		Rec_t* newrec;
		newrec = malloc(sizeof(Rec_t) + DATA_SZ);

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
			//if (!scanned) { res = 0; break; }
			//sh->keys[key_inx] = rec_inx;


			for (uint32_t i=0; i<sh->data_sz; i++)
			{
			  res = 1;
			  scanned = fscanf(f, "%02X", &sym);
			  newrec->dt[i] = (uint8_t) sym;
			  if (!scanned) { res = 0; break; }
			}
			if (!res) break;
			sheet_addrec(sh,newrec->dt);
		}
		if (newrec) free(newrec);
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
			"              sh->data_be=%u,\n"
			"              sh->data_sz=%u,\n"
			"              sh->max_qtty=%u,\n"
			"              sh->qtty=%u,\n"
			"              sh->recs=%lu,\n"
			"              sh->rii=%u,\n"
			"              sh->keys=%lu\n",
			(unsigned int)sh->data_be,
			(unsigned int)sh->data_sz,
			(unsigned int)sh->max_qtty,
			(unsigned int)sh->qtty,
			(long unsigned int)sh->recs,
			(unsigned int)sh->rii,
			(long unsigned int)sh->keys
			);
	printf( "Sheet in fact order:\n"
			"       data_index(from recs) / key_index(from recs)"
			" | data_index(from keys) * data-existed /\\ data \n");
	for (uint32_t i = 0; i< sh->qtty; i++)
	{
		printf(
			"       %u / %u | %u * %u /\\ 0x",
			(unsigned int) i,
			(unsigned int) sh->recs[i].key_inx,
			(unsigned int) sh->keys[ sh->recs[i].key_inx ],
			(unsigned int) sh->recs[i].Existed );
		for (uint32_t DD = 0; DD < DATA_SZ; DD++)
			printf("%02X", sh->recs[i].dt[DD]);
		printf("\n");
	}
	printf( "Sheet in key order(sorted order):\n"
			"       key_index(from keys) / key_index(from recs)"
			" | data_index(from keys) * data-existed /\\ data \n");
	for (uint32_t i = 0; i< sh->qtty; i++)
	{
		printf(
			"       %u / %u | %u * %u /\\ 0x",
			(unsigned int) i,
			(unsigned int) sh->recs[sh->keys[i]].key_inx,
			(unsigned int) sh->keys[i],
			(unsigned int) sh->recs[i].Existed );
		for (uint32_t DD = 0; DD < DATA_SZ; DD++)
			printf("%02X", sh->recs[ sh->keys[i] ].dt[DD]);
		printf("\n");
	}

}
