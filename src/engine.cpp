#include "pcompiler/engine.hpp"

#include <QDebug>

using namespace Compiler;

template <typename T>
struct PtrLess
{
	bool operator()(const T* a, const T* b) const
	{
		return *a < *b;
	} 
};

Engine::Engine()
{
	
}

Engine::Engine(const QList<const Base *>& compilers)
	: m_compilers(compilers)
{
	qSort(m_compilers.begin(), m_compilers.end(), PtrLess<Base>());
}

Engine::~Engine()
{
}

void Engine::addCompiler(const Base *compiler)
{
	m_compilers.append(compiler);
	qSort(m_compilers.begin(), m_compilers.end(), PtrLess<Base>());
}

void Engine::addCompilers(const QList<const Base *>& compilers)
{
	m_compilers.append(compilers);
	
	qSort(m_compilers.begin(), m_compilers.end(), PtrLess<Base>());
}

void Engine::removeCompiler(const Base *compiler)
{
	m_compilers.removeAll(compiler);
}

OutputList Engine::compile(const Input& input, Options& options, Progress *progress) const
{
	Input workingInput(input);
	QList<const Base *> workingCompilers(m_compilers);
	foreach(const Base *base, workingCompilers)
		qDebug() << "compiler:" << base->name();
	OutputList ret;
	
	options.expand();
	
	double processed = 0;
	while(!workingCompilers.isEmpty()) {
		const Base *compiler = workingCompilers.takeFirst();
		QStringList workingInputList = QStringList(workingInput.toList());
		if(progress) progress->progress(processed / (double)workingInputList.size());
		QStringList applicableInput;
		foreach(const QString& file, workingInputList) {
			foreach(const QString& ext, compiler->extensions()) {
				if(!file.endsWith("." + ext)) continue;
				applicableInput += file;
				break;
			}
		}
		if(!applicableInput.isEmpty()) {
			qDebug() << "Calling compiler" << compiler->name() << "with" << applicableInput;
			OutputList outList = compiler->transform(applicableInput, options);
			foreach(const Output& out, outList) {
				if(!out.isSuccess()) return ret + outList;
				workingInput += out.generatedFiles().toSet();
			}
			ret += outList;
		}
		workingInput -= applicableInput.toSet();
		processed += applicableInput.size();
	}
	
	return ret;
}