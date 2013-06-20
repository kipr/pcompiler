#include "pcompiler/root_manager.hpp"

#include <QDir>
#include <QDebug>

bool RootManager::install(Compiler::OutputList outputs, const QString &root, const QString &project)
{
	using namespace Compiler;

	QDir rootDir(root);
	const QString &binDir = "bin/" + project + "/";
	const QString &libDir = "lib/" + project + "/";
	const QString &includeDir = "include/" + project + "/";

	if(!rootDir.exists(binDir) && !rootDir.mkpath(binDir)) return false;
	if(!rootDir.exists(libDir) && !rootDir.mkpath(libDir)) return false;
	if(!rootDir.exists(includeDir) && !rootDir.mkpath(includeDir)) return false;

	foreach(const Output& out, outputs) {
		foreach(QString file, out.generatedFiles()) {
			QFileInfo fileInfo(file);
			QString relativeDest;
			switch(out.terminal()) {
				case Output::NotTerminal:
					qDebug() << "Warning: skipping non-terminal output";
					break;
				case Output::BinaryTerminal:
					relativeDest = binDir + (fileInfo.suffix().isEmpty() ? project : project + "." + fileInfo.suffix());
					break;
				case Output::LibraryTerminal:
					relativeDest = libDir + "lib" + (fileInfo.suffix().isEmpty() ? project : project + "." + fileInfo.suffix());
					break;
				case Output::HeaderTerminal:
					relativeDest = includeDir + fileInfo.fileName();
					break;
				default:
					qDebug() << "Warning: unhandled terminal type";
			}
			rootDir.remove(relativeDest);
			if(!QFile::copy(fileInfo.absoluteFilePath(), rootDir.absoluteFilePath(relativeDest))) {
				qDebug() << "Error: failed to copy" << fileInfo.absoluteFilePath() << "to" << rootDir.absoluteFilePath(relativeDest);
				return false;
			}
		}
	}

	return true;
}

bool RootManager::uninstall(const QString &project) {
	return false;
}

bool RootManager::clean() {
	return false;
}
