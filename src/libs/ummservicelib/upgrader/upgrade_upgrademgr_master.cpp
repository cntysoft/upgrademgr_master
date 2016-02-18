#include <QDebug>
#include <QProcess>
#include <QDir>
#include <QList>
#include <QVariant>
#include <QStringList>

#include "corelib/io/filesystem.h"

#include "ummlib/network/multi_thread_server.h"
#include "ummlib/kernel/stddir.h"
#include "upgrade_upgrademgr_master.h"

namespace ummservice{
namespace upgrader{

using ummlib::kernel::StdDir;
using sn::corelib::Filesystem;

const QString UpgradeUpgradeMgrMaster::RPM_FILENAME_TPL = "upgrademgr_master-%1-1.x86_64.rpm";

UpgradeUpgradeMgrMaster::UpgradeUpgradeMgrMaster(sn::corelib::network::ServiceProvider &provider)
   : AbstractService(provider),
     m_softwareRepoDir(StdDir::getSoftwareRepoDir())
{
}

ServiceInvokeResponse UpgradeUpgradeMgrMaster::upgrade(const ServiceInvokeRequest &request)
{
//   QList<QVariant> requestParams = request.getArgs();
//   QString targetVersion = requestParams.takeLast().toString();
//   ServiceInvokeResponse response("Upgrader/UpgradeUpgrademgrMaster/upgrade", true);
//   response.setSerial(request.getSerial());
//   response.setDataItem("msg", "开始准备更新");
//   writeInterResponse(request.getSocketNum(), response);
//   response.setDataItem("msg", "比较当前服务器的版本");
//   QString currentVersion = upgrademgr::master::get_upgrademgr_master_version();
//   if(versionCompare(currentVersion, targetVersion) >= 0){
//      response.setDataItem("lasted", true);
//      response.setDataItem("msg", "当前已是最新版本，无须更新");
//      return response;
//   }
//   response.setDataItem("msg", "开始检查指定版本的服务器RPM包");
//   QString filename = m_softwareRepoDir + "/" +QString(RPM_FILENAME_TPL).arg(targetVersion);
//   if(!Filesystem::fileExist(filename)){
//      response.setError({1, QString("软件包:%1不存在").arg(filename)});
//      response.setStatus(false);
//      return response;
//   }
//   response.setDataItem("msg", "开始安装RPM包");
//   writeInterResponse(request.getSocketNum(), response);
//   QString errorString;
//   int status = installRpmPackage(filename, errorString);
//   if(!status){
//      response.setStatus(false);
//      response.setError({1, errorString});
//      return response;
//   }
//   response.setDataItem("msg", "更新完成");
//   m_serviceProvider.disconnectUnderlineSockets();
//   ummlib::network::MultiThreadServer *& server = ummlib::network::get_global_server();
//   server->close();
//   QStringList args = Application::instance()->arguments();
//   args.takeFirst();
//   args.removeAll("--daemon");
//   if(QProcess::startDetached(upgrademgr::master::get_application_filepath(), args, QDir::currentPath())){
//      Application::instance()->exit(EXIT_SUCCESS);
//   }
//   Application::instance()->exit(1);
//   return response;
   return ServiceInvokeResponse();
}

int UpgradeUpgradeMgrMaster::versionCompare(const QString &version1, const QString &version2)
{
   QString left = version1.mid(1);
   QString right = version2.mid(2);
   QStringList leftParts = left.split('.');
   QStringList rightParts = right.split('.');
   for(int i = 0; i < leftParts.size(); i++){
      int leftValue = leftParts[i].toInt();
      int rightValue = rightParts[i].toInt();
      if(leftValue < rightValue){
         return -1;
      }else if(leftValue > rightValue){
         return 1;
      }
   }
   return 0;
}

bool UpgradeUpgradeMgrMaster::installRpmPackage(const QString &filename, QString &errorString)
{
   QProcess process;
   QStringList args;
   args << "-i" << "--force" << filename;
   process.start("rpm", args);
   process.waitForFinished();
   if(0 != process.exitCode()){
      errorString = process.readAllStandardError();
      errorString.remove('\n');
      return false;
   }
   return true;
}

}//upgrader
}//ummservice