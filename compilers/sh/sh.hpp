#ifndef _SH_HPP_
#define _SH_HPP_

#include "pcompiler/base.hpp"

namespace Compiler
{
	class Shell : public Base
	{
	public:
		Shell();
		
		virtual OutputList transform(const QStringList &input, Options &options) const;
	private:
	};
}

#endif
