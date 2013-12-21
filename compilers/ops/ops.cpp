#include "ops.hpp"
#include "pcompiler/compilers.hpp"
#include "pcompiler/compiler_options.hpp"
using namespace Compiler;

Ops::Ops()
	: Passthrough("ops", QStringList() << "ops")
{
}

OutputList Ops::transform(const QStringList &input, Options &options) const
{
	foreach(const QString &file, input) {
		Options projOptions = Options::load(file);

		QMap<QString, QVariant>::const_iterator it = projOptions.constBegin();
		for(; it != projOptions.constEnd(); ++it) {
			if(it.key() == KEY_DEPS)
				options.insert(KEY_DEPS, it.value().toStringList() + options.value(KEY_DEPS).toStringList());
		    else
				options.insert(it.key(), (options.value(it.key()).toString() + " " + it.value().toString()).trimmed());
		}
	}
	options.expand();

	return Passthrough::transform(input, options);
}

REGISTER_COMPILER(Ops)