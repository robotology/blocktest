################################################################################
#                                                                              #
# Copyright (C) 2019 Fondazione Istitito Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# @author Andrea Ruzzenenti <andrea.ruzzenenti@iit.it>, Nicolo' Genesio <nicolo.genesio@iit.it>, Luca Tricerri <luca.tricerri@iit.it>

include(FindPackageHandleStandardArgs)
include(SelectLibraryConfigurations)
if(WIN32)
  set(librarydirectory_debug lib/Debug/)
  set(librarydirectory_release lib/Release/)
  set(libname_debug blocktestcore)
  set(libname_release blocktestcore)
else()
  set(librarydirectory_debug lib/)
  set(librarydirectory_release lib/)
  set(libname_debug blocktestcored)
  set(libname_release blocktestcore)
endif()
find_path(blocktestcore_INCLUDE_DIR
          NAMES actionDepotStart.h
                action.h
                actionRegister.h
                api.h
                clockFacility.h
                dataLogger.h
                exprtk.hpp
                general.h
                infoLogger.h
                logconst.h
                logger.h
                loggerRegister.h
                report.h
                table.h
                tableExpression.h
                tableIncrement.h
                tableNormal.h
                tables.h
                tableWave.h
                type.h

PATHS   ${blocktestcore_DIR}/../src/blocktestcore
        /usr/include/
        /usr/local/include
      DOC "blocktest include directory")

find_library(blocktestcore_LIBRARY_RELEASE
             NAMES ${libname_release}
             PATHS $ENV{blocktestcore_DIR}/../${librarydirectory_release}
             DOC "BlockTestCore library file (release version)")
find_library(blocktestcore_LIBRARY_DEBUG
             NAMES ${libname_debug}
             PATHS $ENV{blocktestcore_DIR}/../${librarydirectory_debug}
             DOC "BlockTestCore library file (debug version)")

mark_as_advanced(blocktestcore_INCLUDE_DIR
                 blocktestcore_LIBRARY_RELEASE
                 blocktestcore_LIBRARY_DEBUG)

select_library_configurations(blocktestcore)

set(blocktestcore_LIBRARIES ${blocktestcore_LIBRARY})
set(blocktestcore_INCLUDE_DIRS ${blocktestcore_INCLUDE_DIR})
find_package_handle_standard_args(blocktestcore
                                  FOUND_VAR blocktestcore_FOUND
                                  REQUIRED_VARS blocktestcore_LIBRARIES blocktestcore_INCLUDE_DIRS)

# Set package properties if FeatureSummary was included
if(COMMAND set_package_properties)
    set_package_properties(blocktestcore PROPERTIES DESCRIPTION "The application provides functionalities for developing and running not regression tests in a likely natural language as close as possible to test case writing"
                                          URL "https://github.com/robotology/blocktest")
endif()
