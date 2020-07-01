/**
 * @file ev-port-windows.h
 * @project name	: ev-utils
 * @version 		: ev-utils 1.2
 * @brief RTOS abstraction layer (Windows)
 * @section License
 * SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) Emmet, LLC. All rights reserved.
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
 * @author Evgeny Voropaev
 **/

#ifndef _EV_PORT_WINDOWS_H
#define _EV_PORT_WINDOWS_H

#include <windows.h>

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

#define evOK 	TRUE
#define evNOK	FALSE

#define evTRUE 	TRUE
#define evFALSE FALSE

typedef DWORD base_et;//BaseType of system functions results
typedef DWORD systime_et;//systemtime in miliseconds
typedef DWORD tick_et;//
#define evMAXDELAY INFINITE

/**
 * @brief Mutex object
 **/
typedef struct
{
   void *handle;
} evMutex;

//Mutex management
bool_et evCreateMutex(evMutex *mutex);
void evDeleteMutex(evMutex *mutex);
bool_et evTakeMutex(evMutex *mutex, tick_et delay);
void evReleaseMutex(evMutex *mutex);

//System time
systime_et evGetSystemTime(void);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
