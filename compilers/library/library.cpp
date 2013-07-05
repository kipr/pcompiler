#include "library.hpp"
#include "pcompiler/compilers.hpp"
#include "../common/platform.hpp"

#include <QFileInfo>

using namespace Compiler;

Library::Library()
	: Base("library", QStringList() << "so" << "dylib" << "dll", -1)
{
}

OutputList Library::transform(const QStringList &input, Options &options) const
{
	OutputList ret;
	const QString validExt = Platform::libExtension();

	foreach(const QString &file, input) {
		if(QFileInfo(file).suffix() != validExt) continue;

		Output out;
		out.setFile(file);
		out.setExitCode(0);
		out.setGeneratedFiles(QStringList() << file);
		out.setTerminal(Output::LibraryTerminal);
		ret << out;
	}

	return ret;
}

REGISTER_COMPILER(Library)