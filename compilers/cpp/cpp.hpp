#ifndef _CPP_HPP_
#define _CPP_HPP_

#include "pcompiler/base.hpp"
#include "../passthrough/passthrough.hpp"

namespace Compiler
{	
	class Hpp : public Passthrough
	{
	public:
		Hpp();
	};
	
	class Cpp : public Base
	{
	public:
		Cpp();
		
		virtual OutputList transform(const QStringList& input, const Options& options) const;
	private:
		Output transform(const QString& file, const Options& options) const;
	};
}

#endif
