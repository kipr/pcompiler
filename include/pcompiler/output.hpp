#ifndef _OUTPUT_HPP_
#define _OUTPUT_HPP_

#include <QByteArray>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QMetaType>

class QProcess;

namespace Compiler
{
	class Output
	{
	public:
		Output();
		Output(const QString& file, const int& exitCode, const QByteArray& output, const QByteArray& error);

		enum TerminalType {
			NotTerminal,
			BinaryTerminal,
			LibraryTerminal
		};
		
		void setFile(const QString& file);
		void setFiles(const QStringList& files);
		void setExitCode(const int& exitCode);
		void setOutput(const QByteArray& output);
		void setError(const QByteArray& error);
		void setGeneratedFiles(const QStringList& generatedFiles);
		void setTerminal(const TerminalType& terminal);
		
		const QStringList& files() const;
		const int& exitCode() const;
		const QByteArray& output() const;
		const QByteArray& error() const;
		const QStringList& generatedFiles() const;
		const TerminalType& terminal() const;
		const bool isTerminal() const;
		
		bool isSuccess() const;
		
		static bool isSuccess(const QList<Output> outputList);
		
		void dump(QTextStream &stream) const;
		
		static Output fromProcess(const QString& file, QProcess *process);
		
	private:
		QStringList m_files;
		int m_exitCode;
		QByteArray m_output;
		QByteArray m_error;
		QStringList m_generatedFiles;
		TerminalType m_terminal;
	};
	
	typedef QList<Output> OutputList;
}

Q_DECLARE_METATYPE(Compiler::Output);
Q_DECLARE_METATYPE(Compiler::OutputList);

QDataStream& operator<<(QDataStream& out, const Compiler::Output& rhs);
QDataStream& operator>>(QDataStream& in, Compiler::Output& rhs);

#endif
