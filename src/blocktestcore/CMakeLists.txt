################################################################################
#                                                                              #
# Copyright (C) 2019 Fondazione Istitito Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# @author Nicolo Genesio <nicolo.genesio@iit.it> 
include(CMakePackageConfigHelpers)

set(BlockTestCoreSrc    action.cpp
                        actionDepotStart.cpp
                        clockFacility.cpp
                        dataLogger.cpp
                        infoLogger.cpp
                        logger.cpp
                        report.cpp
                        tables.cpp
                        table.cpp
                        tableExpression.cpp
                        tableIncrement.cpp
                        tableNormal.cpp
                        tableWave.cpp
                        )

set(BlockTestCoreHdr   api.h
                       action.h
                       actionDepotStart.h
                       actionRegister.h
                       clockFacility.h
                       dataLogger.h
                       general.h
                       infoLogger.h
                       logconst.h
                       logger.h
                       loggerRegister.h
                       report.h
                       tables.h
                       table.h
                       tableExpression.h
                       tableIncrement.h
                       tableNormal.h
                       tableWave.h
                       type.h
                       exprtk.hpp
                       )

add_library(blocktestcore SHARED ${BlockTestCoreHdr} ${BlockTestCoreSrc})
target_link_libraries(blocktestcore ${Boost_LIBRARIES}
                                    ${Common_LIBRARIES}
                                    pugixml)
target_include_directories(blocktestcore PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
set_property(TARGET blocktestcore PROPERTY PUBLIC_HEADER ${BlockTestCoreHdr})

write_basic_package_version_file(${CMAKE_BINARY_DIR}/blocktestcoreConfigVersion.cmake
                                 VERSION ${PROJECT_VERSION}
                                 COMPATIBILITY SameMajorVersion )

file(COPY ${PROJECT_SOURCE_DIR}/cmake/blocktestcoreConfig.cmake DESTINATION ${CMAKE_BINARY_DIR})


install(TARGETS blocktestcore
        EXPORT blocktestcore
        COMPONENT blocktestcore
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/blocktestcore)
install(FILES ${PROJECT_SOURCE_DIR}/cmake/blocktestcoreConfig.cmake
              ${CMAKE_BINARY_DIR}/blocktestcoreConfigVersion.cmake
              DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/blocktestcore)
