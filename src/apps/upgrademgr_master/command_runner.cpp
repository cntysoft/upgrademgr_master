#include "corelib/kernel/errorinfo.h"
#include "corelib/io/terminal.h"
#include "corelib/command/abstract_command.h"
#include "corelib/command/command_meta.h"

#include "command_runner.h"
#include "application.h"
#include "command/command_repo.h"

namespace upgrademgr{
namespace master{

using sn::corelib::ErrorInfo;
using sn::corelib::TerminalColor;
using sn::corelib::CommandMeta;
using sn::corelib::AbstractCommand;
using sn::corelib::AbstractCommandRunner;

using upgrademgr::master::command::GlobalVersionCommand;
using upgrademgr::master::command::GlobalHelpCommand;
using upgrademgr::master::command::StartServerCommand;
using upgrademgr::master::command::CcUpgradeTestCommand;

CommandRunner::CommandRunner(Application &app)
   : AbstractCommandRunner(app)
{
   addUsageText("welcome to use sheneninfo upgrademgr master system\n", TerminalColor::Green);
   addUsageText("usage: \n", TerminalColor::LightBlue);
   addUsageText("--version  print main system version number\n");
   addUsageText("--help     print help document\n");
   addUsageText("start [--daemon] [--port] start upgrademgr server\n");
   addUsageText("ccupgradetest --script= test upgrade script\n");
   initCommandPool();
   initRouteItems();
}

void CommandRunner::initCommandPool()
{
   m_cmdRegisterPool.insert("Global_Version", [](AbstractCommandRunner& runner, const CommandMeta& meta)->AbstractCommand*{
      GlobalVersionCommand *cmd = new GlobalVersionCommand(dynamic_cast<CommandRunner&>(runner), meta);
      return cmd;
   });
   m_cmdRegisterPool.insert("Global_Help", [](AbstractCommandRunner& runner, const CommandMeta& meta)->AbstractCommand*{
      GlobalHelpCommand *cmd = new GlobalHelpCommand(dynamic_cast<CommandRunner&>(runner), meta);
      return cmd;
   });
   m_cmdRegisterPool.insert("Global_StartServer", [](AbstractCommandRunner& runner, const CommandMeta& meta)->AbstractCommand*{
      StartServerCommand *cmd = new StartServerCommand(dynamic_cast<CommandRunner&>(runner), meta);
      return cmd;
   });
   m_cmdRegisterPool.insert("CC_UpgradeTest", [](AbstractCommandRunner& runner, const CommandMeta& meta)->AbstractCommand*{
      CcUpgradeTestCommand *cmd = new CcUpgradeTestCommand(dynamic_cast<CommandRunner&>(runner), meta);
      return cmd;
   });
}

void CommandRunner::initRouteItems()
{
   addCmdRoute("version", "--version", 1, {
                  {"category", "Global"},
                  {"name", "Version"}
               });
   addCmdRoute("help", "--help", 1, {
                  {"category", "Global"},
                  {"name", "Help"}
               });
   addCmdRoute("startserver", "start [--daemon] [--port]", 1, {
                  {"category", "Global"},
                  {"name", "StartServer"}
               });
   addCmdRoute("ccupgradetest", "ccupgradetest --script= [--upgradedir=] [--upgradedb=]", 1, {
                  {"category", "CC"},
                  {"name", "UpgradeTest"}
               });
}

}//master
}//upgrademgr