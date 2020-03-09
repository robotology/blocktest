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
                       testrepetitions.h
                       )

add_library(blocktestcore SHARED ${BlockTestCoreHdr} ${BlockTestCoreSrc})
add_library(${PROJECT_NAME}::${LIBRARY_TARGET_NAME} ALIAS blocktestcore)
target_link_libraries(blocktestcore PUBLIC Boost::system
                                           Boost::filesystem
                                           ${Common_LIBRARIES}
                                    PRIVATE pugixml)

target_include_directories(blocktestcore PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

set_target_properties(blocktestcore PROPERTIES VERSION  ${${PROJECT_NAME}_VERSION}
                                                        PUBLIC_HEADER ${BlockTestCoreHdr})

install(TARGETS blocktestcore
        EXPORT blocktestcore
        COMPONENT blocktestcore
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/blocktestcore)

include(InstallBasicPackageFiles)
install_basic_package_files(blocktestcore
                            VERSION ${${PROJECT_NAME}_VERSION}
                            COMPATIBILITY AnyNewerVersion
                            VARS_PREFIX blocktestcore
                            NO_CHECK_REQUIRED_COMPONENTS_MACRO)
