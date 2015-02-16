project(pco)

cmake_minimum_required(VERSION 2.8.0)

find_package(Qt4 REQUIRED)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(pco pco.cpp)
target_link_libraries(pco pcompiler kar ${QT_LIBRARIES})

if(WIN32)
install(TARGETS pco RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/../prefix/bin)
else()
install(TARGETS pco RUNTIME DESTINATION bin)
endif()
