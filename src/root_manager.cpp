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
  
  // TODO: move the already running logic into ServerThread
  if(!ensureSetup(project)) return OutputList() << Output(m_root, 1, QByteArray(),
    "error: Unable to create root directories. Make sure that the target isn't already running a"
      "program, and that you have privileges to the working directory.");

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
          dest = board(project).filePath(fileInfo.fileName());
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
	success &= bin(project).removeRecursively();
	success &= lib(project).removeRecursively();
	success &= include(project).removeRecursively();
  success &= board(project).removeRecursively();

	return success;
}

bool RootManager::clean() const
{
	return archives().removeRecursively() && bin().removeRecursively() && lib().removeRecursively()
    && include().removeRecursively() && board().removeRecursively();
}

bool RootManager::ensureSetup(const QString &project) const
{
  bool success = true;
  QDir archivesDir(archives());
  QDir binDir(bin(project));
  QDir libDir(lib(project));
  QDir includeDir(include(project));
  QDir boardDir(board(project));
  
  if(!project.isEmpty()) {
    if(binDir.exists()) success &= binDir.removeRecursively();
    if(libDir.exists()) success &= libDir.removeRecursively();
    if(includeDir.exists()) success &= includeDir.removeRecursively();
    if(boardDir.exists()) success &= boardDir.removeRecursively();
  }
  
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
