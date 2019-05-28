################################################################################
#                                                                              #
# Copyright (C) 2019 Fondazione Istitito Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# @author Andrea Ruzzenenti <andrea.ruzzenenti@iit.it>, Nicolo' Genesio <nicolo.genesio@iit.it>

include(FindPackageHandleStandardArgs)
include(SelectLibraryConfigurations)
if(WIN32)
set(librarydirectory_debug lib/Debug)
set(librarydirectory_release lib/Release)
set(libname_debug BlockTestCore)
set(libname_release BlockTestCore)
else()
set(librarydirectory_debug lib/)
set(librarydirectory_release lib/)
set(libname_debug BlockTestCored)
set(libname_release BlockTestCore)
endif()
find_path(blocktestcore_INCLUDE_DIR
          NAMES action.h
				actionDepotStart.h
				actionRegister.h
				blockTest.h
				clockFacility.h
				command.h
				dataLogger.h
				fixture.h
				general.h
				infoLogger.h
				libraryLoader.h
				logconst.h
				logger.h
				loggerRegister.h
				report.h
				scheduler.cpp
				scheduler.h
				tables.h
				test.h
				testsDepot.h
				type.h
				api.h

          PATHS $ENV{blocktestcore_DIR}/../src
          DOC "blocktest include directory")
find_library(blocktestcore_LIBRARY_RELEASE
             NAMES ${libname_release}
             PATHS $ENV{blocktestcore_DIR}/${librarydirectory_release}
             DOC "BlockTestCore library file (release version)")
find_library(blocktestcore_LIBRARY_DEBUG
             NAMES ${libname_debug}
             PATHS $ENV{blocktestcore_DIR}/${librarydirectory_debug}
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
                                          URL "http://computation.llnl.gov/projects/floating-point-compression")
endif()