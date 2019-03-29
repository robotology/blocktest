################################################################################
#                                                                              #
# Copyright (C) 2019 Fondazione Istitito Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# @author Luca Tricerri <luca.tricerri@iit.it>

set(PROJECT_NAME genericactiondepot)

project(${PROJECT_NAME})

file(GLOB allSrc "*.h" "*.cpp" "include/*.h" "src/*.cpp")
add_library(${PROJECT_NAME} SHARED ${allSrc})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/../include)

# Add install target
install(TARGETS ${PROJECT_NAME}
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION "${CMAKE_CURRENT_BINARY_DIR}"
        LIBRARY DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
