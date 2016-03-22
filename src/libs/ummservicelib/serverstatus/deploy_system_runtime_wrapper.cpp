#include "deploy_system_runtime.h"

#include <QProcess>

#include "corelib/io/filesystem.h"
#include "ummlib/global/const.h"

namespace ummservice{
namespace serverstatus{

using sn::corelib::Filesystem;

DeploySystemRuntimeWrapper::DeploySystemRuntimeWrapper(ServiceProvider &provider)
   : AbstractService(provider)
{
}


ServiceInvokeResponse DeploySystemRuntimeWrapper::startMetaServer(const ServiceInvokeRequest &request)
{
   ServiceInvokeResponse response("ServerStatus/DeploySystemRuntime/startMetaServer", true);
   response.setSerial(request.getSerial());
   response.setIsFinal(true);
   int pid = getMetaServerPid();
   if(-1 != pid){
      response.setDataItem("msg", "服务器正在运行");
      return response;
   }
   if(!QProcess::startDetached(MS_SBIN_NAME, {"start"})){
      response.setDataItem("msg", "启动服务器失败, 请重新尝试");
      return response;
   }
   response.setDataItem("msg", "启动服务器成功");
   return response;
}


int DeploySystemRuntimeWrapper::getMetaServerPid()
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

}//serverstatus
}//ummservice