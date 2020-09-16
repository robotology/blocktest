################################################################################
#                                                                              #
# Copyright (C) 2019 Fondazione Istitito Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# @author Nicolo Genesio <nicolo.genesio@iit.it> 

set(BlockTestRunnerSrc  main.cpp
                        blockTestRunner.cpp
                        )
set(BlockTestRunnerHdr  blockTestRunner.h
                        )

add_executable(blocktestrunner ${BlockTestRunnerSrc} ${BlockTestRunnerHdr} )
target_link_libraries(blocktestrunner PUBLIC blocktestcore
                                             pugixml
                                             Boost::boost)

# install
install(TARGETS blocktestrunner RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
