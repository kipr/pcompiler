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
	};
}

#endif
