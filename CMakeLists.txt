project(pcompiler)

option(docker_cross "Cross compile option for docker container" OFF)
cmake_minimum_required(VERSION 2.8.12)

find_package(Qt6 REQUIRED COMPONENTS Core)

include_directories(/usr/local/include)

set(INCLUDE ${CMAKE_SOURCE_DIR}/include)
set(SRC ${CMAKE_SOURCE_DIR}/src)
set(COMPILER ${CMAKE_SOURCE_DIR}/compilers)

include_directories(${INCLUDE})

file(GLOB INCLUDES ${INCLUDE}/pcompiler/*.h ${INCLUDE}/pcompiler/*.hpp)
file(GLOB SOURCES ${SRC}/*.c ${SRC}/*.cpp)
file(GLOB_RECURSE COMPILER_SOURCES ${COMPILER}/*.c ${COMPILER}/*.cpp)

set(SOURCES ${SOURCES} ${COMPILER_SOURCES})
set(MOC_SOURCES ${INCLUDES})
set(LIBKAR_ROOT ${CMAKE_SOURCE_DIR}/../libkar)
qt6_wrap_cpp(SOURCES ${MOC_SOURCES})

add_definitions(-Wall)

if(APPLE)
  link_directories(/Library/Frameworks/)
endif(APPLE)

if(docker_cross)
	link_directories("${LIBKAR_ROOT}/lib")
endif(docker_cross)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
add_library(pcompiler SHARED ${SOURCES} ${INCLUDES})
target_link_libraries(pcompiler Qt6::Core)


link_directories(${pcompiler_SOURCE_DIR}/lib)

if(APPLE)
set(OSX_INSTALL_NAMES_SCRIPT "${pcompiler_SOURCE_DIR}/scripts/osx_install_names.sh")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -headerpad_max_install_names")
add_custom_target(osx_install_names ALL 
	COMMAND sh ${OSX_INSTALL_NAMES_SCRIPT} ${LIBRARY_OUTPUT_PATH}/libpcompiler.dylib
		${_qt6Core_install_prefix}/lib
    QtCore.framework/Versions/6/QtCore
	WORKING_DIRECTORY ${LIBRARY_OUTPUT_PATH}
	DEPENDS pcompiler)
endif()

if(WIN32 AND CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/../prefix)
endif()

install(FILES ${INCLUDES} DESTINATION include/pcompiler)
install(TARGETS pcompiler DESTINATION lib)

add_subdirectory(tools)
