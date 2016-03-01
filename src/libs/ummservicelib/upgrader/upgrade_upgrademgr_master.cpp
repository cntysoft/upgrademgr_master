#include <QDebug>
#include <QProcess>
#include <QDir>
#include <QList>
#include <QVariant>
#include <QStringList>

#include "upgrade_upgrademgr_master_wrapper.h"

#include "corelib/io/filesystem.h"

#include "ummlib/network/multi_thread_server.h"
#include "ummlib/kernel/stddir.h"
#include "ummlib/global/common_funcs.h"
#include "ummlib/global/common_funcs.h"

#include "corelib/kernel/application.h"
#include "corelib/utils/version.h"

namespace ummservice{
namespace upgrader{

using ummlib::kernel::StdDir;

using sn::corelib::Filesystem;
using sn::corelib::utils::Version;
using sn::corelib::Application;

const QString UpgradeUpgradeMgrMasterWrapper::RPM_FILENAME_TPL = "upgrademgr_master-%1-1.x86_64.rpm";

UpgradeUpgradeMgrMasterWrapper::UpgradeUpgradeMgrMasterWrapper(sn::corelib::network::ServiceProvider &provider)
   : AbstractService(provider),
     m_softwareRepoDir(StdDir::getSoftwareRepoDir())
{
}

ServiceInvokeResponse UpgradeUpgradeMgrMasterWrapper::upgrade(const ServiceInvokeRequest &request)
{
   const QMap<QString, QVariant> &args = request.getArgs();
   checkRequireFields(args, {"version"});
   ServiceInvokeResponse response("Upgrader/UpgradeUpgrademgrMaster/upgrade", true);
   response.setSerial(request.getSerial());
   response.setDataItem("msg", "开始准备更新");
   writeInterResponse(request, response);
   response.setDataItem("msg", "比较当前服务器的版本");
   writeInterResponse(request, response);
   QString ummVerStr = ummlib::global::get_upgrademgr_master_version();
   response.setDataItem("msg", QString("当前版本号 %1").arg(ummVerStr));
   writeInterResponse(request, response);
   QString targetVersionStr = args.value("version").toString();
   Version targetVersion(targetVersionStr);
   Version currentVersion(ummVerStr);
   if(targetVersion <= currentVersion){
      response.setStatus(false);
      response.setError({-1, "目标版本比当前版本低，无须更新"});
      return response;
   }
   
   response.setDataItem("msg", "开始检查指定版本的服务器RPM包");
   QString filename = m_softwareRepoDir + "/" +QString(RPM_FILENAME_TPL).arg(targetVersionStr);
   if(!Filesystem::fileExist(filename)){
      response.setError({1, QString("软件包:%1不存在").arg(filename)});
      response.setStatus(false);
      return response;
   }
   response.setDataItem("msg", "开始安装RPM包");
   writeInterResponse(request, response);
   QString errorString;
   int status = installRpmPackage(filename, errorString);
   if(!status){
      response.setStatus(false);
      response.setError({1, errorString});
      return response;
   }
   response.setDataItem("msg", "更新成功,请手动重新启动服务器");
   writeInterResponse(request, response);
   m_serviceProvider.disconnectUnderlineSockets();
   ummlib::network::MultiThreadServer *& server = ummlib::network::get_global_server();
   server->close();
//   QStringList appArgs = Application::instance()->arguments();
//   appArgs.takeFirst();
//   appArgs.removeAll("--daemon");
//   qDebug() << ummlib::global::get_application_filepath();
//   if(QProcess::startDetached(ummlib::global::get_application_filepath(), appArgs, QDir::currentPath())){
//      Application::instance()->exit(EXIT_SUCCESS);
//   }
   Application::instance()->exit(1);
   return response;
}


bool UpgradeUpgradeMgrMasterWrapper::installRpmPackage(const QString &filename, QString &errorString)
{
   QProcess process;
   QStringList args;
   args << "-i" << "--force" << "--nodeps" << filename;
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