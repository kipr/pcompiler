#include "passthrough.hpp"

using namespace Compiler;

Passthrough::Passthrough(const QString& name, const QStringList& extensions)
	: Base(name, extensions, -1)
{}

OutputList Passthrough::transform(const QStringList& input, const Options& options) const
{
	Output ret;
	ret.setFiles(input);
	ret.setExitCode(0);
	return OutputList() << ret;
}