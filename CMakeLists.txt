project(pcompiler)

cmake_minimum_required(VERSION 2.8.12)

find_package(Qt5Core REQUIRED)

set(INCLUDE ${CMAKE_SOURCE_DIR}/include)
set(SRC ${CMAKE_SOURCE_DIR}/src)
set(COMPILER ${CMAKE_SOURCE_DIR}/compilers)

include_directories(${INCLUDE})

file(GLOB INCLUDES ${INCLUDE}/pcompiler/*.h ${INCLUDE}/pcompiler/*.hpp)
file(GLOB SOURCES ${SRC}/*.c ${SRC}/*.cpp)
file(GLOB_RECURSE COMPILER_SOURCES ${COMPILER}/*.c ${COMPILER}/*.cpp)

set(SOURCES ${SOURCES} ${COMPILER_SOURCES})
set(MOC_SOURCES ${INCLUDES})

qt5_wrap_cpp(SOURCES ${MOC_SOURCES})

add_definitions(-Wall)

if(APPLE)
  link_directories(/Library/Frameworks/)
endif(APPLE)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
add_library(pcompiler SHARED ${SOURCES} ${INCLUDES})
qt5_use_modules(pcompiler Core)

link_directories(${pcompiler_SOURCE_DIR}/lib)

if(APPLE)
set(OSX_INSTALL_NAMES_SCRIPT "${pcompiler_SOURCE_DIR}/scripts/osx_install_names.sh")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -headerpad_max_install_names")
add_custom_target(osx_install_names ALL 
	COMMAND sh ${OSX_INSTALL_NAMES_SCRIPT} ${LIBRARY_OUTPUT_PATH}/libpcompiler.dylib
		QtCore.framework/Versions/5/QtCore
	WORKING_DIRECTORY ${LIBRARY_OUTPUT_PATH}
	DEPENDS pcompiler)
endif()

if(WIN32)
install(FILES ${INCLUDES} DESTINATION ${CMAKE_SOURCE_DIR}/../prefix/include/pcompiler)
install(TARGETS pcompiler
	ARCHIVE DESTINATION ${CMAKE_SOURCE_DIR}/../prefix/lib
	RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/../prefix/lib)
else()
install(FILES ${INCLUDES} DESTINATION include/pcompiler)
install(TARGETS pcompiler LIBRARY DESTINATION lib)
endif()
