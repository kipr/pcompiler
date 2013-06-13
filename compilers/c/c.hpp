#ifndef _C_HPP_
#define _C_HPP_

#include "pcompiler/base.hpp"
#include "../passthrough/passthrough.hpp"

namespace Compiler
{	
	class H : public Passthrough
	{
	public:
		H();
	};
	
	class C : public Base
	{
	public:
		C();
		
		virtual OutputList transform(const QStringList& input, Options& options) const;
	private:
		Output transform(const QString& file, Options& options) const;
	};
}

#endif
