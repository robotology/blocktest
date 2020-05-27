################################################################################
#                                                                              #
# Copyright (C) 2019 Fondazione Istitito Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# @author Luca Tricerri <luca.tricerri@iit.it>
include(FetchContent)

if(USE_SERIAL_LIB)
  FetchContent_Declare(
    serial
    GIT_REPOSITORY https://github.com/dic-iit/serial
    GIT_TAG        origin/master
  )

  FetchContent_GetProperties(serial)
  if(NOT serial_POPULATED)
    FetchContent_Populate(serial)
    add_subdirectory(${serial_SOURCE_DIR} ${serial_BINARY_DIR})
  endif()
endif()

project(genericactiondepot)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME})

file(GLOB allSrc "src/*.h" "src/*.cpp")
include_directories(${Boost_INCLUDE_DIRS})
add_library(${PROJECT_NAME} MODULE ${allSrc})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/../src )

# Add install target
if(USE_SERIAL_LIB)
  target_link_libraries(${PROJECT_NAME} PUBLIC blocktestcore
                                               serial
                                               ${Boost_LIBRARIES}
                                        PRIVATE pugixml)
  target_compile_definitions(${PROJECT_NAME} PRIVATE -DCOMPILE_WITHEXTSERIAL)
else()
  target_link_libraries(${PROJECT_NAME} PUBLIC blocktestcore
                                               ${Boost_LIBRARIES}
                                        PRIVATE pugixml)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME}")
install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME})



