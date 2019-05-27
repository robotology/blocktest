################################################################################
#                                                                              #
# Copyright (C) 2019 Fondazione Istitito Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# @author Luca Tricerri <luca.tricerri@iit.it>  

#Thrift for clock
set(YARP_MINIMUM_REQUIRED_VERSION 3.0.102)
find_package(YARP REQUIRED COMPONENTS OS conf dev sig math rosmsg name idl_tools)
if(${YARP_VERSION} VERSION_LESS ${YARP_MINIMUM_REQUIRED_VERSION})
    message(FATAL_ERROR "YARP version ${YARP_VERSION} not sufficient, at least version ${YARP_MINIMUM_REQUIRED_VERSION} is required.")
endif()

yarp_add_idl(CLOCK_RPC_FILES thrift/clock_rpc.thrift)
include_directories(${Boost_INCLUDE_DIRS})
add_library(ClockRpc STATIC ${CLOCK_RPC_FILES} thrift/clock_rpc.thrift)
set_target_properties(ClockRpc PROPERTIES POSITION_INDEPENDENT_CODE ON)
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

file(GLOB allSrc "src/*.h" "src/*.cpp")
#temporary removed files
list(REMOVE_ITEM allSrc "${CMAKE_CURRENT_SOURCE_DIR}/src/actionGenerateTrajectory.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/actionPrepareStraightWalking.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/actionResetWalking.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/actionSetVelocity.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/actionStartWalking.cpp")

add_library(${PROJECT_NAME} MODULE ${allSrc})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

target_include_directories(${PROJECT_NAME} PRIVATE ${YARP_HELPERS_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/../src)
target_link_libraries(${PROJECT_NAME} PRIVATE BlockTestCore ${Boost_LIBRARIES} YARP::YARP_OS YARP::YARP_init YARP::YARP_dev YARP::YARP_conf YARP::YARP_sig YARP::YARP_math YARP::YARP_rosmsg ClockRpc Pugixml::Pugixml)

# Add install target
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME}")
install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME})
