#ifndef UPGRADEMGR_MASTER_COMMAND_GLOBAL_VERSION_COMMAND_H
#define UPGRADEMGR_MASTER_COMMAND_GLOBAL_VERSION_COMMAND_H

#include "corelib/command/abstract_command.h"

namespace upgrademgr{
namespace master{
namespace command{

using sn::corelib::AbstractCommand;
using sn::corelib::AbstractCommandRunner;
using sn::corelib::CommandMeta;

class GlobalVersionCommand : public AbstractCommand 
{
public:
   GlobalVersionCommand(AbstractCommandRunner& runner, const CommandMeta& invokeMeta);
public:
   virtual void exec();
   virtual ~GlobalVersionCommand();
};

}//command
}//master
}//upgrademgr

#endif // UPGRADEMGR_MASTER_COMMAND_GLOBAL_VERSION_COMMAND_H
