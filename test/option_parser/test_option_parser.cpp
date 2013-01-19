#include <pcompiler/options.hpp>
#include <QDebug>
#include <QStringList>

int main(int argc, char *argv[])
{
	qDebug() << Compiler::OptionParser::arguments("test \"asd fikmasd \\\"hello\\\"\" test2");
}