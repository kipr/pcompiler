#ifndef _OPTIONS_HPP_
#define _OPTIONS_HPP_

#include <QMap>
#include <QString>

namespace Compiler
{
	class Options : public QMap<QString, QString>
	{
	public:
		static Options load(const QString& path);
		bool save(const QString& path) const;

		void setVariable(const QString &str, const QString &value);
		void removeVariable(const QString &str);
		
		QString variable(const QString &str) const;
		QStringList variables() const;
		
		void expand();

	private:
		void replace(const QString &str, const QString &value);

		QMap<QString, QString> m_vars;
	};
	
	class OptionParser
	{
	public:
		static QStringList arguments(const QString &argumentString);
	};
}

#endif
