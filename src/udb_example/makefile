#/*******************************************************************************
#  * @file           : udb_example/Makefile
#  * @project name	: udb
#  * @version 		: udb-2.0
#  * @brief          : Makefile for Microdatabase library example
#  * @author         : Evgeny Voropaev, evoro@emmet.pro
#  * @creation date  : 24.06.2020
#  * @original proj. : torock.pro
#  * @section License
#  * SPDX-License-Identifier: GPL-2.0-or-later
#  * Copyright (C) 2020 Emmet, LLC. All rights reserved.
#  * This program is free software; you can redistribute it and/or
#  * modify it under the terms of the GNU General Public License
#  * as published by the Free Software Foundation; either version 2
#  * of the License, or (at your option) any later version.
#  * This program is distributed in the hope that it will be useful,
#  * but WITHOUT ANY WARRANTY; without even the implied warranty of
#  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  * GNU General Public License for more details.
#  * You should have received a copy of the GNU General Public License
#  * along with this program.
#  *****************************************************************************/

CC = gcc
CXX = g++

CFLAGS += -g

#printf-func-family format checking
CFLAGS += -Wformat 
#dependencies from headers building
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

#######################################
# GNU utils
#######################################
GNU := c:/dev/GnuWin32/bin
MKDIR := $(GNU)/mkdir

#######################################
# paths
#######################################
drOut = ../../.exe
# Build path
drBld = ../../.bld
drSrc = ../../src

drBoost = C:/dev/boost_1_73_0
drIncCpp = C:/dev/MinGW/lib/gcc/mingw32/8.2.0/include/c++ 

######################################
# Sources And Includes
######################################
SrcCxx +=
SrcC   += 
SrcA   +=
IncCxx += -I$(drBoost) -I$(drIncCpp)
IncC   +=

#Embedded microdatabase example
SrcCxx += $(drSrc)/udb_example/main.cpp
SrcCxx += $(drSrc)/udb_example/gendtsh.cpp
SrcC +=   $(drSrc)/udb_example/udb_example.c

IncCxx += -I$(drSrc)/udb_example
IncC +=   -I$(drSrc)/udb_example

#Embedded microdatabase
SrcC += $(drSrc)/udb/udb.c 
IncC += -I$(drSrc)/udb

#port_utils
SrcC +=   $(drSrc)/udb/ev-utils/ev-port-windows.c
IncC += -I$(drSrc)/udb/ev-utils

#######################################
# build the application
#######################################
OBJS  = $(SrcC:$(drSrc)/%.c=$(drBld)/%.o)
OBJS += $(SrcCxx:$(drSrc)/%.cpp=$(drBld)/%.o)
OBJS += $(SrcA:$(srSrc)/%.s=$(drBld)/%.o)

all: udbexm

udbexm: $(OBJS)
	@echo ********Target:$@  ********
	$(MKDIR) -p $(drOut)
	$(CXX) -o $(drOut)/$@ $^

$(drBld)/%.o: $(drSrc)/%.cpp
	@echo \*\*\*\*Target:$@\*\*\*\*
	@echo CFLAGS: $(CFLAGS)
	@echo CXXFLAGS: $(CXXFLAGS)
	@echo CPPFLAGS: $(CPPFLAGS)
	@echo CXX= $(CXX)
	@echo CC= $(CC)	
	$(MKDIR) -p $(dir $@)
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(IncCxx) $(IncC) -o $@ $<

#%.o:	$(PROJECT_ROOT)%.c
$(drBld)/%.o: $(drSrc)/%.c
	$(MKDIR) -p $(dir $@)
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(IncC) -o $@ $<

echotest:
	@echo !!!echotest!!!!!
	@echo $(OBJS)
clean:
	rm -fR $(drOut)
	rm -fR $(drBld)

#######################################
# dependencies
#######################################
-include $(OBJS:.o=.d)

#/************************ (C) COPYRIGHT Emmet for Torock *****END OF FILE****/