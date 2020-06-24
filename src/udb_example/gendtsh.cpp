/*******************************************************************************
  * @file           : gendtsh.cpp
  * @brief          : datasheet generator unit
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

#include <stdio.h>
#include <iostream>
#include "gendtsh.hpp"
#include "udb.h"

//#include <string>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

boost::uuids::uuid uuid;

uint32_t recs_qtty=0;

uint8_t fl_arg_lgsrch = 0;

uint8_t GenerateDataSheet(uint32_t recs_qtty, uint32_t data_sz)
{
	FILE* f = NULL;
	uint8_t res;
	printf("GenerateDataSheet func is started.\n");
	do
	{
		f = fopen("dt.sht", "w");
		if (!f)
		{
		 printf("File openning error!\n");
		 break;
		}

		fprintf( f, "%u", recs_qtty);
		printf("    Recs Quantity=%u", recs_qtty);

		for (uint32_t rec_inx = 0; rec_inx<recs_qtty; rec_inx++)
		{	fprintf( f, ",\n");
			printf(",\n");
			uint32_t data_inx=0;
			uint8_t  uuid_inx=0;
			uuid = boost::uuids::random_generator()();

			fprintf( f, "    %02X,", (uint32_t)rec_inx);
			printf( "    %02X,", (uint32_t)rec_inx);
			for ( ;data_inx<data_sz; data_inx++, uuid_inx++)
			{	if(uuid_inx >= 16)
				{
					uuid = boost::uuids::random_generator()();
					uuid_inx = 0;
				}
				fprintf( f, "%02X", (uint32_t)uuid.data[uuid_inx]);
				printf("%02X", (uint32_t)uuid.data[uuid_inx]);
			}
		}
		fprintf(f,"\n");
		printf("\n");
	}while(0);

	if(f)
	{	fclose(f);
		res = 1;
		printf("GenerateDataSheet func finish. Result-OK.\n");
	}
	else
	{	res = 0;
		printf("GenerateDataSheet func finish. Result-NOK.\n");
	}

	return res;
}
