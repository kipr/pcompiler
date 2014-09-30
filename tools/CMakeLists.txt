project(programcompiler)

cmake_minimum_required(VERSION 2.8.12)

find_package(Qt5Core REQUIRED)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(programcompiler programcompiler.cpp)
target_link_libraries(programcompiler kar pcompiler)

if(WIN32)
install(TARGETS programcompiler RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/../prefix/bin)
else()
install(TARGETS programcompiler RUNTIME DESTINATION bin)
endif()
