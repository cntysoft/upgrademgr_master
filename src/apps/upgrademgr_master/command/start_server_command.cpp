#include <QString>
#include <QTcpServer>
#include <QProcess>
#include <QDir>
#include <QVariant>

#include "start_server_command.h"
#include "utils/common_funcs.h"

#include "corelib/command/command_meta.h"
#include "corelib/kernel/errorinfo.h"
#include "corelib/kernel/settings.h"
#include "ummlib/global/const.h"
#include "ummlib/network/multi_thread_server.h"

namespace upgrademgr{
namespace master{
namespace command{

using sn::corelib::CommandMeta;
using sn::corelib::ErrorInfo;
using sn::corelib::Settings;

using ummlib::network::MultiThreadServer;

using upgrademgr::master::Application;
using upgrademgr::master::get_app_ref;

StartServerCommand::StartServerCommand(AbstractCommandRunner& runner, const CommandMeta& invokeMeta)
   : AbstractCommand(runner, invokeMeta)
{
}

void StartServerCommand::exec()
{
   qint16 port = getMetaServerListenPort();
   if(0 == port){
      throw ErrorInfo(QString("port %1 is not allow").arg(port));
   }
   bool daemon = m_invokeMeta.getCmdArgs().value("daemon") == "true" ? true : false;
   Application& app = *get_app_ref();
   if(daemon){
      QStringList args = app.arguments();
      args.takeFirst();
      args.removeAll("--daemon");
      if(QProcess::startDetached(Application::applicationFilePath(), args, QDir::currentPath())){
         app.exit(EXIT_SUCCESS);
      }else{
         throw ErrorInfo("start daemon failure, Failed to create process");
      }
   }
   MultiThreadServer* server = new MultiThreadServer(app);
   server->setHost(QHostAddress::Any);
   server->setPort(port);
   app.createPidFile();
   bool status = server->run();
   //防止内存泄漏,这里利用闭包复制指针
   QObject::connect(&app, &Application::aboutToQuit, [server, &app](){
      delete server;
      app.deletePidFile();
   });
   if(!status){
      throw ErrorInfo(server->errorString());
   }
}

qint16 StartServerCommand::getMetaServerListenPort()
{
   //优先级 命令行 > 配置文件 > 系统常量定义
   qint16 port;
   const CommandMeta::CmdArgType& args = m_invokeMeta.getCmdArgs();
   if(args.contains("port")){
      port = args.value("port").toInt();
      if(0 != port){
         return port;
      }
   }
   Settings& settings = m_cmdRunner.getSysSettings();
   if(settings.hasKey("listenPort")){
      port = settings.getValue("listenPort", UMM_CFG_GROUP_GLOBAL).toInt();
      if(0 != port){
         return port;
      }
   }
   port = UMM_LISTEN_PORT;
   return port;
}

}//command
}//master
}//upgrademgr