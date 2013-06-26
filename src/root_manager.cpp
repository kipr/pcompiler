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

	if(!binDir.exists() && !binDir.mkpath(".")) {
		return OutputList() << Output(m_root, 1, QByteArray(), "error: unable to create project bin dir");
	}
	
	if(!libDir.exists() && !libDir.mkpath(".")) {
		return OutputList() << Output(m_root, 1, QByteArray(), "error: unable to create project lib dir");
	}
	
	if(!includeDir.exists() && !includeDir.mkpath(".")) {
		return OutputList() << Output(m_root, 1, QByteArray(), "error: unable to create project include dir");
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
				case Output::LibraryTerminal:
					dest = libDir.filePath("lib" + (fileInfo.suffix().isEmpty()
						? project : project + "." + fileInfo.suffix()));
					break;
				case Output::HeaderTerminal:
					dest = includeDir.filePath(fileInfo.fileName());
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
	QStringList ret;
	const QFileInfoList &dirs = lib().entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
	foreach(const QFileInfo &dir, dirs) ret << dir.absoluteFilePath();

	return ret;
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

QStringList RootManager::projectLibPaths() const
{
	return lib().entryList(QDir::NoDot | QDir::NoDotDot | QDir::Dirs);
}

QDir RootManager::bin() const
{
	return QDir(binPath());
}

QDir RootManager::lib() const
{
	return QDir(libPath());
}

QDir RootManager::include() const
{
	return QDir(includePath());
}

QString RootManager::binPath() const
{
	QDir d(m_root);
	d.makeAbsolute();
	return d.filePath("bin");
}

QString RootManager::libPath() const
{
	QDir d(m_root);
	d.makeAbsolute();
	return d.filePath("lib");
}

QString RootManager::includePath() const
{
	QDir d(m_root);
	d.makeAbsolute();
	return d.filePath("include");
}