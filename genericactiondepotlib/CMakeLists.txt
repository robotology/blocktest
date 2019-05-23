################################################################################
#                                                                              #
# Copyright (C) 2019 Fondazione Istitito Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# @author Luca Tricerri <luca.tricerri@iit.it>

set(PROJECT_NAME genericactiondepot)

project(${PROJECT_NAME})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME})

file(GLOB allSrc "src/*.h" "src/*.cpp")
include_directories(${Boost_INCLUDE_DIRS})
add_library(${PROJECT_NAME} MODULE ${allSrc})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/../src)

target_link_libraries(${PROJECT_NAME} BlockTestCore ${Boost_LIBRARIES} Pugixml::Pugixml)
# Add install target
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME}")
install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME})

