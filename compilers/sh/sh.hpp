#ifndef _SH_HPP_
#define _SH_HPP_

#include "../passthrough/passthrough.hpp"

namespace Compiler
{
	class Shell : public Passthrough
	{
	public:
		Shell();
		
		virtual OutputList transform(const QStringList &input, Options &options) const;
	private:
	};
}

#endif
