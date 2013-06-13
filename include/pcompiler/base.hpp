#ifndef _COMPILER_HPP_
#define _COMPILER_HPP_

#include "options.hpp"
#include "output.hpp"

#include <QObject>
#include <QString>
#include <QStringList>
#include <QFuture>

namespace Compiler
{
	class Base
	{
	public:
		Base(const QString& name, const QStringList& extensions, const int& precedence, const QStringList& flags = QStringList());
		
		virtual OutputList transform(const QStringList& input, Options& options) const = 0;
		
		const QString& name() const;
		const QStringList& extensions() const;
		const int& precedence() const;
		const QStringList& flags() const;
	private:
		QString m_name;
		QStringList m_extensions;
		int m_precedence;
		QStringList m_flags;
	};
}

bool operator<(const Compiler::Base& lhs, const Compiler::Base& rhs);
bool operator>(const Compiler::Base& lhs, const Compiler::Base& rhs);
bool operator==(const Compiler::Base& lhs, const Compiler::Base& rhs);

#define REGISTER_COMPILER(x) __attribute__((constructor)) \
static void __##x##_register() \
{ \
	Compilers::instance()->addCompiler(new x()); \
}

#endif
