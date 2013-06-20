#include "o.hpp"
#include "pcompiler/compilers.hpp"
#include "../common/platform.hpp"
#include "../common/options.hpp"

#include <QFileInfo>
#include <QProcess>
#include <QDebug>

using namespace Compiler;

#define O_FLAGS "LD_FLAGS"

O::O()
	: Base("ld (c++)", QStringList() << "o", 1, QStringList() << O_FLAGS << OUTPUT_DIR)
{
}

Output O::produceBinary(const QStringList& input, Options& options) const
{
	QProcess linker;
	Output ret;
	ret.setFiles(input);
	
	const QString& ext = Platform::exeExtension();
	QString name = input.size() == 1 ? QFileInfo(input[0]).baseName() : "application";
	QString output = (options.contains(OUTPUT_DIR) ? options[OUTPUT_DIR] : QFileInfo(input[0]).absolutePath())
		+ "/" + name + (ext.isEmpty() ? "" : "." + ext);
	
	QString rawFlags = options[O_FLAGS].trimmed();
	QStringList flags = OptionParser::arguments(rawFlags);
	//qDebug() << "LD_FLAGS" << flags;

	QStringList args;
	args << (input + flags) << "-o" << output;
	Options::const_iterator it = options.find("PROJECT_DEPS");
	if(it != options.end()) {
		foreach(QString dep, OptionParser::arguments(it.value())) {
			args << "-L/kovan/prefix/lib/" + dep; // FIXME: This path shouldn't be hard-coded!
			args << "-l" + dep;
		}
	}
	//qDebug() << "ld" << args;
	
	linker.start(Platform::cppPath(), args);
	if(!linker.waitForStarted()) {
		ret = Output(Platform::ccPath(), 1, "", "ERROR: Couldn't start linker to produce binary.\n");
		return ret;
	}
	linker.waitForFinished();
	
	ret.setExitCode(linker.exitCode());
	ret.setOutput(linker.readAllStandardOutput());
	ret.setError(linker.readAllStandardError());
	ret.setGeneratedFiles(QStringList() << output);
	ret.setTerminal(Output::BinaryTerminal);

	return ret;
}

Output O::produceLibrary(const QStringList& input, Options& options) const
{
	QProcess linker;
	Output ret;
	ret.setFiles(input);
	
	const QString& ext = "so";
	QString name = input.size() == 1 ? QFileInfo(input[0]).baseName() : "library";
	QString output = (options.contains(OUTPUT_DIR) ? options[OUTPUT_DIR] : QFileInfo(input[0]).absolutePath())
		+ "/" + name + (ext.isEmpty() ? "" : "." + ext);

	QStringList args;
	args << "-shared" << "-o" << output << input;

	linker.start(Platform::cppPath(), args);
	if(!linker.waitForStarted()) {
		ret = Output(Platform::ccPath(), 1, "", "ERROR: Couldn't start linker to produce library.\n");
		return ret;
	}
	linker.waitForFinished();
	
	ret.setExitCode(linker.exitCode());
	ret.setOutput(linker.readAllStandardOutput());
	ret.setError(linker.readAllStandardError());
	ret.setGeneratedFiles(QStringList() << output);
	ret.setTerminal(Output::LibraryTerminal);

	return ret;
}

OutputList O::transform(const QStringList& input, Options& options) const
{
	return OutputList() << produceBinary(input, options) << produceLibrary(input, options);
}

REGISTER_COMPILER(O)