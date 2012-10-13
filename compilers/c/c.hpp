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
		
		virtual OutputList transform(const QStringList& input, const Options& options) const;
	private:
		Output transform(const QString& file, const Options& options) const;
	};
}

#endif
