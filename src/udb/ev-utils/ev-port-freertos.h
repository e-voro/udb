/**
 * @file ev-port-freertos.h
 * @project name	: ev-utils
 * @version 		: ev-utils 1.2
 * @brief RTOS abstraction layer (FreeRTOS)
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
 * along with this program
 *
 * @author Evgeny Voropaev
 **/

#ifndef _EV_PORT_FREERTOS_H
#define _EV_PORT_FREERTOS_H

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "ev-port.h"

//Static object allocation
#ifndef configSUPPORT_STATIC_ALLOCATION
   #define configSUPPORT_STATIC_ALLOCATION 0
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif



#define evOK 	pdTRUE
#define evNOK	pdFALSE

typedef TickType_t tick_et;
typedef BaseType_t base_et;
typedef uint32_t systime_et;//systemtime in miliseconds
#define evMAXDELAY portMAX_DELAY

/**
 * @brief Mutex object
 **/

typedef struct
{
   SemaphoreHandle_t handle;
#if (configSUPPORT_STATIC_ALLOCATION == 1)
   StaticSemaphore_t buffer;
#endif
} evMutex;

//Mutex management
bool_et evCreateMutex(evMutex *mutex);
void evDeleteMutex(evMutex *mutex);
bool_et evTakeMutex(evMutex *mutex, tick_et delay);
void evReleaseMutex(evMutex *mutex);

//System time
systime_t evGetSystemTime(void);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
