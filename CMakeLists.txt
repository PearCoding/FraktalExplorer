# This file is part of the FraktalExplorer project.
# FraktalExplorer is a dynamic fractal explorer with an internal L-System parser.
# Copyright (C) 2010-2014 Ömercan Yazici <pearcoding@gmail.com>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not,
# write to the Free Software Foundation, Inc.,
# 51 Franklin St, Fifth Floor, Boston, MA 02110, USA
#
# More information about FraktalExplorer here:
# http://pearcoding.eu/projects/fraktalexplorer

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

PROJECT(FraktalExplorer)

#SETS
SET(VENDOR "FraktalExplorer project 2014")
SET(VERSION_MAJOR 0)
SET(VERSION_MINOR 1)
SET(VERSION_HEX 0x0001)
SET(FE_Version ${VERSION_MAJOR}.${VERSION_MINOR})
SET(FE_TARGET "FraktalExplorer")

SET(FE_Root ${CMAKE_CURRENT_SOURCE_DIR}/src)

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/plugins/)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/plugins/)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

#CHECKS
MESSAGE(STATUS "Building FraktalExplorer ${FE_Version}")
MESSAGE(STATUS "Root: ${FE_Root}")

IF(MINGW)
	MESSAGE(WARNING "Please don't use MinGW with FraktalExplorer, use the MSVC version instead")
ENDIF()

IF(MSYS)
	MESSAGE(WARNING "Please don't use MSYS with FraktalExplorer, use the MSVC version instead")
ENDIF()

IF(CYGWIN)
	MESSAGE(WARNING "Please don't use CYGWIN with FraktalExplorer, use the MSVC version instead")
ENDIF()

#PACKAGES
set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Concurrent REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5LinguistTools REQUIRED)

MESSAGE(STATUS "Using Qt: ${Qt5Widgets_VERSION_STRING}")

#DEFINITIONS AND FLAGS
include_directories(${FE_Root}/main ${Qt5Widgets_INCLUDE_DIRS} ${Qt5Concurrent_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/src/lib)
link_directories(${CMAKE_CURRENT_BINARY_DIR})
add_definitions(${Qt5Widgets_DEFINITIONS} ${Qt5Concurrent_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS} ${Qt5Concurrent_EXECUTABLE_COMPILE_FLAGS}")
IF(MSVC)
	#SET(DEFS "_UNICODE;UNICODE;_MT;_USRDLL")
	#SET(DEFS_DEBUG "_DEBUG")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Oi /fp:fast /EHsc")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox /Ob2 /Ot /GL")
	#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od /MDd")
	#set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /DEBUG")
	IF(CMAKE_CL_64)
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /bigobj")
	ENDIF()
ELSE()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ENDIF()

SET(DEFS_DEBUG "${DEFS_DEBUG};FE_DEBUG")

add_subdirectory(${FE_Root}/main)
add_subdirectory(${FE_Root}/plugins)