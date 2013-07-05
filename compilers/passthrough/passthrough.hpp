#ifndef _PASSTHROUGH_HPP_
#define _PASSTHROUGH_HPP_

#include "pcompiler/base.hpp"

namespace Compiler
{
	class Passthrough : public Base
	{
	public:
		Passthrough(const QString &name, const QStringList &extensions);
		virtual OutputList transform(const QStringList &input, Options &options) const;
	};
}


#endif
