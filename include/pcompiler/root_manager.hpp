#ifndef _PCOMPILER_ROOT_MANAGER_HPP_
#define _PCOMPILER_ROOT_MANAGER_HPP_

#include "output.hpp"
#include "compat.hpp"

#include <QDir>

namespace Compiler
{
	class DLL_EXPORT RootManager
	{
	public:
		RootManager(const QString &root);

		Compiler::OutputList install(const Compiler::OutputList &output, const QString &project) const;
		bool uninstall(const QString &project) const;
		bool clean() const;
    bool ensureSetup(const QString &project = QString()) const;

		QStringList libDirectories() const;
		QStringList libDirectoryPaths() const;

    QDir archives(const QString &name = QString()) const;
		QDir bin(const QString &name = QString()) const;
		QDir lib(const QString &name = QString()) const;
		QDir include(const QString &name = QString()) const;
    QDir board(const QString &name = QString()) const;

    virtual QString archivesPath(const QString &name = QString()) const;
		virtual QString binPath(const QString &name = QString()) const;
		virtual QString libPath(const QString &name = QString()) const;
		virtual QString includePath(const QString &name = QString()) const;
		virtual QString boardPath(const QString &name = QString()) const;

	private:
		bool removeDir(const QString &path) const;

		QString m_root;
	};
}

#endif