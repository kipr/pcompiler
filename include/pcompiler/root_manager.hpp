#ifndef _PCOMPILER_ROOT_MANAGER_HPP_
#define _PCOMPILER_ROOT_MANAGER_HPP_

#include "pcompiler/output.hpp"

#include <QDir>

namespace Compiler
{
	class RootManager
	{
	public:
		static Compiler::OutputList install(const Compiler::OutputList &output, const QString &root,
			const QString &project);
		static bool uninstall(const QString &root, const QString &project);
		static bool clean(const QString &root);

		static QDir bin(const QString &root);
		static QDir lib(const QString &root);
		static QDir include(const QString &root);
		static QString binPath(const QString &root);
		static QString libPath(const QString &root);
		static QString includePath(const QString &root);

	private:
		static bool removeDir(const QString &path);
	};
}

#endif