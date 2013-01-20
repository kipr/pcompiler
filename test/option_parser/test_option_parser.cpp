#include <pcompiler/options.hpp>
#include <QDebug>
#include <QStringList>

int main(int argc, char *argv[])
{
	qDebug() << Compiler::OptionParser::arguments("test arg1 arg2 arg3 \"asd fikmasd \\\"hello\\\"\" test2");
}