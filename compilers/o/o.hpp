#ifndef _O_HPP_
#define _O_HPP_

#include "pcompiler/base.hpp"

namespace Compiler
{
	class O : public Base
	{
	public:
		O();
		
		virtual OutputList transform(const QStringList& input, Options& options) const;
	private:
		Output produceBinary(const QStringList& input, Options& options) const;
		Output produceLibrary(const QStringList& input, Options& options) const;
	};
}

#endif
