################################################################################
#                                                                              #
# Copyright (C) 2019 Fondazione Istitito Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# @author Luca Tricerri <luca.tricerri@iit.it>  

set(PROJECT_NAME yarpactiondepot)

project(${PROJECT_NAME})

file(GLOB allSrc "*.h" "*.cpp" "include/*.h" "src/*.cpp")
#temporary removed files
list(REMOVE_ITEM allSrc "${CMAKE_CURRENT_SOURCE_DIR}/src/actionGenerateTrajectory.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/actionPrepareStraightWalking.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/actionResetWalking.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/actionSetVelocity.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/actionStartWalking.cpp")

add_library(${PROJECT_NAME} MODULE ${allSrc})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

target_include_directories(${PROJECT_NAME} PRIVATE ${YARP_HELPERS_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/../include)
target_link_libraries(${PROJECT_NAME} PRIVATE ${YARP_LIBRARIES} )

# Add install target
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME}")
install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME})
