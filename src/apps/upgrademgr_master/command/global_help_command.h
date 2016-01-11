#ifndef UPGRADEMGR_MASTER_COMMAND_GLOBAL_HELP_COMMAND_H
#define UPGRADEMGR_MASTER_COMMAND_GLOBAL_HELP_COMMAND_H

#include "corelib/command/abstract_command.h"

namespace upgrademgr{
namespace master{
namespace command{

using sn::corelib::AbstractCommand;
using sn::corelib::AbstractCommandRunner;
using sn::corelib::CommandMeta;

class GlobalHelpCommand : public AbstractCommand 
{
public:
   GlobalHelpCommand(AbstractCommandRunner &runner, const CommandMeta &invokeMeta);
public:
   virtual void exec();
   virtual ~GlobalHelpCommand();
};

}//command
}//master
}//upgrademgr

#endif // UPGRADEMGR_MASTER_COMMAND_GLOBAL_HELP_COMMAND_H

