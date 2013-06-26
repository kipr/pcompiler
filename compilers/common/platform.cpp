#include "platform.hpp"

#include <QtGlobal>
#include <QDir>
#include <QFile>

using namespace Compiler;

QString Platform::ccPath()
{
#ifndef Q_OS_WIN
	return "/usr/bin/cc";
#else
	return QDir::currentPath() + "/../MinGW/bin/cc";
#endif
}

QString Platform::cppPath()
{
#ifndef Q_OS_WIN
	return "/usr/bin/c++";
#else
	return QDir::currentPath() + "/../MinGW/bin/c++";
#endif
}

QString Platform::libExtension()
{
#ifndef Q_OS_WIN
	return "dll";
#elif defined(Q_OS_MAC)
	return "dylib";
#else
	return "so";
#endif
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