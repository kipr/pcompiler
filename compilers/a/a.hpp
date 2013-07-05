#ifndef _A_HPP_
#define _A_HPP_

#include "pcompiler/base.hpp"

namespace Compiler
{	
	class A: public Base
	{
	public:
		A();
		
		virtual OutputList transform(const QStringList &input, Options &options) const;

	private:
		Output transform(const QString &file, Options &options) const;
		QStringList files(const QString &file) const;
	};
}

#endif
