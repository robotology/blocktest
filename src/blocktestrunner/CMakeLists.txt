################################################################################
#                                                                              #
# Copyright (C) 2019 Fondazione Istitito Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# @author Nicolo Genesio <nicolo.genesio@iit.it> 

set(BlockTestRunnerSrc  main.cpp
                        blockTestRunner.cpp
                        libraryLoader.cpp
                        testsDepot.cpp
                        fixture.cpp
                        test.cpp
                        command.cpp
                        server.cpp
                        connection.cpp
                        )
set(BlockTestRunnerHdr  blockTestRunner.h
                        libraryLoader.h
                        testsDepot.h
                        fixture.h
                        command.h
                        test.h
                        server.h
                        connection.h
                        )

include_directories(${Boost_INCLUDE_DIRS})
add_executable(blocktestrunner ${BlockTestRunnerSrc} ${BlockTestRunnerHdr} )
target_link_libraries(blocktestrunner blocktestcore pugixml)

# install
install(TARGETS blocktestrunner RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
