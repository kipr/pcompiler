#include "java.hpp"
#include "pcompiler/compilers.hpp"
#include "../common/platform.hpp"
#include "pcompiler/compiler_options.hpp"

#include <QFileInfo>
#include <QProcess>
#include <QDebug>

using namespace Compiler;

Java::Java()
	: Base("java", QStringList() << "java", -1, QStringList() << JAVAC_FLAGS << TEMPORARY_DIR)
{
}

OutputList Java::transform(const QStringList &input, Options &options) const
{
	Output ret;
	ret.setFiles(input);
	
	QProcess compiler;
	
	QString rawFlags = options[JAVAC_FLAGS].toString().trimmed();
	QStringList flags = OptionParser::arguments(rawFlags);
	compiler.start(javacPath(), flags + input);
	if(!compiler.waitForStarted()) {
		ret = Output(Platform::ccPath(), 1, "", "error: couldn't start the Java compiler");
		return OutputList() << ret;
	}
	compiler.waitForFinished();
	
	ret.setExitCode(compiler.exitCode());
	ret.setOutput(compiler.readAllStandardOutput());
	ret.setError(compiler.readAllStandardError());
	ret.setGeneratedFiles(QStringList(input).replaceInStrings(QRegExp("\\.java$"), ".class"));
	
	return OutputList() << ret;
}

QString Java::javacPath()
{
	return "/usr/bin/javac";
}

REGISTER_COMPILER(Java)