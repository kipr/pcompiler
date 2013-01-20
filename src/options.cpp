#include "pcompiler/options.hpp"

#include <QSettings>
#include <QStringList>

using namespace Compiler;

void Options::replace(const QString& str, const QString& value)
{
	foreach(const QString& key, keys()) {
		const QString replacement = take(key).replace(str, value);
		insert(key, replacement);
	}
}

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
		const QString previous = ret.take(key);
		ret.insert(key, previous + " " + settings.value(key).toString());
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
	return true;
}

QStringList OptionParser::arguments(const QString &argumentString)
{
	QString current;
	QStringList ret;
	QChar l = 0;
	for(int i = 0; i < argumentString.size(); ++i) {
		QChar c = argumentString[i];
		if(c == '\"') {
			if(l == '\\') current += "\"";
			else {
				ret << current.trimmed();
				current = "";
			}
		} else if(c == '\\');
		else if(c == ' ') {
			ret << current.trimmed();
			current = "";
		} else current += c;
		l = c;
	}
	if(!current.isEmpty()) ret << current.trimmed();
	return ret;
}
