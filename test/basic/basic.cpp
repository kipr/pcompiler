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
	
	Options options = Options();
	engine.compile(input, options);
	
	return 0;
}