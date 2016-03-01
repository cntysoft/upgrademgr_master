#include <QString>
#include <QTcpServer>
#include <QProcess>
#include <QDir>
#include <QVariant>
#include <QDebug>

#include "start_server_command.h"
#include "utils/common_funcs.h"

#include "corelib/command/command_meta.h"
#include "corelib/kernel/errorinfo.h"
#include "corelib/kernel/settings.h"
#include "corelib/network/web_service_server.h"
#include "ummlib/network/multi_thread_server.h"
#include "ummlib/global/const.h"

namespace upgrademgr{
namespace master{
namespace command{

using sn::corelib::CommandMeta;
using sn::corelib::ErrorInfo;
using sn::corelib::Settings;

using ummlib::network::MultiThreadServer;
using sn::corelib::network::WebServiceServer;

using upgrademgr::master::Application;
using upgrademgr::master::get_app_ref;

StartServerCommand::StartServerCommand(AbstractCommandRunner& runner, const CommandMeta& invokeMeta)
   : AbstractCommand(runner, invokeMeta)
{
}

void StartServerCommand::exec()
{
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
   MultiThreadServer *server = new MultiThreadServer(app);
   WebServiceServer *webServiceServer = new WebServiceServer(app, "websocketserver");
   ummlib::network::set_global_server(server);
   sn::corelib::network::set_global_web_service_server(webServiceServer);
   server->setHost(QHostAddress::Any);
   server->setPort(UMM_LISTEN_PORT);
   webServiceServer->setHost(QHostAddress::Any);
   webServiceServer->setPort(UMM_LISTEN_PORT + 1);
   app.createPidFile();
   bool status = server->run();
   bool webServiceStatus = webServiceServer->run();
   //防止内存泄漏,这里利用闭包复制指针
   QObject::connect(&app, &Application::aboutToQuit, [&app](){
      MultiThreadServer *&server = ummlib::network::get_global_server();
      if(nullptr != server){
         delete server;
      }
      WebServiceServer *&webServiceServer = sn::corelib::network::get_global_web_service_server();
      if(nullptr != webServiceServer){
         delete webServiceServer;
      }
      app.deletePidFile();
   });
   if(!status){
      throw ErrorInfo(server->errorString());
   }
   if(!webServiceStatus){
      throw ErrorInfo(webServiceServer->errorString());
   }
}


}//command
}//master
}//upgrademgr