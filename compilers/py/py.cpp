#include "py.hpp"
#include "pcompiler/compilers.hpp"
#include "../common/platform.hpp"
#include "../common/options.hpp"

#include <QFileInfo>
#include <QProcess>
#include <QDebug>

using namespace Compiler;

#define PY_FLAGS "PY_FLAGS"

Python::Python()
	: Base("python", QStringList() << "py", 1, QStringList() << PY_FLAGS << OUTPUT_DIR)
{
}

OutputList Python::transform(const QStringList& input, Options& options) const
{
	Output ret;
	ret.setExitCode(1);
	ret.setError("error: pcompiler doesn't know how to compile python files.");
	return OutputList() << ret;
}

REGISTER_COMPILER(Python)