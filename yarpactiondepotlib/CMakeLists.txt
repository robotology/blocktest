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

add_library(${PROJECT_NAME} SHARED ${allSrc})

target_include_directories(${PROJECT_NAME} PUBLIC ${YARP_HELPERS_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/../include)
target_link_libraries(${PROJECT_NAME} ${YARP_LIBRARIES} )

# Add install target
install(TARGETS ${PROJECT_NAME}
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION "${CMAKE_CURRENT_BINARY_DIR}"
        LIBRARY DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
