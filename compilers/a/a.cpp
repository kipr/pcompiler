#include "a.hpp"
#include "pcompiler/compilers.hpp"
#include "../common/platform.hpp"
#include "../common/options.hpp"

#include <QProcess>
#include <QDebug>
#include <QDir>

using namespace Compiler;

A::A()
	: Base("a", QStringList() << "a", -1)
{
}

OutputList A::transform(const QStringList &input, Options &options) const
{
	foreach(const QString &file, input)
		transform(file, options);
	return OutputList();
}

Output A::transform(const QString &file, Options &options) const
{
	QProcess compiler;
	Output ret;
	QFileInfo fileInfo(file);
	const QString dir = options.contains(TEMPORARY_DIR) ? options[TEMPORARY_DIR] : fileInfo.absolutePath();

	compiler.setProcessChannelMode(QProcess::ForwardedChannels);
	compiler.start(Platform::arPath(), QStringList() << "-xv" << file);
	if(!compiler.waitForStarted()) {
		ret = Output(Platform::arPath(), 1, "", "error: couldn't start the ar process\n");
		return ret;
	}
	compiler.waitForFinished();

	const QStringList filesList = files(file);
	QStringList pathsList;
	for(int i = 0; i < filesList.size(); ++i) {
		pathsList << (dir + "/" + filesList.at(i));
	}

	qDebug() << "dir contents:" << QDir(dir).entryList();
	
	ret.setFile(file);
	ret.setExitCode(compiler.exitCode());
	ret.setOutput(compiler.readAllStandardOutput());
	ret.setError(compiler.readAllStandardError());
	ret.setGeneratedFiles(pathsList);

	return ret;
}
QStringList A::files(const QString &file) const
{
	QProcess process;
	process.start(Platform::arPath(), QStringList() << "-t" << file);
	if(!process.waitForStarted()) {
		qDebug() << "Failed to start ar!";
		return QStringList();
	}
	process.waitForFinished();
	const QString output = QString(process.readAllStandardOutput()).trimmed();

	return output.split("\n");
}

REGISTER_COMPILER(A)