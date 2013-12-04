#include "board.hpp"
#include "pcompiler/compilers.hpp"
#include "../common/platform.hpp"

#include <QFileInfo>

using namespace Compiler;

Board::Board()
	: Base("board", QStringList() << "board", -1)
{
}

OutputList Board::transform(const QStringList &input, Options &options) const
{
  Output out;
  out.setFiles(input);
  out.setExitCode(0);
  out.setGeneratedFiles(input);
  out.setTerminal(Output::BoardTerminal);
  
  return OutputList() << out;
}

REGISTER_COMPILER(Board)