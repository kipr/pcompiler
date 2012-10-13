#include "platform.hpp"

#include <QtGlobal>

using namespace Compiler;

QString Platform::ccPath()
{
	return "/usr/bin/cc";
}

QString Platform::cppPath()
{
	return "/usr/bin/c++";
}

QString Platform::exeExtension()
{
	return
#ifdef Q_OS_WIN
	"exe"
#else
	""
#endif
	;
}