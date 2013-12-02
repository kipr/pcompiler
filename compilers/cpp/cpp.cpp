#include "cpp.hpp"
#include "pcompiler/compilers.hpp"
#include "../common/platform.hpp"
#include "../common/options.hpp"

#include <QFileInfo>
#include <QProcess>
#include <QDebug>

using namespace Compiler;

#define CPP_FLAGS "CPP_FLAGS"
#define PLATFORM_CPP_FLAGS Platform::platform() + "_" + CPP_FLAGS

Hpp::Hpp()
	: Passthrough("h++", QStringList() << "hpp" << "hxx" << "hh")
{}

OutputList Hpp::transform(const QStringList &input, Options &options) const
{
	Output ret;
	ret.setFiles(input);
	ret.setExitCode(0);
	ret.setGeneratedFiles(input);
	ret.setTerminal(Output::HeaderTerminal);
	return OutputList() << ret;
}

REGISTER_COMPILER(Hpp)

Cpp::Cpp()
	: Base("c++", QStringList() << "cpp" << "cc" << "cxx", 0, QStringList() << CPP_FLAGS << TEMPORARY_DIR)
{
}

OutputList Cpp::transform(const QStringList &input, Options &options) const
{
	options.insert(CPP_FLAGS, options.value(CPP_FLAGS).toString() + " \"-I${USER_ROOT}/include\"");
	options.expand();
	
	Options::const_iterator it = options.find(PLATFORM_CPP_FLAGS);
	if(it != options.end()) {
		options.insert(CPP_FLAGS, options.value(CPP_FLAGS).toString() + " " + it.value().toString());
	}

	OutputList ret;
	foreach(const QString &file, input) ret << transform(file, options);
	return ret;
}

Output Cpp::transform(const QString &file, Options &options) const
{
	QProcess compiler;
	Output ret;
	QFileInfo fileInfo(file);
	ret.setFile(file);
	
	QString output = options.contains(TEMPORARY_DIR) ? options[TEMPORARY_DIR].toString() : fileInfo.absolutePath();
	output += "/" + fileInfo.fileName() + ".o";

	QString rawFlags = options[CPP_FLAGS].toString().trimmed();
	QStringList flags = OptionParser::arguments(rawFlags);
	compiler.start(Platform::cppPath(), flags <<
#ifndef _WIN32
		"-fPIC" <<
#endif
		"-c" << file << "-o" << output);
	if(!compiler.waitForStarted()) {
		ret = Output(Platform::ccPath(), 1, "", "error: couldn't start the C++ compiler\n");
		return ret;
	}
	compiler.waitForFinished();
	
	ret.setExitCode(compiler.exitCode());
	ret.setOutput(compiler.readAllStandardOutput());
	ret.setError(compiler.readAllStandardError());
	ret.setGeneratedFiles(QStringList() << output);
	
	return ret;
}

REGISTER_COMPILER(Cpp)