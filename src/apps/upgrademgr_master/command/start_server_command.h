#ifndef UPGRADEMGR_MASTER_COMMAND_START_SERVER_COMMAND_H
#define UPGRADEMGR_MASTER_COMMAND_START_SERVER_COMMAND_H

#include "corelib/command/abstract_command.h"

namespace upgrademgr{
namespace master{
namespace command{

using sn::corelib::AbstractCommand;
using sn::corelib::AbstractCommandRunner;
using sn::corelib::CommandMeta;

class StartServerCommand : public AbstractCommand
{
public:
   StartServerCommand(AbstractCommandRunner& runner, const CommandMeta& invokeMeta);
public:
   virtual void exec();
protected:
   qint16 getMetaServerListenPort();
};

}//command
}//master
}//upgrademgr

#endif // UPGRADEMGR_MASTER_COMMAND_START_SERVER_COMMAND_H
