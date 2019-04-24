################################################################################
#                                                                              #
# Copyright (C) 2019 Fondazione Istitito Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# @author Luca Tricerri <luca.tricerri@iit.it>

# Fetch FMI library for downloading and installing
include(FetchContent)
FetchContent_Declare(TinyProcessLibrary 
                        GIT_REPOSITORY https://gitlab.com/eidheim/tiny-process-library.git
                        GIT_PROGRESS true 
                        QUIET
                        )

if(NOT TinyProcessLibrary_POPULATED)
        #set(BUILD_SHARED_LIBS ON)
        FetchContent_Populate(TinyProcessLibrary)
        FetchContent_GetProperties(TinyProcessLibrary 
                                        SOURCE_DIR TinyProcessLibrary_SOURCE_DIR 
                                        BINARY_DIR TinyProcessLibrary_BINARY_DIR
                                        POPULATED TinyProcessLibrary_POPULATED
                                        )
endif()
add_subdirectory(${TinyProcessLibrary_SOURCE_DIR} ${TinyProcessLibrary_BINARY_DIR} )
add_library(TinyProcessLibrary::TinyProcessLibrary ALIAS tiny-process-library)
