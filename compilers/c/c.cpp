#include "c.hpp"
#include "pcompiler/compilers.hpp"
#include "../common/platform.hpp"
#include "../common/options.hpp"

#include <QFileInfo>
#include <QProcess>
#include <QDebug>

using namespace Compiler;

#define C_FLAGS "C_FLAGS"
#define PLATFORM_C_FLAGS Platform::platform() + "_" + C_FLAGS

H::H()
	: Passthrough("h", QStringList() << "h")
{}

OutputList H::transform(const QStringList &input, Options &options) const
{
	Output ret;
	ret.setFiles(input);
	ret.setExitCode(0);
	ret.setGeneratedFiles(input);
	ret.setTerminal(Output::HeaderTerminal);
	return OutputList() << ret;
}

REGISTER_COMPILER(H)

C::C()
	: Base("c", QStringList() << "c", 0, QStringList() << C_FLAGS << TEMPORARY_DIR)
{
}

OutputList C::transform(const QStringList &input, Options &options) const
{
	options.insert(C_FLAGS, options.value(C_FLAGS) + " \"-I${USER_ROOT}/include\"");
	options.expand();

	Options::const_iterator it = options.find(PLATFORM_C_FLAGS);
	if(it != options.end()) {
		options.insert(C_FLAGS, options.value(C_FLAGS) + " " + it.value());
	}

	OutputList ret;
	foreach(const QString &file, input) ret << transform(file, options);
	return ret;
}

Output C::transform(const QString &file, Options &options) const
{
	QProcess compiler;
	Output ret;
	QFileInfo fileInfo(file);
	ret.setFile(file);
	
	QString output = options.contains(TEMPORARY_DIR) ? options[TEMPORARY_DIR] : fileInfo.absolutePath();
	output += "/" + fileInfo.fileName() + ".o";

	QString rawFlags = options[C_FLAGS].trimmed();
	QStringList flags = OptionParser::arguments(rawFlags);
	compiler.start(Platform::ccPath(), flags <<
#ifndef _WIN32
		"-fPIC" <<
#endif
		"-c" << file << "-o" << output);
	if(!compiler.waitForStarted()) {
		ret = Output(Platform::ccPath(), 1, "", "error: couldn't start the C compiler\n");
		return ret;
	}
	compiler.waitForFinished();
	
	ret.setExitCode(compiler.exitCode());
	ret.setOutput(compiler.readAllStandardOutput());
	ret.setError(compiler.readAllStandardError());
	ret.setGeneratedFiles(QStringList() << output);
	
	return ret;
}

REGISTER_COMPILER(C)