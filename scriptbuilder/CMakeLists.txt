# Copyright (C) 2006-2019 Istituto Italiano di Tecnologia (IIT)
# All rights reserved.
#
# This software may be modified and distributed under the terms of the
# BSD-3-Clause license. See the accompanying LICENSE file for details.

project (Scriptbuilder)
set(CMAKE_AUTOMOC TRUE)
file(GLOB scriptbuilder_src "*.cpp")
file(GLOB scriptbuilder_header "*.h" )

if(COMPILE_SCRIPTBUILDER)

  find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets)
  
  set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

  set(scriptbuilder_qrc resource.qrc)
  set(scriptbuilder_ui mainwindow.ui)

  qt5_add_resources(qrc_moc ${scriptbuilder_qrc})
  qt5_wrap_cpp(moc_src ${scriptbuilder_header})
  qt5_wrap_ui (ui_wrap  ${scriptbuilder_ui})
  
  include_directories (${PROJECT_SOURCE_DIR})
  include_directories (${PROJECT_BINARY_DIR})

  add_library(resourcelib  STATIC                                 
                          ${moc_src}
                          ${ui_wrap}
                          ${qrc_moc}
                          
              )

  target_link_libraries(resourcelib  
                        PUBLIC
                        Qt5::Core  
                        Qt5::Widgets
                        Qt5::Gui
                        stdc++fs
                       )

  add_executable(${PROJECT_NAME}  
                  ${scriptbuilder_src} 
                  ${qrc_moc}
                )

  target_link_libraries(${PROJECT_NAME}  
                          PUBLIC
                          Qt5::Core  
                          Qt5::Widgets
                          Qt5::Gui
                          stdc++fs
                          resourcelib
                       )

  #install
  install(TARGETS Scriptbuilder RUNTIME DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/..)

  #xml template folder
  file(COPY xmltemplate DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    
endif()
