#ifndef _LIBRARY_HPP_
#define _LIBRARY_HPP_

#include "pcompiler/base.hpp"

namespace Compiler
{	
	class Library : public Base
	{
	public:
		Library();
		virtual OutputList transform(const QStringList &input, Options &options) const;
	};
}

#endif
