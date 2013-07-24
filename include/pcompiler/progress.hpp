#ifndef _PROGRESS_HPP_
#define _PROGRESS_HPP_

#include "export.h"

namespace Compiler
{
	class DLL_EXPORT Progress
	{
	public:
		virtual void progress(double fraction) = 0;
	};
}

#endif
