################################################################################
#                                                                              #
# Copyright (C) 2019 Fondazione Istitito Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# @author Luca Tricerri <luca.tricerri@iit.it>  

#Thrift for clock
find_package(YARP 3.0.0 REQUIRED)
yarp_add_idl(CLOCK_RPC_FILES thrift/clock_rpc.thrift)
add_library(ClockRpc ${CLOCK_RPC_FILES} thrift/clock_rpc.thrift)
set_target_properties(ClockRpc PROPERTIES POSITION_INDEPENDENT_CODE ON)
find_package(YARP 3.0.0 REQUIRED)
target_link_libraries(ClockRpc YARP::YARP_OS)

# Extract the include directory from the files names
foreach(file ${CLOCK_RPC_FILES})
    STRING(REGEX MATCH ".+\\.h?h$" file ${file})
    if(file)
        get_filename_component(include_dir ${file} DIRECTORY)
        list(APPEND CLOCKRPC_INCLUDE_DIRS ${include_dir})
    endif()
endforeach()

# Setup the include directories
target_include_directories(ClockRpc PUBLIC
    $<BUILD_INTERFACE:${CLOCKRPC_INCLUDE_DIRS}>)


#Module creation
set(PROJECT_NAME yarpactiondepot)

project(${PROJECT_NAME})

file(GLOB allSrc "*.h" "*.cpp" "include/*.h" "src/*.cpp")
#temporary removed files
list(REMOVE_ITEM allSrc "${CMAKE_CURRENT_SOURCE_DIR}/src/actionGenerateTrajectory.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/actionPrepareStraightWalking.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/actionResetWalking.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/actionSetVelocity.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/actionStartWalking.cpp")

add_library(${PROJECT_NAME} MODULE ${allSrc})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

target_include_directories(${PROJECT_NAME} PRIVATE ${YARP_HELPERS_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/../include)
target_link_libraries(${PROJECT_NAME} PRIVATE ${YARP_LIBRARIES} ClockRpc)

# Add install target
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME}")
install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME})
