################################################################################
#                                                                              #
# Copyright (C) 2019 Fondazione Istitito Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# @author Luca Tricerri <luca.tricerri@iit.it>

project (Scriptbuilder)
set(CMAKE_AUTOMOC TRUE)
file(GLOB scriptbuilder_src "*.cpp")
file(GLOB scriptbuilder_header "*.h" )

if(COMPILE_SCRIPTBUILDER)

  find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets)
  
  set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

  set(scriptbuilder_qrc resource.qrc)
  set(scriptbuilder_ui mainwindow.ui settingdialog.ui)

  qt5_add_resources(qrc_moc ${scriptbuilder_qrc})
  qt5_wrap_cpp(moc_src ${scriptbuilder_header})
  qt5_wrap_ui (ui_wrap  ${scriptbuilder_ui})
  
  include_directories (${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR})


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
                          pthread
                          resourcelib
                          ${Boost_LIBRARIES}
                       )

  #xml template folder
  file(COPY xmltemplate DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

  #install
  install(TARGETS Scriptbuilder RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

    
endif()
