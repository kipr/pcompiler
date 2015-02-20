#include "c.hpp"
#include "pcompiler/compilers.hpp"
#include "../common/platform.hpp"
#include "pcompiler/compiler_options.hpp"

#include <QFileInfo>
#include <QProcess>
#include <QDebug>

using namespace Compiler;

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
	: Base("c", QStringList() << "c", 1, QStringList() << C_FLAGS << TEMPORARY_DIR)
{
}

OutputList C::transform(const QStringList &input, Options &options) const
{
	options.insert(C_FLAGS, options.value(C_FLAGS).toString() + " \"-I${USER_ROOT}/include\"");
	options.expand();

	Options::const_iterator it = options.find(PLATFORM_C_FLAGS);
	if(it != options.end()) {
		options.insert(C_FLAGS, options.value(C_FLAGS).toString() + " " + it.value().toString());
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
	
	QString output = options.contains(TEMPORARY_DIR) ? options[TEMPORARY_DIR].toString() : fileInfo.absolutePath();
	output += "/" + fileInfo.fileName() + ".o";

	QString rawFlags = options[C_FLAGS].toString().trimmed();
	QStringList flags = OptionParser::arguments(rawFlags);
	qDebug () << "CALLING C COMPILER WITH FLAGS:" << flags << "-c" << file << "-o" << output;
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