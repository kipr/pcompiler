#include "pcompiler/root_manager.hpp"

#include <QDir>
#include <QDebug>

Compiler::OutputList RootManager::install(const Compiler::OutputList &terminals, const QString &root, const QString &project)
{
	using namespace Compiler;

	QDir rootDir(root);
	
	const QString binDir = "bin/" + project + "/";
	const QString libDir = "lib/" + project + "/";
	const QString includeDir = "include/" + project + "/";

	if((!rootDir.exists(binDir) && !rootDir.mkpath(binDir)) ||
		(!rootDir.exists(libDir) && !rootDir.mkpath(libDir)) ||
		(!rootDir.exists(includeDir) && !rootDir.mkpath(includeDir))) {
		return OutputList() << Output(root, 1, QByteArray(), "error: unable to create required directory");
	}

	foreach(const Output &term, terminals) {
		foreach(QString file, term.generatedFiles()) {
			QFileInfo fileInfo(file);
			QString relativeDest;
			switch(term.terminal()) {
				case Output::NotTerminal:
					break;
				case Output::BinaryTerminal:
					relativeDest = binDir + (fileInfo.suffix().isEmpty() ? project : project + "."
						+ fileInfo.suffix());
					break;
				case Output::LibraryTerminal:
					relativeDest = libDir + "lib" + (fileInfo.suffix().isEmpty() ? project : project
						+ "." + fileInfo.suffix());
					break;
				case Output::HeaderTerminal:
					relativeDest = includeDir + fileInfo.fileName();
					break;
				default:
					qDebug() << "Warning: unhandled terminal type";
			}
			rootDir.remove(relativeDest);
			if(!QFile::copy(fileInfo.absoluteFilePath(), rootDir.absoluteFilePath(relativeDest))) {
				return OutputList() << Output(root, 1, QByteArray(),
					("error: failed to copy " + fileInfo.absoluteFilePath() + " to "
						+ rootDir.absoluteFilePath(relativeDest)).toLatin1());
			}
		}
	}

	return OutputList();
}

bool RootManager::uninstall(const QString &root, const QString &project)
{
	bool success = true;
	QDir rootDir(root);
	
	if(rootDir.cd("bin/" + project)) {
		success &= removeDir(rootDir.absolutePath());
		rootDir.cd("../..");
	}
	if(rootDir.cd("lib/" + project)) {
		success &= removeDir(rootDir.absolutePath());
		rootDir.cd("../..");
	}
	if(rootDir.cd("include/" + project)) {
		success &= removeDir(rootDir.absolutePath());
		rootDir.cd("../..");
	}

	return success;
}

bool RootManager::clean(const QString &root)
{
	QDir rootDir(root);
	return removeDir(rootDir.filePath("bin/")) &&
		removeDir(rootDir.filePath("lib/")) &&
		removeDir(rootDir.filePath("include/"));
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