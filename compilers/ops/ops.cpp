#include "ops.hpp"
#include "pcompiler/compilers.hpp"

using namespace Compiler;

Ops::Ops()
	: Passthrough("ops", QStringList() << "ops")
{
}

OutputList Ops::transform(const QStringList& input, Options& options) const
{
	foreach(const QString& file, input) {
		Options projOptions = Options::load(file);

		QMap<QString, QString>::const_iterator it = projOptions.constBegin();
		for(; it != projOptions.constEnd(); ++it) {
		    options.insert(it.key(), (options.value(it.key()) + " " + it.value()).trimmed());
		}
	}

	return Passthrough::transform(input, options);
}

REGISTER_COMPILER(Ops)