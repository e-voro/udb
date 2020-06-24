/*******************************************************************************
  * @file           : gendtsh.cpp
  * @brief          : Header for datasheet generator unit
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
#ifndef __GENDTSH_H
#define __GENDTSH_H

#define RECQTTY 20

uint8_t GenerateDataSheet(uint32_t recs_qtty, uint32_t data_sz);

#endif /* __GENDTSH_H */

/************************ (C) COPYRIGHT Emmet for Torock *****END OF FILE****/
