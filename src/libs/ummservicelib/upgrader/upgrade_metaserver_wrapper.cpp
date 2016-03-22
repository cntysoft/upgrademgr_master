#include "upgrade_metaserver.h"

#include <QDebug>
#include <QProcess>
#include <QDir>
#include <QList>
#include <QVariant>
#include <QStringList>
#include <signal.h>
#include "ummlib/global/const.h"

#include "upgrade_upgrademgr_master.h"

#include "corelib/io/filesystem.h"
#include "corelib/kernel/application.h"
#include "corelib/utils/version.h"

#include "ummlib/network/multi_thread_server.h"
#include "ummlib/kernel/stddir.h"
#include "ummlib/global/common_funcs.h"


namespace ummservice{
namespace upgrader{

using ummlib::kernel::StdDir;

using sn::corelib::Filesystem;
using sn::corelib::utils::Version;
using sn::corelib::Application;

const QString UpgradeMetaServerWrapper::RPM_FILENAME_TPL = "meta_server-%1-1.x86_64.rpm";

UpgradeMetaServerWrapper::UpgradeMetaServerWrapper(ServiceProvider &provider)
   : AbstractService(provider),
     m_softwareRepoDir(StdDir::getSoftwareRepoDir())
{
}

ServiceInvokeResponse UpgradeMetaServerWrapper::upgrade(const ServiceInvokeRequest &request)
{
   const QMap<QString, QVariant> &args = request.getArgs();
   checkRequireFields(args, {"version"});
   ServiceInvokeResponse response("Upgrader/UpgradeMetaServerWrapper/upgrade", true);
   response.setSerial(request.getSerial());
   response.setDataItem("msg", "开始准备更新");
   writeInterResponse(request, response);
   QString targetVersionStr = args.value("version").toString();
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
   response.setDataItem("msg", "更新成功,正在重启服务器");
   writeInterResponse(request, response);
   int pid = getMetaServerPid();
   if(pid != -1){
      kill(pid, SIGINT);
   }
   if(!QProcess::startDetached(MS_SBIN_NAME, {"start"})){
      response.setStatus(false);
      response.setError({1, "重启服务器失败, 升级失败"});
      return response;
   }
   response.setDataItem("msg", "升级成功");
   return response;
}

int UpgradeMetaServerWrapper::getMetaServerPid()
{
   QProcess process;
   QStringList args;
   args << "pidfilename";
   process.start(MS_SBIN_NAME, args);
   process.waitForFinished(-1);
   if(0 != process.exitCode()){
      return -1;
   }
   QString pidFilename = process.readAllStandardOutput();
   if(!Filesystem::fileExist(pidFilename)){
      return -1;
   }
   return Filesystem::fileGetContents(pidFilename).toInt();
}

bool UpgradeMetaServerWrapper::installRpmPackage(const QString &filename, QString &errorString)
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