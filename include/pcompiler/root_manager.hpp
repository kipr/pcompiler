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

		Compiler::OutputList install(const Compiler::OutputList &output, const QString &project) const;
		bool uninstall(const QString &project) const;
		bool clean() const;

		QStringList libDirectories() const;

		QDir bin() const;
		QDir lib() const;
		QDir include() const;

		virtual QString binPath() const;
		virtual QString libPath() const;
		virtual QString includePath() const;

	private:
		bool removeDir(const QString &path) const;

		QString m_root;
	};
}

#endif