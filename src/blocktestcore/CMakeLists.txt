################################################################################
#                                                                              #
# Copyright (C) 2019 Fondazione Istitito Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# @author Nicolo Genesio <nicolo.genesio@iit.it> 
include(CMakePackageConfigHelpers)

set(LIBRARY_TARGET_NAME blocktestcore)

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
                        general.cpp
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
                       sintax.h
                       )

add_library(${LIBRARY_TARGET_NAME} SHARED ${BlockTestCoreHdr} ${BlockTestCoreSrc})
add_library(${PROJECT_NAME}::${LIBRARY_TARGET_NAME} ALIAS ${LIBRARY_TARGET_NAME})
target_link_libraries(${LIBRARY_TARGET_NAME} PUBLIC Boost::system
                                                    Boost::filesystem
                                                    ${Common_LIBRARIES}
                                             PRIVATE pugixml)

target_include_directories(${LIBRARY_TARGET_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                                                         "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

set_target_properties(${LIBRARY_TARGET_NAME} PROPERTIES VERSION  ${${PROJECT_NAME}_VERSION}
                                                        PUBLIC_HEADER "${BlockTestCoreHdr}")

install(TARGETS ${LIBRARY_TARGET_NAME}
        EXPORT ${LIBRARY_TARGET_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${LIBRARY_TARGET_NAME})

include(InstallBasicPackageFiles)
install_basic_package_files(${LIBRARY_TARGET_NAME}
                            VERSION ${${PROJECT_NAME}_VERSION}
                            COMPATIBILITY AnyNewerVersion
                            VARS_PREFIX ${LIBRARY_TARGET_NAME}
                            NO_CHECK_REQUIRED_COMPONENTS_MACRO
                            DEPENDENCIES "Boost ${BOOST_MIN_VERSION} COMPONENTS filesystem system")
