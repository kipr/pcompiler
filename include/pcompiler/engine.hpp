#ifndef _ENGINE_HPP_
#define _ENGINE_HPP_

#include "base.hpp"
#include "output.hpp"
#include "input.hpp"
#include "progress.hpp"
#include "export.h"

#include <QList>
#include <QSemaphore>

namespace Compiler
{
	class DLL_EXPORT Engine
	{
	public:
		Engine();
		Engine(const QList<const Base *>& compilers);
		~Engine();
		
		void addCompiler(const Base *compiler);
		void addCompilers(const QList<const Base *>& compilers);
		void removeCompiler(const Base *compiler);
		
		OutputList compile(const Input& input, Options& options, Progress *progress = 0) const;
		
	private:
		QList<const Base *> m_compilers;
		QSemaphore m_instances;
	};
}

#endif
