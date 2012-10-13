#ifndef _PLATFORM_HPP_
#define _PLATFORM_HPP_

#include <QString>

namespace Compiler
{
	class Platform
	{
	public:
		static QString ccPath();
		static QString cppPath();
		static QString exeExtension();
	};
}

#endif
