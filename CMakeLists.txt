project(pcompiler)

cmake_minimum_required(VERSION 2.8.0)

find_package(Qt4 REQUIRED)

set(QT_DONT_USE_QTGUI TRUE)

include(${QT_USE_FILE})

set(INCLUDE ${pcompiler_SOURCE_DIR}/include)
set(SRC ${pcompiler_SOURCE_DIR}/src)
set(COMPILER ${pcompiler_SOURCE_DIR}/compilers)

include_directories(
	${CMAKE_SOURCE_DIR} 
	${CMAKE_SOURCE_DIR}/src
	${pcompiler_SOURCE_DIR}
	${pcompiler_BINARY_DIR}
	${INCLUDE}
)

if(WIN32)
include_directories(/include)
endif()

file(GLOB INCLUDES ${INCLUDE}/pcompiler/*.hpp)
file(GLOB SOURCES ${SRC}/*.cpp)
file(GLOB_RECURSE COMPILER_SOURCES ${COMPILER}/*.cpp)

set(SOURCES ${SOURCES} ${COMPILER_SOURCES})
set(MOC_SOURCES ${INCLUDES})

qt4_wrap_cpp(SOURCES ${MOC_SOURCES})

set(CMAKE_CXX_FLAGS "-Wall")

if(APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch x86_64 -g")
	link_directories(/Library/Frameworks/)
endif()

set(LIBRARY_OUTPUT_PATH ${pcompiler_SOURCE_DIR}/lib)
add_library(pcompiler SHARED ${SOURCES})
target_link_libraries(pcompiler ${QT_LIBRARIES})

link_directories(${pcompiler_SOURCE_DIR}/lib)

if(APPLE)
set(OSX_INSTALL_NAMES_SCRIPT "${pcompiler_SOURCE_DIR}/scripts/osx_install_names.sh")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -headerpad_max_install_names")
add_custom_target(osx_install_names ALL 
	COMMAND sh ${OSX_INSTALL_NAMES_SCRIPT} ${LIBRARY_OUTPUT_PATH}/libpcompiler.dylib
		QtCore.framework/Versions/4/QtCore
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
