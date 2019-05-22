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

set(BlockTestSrc src/main.cpp)
set(BlockTestHdr )
set(BlockTestCoreSrc    src/action.cpp
                        src/blockTest.cpp
                        src/clockFacility.cpp
                        src/command.cpp
                        src/dataLogger.cpp
                        src/fixture.cpp
                        src/infoLogger.cpp
                        src/libraryLoader.cpp
                        src/logger.cpp
                        src/main.cpp
                        src/report.cpp
                        src/scheduler.cpp
                        src/tables.cpp
                        src/test.cpp
                        src/testsDepot.cpp)

set(BlockTestCoreHdr    include/action.h
                        include/actionDepotStart.h
                        include/actionRegister.h
                        include/blockTest.h
                        include/clockFacility.h
                        include/command.h
                        include/dataLogger.h
                        include/fixture.h
                        include/general.h
                        include/infoLogger.h
                        include/libraryLoader.h
                        include/logconst.h
                        include/logger.h
                        include/loggerRegister.h
                        include/report.h
                        include/scheduler.h
                        include/tables.h
                        include/test.h
                        include/testsDepot.h
                        include/type.h)
set(CMAKE_AUTOMOC ON)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
if (CMAKE_VERSION VERSION_LESS 3.11)
  list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake-3.11)
endif() 

include(GNUInstallDirs)

option(USES_YARP "Compile software that depends on the YARP library" ON)
option(COMPILE_SCRIPTBUILDER "Compile the UI for creating the test" ON)

set(Boost_NO_BOOST_CMAKE ON)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREAD ON)
set(BOOST_MIN_VERSION "1.64.0")
find_package(Boost ${BOOST_MIN_VERSION} REQUIRED COMPONENTS filesystem system)
if (NOT Boost_FOUND)
      message(FATAL_ERROR "Fatal error: Boost (version >= ${BOOST_MIN_VERSION}) required.\n")
endif (NOT Boost_FOUND)


#Tiny process library
#find_package(TinyProcessLibrary QUIET)
#option(USE_SYSTEM_FMILIBRARY "If TRUE/ON use system TinyProcessLibrary, otherwise download and compile using FetchContent" ${TinyProcessLibrary_FOUND})
#if (USE_SYSTEM_FMILIBRARY)
#    find_package(TinyProcessLibrary REQUIRED)
#else()
#    include(FetchTinyProcessLibrary)  
#endif()

#Pugixml library
#find_package(TinyProcessLibrary QUIET)
#option(USE_SYSTEM_FMILIBRARY "If TRUE/ON use system TinyProcessLibrary, otherwise download and compile using FetchContent" ${TinyProcessLibrary_FOUND})
#if (USE_SYSTEM_FMILIBRARY)
#    find_package(TinyProcessLibrary REQUIRED)
#else()
    include(FetchPugixml)
#endif()

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

#-------Important
#Let the singleton to be unique
#-------Important
set(CMAKE_EXE_LINKER_FLAGS  "-rdynamic")

#add the action libraries
add_subdirectory(genericactiondepotlib)
if(USES_YARP)
    add_subdirectory(yarpactiondepotlib)
endif()
if(WIN32)
    #macro(get_WIN32_WINNT version)
    #    if (WIN32 AND CMAKE_SYSTEM_VERSION)
    #        set(ver ${CMAKE_SYSTEM_VERSION})
    #        string(REPLACE "." "" ver ${ver})
    #        string(REGEX REPLACE "([0-9])" "0\\1" ver ${ver})

 #           set(${version} "0x${ver}")
 #       endif()
 #   endmacro()

 #   get_WIN32_WINNT(win_ver)
 #  add_definitions(-D_WIN32_WINNT=${win_ver})
    set(Common_LIBRARIES)
else()
    set(Common_LIBRARIES dl pthread stdc++fs)
endif()

add_library(BlockTestCore ${BlockTestCoreHdr} ${BlockTestCoreSrc})
target_include_directories(BlockTestCore PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(BlockTestCore ${Boost_LIBRARIES} ${Common_LIBRARIES} Pugixml::Pugixml)

include_directories(${Boost_INCLUDE_DIRS})
add_executable(${PROJECT_NAME} ${BlockTestSrc} ${BlockTestHdr} )
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)


target_link_libraries(${PROJECT_NAME} BlockTestCore)

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


#install
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY test DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES config.xml DESTINATION ${CMAKE_INSTALL_BINDIR})

add_subdirectory(scriptbuilder)
