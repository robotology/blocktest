################################################################################
#                                                                              #
# Copyright (C) 2020 Fondazione Istitito Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# @author Nicolo Genesio <nicolo.genesio@iit.it>


# If we use the pugixml internal source code,
# define a pugixml::pugixml target that add in any linked
# libraries the necessary pugixml files
add_library(pugixml INTERFACE)
add_library(pugixml::pugixml ALIAS pugixml)
target_sources(pugixml INTERFACE
                        ${CMAKE_CURRENT_SOURCE_DIR}/pugixml.hpp
                        ${CMAKE_CURRENT_SOURCE_DIR}/pugiconfig.hpp
                        ${CMAKE_CURRENT_SOURCE_DIR}/pugixml.cpp)

target_include_directories(pugixml INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                             $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
