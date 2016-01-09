#ifndef UPGRADEMGR_MASTER_COMMAND_WEBSERVER_ENV_SOFTWARE_COMANND
#define UPGRADEMGR_MASTER_COMMAND_WEBSERVER_ENV_SOFTWARE_COMANND

#include "corelib/command/abstract_command.h"

namespace upgrademgr{
namespace master{
namespace command{

using sn::corelib::AbstractCommand;
using sn::corelib::AbstractCommandRunner;
using sn::corelib::CommandMeta;

class SetWebServerSoftwareItem : public AbstractCommand
{
public:
   AddWebServerSoftwareItem(AbstractCommandRunner& runner, const CommandMeta& invokeMeta);
public:
   virtual void exec();
};

class DeleteWebServerSoftwareItem : public AbstractCommand
{
public:
   DeleteWebServerSoftwareItem(AbstractCommandRunner& runner, const CommandMeta& invokeMeta);
public:
   virtual void exec();
};

class ListWebServerSoftwareItem : public AbstractCommand
{
public:
   ListWebServerSoftwareItem(AbstractCommandRunner& runner, const CommandMeta& invokeMeta);
public:
   virtual void exec();
};

}//command
}//master
}//upgrademgr

#endif // UPGRADEMGR_MASTER_COMMAND_WEBSERVER_ENV_SOFTWARE_COMANND

