#ifndef _BOARD_HPP_
#define _BOARD_HPP_

#include "pcompiler/base.hpp"

namespace Compiler
{	
	class Board : public Base
	{
	public:
		Board();
		virtual OutputList transform(const QStringList &input, Options &options) const;
	};
}

#endif
