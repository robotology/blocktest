project(unittest)

set(SOURCES main.cpp
            test1.cpp
            )

add_executable(${PROJECT_NAME} ${SOURCES})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} blocktestcore gtest Boost::boost)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
#install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../msgdepot DESTINATION ${CMAKE_INSTALL_BINDIR})