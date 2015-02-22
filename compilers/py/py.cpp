#include "py.hpp"
#include "pcompiler/compilers.hpp"
#include "../common/platform.hpp"
#include "pcompiler/compiler_options.hpp"

#include <QFileInfo>
#include <QProcess>
#include <QDir>
#include <QDebug>

using namespace Compiler;

Python::Python()
	: Passthrough("py", QStringList() << "py")
{}

OutputList Python::transform(const QStringList &input, Options &options) const
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
    if(fi.baseName().toLower() != "main") continue;
    foundMain = true;
    selectedRunnee = fi;
    break;
  }
  
  if(!foundMain && input.size() > 1)
  {
    runnee.setOutput(QObject::tr("warning: Couldn't find main.py."
      " Assuming %1 contains entry point.").arg(selectedRunnee.fileName()).toUtf8());
  }
  
  const QString outRPath = selectedRunnee.absoluteDir().absoluteFilePath("runnee.sh");
  QFile runCode(outRPath);
  
  if(runCode.open(QIODevice::WriteOnly))
  {
    runCode.write(QString("#!/bin/bash\n"
      "python $(dirname $0)/%1").arg(selectedRunnee.fileName()).toUtf8());
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

REGISTER_COMPILER(Python)