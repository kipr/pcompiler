#ifndef _PROGRESS_HPP_
#define _PROGRESS_HPP_

namespace Compiler
{
	class Progress
	{
	public:
		virtual void progress(double fraction) = 0;
	};
}

#endif
