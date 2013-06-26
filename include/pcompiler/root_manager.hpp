#ifndef _PCOMPILER_ROOT_MANAGER_HPP_
#define _PCOMPILER_ROOT_MANAGER_HPP_

#include "pcompiler/output.hpp"

#include <QDir>

namespace Compiler
{
	class RootManager
	{
	public:
		RootManager(const QString &root);

		Compiler::OutputList install(const Compiler::OutputList &output, const QString &project);
		bool uninstall(const QString &project);
		bool clean();

		QDir bin();
		QDir lib();
		QDir include();
		
		virtual QString binPath();
		virtual QString libPath();
		virtual QString includePath();

	private:
		bool removeDir(const QString &path);

		QString m_root;
	};
}

#endif