#include <pcompiler/pcompiler.hpp>

#include <QDebug>

using namespace Compiler;

int main(int argc, char *argv[])
{
	qDebug() << "Compilers: " << Compilers::instance()->compilerNames();
	
	Engine engine(Compilers::instance()->compilers());
	
	Input input;
	input += "ehh.c";
	input += "bee.c";
	
	engine.compile(input);
	
	return 0;
}