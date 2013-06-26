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
	QString name = input.size() == 1 ? QFileInfo(input[0]).baseName() : "executable";
	QString output = (options.contains(OUTPUT_DIR) ? options[OUTPUT_DIR] : QFileInfo(input[0]).absolutePath())
		+ "/" + name + (ext.isEmpty() ? "" : "." + ext);
	
	Options localOptions = options;
	
	Options::const_iterator it = options.find("PROJECT_DEPS");
	if(it != options.end()) {
		QStringList addOFlags;
		foreach(QString dep, OptionParser::arguments(it.value())) {
			addOFlags << "\"-L${USER_ROOT}/lib/" + dep + "\"";
			addOFlags << "\"-l" + dep + "\"";
		}
		localOptions.insert(O_FLAGS, options.value(O_FLAGS) + addOFlags.join(" "));
		localOptions.expand();
	}

	QString rawFlags = localOptions[O_FLAGS].trimmed();
	QStringList flags = OptionParser::arguments(rawFlags);

	QStringList args;
	args << (input + flags) << "-o" << output;

	linker.start(Platform::cppPath(), args);
	if(!linker.waitForStarted()) {
		return Output(Platform::ccPath(), 1, "", "error: couldn't start linker to produce binary\n");
	}
	linker.waitForFinished();
	
	ret.setExitCode(linker.exitCode());
	ret.setOutput(linker.readAllStandardOutput());
	ret.setError(linker.readAllStandardError());
	ret.setGeneratedFiles(QStringList() << output);
	ret.setTerminal(Output::BinaryTerminal);

	return ret;
}

Output O::produceLibrary(const QStringList& input, Options &options) const
{
	QProcess linker;
	Output ret;
	ret.setFiles(input);
	
	Options localOptions = options;
	
	const QString ext = Platform::libExtension();
	const QString name = input.size() == 1 ? QFileInfo(input[0]).baseName() : "library";
	const QString output = (options.contains(OUTPUT_DIR) ? options[OUTPUT_DIR] : QFileInfo(input[0]).absolutePath())
		+ "/" + name + (ext.isEmpty() ? "" : "." + ext);
	
	Options::const_iterator it = options.find("PROJECT_DEPS");
	if(it != options.end()) {
		QStringList addOFlags;
		foreach(QString dep, OptionParser::arguments(it.value())) {
			addOFlags << "\"-L${USER_ROOT}/lib/" + dep + "\"";
			addOFlags << "\"-l" + dep + "\"";
			qDebug() << "ADDING OPTIONS for " << name;
		}
		QString oFlags = localOptions.value(O_FLAGS);
		qDebug() << "SDFADFASD" << oFlags;
		localOptions.insert(O_FLAGS, oFlags + " " + addOFlags.join(" "));
		localOptions.expand();
	}

	QString rawFlags = localOptions[O_FLAGS].trimmed();
	qDebug() << rawFlags;
	QStringList flags = OptionParser::arguments(rawFlags);
	qDebug() << flags;

	QStringList args;
	args << "-shared" << "-o" << output << input << flags;

	linker.start(Platform::cppPath(), args);
	if(!linker.waitForStarted()) {
		return Output(Platform::ccPath(), 1, "", "error: couldn't start linker to produce library\n");
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