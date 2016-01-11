#include "global_help_command.h"
#include "corelib/io/terminal.h"

namespace upgrademgr{
namespace master{
namespace command{

using sn::corelib::TerminalColor;

GlobalHelpCommand::GlobalHelpCommand(AbstractCommandRunner& runner, const CommandMeta& invokeMeta)
   : AbstractCommand(runner, invokeMeta)
{
}

void GlobalHelpCommand::exec()
{
   m_cmdRunner.printUsage();
   exit(EXIT_SUCCESS);
}

GlobalHelpCommand::~GlobalHelpCommand()
{}

}//command
}//master
}//upgrademgr