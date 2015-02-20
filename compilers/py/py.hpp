#ifndef _PY_HPP_
#define _PY_HPP_

#include "../passthrough/passthrough.hpp"

namespace Compiler
{
	class Python : public Passthrough
	{
	public:
		Python();
		
		virtual OutputList transform(const QStringList &input, Options &options) const;
	private:
	};
}

#endif
