#include "lisp.hpp"
#include "pcompiler/compilers.hpp"
#include "../common/platform.hpp"
#include "pcompiler/compiler_options.hpp"

#include <QFileInfo>
#include <QProcess>
#include <QDebug>

using namespace Compiler;

Lisp::Lisp()
	: Base("lisp", QStringList() << "lisp", 1, QStringList() << LISP_FLAGS << OUTPUT_DIR)
{
}

OutputList Lisp::transform(const QStringList &input, Options &options) const
{
	Output ret;
	ret.setExitCode(1);
	ret.setError("error: pcompiler doesn't know how to compile lisp files");
	return OutputList() << ret;
}

REGISTER_COMPILER(Lisp)