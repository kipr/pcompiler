#include "class.hpp"
#include "pcompiler/compilers.hpp"
#include "../common/platform.hpp"
#include "pcompiler/compiler_options.hpp"

#include <QFileInfo>
#include <QProcess>
#include <QDebug>

using namespace Compiler;

Class::Class()
	: Base("jar", QStringList() << "class", 1, QStringList() << OUTPUT_DIR)
{
}

OutputList Class::transform(const QStringList &input, Options &options) const
{
	Output ret;
	ret.setFiles(input);
	
	QProcess compiler;
	QString output = (options.contains(OUTPUT_DIR) ? options[OUTPUT_DIR].toString() : QFileInfo(input[0]).absolutePath() + "/a.jar");
	
	compiler.start(jarPath(), (QStringList() << "cvf" << output) + input);
	if(!compiler.waitForStarted()) {
		ret = Output(Platform::ccPath(), 1, "", "error: couldn't start the java archiver");
		return OutputList() << ret;
	}
	compiler.waitForFinished();
	
	ret.setExitCode(compiler.exitCode());
	ret.setOutput(compiler.readAllStandardOutput());
	ret.setError(compiler.readAllStandardError());
	ret.setGeneratedFiles(QStringList() << output);
  ret.setTerminal(Output::BinaryTerminal);
	
	return OutputList() << ret;
}

QString Class::jarPath()
{
	return "jar";
}

REGISTER_COMPILER(Class)