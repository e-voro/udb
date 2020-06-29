/*******************************************************************************
  * @file           : main.cpp
  * @brief          : Main file of UDB example utility
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
		//shsz = RECQTTY;
		for (uint32_t j=0; j<argc; j++)
		{
			if ( !strncmp(argv[j], "-qtty=", 6) )
			{
				if ( sscanf(argv[j]+6,"%5u", &recqtty) > 0 )
				{
					flagRecQtty = 1;
					//shsz=RECQTTY;
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

  return 0;
}
