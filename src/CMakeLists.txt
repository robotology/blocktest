################################################################################
#                                                                              #
# Copyright (C) 2019 Fondazione Istitito Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# @author Nicolo Genesio <nicolo.genesio@iit.it> ,Luca Tricerri <luca.tricerri@iit.it>

include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(COMPILE_WITHUNITTEST "Compile also unittest" OFF)

# The unit tests uses io_service::run_for that is available from boost 1.66
if(COMPILE_WITHUNITTEST AND Boost_VERSION_STRING GREATER_EQUAL 1.66.0)
  FetchContent_Declare(
    unittest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.11.0
  )
  FetchContent_GetProperties(unittest)
  if(NOT unittest)
    FetchContent_Populate(unittest)
    add_subdirectory(unittest)
    add_subdirectory(${unittest_SOURCE_DIR} ${unittest_BINARY_DIR})
  endif()
endif()

add_subdirectory(blocktestcore)
add_subdirectory(blocktestrunner)
add_subdirectory(genericactiondepotlib)
if(Qt5_FOUND)
  add_subdirectory(scriptbuilder)
endif()
