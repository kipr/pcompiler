project(pco)

cmake_minimum_required(VERSION 2.8.12)

find_package(Qt6Core REQUIRED)

if(WIN32)
include_directories(${CMAKE_INSTALL_PREFIX}/include)
link_directories(${CMAKE_INSTALL_PREFIX}/lib)
endif()

add_executable(pco pco.cpp)
target_link_libraries(pco kar pcompiler)
#qt6_use_modules(pco Core)

install(TARGETS pco RUNTIME DESTINATION bin)
