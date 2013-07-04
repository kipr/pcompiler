#ifndef _CLASS_HPP_
#define _CLASS_HPP_

#include "pcompiler/base.hpp"

namespace Compiler
{	
	class Class : public Base
	{
	public:
		Class();
		virtual OutputList transform(const QStringList &input, Options &options) const;
	private:
		static QString jarPath();
	};
}

#endif
