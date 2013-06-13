#include <pcompiler/pcompiler.hpp>
#include <QtCore>

using namespace Compiler;

void usage(char *path)
{
	printf("%s [directory]\n", path);
}

int main(int argc, char *argv[])
{
	if(argc != 2) {
		usage(argv[0]);
		return 1;
	}
	
	QFileInfoList files = QDir(argv[1]).entryInfoList(QDir::NoDot | QDir::NoDotDot | QDir::Files);
	
	Input input;
	foreach(const QFileInfo& file, files) input += file.absoluteFilePath();
	
	Engine engine(Compilers::instance()->compilers());
	Options options = Options();
	OutputList res = engine.compile(input, options);
	
	QTextStream out(stdout);
	foreach(const Output& output, res) output.dump(out);
	
	return 0;
}