/*******************************************************************************
  * @file           : gendtsh.cpp
  * @brief          : Header for datasheet generator unit
  * @project name	: udb
  * @version 		: udb-2.0
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GENDTSH_HPP
#define __GENDTSH_HPP

//#define RECQTTY 20

uint8_t GenerateDataSheet(uint32_t recs_qtty, uint32_t data_sz);

#endif /* __GENDTSH_HPP */

/************************ (C) COPYRIGHT Emmet for Torock *****END OF FILE****/
