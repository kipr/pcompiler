#ifndef _PY_HPP_
#define _PY_HPP_

#include "pcompiler/base.hpp"

namespace Compiler
{
	class Python : public Base
	{
	public:
		Python();
		
		virtual OutputList transform(const QStringList &input, Options &options) const;
	private:
	};
}

#endif
