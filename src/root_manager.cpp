#include "pcompiler/root_manager.hpp"

#include <QDir>
#include <QDebug>

using namespace Compiler;

RootManager::RootManager(const QString &root)
	:m_root(root)
{

}

Compiler::OutputList RootManager::install(const Compiler::OutputList &output, const QString &project) const
{
	using namespace Compiler;
	
	const QDir binDir(bin().filePath(project));
	const QDir libDir(lib().filePath(project));
	const QDir includeDir(include().filePath(project));
  const QDir boardDir = board();

	if(!binDir.exists() && !binDir.mkpath(".")) {
		return OutputList() << Output(m_root, 1, QByteArray(), "error: unable to create project bin dir");
	}
	
	if(!libDir.exists() && !libDir.mkpath(".")) {
		return OutputList() << Output(m_root, 1, QByteArray(), "error: unable to create project lib dir");
	}
	
	if(!includeDir.exists() && !includeDir.mkpath(".")) {
		return OutputList() << Output(m_root, 1, QByteArray(), "error: unable to create project include dir");
	}
  
	if(!boardDir.exists() && !boardDir.mkpath(".")) {
		return OutputList() << Output(m_root, 1, QByteArray(), "error: unable to create project board dir");
	}

	foreach(const Output &term, output) {
		if(!term.isTerminal() || !term.isSuccess()) continue;
		foreach(const QString &file, term.generatedFiles()) {
			const QFileInfo fileInfo(file);
			QString dest;
			switch(term.terminal()) {
				case Output::NotTerminal: break;
				case Output::BinaryTerminal:
					dest = binDir.filePath(fileInfo.suffix().isEmpty()
						? project : project + "." + fileInfo.suffix());
					break;
				case Output::DependencyTerminal:
					dest = libDir.filePath("lib" + (fileInfo.suffix().isEmpty()
						? project : project + "." + fileInfo.suffix()));
					break;
				case Output::LibraryTerminal:
					dest = libDir.filePath(fileInfo.fileName());
					break;
				case Output::HeaderTerminal:
					dest = includeDir.filePath(fileInfo.fileName());
					break;
        case Output::BoardTerminal:
          dest = board().filePath(fileInfo.fileName());
          break;
				default:
					qDebug() << "Warning: unhandled terminal type";
					break;
			}
			
			qDebug() << "Removing " << dest;
			if(QFile::exists(dest) && !QFile::remove(dest)) {
				return OutputList() << Output(m_root, 1, QByteArray(),
					("error: failed to remove " + dest).toLatin1());
			}
			if(!QFile::copy(fileInfo.absoluteFilePath(), dest)) {
				return OutputList() << Output(m_root, 1, QByteArray(),
					("error: failed to copy " + fileInfo.absoluteFilePath() + " to "
					+ dest).toLatin1());
			}
			qDebug() << fileInfo.absoluteFilePath() << " -> " << dest;
		}
	}

	return OutputList();
}

bool RootManager::uninstall(const QString &project) const
{
	bool success = true;
	const QDir projBinDir(bin().filePath(project));
	const QDir projLibDir(lib().filePath(project));
	const QDir projIncludeDir(include().filePath(project));
	
	success &= removeDir(projBinDir.absolutePath());
	success &= removeDir(projLibDir.absolutePath());
	success &= removeDir(projIncludeDir.absolutePath());

	return success;
}

bool RootManager::clean() const
{
	QDir rootDir(m_root);
	return removeDir(binPath()) && removeDir(libPath()) && removeDir(includePath());
}

QStringList RootManager::libDirectories() const
{
	return lib().entryList(QDir::Dirs | QDir::NoDotAndDotDot);
}

QStringList RootManager::libDirectoryPaths() const
{
	QStringList paths;
	QFileInfoList infos = lib().entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
	foreach(const QFileInfo &info, infos) paths << info.absoluteFilePath();

	return paths;
}

bool RootManager::removeDir(const QString &path) const
{
	bool success = true;
	QDir directory(path);

	QFileInfoList files = directory.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
	foreach(const QFileInfo &file, files) success &= directory.remove(file.fileName());

	QFileInfoList dirs = directory.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
	foreach(const QFileInfo &dir, dirs) success &= removeDir(dir.absoluteFilePath());

	success &= directory.rmdir(directory.absolutePath());

	return success;
}

QDir RootManager::bin(const QString &name) const
{
	return QDir(binPath(name));
}

QDir RootManager::lib(const QString &name) const
{
	return QDir(libPath(name));
}

QDir RootManager::include(const QString &name) const
{
	return QDir(includePath(name));
}

QDir RootManager::board(const QString &name) const
{
  return QDir(boardPath(name));
}

QString RootManager::binPath(const QString &name) const
{
	QDir d(m_root);
	d.makeAbsolute();
	return d.filePath("bin" + (name.isEmpty() ? QString() : ("/" + name)));
}

QString RootManager::libPath(const QString &name) const
{
	QDir d(m_root);
	d.makeAbsolute();
	return d.filePath("lib" + (name.isEmpty() ? QString() : ("/" + name)));
}

QString RootManager::includePath(const QString &name) const
{
	QDir d(m_root);
	d.makeAbsolute();
	return d.filePath("include" + (name.isEmpty() ? QString() : ("/" + name)));
}

QString RootManager::boardPath(const QString &name) const
{
	QDir d(m_root);
	d.makeAbsolute();
	return d.filePath("board" + (name.isEmpty() ? QString() : ("/" + name)));
}
