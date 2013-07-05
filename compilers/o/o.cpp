#include "o.hpp"
#include "pcompiler/compilers.hpp"
#include "../common/platform.hpp"
#include "../common/options.hpp"

#include <QFileInfo>
#include <QDir>
#include <QProcess>
#include <QDebug>

using namespace Compiler;

#define O_FLAGS "LD_FLAGS"
#define PLATFORM_O_FLAGS Platform::platform() + "_" + O_FLAGS
#define PROJECT_DEPS "PROJECT_DEPS"
#define TERMINAL_TYPE "TERMINAL_TYPE"
#define LIBRARY "LIBRARY"
#define LIBRARY_NAME "LIBRARY_NAME"

O::O()
	: Base("ld (c++)", QStringList() << "o", 1, QStringList()
		<< O_FLAGS << OUTPUT_DIR << LIBRARY_NAME)
{
}

Output O::produceBinary(const QStringList &input, Options &options) const
{
	QProcess linker;
	Output ret;
	ret.setFiles(input);

	Options localOptions = options;

	Options::const_iterator it = localOptions.find(PLATFORM_O_FLAGS);
	if(it != localOptions.end()) {
		localOptions.insert(O_FLAGS, localOptions.value(O_FLAGS) + " " + it.value());
	}

	Options::const_iterator depsIt = localOptions.find(PROJECT_DEPS);
	if(depsIt != localOptions.end()) {
		QStringList addOFlags;
		foreach(const QString &dep, OptionParser::arguments(depsIt.value())) {
			addOFlags << "\"-L${USER_ROOT}/lib/" + dep + "\"";
			addOFlags << "\"-l" + dep + "\"";
		}
		localOptions.insert(O_FLAGS, localOptions.value(O_FLAGS) + " " + addOFlags.join(" "));
		localOptions.expand();
	}

	QString rawFlags = localOptions[O_FLAGS].trimmed();
	QStringList flags = OptionParser::arguments(rawFlags);

	const QString ext = Platform::exeExtension();
	const QString name = localOptions.contains(LIBRARY_NAME) ?
		localOptions[LIBRARY_NAME] :
		(input.size() == 1 ? QFileInfo(input[0]).baseName() : "executable");
	const QString output = (localOptions.contains(OUTPUT_DIR) ? localOptions[OUTPUT_DIR] : QFileInfo(input[0]).absolutePath())
		+ "/" + name + (ext.isEmpty() ? "" : "." + ext);

	QStringList args;
	args << (input + flags) << "-o" << output;

	linker.start(Platform::cppPath(), args);
	if(!linker.waitForStarted()) {
		return Output(Platform::cppPath(), 1, "", "error: couldn't start linker to produce binary\n");
	}
	linker.waitForFinished();
	
	// Fix rpath
#ifdef Q_OS_MAC
	QProcess otool;
	otool.start("otool", QStringList() << "-L" << output);
	if(!otool.waitForStarted()) {
		return Output(Platform::cppPath(), 1, "", "error: couldn't start otool\n");
	}
	
	otool.waitForFinished();
	QString string = QString(otool.readAllStandardOutput());
	QStringList lines = string.split("\n");
	if(lines.size() > 0) lines.removeAt(0);
	foreach(const QString &line, lines) {
		const QString part = line.left(line.indexOf("(") - 1).trimmed();
		if(part.isEmpty()) continue;
		if(part.startsWith("/var/folder")) {
			QProcess installNames;
			QStringList args = QStringList() << "-change" << part << "@rpath/" + QFileInfo(part).fileName() << output;
			qDebug() << "install_name_tool" << args;
			installNames.start("install_name_tool", args);
			if(!installNames.waitForStarted()) {
				return Output(Platform::cppPath(), 1, "", "error: couldn't start install_name_tool\n");
			}
			installNames.waitForFinished();
			qDebug() << installNames.readAllStandardOutput();
			qDebug() << installNames.readAllStandardError();
			qDebug() << installNames.exitCode();
		}
		qDebug() << "otool line:" << part;
	}
#endif
	
	ret.setExitCode(linker.exitCode());
	ret.setOutput(linker.readAllStandardOutput());
	ret.setError(linker.readAllStandardError());
	ret.setGeneratedFiles(QStringList() << output);
	ret.setTerminal(Output::BinaryTerminal);

	return ret;
}

Output O::produceLibrary(const QStringList &input, Options &options) const
{
	QProcess linker;
	Output ret;
	ret.setFiles(input);
	
	Options localOptions = options;

	Options::const_iterator it = localOptions.find(PLATFORM_O_FLAGS);
	if(it != localOptions.end()) {
		localOptions.insert(O_FLAGS, localOptions.value(O_FLAGS) + " " + it.value());
	}
	
	Options::const_iterator depsIt = localOptions.find(PROJECT_DEPS);
	if(depsIt != localOptions.end()) {
		QStringList addOFlags;
		foreach(const QString &dep, OptionParser::arguments(depsIt.value())) {
			addOFlags << "\"-L${USER_ROOT}/lib/" + dep + "\"";
			addOFlags << "\"-l" + dep + "\"";
		}
		localOptions.insert(O_FLAGS, localOptions.value(O_FLAGS) + " " + addOFlags.join(" "));
		localOptions.expand();
	}

	QString rawFlags = localOptions[O_FLAGS].trimmed();
	QStringList flags = OptionParser::arguments(rawFlags);

	const QString ext = Platform::libExtension();
	const QString name = localOptions.contains(LIBRARY_NAME) ?
		localOptions[LIBRARY_NAME] :
		(input.size() == 1 ? QFileInfo(input[0]).baseName() : "library");
	const QString output = (localOptions.contains(OUTPUT_DIR) ? localOptions[OUTPUT_DIR] : QFileInfo(input[0]).absolutePath())
		+ "/" + name + (ext.isEmpty() ? "" : "." + ext);

	QStringList args;
	args << "-shared" << "-o" << output << input << flags;

	linker.start(Platform::cppPath(), args);
	if(!linker.waitForStarted()) {
		return Output(Platform::cppPath(), 1, "", "error: couldn't start linker to produce library\n");
	}
	linker.waitForFinished();

	// Fix rpath
#ifdef Q_OS_MAC
	if(localOptions.contains(LIBRARY_NAME)) {
		QProcess installNames;
		QStringList args = QStringList() << "-id" << "lib" + localOptions[LIBRARY_NAME] + ".dylib" << output;
		qDebug() << "install_name_tool" << args;
		installNames.start("install_name_tool", args);
		if(!installNames.waitForStarted()) {
			return Output(Platform::cppPath(), 1, "", "error: couldn't start install_name_tool\n");
		}
		installNames.waitForFinished();
	}
	
	QProcess otool;
	otool.start("otool", QStringList() << "-L" << output);
	if(!otool.waitForStarted()) {
		return Output(Platform::cppPath(), 1, "", "error: couldn't start otool\n");
	}
	
	otool.waitForFinished();
	QString string = QString(otool.readAllStandardOutput());
	QStringList lines = string.split("\n");
	if(lines.size() > 0) lines.removeAt(0);
	foreach(const QString &line, lines) {
		const QString part = line.left(line.indexOf("(") - 1).trimmed();
		if(part.isEmpty()) continue;
		if(part.startsWith("/var/folder")) {
			QProcess installNames;
			QStringList args = QStringList() << "-change" << part << "@rpath/" + QFileInfo(part).fileName() << output;
			qDebug() << "install_name_tool" << args;
			installNames.start("install_name_tool", args);
			if(!installNames.waitForStarted()) {
				return Output(Platform::cppPath(), 1, "", "error: couldn't start install_name_tool\n");
			}
			installNames.waitForFinished();
			qDebug() << installNames.readAllStandardOutput();
			qDebug() << installNames.readAllStandardError();
			qDebug() << installNames.exitCode();
		}
		qDebug() << "otool line:" << part;
	}
#endif

	
	ret.setExitCode(linker.exitCode());
	ret.setOutput(linker.readAllStandardOutput());
	ret.setError(linker.readAllStandardError());
	ret.setGeneratedFiles(QStringList() << output);
	ret.setTerminal(Output::DependencyTerminal);

	return ret;
}

OutputList O::transform(const QStringList &input, Options &options) const
{
	Options::const_iterator it = options.find(TERMINAL_TYPE);
	if(it != options.end() && it.value() == LIBRARY) {
		return OutputList() << produceLibrary(input, options);
	}

	return OutputList() << produceBinary(input, options);
}

REGISTER_COMPILER(O)