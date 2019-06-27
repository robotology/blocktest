################################################################################
#                                                                              #
# Copyright (C) 2019 Fondazione Istitito Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# @author Luca Tricerri <luca.tricerri@iit.it>

cmake_minimum_required(VERSION 3.5)

project(blockTest
        LANGUAGES C CXX
        VERSION 0.1)

set(BlockTestSrc        src/main.cpp
                        src/blockTest.cpp
                        src/libraryLoader.cpp
                        src/testsDepot.cpp
                        src/fixture.cpp
                        src/test.cpp
                        src/command.cpp
                        )
set(BlockTestHdr        src/blockTest.h
                        src/libraryLoader.h
                        src/testsDepot.h
                        src/fixture.h
                        src/command.h
                        src/test.h
                        )

set(BlockTestCoreSrc    src/action.cpp
                        src/clockFacility.cpp
                        src/dataLogger.cpp
                        src/infoLogger.cpp
                        src/logger.cpp
                        src/report.cpp
                        src/tables.cpp
                        src/table.cpp
                        src/tableIncrement.cpp
                        src/tableNormal.cpp
                        src/tableWave.cpp
                        src/actionDepotStart.cpp
                        )

set(BlockTestCoreHdr    src/api.h
                        src/action.h
                        src/actionDepotStart.h
                        src/actionRegister.h
                        src/clockFacility.h
                        src/dataLogger.h
                        src/general.h
                        src/infoLogger.h
                        src/logconst.h
                        src/logger.h
                        src/loggerRegister.h
                        src/report.h
                        src/tables.h
                        src/table.h
                        src/tableIncrement.h
                        src/tableNormal.h
                        src/tableWave.h
                        src/type.h
                        )
set(CMAKE_AUTOMOC ON)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
if (CMAKE_VERSION VERSION_LESS 3.11)
  list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake-3.11)
endif() 

include(GNUInstallDirs)

option(COMPILE_SCRIPTBUILDER "Compile the UI for creating the test" ON)

set(Boost_NO_BOOST_CMAKE ON)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREAD ON)
set(BOOST_MIN_VERSION "1.64.0")

find_package(Boost ${BOOST_MIN_VERSION} REQUIRED COMPONENTS filesystem system)
find_package(pugixml REQUIRED)

if (NOT Boost_FOUND)
      message(FATAL_ERROR "Fatal error: Boost (version >= ${BOOST_MIN_VERSION}) required.\n")
endif (NOT Boost_FOUND)

# Control where libraries and executables are placed during the build.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
if (CMAKE_VERSION VERSION_LESS 3.5)
  list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake-3.5)
endif() 

# Encourage user to specify a build type (e.g. Release, Debug, etc.), otherwise set it to Debug.
if(NOT CMAKE_CONFIGURATION_TYPES)
    if(NOT CMAKE_BUILD_TYPE)
        message(STATUS "Setting build type to 'Debug' as none was specified.")
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY VALUE "Debug")
    endif()
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Build position independent code.
# Position Independent Code (PIC) is commonly used for shared libraries so that
# the same shared library code can be loaded in each program address space in a
# location where it will not overlap with any other uses of such memory.
# In particular, this option avoids problems occurring when a process wants to
# load more than one shared library at the same virtual address.
# Since shared libraries cannot predict where other shared libraries could be
# loaded, this is an unavoidable problem with the traditional shared library
# concept.
# Generating position-independent code is often the default behavior for most
# modern compilers.
# Moreover linking a static library that is not built with PIC from a shared
# library will fail on some compiler/architecture combinations.
# Further details on PIC can be found here:
# https://eli.thegreenplace.net/2011/11/03/position-independent-code-pic-in-shared-libraries/
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


#-------Important
#Let the singleton to be unique
#-------Important
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_EXE_LINKER_FLAGS  "-rdynamic")

#add the action libraries
add_subdirectory(genericactiondepotlib)
if(WIN32)
    set(Common_LIBRARIES)
else()
    set(Common_LIBRARIES dl pthread)
endif()

# BlockTestCore
add_library(BlockTestCore SHARED ${BlockTestCoreHdr} ${BlockTestCoreSrc})
target_include_directories(BlockTestCore PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(BlockTestCore ${Boost_LIBRARIES}
                                    ${Common_LIBRARIES}
                                    pugixml)
# BlockTest
include_directories(${Boost_INCLUDE_DIRS})
add_executable(${PROJECT_NAME} ${BlockTestSrc} ${BlockTestHdr} )
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} BlockTestCore pugixml)

if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/test
    ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/$<CONFIG>/test)
    file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/$<CONFIG>/config.xml"
                  INPUT "${CMAKE_CURRENT_LIST_DIR}/config.xml")
else()
    file(COPY ${PROJECT_SOURCE_DIR}/test
         DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
    file(COPY "${CMAKE_CURRENT_LIST_DIR}/config.xml"
         DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
endif()

file(COPY ${PROJECT_SOURCE_DIR}/cmake/blocktestcoreConfig.cmake DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

#install
install(TARGETS BlockTestCore LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR})
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY test DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES config.xml DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${PROJECT_SOURCE_DIR}/cmake/blocktestcoreConfig.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/blocktestcore)

add_subdirectory(scriptbuilder)
