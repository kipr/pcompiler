#ifndef _PROCESS_POOL_HPP_
#define _PROCESS_POOL_HPP_

#include 

namespace Compiler
{
	class Pool
	{
	public:
		Pool();
		
		void retain();
		void release
	private:
		unsigned int m_slots;
	};
}

#endif
