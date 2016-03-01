#include "global_version_command.h"
#include "corelib/io/terminal.h"

namespace upgrademgr{
namespace master{
namespace command{

using sn::corelib::TerminalColor;

GlobalVersionCommand::GlobalVersionCommand(AbstractCommandRunner& runner, const CommandMeta& invokeMeta)
   : AbstractCommand(runner, invokeMeta)
{
}

void GlobalVersionCommand::exec()
{
   printConsoleMsg("upgrademgr master version ");
   printConsoleMsg(QString("%1\n").arg(UMM_VERSION).toLocal8Bit(), TerminalColor::Cyan);
   exit(EXIT_SUCCESS);
}

GlobalVersionCommand::~GlobalVersionCommand()
{}

}//command
}//master
}//upgrademgr