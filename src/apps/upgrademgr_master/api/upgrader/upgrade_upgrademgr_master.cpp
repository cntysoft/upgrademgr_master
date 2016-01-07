#include <QDebug>
#include <QProcess>
#include <QDir>
#include "upgrade_upgrademgr_master.h"
#include "application.h"
#include "ummlib/network/multi_thread_server.h"

namespace upgrademgr{
namespace master{
namespace api{
namespace upgrader{

using upgrademgr::master::Application;

UpgradeUpgradeMgrMaster::UpgradeUpgradeMgrMaster(sn::corelib::network::ApiProvider &provider)
   : AbstractApi(provider)
{
}

ApiInvokeResponse UpgradeUpgradeMgrMaster::upgrade(const ApiInvokeRequest&)
{
   m_apiProvider.disconnectUnderlineSockets();
   ummlib::network::MultiThreadServer *& server = ummlib::network::get_global_server();
   server->close();
   QStringList args = Application::instance()->arguments();
   args.takeFirst();
   args.removeAll("--daemon");
   if(QProcess::startDetached(Application::applicationFilePath(), args, QDir::currentPath())){
      Application::instance()->exit(EXIT_SUCCESS);
   }
   Application::instance()->exit(1);
   return ApiInvokeResponse("Upgrader/UpgradeUpgrademgrMaster/upgrade", true);
}

}//upgrader
}//api
}//master
}//upgrademgr