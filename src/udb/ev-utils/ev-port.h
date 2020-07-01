/*******************************************************************************
  * @file           : ev-port.h
  * @project name	: ev-utils
  * @version 		: ev-utils 1.2
  * @brief          : Macros and functions for OS port
  * @author         : Evgeny Voropaev, evoro@emmet.pro
  * @creation date  : 29.06.2020
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
#ifndef __EV_PORT_H
#define __EV_PORT_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "ev-port-config.h"

//Types
typedef char char_et;
typedef signed int int_et;
typedef unsigned int uint_et;
typedef int bool_et;

#if defined(_WIN32)//Windows port?
	#include "ev-port-windows.h"
#elif defined(USE_FREERTOS)//FreeRTOS port?
    #include "ev-port-freertos.h"
#endif

#ifdef __cplusplus
 }
#endif

#endif /*__EV_PORT_H */

/************************ (C) COPYRIGHT Emmet LLC for Torock Project *****END OF FILE****/
