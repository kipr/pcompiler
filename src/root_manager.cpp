#include "pcompiler/root_manager.hpp"

#include <QDir>
#include <QDebug>
#include "../compilers/common/platform.hpp"

using namespace Compiler;

RootManager::RootManager(const QString &root)
	:m_root(root)
{

}

Compiler::OutputList RootManager::install(const Compiler::OutputList &output, const QString &project) const
{
	using namespace Compiler;
  
  if(!ensureSetup(project)) return OutputList() << Output(m_root, 1, QByteArray(),
    "error: unable to create root directories");

	foreach(const Output &term, output) {
		if(!term.isTerminal() || !term.isSuccess()) continue;
		foreach(const QString &file, term.generatedFiles()) {
			const QFileInfo fileInfo(file);
			QString dest;
			switch(term.terminal()) {
				case Output::NotTerminal: break;
				case Output::BinaryTerminal:
					dest = bin(project).filePath(fileInfo.suffix().isEmpty()
						? project : project + "." + fileInfo.suffix());
					break;
				case Output::DependencyTerminal:
					dest = lib(project).filePath(Platform::libPrefix() + (fileInfo.suffix().isEmpty()
						? project : project + "." + fileInfo.suffix()));
					break;
				case Output::LibraryTerminal:
					dest = lib(project).filePath(fileInfo.fileName());
					break;
				case Output::HeaderTerminal:
					dest = include(project).filePath(fileInfo.fileName());
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
	
  success &= archives().remove(project);
	success &= removeDir(binPath(project));
	success &= removeDir(libPath(project));
  success &= removeDir(includePath(project));

	return success;
}

bool RootManager::clean() const
{
	return removeDir(archivesPath()) && removeDir(binPath()) && removeDir(libPath()) &&
    removeDir(includePath()) && removeDir(boardPath());
}

bool RootManager::ensureSetup(const QString &project) const
{
  bool success = true;
  QDir archivesDir(archives());
  QDir binDir(bin(project));
  QDir libDir(lib(project));
  QDir includeDir(include(project));
  QDir boardDir(board());
  
  if(binDir.exists()) success &= removeDir(binDir.path());
  if(libDir.exists()) success &= removeDir(libDir.path());
  if(includeDir.exists()) success &= removeDir(includeDir.path());
  
  success &= archivesDir.exists() || archivesDir.mkpath(".");
  success &= binDir.exists() || binDir.mkpath(".");
  success &= libDir.exists() || libDir.mkpath(".");
  success &= includeDir.exists() || includeDir.mkpath(".");
  success &= boardDir.exists() || boardDir.mkpath(".");
  
  return success;
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

QDir RootManager::archives(const QString &name) const
{
  return QDir(archivesPath(name));
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

QString RootManager::archivesPath(const QString &name) const
{
  QDir d(m_root);
  d.makeAbsolute();
  return d.filePath("archives/" + (name.isEmpty() ? QString() : name));
}

QString RootManager::binPath(const QString &name) const
{  
  QDir d(m_root);
  d.makeAbsolute();
  return d.filePath("bin/" + (name.isEmpty() ? QString() : name));
}

QString RootManager::libPath(const QString &name) const
{
  QDir d(m_root);
  d.makeAbsolute();
  return d.filePath("lib/" + (name.isEmpty() ? QString() : name));
}

QString RootManager::includePath(const QString &name) const
{
  QDir d(m_root);
  d.makeAbsolute();
  return d.filePath("include/" + (name.isEmpty() ? QString() : name));
}

QString RootManager::boardPath(const QString &name) const
{
  QDir d(m_root);
  d.makeAbsolute();
  return d.filePath("board/" + (name.isEmpty() ? QString() : name));
}
