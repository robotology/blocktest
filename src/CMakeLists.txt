################################################################################
#                                                                              #
# Copyright (C) 2019 Fondazione Istitito Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# @author Nicolo Genesio <nicolo.genesio@iit.it> 

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(blocktestcore)
add_subdirectory(blocktestrunner)
add_subdirectory(genericactiondepotlib)
if(Qt5_FOUND)
  add_subdirectory(scriptbuilder)
endif()
