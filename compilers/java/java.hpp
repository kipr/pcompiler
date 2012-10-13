#ifndef _CPP_HPP_
#define _CPP_HPP_

#include "pcompiler/base.hpp"

namespace Compiler
{	
	class Java : public Base
	{
	public:
		Java();
		virtual OutputList transform(const QStringList& input, const Options& options) const;
	private:
		static QString javacPath();
	};
}

#endif
