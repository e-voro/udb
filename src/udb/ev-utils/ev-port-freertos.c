/**
 * @file ev-port-freertos.c
 * @project name	: ev-utils
 * @version 		: ev-utils 1.2
 * @brief RTOS abstraction layer (FreeRTOS)
 * @section License
 * SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2010-2019 Oryx Embedded SARL. All rights reserved.
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
 * @author Emmet, LLC
 **/

//Switch to the appropriate trace level
#define TRACE_LEVEL TRACE_LEVEL_OFF

//Dependencies
#include <stdio.h>
#include <stdlib.h>
#include "ev-port.h"
#include "ev-port-freertos.h"

/**
 * @brief Create a mutex object
 * @param[in] mutex Pointer to the mutex object
 * @return The function returns TRUE if the mutex was successfully
 *   created. Otherwise, FALSE is returned
 **/
bool_et evCreateMutex(evMutex *mutex)
{
#if (configSUPPORT_STATIC_ALLOCATION == 1)
   //Create a mutex object
   mutex->handle = xSemaphoreCreateRecursiveMutexStatic(&mutex->buffer);
#else
   //Create a mutex object
   mutex->handle = xSemaphoreCreateRecursiveMutex();
#endif
   //Check whether the returned handle is valid
   if(mutex->handle != NULL)
      return 1;
   else
      return 0;
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
      //Properly dispose the specified mutex
      vSemaphoreDelete(mutex->handle);
   }
}

/**
 * @brief Acquire ownership of the specified mutex object
 * @param[in] mutex Pointer to the mutex object
 * @return 	evOK  aka pdPASS aka pdTRUE from FreeRTOS
 * 			evNOK aka pdFAIL aka pdFALSE from FreeRTOS
 **/
bool_et evTakeMutex(evMutex *mutex, tick_et delay)
{   //Obtain ownership of the mutex object
   return xSemaphoreTakeRecursive(mutex->handle, delay);
}

/**
 * @brief Release ownership of the specified mutex object
 * @param[in] mutex Pointer to the mutex object
 * @return 	evOK  aka pdPASS aka pdTRUE from FreeRTOS
 * 			evNOK aka pdFAIL aka pdFALSE from FreeRTOS
 **/
void evReleaseMutex(evMutex *mutex)
{  //Release ownership of the mutex object
   xSemaphoreGive(mutex->handle);
}

/**
 * @brief Retrieve system time
 * @return Number of milliseconds elapsed since the system was last
 * started OR since RECOUNTED due to systick overflow
 **/
systime_et evGetSystemTime(void)
{
   TickType_t ticks;
   //Get current tick count
   ticks = xTaskGetTickCount();
   //Convert system ticks to milliseconds
   return (systime_et) (ticks * portTICK_PERIOD_MS);
}
