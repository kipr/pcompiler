#include "platform.hpp"

#include <QtGlobal>
#include <QDir>
#include <QFile>

using namespace Compiler;

QString Platform::platform()
{
#ifdef Q_OS_WIN
	return "WIN";
#elif defined(Q_OS_MAC)
	return "MAC";
#else
	return "NIX";
#endif
}

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
#ifdef Q_OS_WIN
	return "dll";
#elif defined(Q_OS_MAC)
	return "dylib";
#else
	return "so";
#endif
}

QString Platform::exeExtension()
{
#ifdef Q_OS_WIN
	return "exe";
#else
	return "";
#endif
}
