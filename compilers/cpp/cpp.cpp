#include "cpp.hpp"
#include "pcompiler/compilers.hpp"
#include "../common/platform.hpp"
#include "../common/options.hpp"

#include <QFileInfo>
#include <QProcess>
#include <QDebug>

using namespace Compiler;

#define CPP_FLAGS "CPP_FLAGS"

Hpp::Hpp()
	: Passthrough("h++", QStringList() << "hpp" << "hxx" << "hh")
{}

REGISTER_COMPILER(Hpp)

Cpp::Cpp()
	: Base("c++", QStringList() << "cpp" << "cc" << "cxx", 0, QStringList() << CPP_FLAGS << TEMPORARY_DIR)
{
}

OutputList Cpp::transform(const QStringList& input, const Options& options) const
{
	OutputList ret;
	foreach(const QString& file, input) ret << transform(file, options);
	return ret;
}

Output Cpp::transform(const QString& file, const Options& options) const
{
	Output ret;
	QFileInfo fileInfo(file);
	ret.setFile(file);
	
	QProcess compiler;
	
	QString output = options.contains(TEMPORARY_DIR) ? options[TEMPORARY_DIR] : fileInfo.absolutePath();
	output += "/" + fileInfo.fileName() + ".o";

	QString rawFlags = options[CPP_FLAGS].trimmed();
	QStringList flags = OptionParser::arguments(rawFlags);
	compiler.start(Platform::cppPath(), flags << "-c" << file << "-o" << output);
	if(!compiler.waitForStarted()) {
		ret = Output(Platform::ccPath(), 1, "", "error: Couldn't start the C++ compiler.");
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