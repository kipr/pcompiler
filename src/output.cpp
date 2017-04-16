#include "pcompiler/output.hpp"

#include <QProcess>
#include <QDebug>
#include <QDataStream>

using namespace Compiler;

Output::Output()
	: m_exitCode(-1),
	m_terminal(NotTerminal)
{
	
}

Output::Output(const QString& file, const int& exitCode, const QByteArray& output, const QByteArray& error)
	: m_files(QStringList(file)),
	m_exitCode(exitCode),
	m_output(output),
	m_error(error),
	m_terminal(NotTerminal)
{}

void Output::setFile(const QString& file)
{
	m_files.clear();
	m_files += file;
}

void Output::setFiles(const QStringList& files)
{
	m_files = files;
}

void Output::setExitCode(const int& exitCode)
{
	m_exitCode = exitCode;
}

void Output::setOutput(const QByteArray& output)
{
	m_output = output;
}

void Output::setError(const QByteArray& error)
{
	m_error = error;
}

void Output::setGeneratedFiles(const QStringList& generatedFiles)
{
	m_generatedFiles = generatedFiles;
}

void Output::setTerminal(const TerminalType	& terminal)
{
	m_terminal = terminal;
}

const QStringList& Output::files() const
{
	return m_files;
}

const int& Output::exitCode() const
{
	return m_exitCode;
}

const QByteArray& Output::output() const
{
	return m_output;
}

const QByteArray& Output::error() const
{
	return m_error;
}

const QStringList& Output::generatedFiles() const
{
	return m_generatedFiles;
}

const Output::TerminalType& Output::terminal() const
{
	return m_terminal;
}

const bool Output::isTerminal() const
{
	return (m_terminal != NotTerminal);
}

bool Output::isSuccess() const
{
	// FIXME: This may or may not be a sweeping
	// generalization.
	return m_exitCode == 0;
}

void Output::dump(QTextStream &stream) const
{
	stream << "Files:           {" << m_files.join(", ") << "}"<< endl;
	if(!m_generatedFiles.isEmpty()) stream << "Generated Files: {" << m_generatedFiles.join(", ") << "}" << endl;
	stream << "Exit Code:       " << m_exitCode << endl;
	stream << "Terminal?:       " << m_terminal << endl;
	if(!m_output.isEmpty()) stream << "Output: " << endl << m_output << endl;
	if(!m_error.isEmpty()) stream << "Error: " << endl << m_error << endl;
}

Output Output::fromProcess(const QString& file, QProcess *process)
{
	return Output(file, process->exitCode(), process->readAllStandardOutput(),
		process->readAllStandardError());
}

bool Output::isSuccess(const QList<Output> outputList)
{
	bool success = true;
	foreach(const Output &out, outputList) {
		success &= out.isSuccess();
	}
	return success;
}

QDataStream& operator<<(QDataStream& out, const Compiler::Output& rhs)
{
	out << rhs.files();
	out << rhs.exitCode();
	out << rhs.output();
	out << rhs.error();
	out << rhs.generatedFiles();
	return out;
}

QDataStream& operator>>(QDataStream& in, Compiler::Output& rhs)
{
	QStringList files;
	in >> files;
	rhs.setFiles(files);

	int exitCode;
	in >> exitCode;
	rhs.setExitCode(exitCode);
	
	QByteArray output;
	in >> output;
	rhs.setOutput(output);
	
	QByteArray error;
	in >> error;
	rhs.setError(error);
	
	QStringList generatedFiles;
	in >> generatedFiles;
	rhs.setGeneratedFiles(generatedFiles);
	
	return in;
}
