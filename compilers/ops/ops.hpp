#ifndef _OPS_HPP_
#define _OPS_HPP_

#include "../passthrough/passthrough.hpp"

namespace Compiler
{	
	class Ops: public Passthrough
	{
	public:
		Ops();
		
		virtual OutputList transform(const QStringList &input, Options &options) const;
	};
}

#endif
