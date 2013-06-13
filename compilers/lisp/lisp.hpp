#ifndef _LISP_HPP_
#define _LISP_HPP_

#include "pcompiler/base.hpp"

namespace Compiler
{
	class Lisp : public Base
	{
	public:
		Lisp();
		
		virtual OutputList transform(const QStringList& input, Options& options) const;
	private:
	};
}

#endif
