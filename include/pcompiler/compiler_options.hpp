#ifndef __OPTIONS_HPP__
#define __OPTIONS_HPP__

#define TEMPORARY_DIR "TEMPORARY_DIR"
#define OUTPUT_DIR "OUTPUT_DIR"
#define KEY_DEPS "DEPENDENCIES"
#define KEY_COMPILE_LIB "COMPILE_LIB"
#define KEY_LIB_NAME "LIBRARY_NAME"
#define C_FLAGS "C_FLAGS"
#define PLATFORM_C_FLAGS Platform::platform() + "_" + C_FLAGS
#define CPP_FLAGS "CPP_FLAGS"
#define PLATFORM_CPP_FLAGS Platform::platform() + "_" + CPP_FLAGS
#define O_FLAGS "LD_FLAGS"
#define PLATFORM_O_FLAGS Platform::platform() + "_" + O_FLAGS
#define JAVAC_FLAGS "JAVAC_FLAGS"
#define LISP_FLAGS "LISP_FLAGS"
#define PY_FLAGS "PY_FLAGS"

#endif
