#ifndef _ROOT_MANAGER_HPP_
#define _ROOT_MANAGER_HPP_

#include "pcompiler/output.hpp"

class RootManager
{
public:
	static Compiler::OutputList install(Compiler::OutputList terminals, const QString &root, const QString &project);
	static bool uninstall(const QString &root, const QString &project);
	static bool clean(const QString &root);

private:
	static bool removeDir(const QString &path);
};

#endif