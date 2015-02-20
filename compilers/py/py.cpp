#include "py.hpp"
#include "pcompiler/compilers.hpp"
#include "../common/platform.hpp"
#include "pcompiler/compiler_options.hpp"

#include <QFileInfo>
#include <QProcess>
#include <QDebug>

using namespace Compiler;

Python::Python()
	: Passthrough("py", QStringList() << "py")
{}

OutputList Python::transform(const QStringList &input, Options &options) const
{
	Output ret;
	ret.setFiles(input);
	ret.setExitCode(0);
	ret.setGeneratedFiles(input);
	ret.setTerminal(Output::BinaryTerminal);
	return OutputList() << ret;
}

REGISTER_COMPILER(Python)