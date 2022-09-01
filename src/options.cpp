#include "pcompiler/options.hpp"

#include <QSettings>
#include <QStringList>

using namespace Compiler;

Options Options::load(const QString &path)
{
	Options ret;
	QSettings settings(path, QSettings::IniFormat);
	foreach(const QString &key, settings.childKeys()) {
		ret.insert(key, settings.value(key));
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

	foreach(const QString &key, settings.childKeys()) {
		if(settings.value(key).typeId() == QVariant::String) {
			const QString previous = ret.take(key).toString();
			ret.insert(key, previous + " " + settings.value(key).toString());
		}
		else ret.insert(key, settings.value(key));
	}
	
	settings.endGroup();
	return ret;
}

bool Options::save(const QString &path) const
{
	QSettings settings(path, QSettings::IniFormat);
	settings.clear();
	foreach(const QString &key, keys()) {
		settings.setValue(key, value(key));
	}
	settings.sync();
	return true;
}

void Options::setVariable(const QString &str, const QString &value)
{
	m_vars.insert(str, value);
}

void Options::removeVariable(const QString &str)
{
	m_vars.remove(str);
}

QString Options::variable(const QString &str) const
{
	QMap<QString, QString>::const_iterator it = m_vars.find(str);
	if(it == m_vars.end()) return QString();
	return *it;
}

QStringList Options::variables() const
{
	return QStringList(m_vars.keys());
}

void Options::expand()
{
	QMap<QString, QString>::const_iterator it = m_vars.constBegin();
	while(it != m_vars.constEnd()) {
		replace(it.key(), it.value());
		++it;
	}
}

void Options::replace(const QString &str, const QString &value)
{
	foreach(const QString &key, keys()) {
		if(this->value(key).typeId() != QVariant::String) continue;
		const QString replacement = take(key).toString().replace(str, value);
		insert(key, replacement);
	}
}

QStringList OptionParser::arguments(const QString &argumentString)
{
	QString current;
	QStringList ret;
	QChar l = QChar(0);
	bool isQuotes = false;
	for(int i = 0; i < argumentString.size(); ++i) {
		QChar c = argumentString[i];
		if(c == '\"') {
			if(l == '\\') current += "\"";
			else {
				isQuotes = !isQuotes;
				if(!current.isEmpty()) ret << current.trimmed();
				current = "";
			}
		} else if(c == '\\');
		else if(c == ' ' && !isQuotes) {
			if(!current.isEmpty()) ret << current.trimmed();
			current = "";
		} else current += c;
		l = c;
	}
	if(!current.isEmpty()) ret << current.trimmed();
	return ret;
}
