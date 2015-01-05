project(programcompiler)

cmake_minimum_required(VERSION 2.8.12)

find_package(Qt5Core REQUIRED)

if(WIN32)
include_directories(${CMAKE_INSTALL_PREFIX}/include)
link_directories(${CMAKE_INSTALL_PREFIX}/lib)
endif()

add_executable(programcompiler programcompiler.cpp)
target_link_libraries(programcompiler kar pcompiler)
qt5_use_modules(programcompiler Core)

install(TARGETS programcompiler RUNTIME DESTINATION bin)
