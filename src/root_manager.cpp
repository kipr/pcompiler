#include "pcompiler/root_manager.hpp"

#include <QDir>
#include <QDebug>

using namespace Compiler;

Compiler::OutputList RootManager::install(const Compiler::OutputList &output, const QString &root, const QString &project)
{
	using namespace Compiler;
	
	const QDir bin(RootManager::bin(root).filePath(project));
	const QDir lib(RootManager::lib(root).filePath(project));
	const QDir include(RootManager::include(root).filePath(project));

	if(!bin.exists() && !bin.mkpath(".")) {
		return OutputList() << Output(root, 1, QByteArray(), "error: unable to create project bin dir");
	}
	
	if(!lib.exists() && !lib.mkpath(".")) {
		return OutputList() << Output(root, 1, QByteArray(), "error: unable to create project lib dir");
	}
	
	if(!include.exists() && !include.mkpath(".")) {
		return OutputList() << Output(root, 1, QByteArray(), "error: unable to create project include dir");
	}

	foreach(const Output &term, output) {
		if(!term.isTerminal() || !term.isSuccess()) continue;
		foreach(const QString &file, term.generatedFiles()) {
			const QFileInfo fileInfo(file);
			QString dest;
			switch(term.terminal()) {
				case Output::NotTerminal: break;
				case Output::BinaryTerminal:
					dest = bin.filePath(fileInfo.suffix().isEmpty() ? project : project + "."
						+ fileInfo.suffix());
					break;
				case Output::LibraryTerminal:
					dest = lib.filePath("lib" + (fileInfo.suffix().isEmpty()
						? project : project + "." + fileInfo.suffix()));
					break;
				case Output::HeaderTerminal:
					dest = include.filePath(fileInfo.fileName());
					break;
				default:
					qDebug() << "Warning: unhandled terminal type";
					break;
			}
			
			qDebug() << "Removing " << dest;
			if(!QDir(dest).remove("")) {
				return OutputList() << Output(root, 1, QByteArray(),
					("error: failed to remove " + dest).toLatin1());
			}
			if(!QFile::copy(fileInfo.absoluteFilePath(), dest)) {
				return OutputList() << Output(root, 1, QByteArray(),
					("error: failed to copy " + fileInfo.absoluteFilePath() + " to "
					+ dest).toLatin1());
			}
			qDebug() << fileInfo.absoluteFilePath() << " -> " << dest;
		}
	}

	return OutputList();
}

bool RootManager::uninstall(const QString &root, const QString &project)
{
	bool success = true;
	const QDir projBinDir(QDir(binPath(root)).filePath(project));
	const QDir projLibDir(QDir(libPath(root)).filePath(project));
	const QDir projIncludeDir(QDir(includePath(root)).filePath(project));
	
	success &= removeDir(projBinDir.absolutePath());
	success &= removeDir(projLibDir.absolutePath());
	success &= removeDir(projIncludeDir.absolutePath());

	return success;
}

bool RootManager::clean(const QString &root)
{
	QDir rootDir(root);
	return removeDir(binPath(root)) && removeDir(libPath(root)) && removeDir(includePath(root));
}

bool RootManager::removeDir(const QString &path)
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

QDir RootManager::bin(const QString &root)
{
	return QDir(binPath(root));
}

QDir RootManager::lib(const QString &root)
{
	return QDir(libPath(root));
}

QDir RootManager::include(const QString &root)
{
	return QDir(includePath(root));
}

QString RootManager::binPath(const QString &root)
{
	QDir d(root);
	d.makeAbsolute();
	return d.filePath("bin");
}

QString RootManager::libPath(const QString &root)
{
	QDir d(root);
	d.makeAbsolute();
	return d.filePath("lib");
}

QString RootManager::includePath(const QString &root)
{
	QDir d(root);
	d.makeAbsolute();
	return d.filePath("include");
}