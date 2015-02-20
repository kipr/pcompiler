#include "class.hpp"
#include "pcompiler/compilers.hpp"
#include "../common/platform.hpp"
#include "pcompiler/compiler_options.hpp"

#include <QFileInfo>
#include <QProcess>
#include <QDebug>
#include <QDir>

using namespace Compiler;

Class::Class()
	: Base("jar", QStringList() << "class", 0, QStringList() << OUTPUT_DIR)
{
}

OutputList Class::transform(const QStringList &input, Options &options) const
{
	Output ret;
	ret.setFiles(input);
	ret.setExitCode(0);
	ret.setGeneratedFiles(input);
  ret.setTerminal(Output::BinaryTerminal);
  
  Output runnee;
  bool foundMain = false;
  QFileInfo selectedRunnee(input[0]);
  Q_FOREACH(const QString &in, input)
  {
    QFileInfo fi(in);
    if(fi.baseName() != "Main") continue;
    foundMain = true;
    selectedRunnee = fi;
    break;
  }
  
  if(!foundMain && input.size() > 1)
  {
    runnee.setOutput(QObject::tr("warning: Couldn't find the Main class."
      " Assuming %1 contains main().").arg(selectedRunnee.baseName()).toUtf8());
  }
  
  const QString outRPath = selectedRunnee.absoluteDir().absoluteFilePath("runnee.c");
  QFile runCode(outRPath);
  
  qDebug() << "Writing " << outRPath;
  if(runCode.open(QIODevice::WriteOnly))
  {
    runCode.write(QString("#include<stdio.h>\n"
      "#include <string.h>\n"
      "int main(int argc, char *argv[]) {"
      "int i = strlen(argv[0]);"
      "for(; i > 0 && argv[0][i] != '/'; --i);"
      "argv[0][i + 1] = 0;"
      "char buffer[4096];"
      "snprintf(buffer, sizeof buffer, \"java -classpath %s %2\", argv[0]);"
      "return system(buffer);"
      "}").arg(selectedRunnee.baseName()).toUtf8());
    runCode.close();
    runnee.setGeneratedFiles(QStringList() << outRPath);
    runnee.setExitCode(0);
  }
  else
  {
    runnee.setError(QObject::tr("error: Couldn't open %1 for writing").arg(outRPath).toUtf8());
    runnee.setExitCode(1);
  }
  
	return OutputList() << ret << runnee;
}

QString Class::jarPath()
{
	return "jar";
}

REGISTER_COMPILER(Class)