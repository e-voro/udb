/*******************************************************************************
  * @file           : main.cpp
  * @project name	: udb
  * @version 		: udb-2.0
  * @brief          : Main file of example utility for udb-2.0
  * @author         : Evgeny Voropaev, evoro@emmet.pro
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

#include <stdio.h>
#include <iostream>
#include <string.h>
#include "gendtsh.hpp"
#include "udb_example.h"
#include "udb.h"

int main(int argc, char **argv)
{
  printf("Microdatabase UDB example Utility is started.\n");
  if (argc<=1)
  {
	  printf("There is no arguments in command line!\n");
	  return 1;
  }
  //Checking "-gendtsh"-argument presence
  for (uint32_t i=0; i<argc; i++)
  {
	if ( !strncmp(argv[i], "-gendtsh", 8) )
	{
		unsigned int recqtty;
		uint8_t flagRecQtty=0;
		for (uint32_t j=0; j<argc; j++)
		{
			if ( !strncmp(argv[j], "-qtty=", 6) )
			{
				if ( sscanf(argv[j]+6,"%5u", &recqtty) > 0 )
				{
					flagRecQtty = 1;
				}
				break;
			}
		}

		if(!flagRecQtty) printf("-qtty is unknown");
		else
		{	printf("record quantity = %u", recqtty);
			GenerateDataSheet(recqtty, VAL_SZ);
		}
	}
  }

  for (uint32_t i=0; i<argc; i++)
  {
	if ( !strncmp(argv[i], "-sortdtsh", 9) ) udb_example_sort("dt.sht");
  }
//***************************************************************************
//	Find Group of Records with same Value
//***************************************************************************
#define SELSTRSZ 5
  for (uint32_t i=0; i<argc; i++)//Search of -findgrp=
  {
	if ( !strncmp(argv[i], "-sel=", SELSTRSZ) )
	{	uint8_t flValSize=0;
		uint32_t val_sz;
		for (uint32_t J=0; J<argc; J++)//Search of valsz
		{	if ( !strncmp(argv[J], "-valsz=", 7) )
			{	if (sscanf( argv[J]+7, "%02u", &val_sz) )
				{	flValSize = 1;
					break;//Search of valsz
				}
			}
		}
		if (!flValSize)
		{	printf("ValSize is not found.\n");
			break;//Search of -findgrp=
		}

#define MAXVALSZ 256

		if (val_sz > MAXVALSZ || val_sz == 0)
		{	printf("ValSize value(%u) is out of boundaries.\n", val_sz);
			break;//Search of -findgrp=
		}
		//Get value length
		size_t ValStrLen = strlen(argv[i]+SELSTRSZ);
		if (ValStrLen != val_sz*2 )
		{
			printf("Size of Value into -findgrp=VALUE is not match to -valsz=VALUESIZE."
					" Maybe you forget leading zero.\n");
			break;//Search of -findgrp=
		}

		uint8_t* srchval;
		srchval = (uint8_t*)malloc(val_sz);
		unsigned int HH;
		for (uint32_t XX = 0; XX<val_sz; XX++)
		{	//			argv[i], "-findgrp=", 9)
			sscanf(argv[i]+SELSTRSZ+2*XX,"%2X", &HH);
			srchval[XX] = (uint8_t)HH;
		}

		udb_example_search_group("dt.sht",srchval, val_sz);
		free(srchval);
	}
  }
  return 0;
}
