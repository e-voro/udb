/**
 * @file 		: ev-port-windows.c
 * @project name: ev-utils
 * @version 	: ev-utils 1.2
 * @brief RTOS abstraction layer (Windows)
 * @section License
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * Copyright (C) Emmet, LLC
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
 * @author Evgeny Voropaev

 **/

//Switch to the appropriate trace level
#define TRACE_LEVEL TRACE_LEVEL_OFF

//Memory leaks detection
#if (defined(_WIN32) && defined(_DEBUG))
   #define _CRTDBG_MAP_ALLOC
   #include <stdlib.h>
   #include <crtdbg.h>
#endif

//Dependencies
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "ev-port.h"
#include "ev-port-windows.h"

bool_et evCreateMutex(evMutex *mutex)
{
   //Create a mutex object
   mutex->handle = CreateMutex(NULL, FALSE, NULL);

   //Check whether the returned handle is valid
   if(mutex->handle != NULL)
      return TRUE;
   else
      return FALSE;
}


/**
 * @brief Delete a mutex object
 * @param[in] mutex Pointer to the mutex object
 **/

void evDeleteMutex(evMutex *mutex)
{
   //Make sure the handle is valid
   if(mutex->handle != NULL)
   {
      //Properly dispose the mutex object
      CloseHandle(mutex->handle);
   }
}


/**
 * @brief Acquire ownership of the specified mutex object
 * @param[in] mutex A handle to the mutex object
 **/

bool_et evTakeMutex(evMutex *mutex, tick_et delay)
{
   DWORD res;
   //Obtain ownership of the mutex object
   res  = WaitForSingleObject(mutex->handle, delay);
   if (res == WAIT_OBJECT_0) return evOK;
   else return evNOK;
}


/**
 * @brief Release ownership of the specified mutex object
 * @param[in] mutex Pointer to the mutex object
 **/

void evReleaseMutex(evMutex *mutex)
{
   //Release ownership of the mutex object
   ReleaseMutex(mutex->handle);
}


/**
 * @brief Retrieve system time
 * @return Number of milliseconds elapsed since the system was last started
 **/

systime_et evGetSystemTime(void)
{
   //Get current tick count
   return GetTickCount();
}
