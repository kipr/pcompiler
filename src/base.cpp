#include "pcompiler/base.hpp"
#include <QDebug>

using namespace Compiler;

Base::Base(const QString& name, const QStringList& extensions, const int& precedence, const QStringList& flags)
	: m_name(name), m_extensions(extensions), m_precedence(precedence), m_flags(flags)
{
	
}

const QString& Base::name() const
{
	return m_name;
}

const QStringList& Base::extensions() const
{
	return m_extensions;
}

const int& Base::precedence() const
{
	return m_precedence;
}

const QStringList& Base::flags() const
{
	return m_flags;
}

bool operator<(const Compiler::Base& lhs, const Compiler::Base& rhs)
{
	return lhs.precedence() < rhs.precedence();
}

bool operator>(const Compiler::Base& lhs, const Compiler::Base& rhs)
{
	return lhs.precedence() > rhs.precedence();
}

bool operator==(const Compiler::Base& lhs, const Compiler::Base& rhs)
{
	return lhs.precedence() == rhs.precedence();
}