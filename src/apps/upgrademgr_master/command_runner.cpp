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
using upgrademgr::master::command::SetWebServerSoftwareItemCommand;
using upgrademgr::master::command::DeleteWebServerSoftwareItemCommand;
using upgrademgr::master::command::ListWebServerSoftwareItemCommand;

CommandRunner::CommandRunner(Application &app)
   : AbstractCommandRunner(app)
{
   addUsageText("welcome to use sheneninfo upgrademgr master system\n", TerminalColor::Green);
   addUsageText("usage: \n", TerminalColor::LightBlue);
   addUsageText("--version  print main system version number\n");
   addUsageText("--help     print help document\n");
   addUsageText("start [--daemon] [--port] start upgrademgr server\n");
   addUsageText("webserversoftware set <name> <version> 在指定的环境列表中添加一个软件\n");
   addUsageText("webserversoftware delete <name> 在指定的环境列表中添加一个软件\n");
   addUsageText("webserversoftware list 获取当前环境软件库列表\n");
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
   m_cmdRegisterPool.insert("Global_SetWebServerSoftwareItem", [](AbstractCommandRunner& runner, const CommandMeta& meta)->AbstractCommand*{
      SetWebServerSoftwareItemCommand *cmd = new SetWebServerSoftwareItemCommand(dynamic_cast<CommandRunner&>(runner), meta);
      return cmd;
   });
   m_cmdRegisterPool.insert("Global_DeleteWebServerSoftwareItem", [](AbstractCommandRunner& runner, const CommandMeta& meta)->AbstractCommand*{
      DeleteWebServerSoftwareItemCommand *cmd = new DeleteWebServerSoftwareItemCommand(dynamic_cast<CommandRunner&>(runner), meta);
      return cmd;
   });
   m_cmdRegisterPool.insert("Global_ListWebServerSoftwareItem", [](AbstractCommandRunner& runner, const CommandMeta& meta)->AbstractCommand*{
      ListWebServerSoftwareItemCommand *cmd = new ListWebServerSoftwareItemCommand(dynamic_cast<CommandRunner&>(runner), meta);
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
   addCmdRoute("setwebserversoftware", "webserversoftware set <name> <version>", 1, {
                  {"category", "Global"},
                  {"name", "SetWebServerSoftwareItem"}
               });
   addCmdRoute("deletewebserversoftware", "webserversoftware delete <name>", 1, {
                  {"category", "Global"},
                  {"name", "DeleteWebServerSoftwareItem"}
               });
   addCmdRoute("listwebserversoftware", "webserversoftware list", 1, {
                  {"category", "Global"},
                  {"name", "ListWebServerSoftwareItem"}
               });
}

}//master
}//upgrademgr