#ifndef _ROOT_MANAGER_HPP_
#define _ROOT_MANAGER_HPP_

#include "pcompiler/output.hpp"

class RootManager
{
public:
	static bool install(Compiler::OutputList outputs, const QString &root, const QString &project);
	static bool uninstall(const QString &project);
	static bool clean();
};

#endif