################################################################################
#                                                                              #
# Copyright (C) 2019 Fondazione Istitito Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# @author Luca Tricerri <luca.tricerri@iit.it>

# Fetch Pugi xml for downloading and installing
include(FetchContent)
FetchContent_Declare(Pugixml 
                        GIT_REPOSITORY https://github.com/zeux/pugixml.git
                        GIT_PROGRESS true 
                        QUIET
                        )

if(NOT Pugixml_POPULATED)
        set(BUILD_SHARED_LIBS ON CACHE BOOL "sss")                                        
        FetchContent_Populate(Pugixml
                            )
        FetchContent_GetProperties(Pugixml
                                        SOURCE_DIR Pugixml_SOURCE_DIR 
                                        BINARY_DIR Pugixml_BINARY_DIR
                                        POPULATED Pugixml_POPULATED
                                        )
        set(BUILD_SHARED_LIBS ON CACHE BOOL "sss")                                        
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(BUILD_SHARED_LIBS ON CACHE BOOL "sss")
add_subdirectory(${Pugixml_SOURCE_DIR} ${Pugixml_BINARY_DIR} )
add_library(Pugixml::Pugixml ALIAS pugixml)

