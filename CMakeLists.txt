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

file(GLOB allSrc "*.h" "*.cpp" "include/*.h" "src/*.cpp")
set(CMAKE_AUTOMOC ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
if (CMAKE_VERSION VERSION_LESS 3.11)
  list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake-3.11)
endif() 

include(GNUInstallDirs)

option(USES_YARP "Compile software that depends on the YARP library" ON)
option(COMPILE_SCRIPTBUILDER "Compile the UI for creating the test" ON)

if(USES_YARP)
    set(YARP_MINIMUM_REQUIRED_VERSION 3.0.102)
    find_package(YARP REQUIRED COMPONENTS OS conf dev sig math rosmsg name OPTIONAL_COMPONENTS rtf)
    if(${YARP_VERSION} VERSION_LESS ${YARP_MINIMUM_REQUIRED_VERSION})
        message(FATAL_ERROR "YARP version ${YARP_VERSION} not sufficient, at least version ${YARP_MINIMUM_REQUIRED_VERSION} is required.")
    endif()
else()
endif() 

#Tiny process library
#find_package(TinyProcessLibrary QUIET)
#option(USE_SYSTEM_FMILIBRARY "If TRUE/ON use system TinyProcessLibrary, otherwise download and compile using FetchContent" ${TinyProcessLibrary_FOUND})
#if (USE_SYSTEM_FMILIBRARY)
#    find_package(TinyProcessLibrary REQUIRED)
#else()
    include(FetchTinyProcessLibrary)
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

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#-------Important
#Let the singleton to be unique
#-------Important
set(CMAKE_EXE_LINKER_FLAGS  "-rdynamic")

#add the action libraries
add_subdirectory(genericactiondepotlib)
if(USES_YARP)
    add_subdirectory(yarpactiondepotlib)
endif()

add_executable(${PROJECT_NAME} ${allSrc} )
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} dl pthread stdc++fs TinyProcessLibrary::TinyProcessLibrary Pugixml::Pugixml)

#test file folder
file(COPY test DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

#install
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY test DESTINATION ${CMAKE_INSTALL_BINDIR})

add_subdirectory(scriptbuilder)
