#include "pcompiler/options.hpp"

#include <QSettings>
#include <QStringList>

using namespace Compiler;

Options Options::load(const QString& path)
{
	Options ret;
	QSettings settings(path, QSettings::IniFormat);
	foreach(const QString& key, settings.childKeys()) {
		ret.insert(key, settings.value(key).toString());
	}
	
	settings.beginGroup(
#ifdef Q_OS_MAC
	"osx"
#elif defined(Q_OS_WIN)
	"win"
#else
	"nix"
#endif
	);

	foreach(const QString& key, settings.childKeys()) {
		ret.insert(key, settings.value(key).toString());
	}
	
	settings.endGroup();
	return ret;
}

bool Options::save(const QString& path) const
{
	QSettings settings(path, QSettings::IniFormat);
	foreach(const QString& key, keys()) {
		settings.setValue(key, value(key));
	}
	settings.sync();
}