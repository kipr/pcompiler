#include "c.hpp"
#include "pcompiler/compilers.hpp"
#include "../common/platform.hpp"
#include "../common/options.hpp"

#include <QFileInfo>
#include <QProcess>
#include <QDebug>

using namespace Compiler;

#define C_FLAGS "C_FLAGS"

H::H()
	: Passthrough("h", QStringList() << "h")
{}

REGISTER_COMPILER(H)

C::C()
	: Base("c", QStringList() << "c", 0, QStringList() << C_FLAGS << TEMPORARY_DIR)
{
}

OutputList C::transform(const QStringList& input, const Options& options) const
{
	OutputList ret;
	foreach(const QString& file, input) ret << transform(file, options);
	return ret;
}

Output C::transform(const QString& file, const Options& options) const
{
	Output ret;
	QFileInfo fileInfo(file);
	ret.setFile(file);
	
	QProcess compiler;
	
	QString output = options.contains(TEMPORARY_DIR) ? options[TEMPORARY_DIR] : fileInfo.absolutePath();
	output += "/" + fileInfo.fileName() + ".o";
	
	qDebug() << "Starting compile with" << Platform::ccPath();

	QString rawFlags = options[C_FLAGS].trimmed();
	QStringList flags = rawFlags.isEmpty() ? QStringList() : rawFlags.split(" ");
	compiler.start(Platform::ccPath(), flags << "-c" << file << "-o" << output);
	if(!compiler.waitForStarted()) {
		qDebug() << "Couldn't start" << Platform::ccPath();
		ret = Output(Platform::ccPath(), 1, "", "error: Couldn't start gcc");
		return ret;
	}
	compiler.waitForFinished();

	qDebug() << "gcc exited with" << compiler.exitCode();
	
	ret.setExitCode(compiler.exitCode());
	ret.setOutput(compiler.readAllStandardOutput());
	ret.setError(compiler.readAllStandardError());
	ret.setGeneratedFiles(QStringList() << output);
	
	return ret;
}

REGISTER_COMPILER(C)