#include "deploy_system_runtime.h"

#include <QProcess>
#include <QThread>
#include <signal.h>

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

ServiceInvokeResponse DeploySystemRuntimeWrapper::stopMetaServer(const ServiceInvokeRequest &request)
{
   ServiceInvokeResponse response("ServerStatus/DeploySystemRuntime/startMetaServer", true);
   response.setSerial(request.getSerial());
   response.setIsFinal(true);
   int pid = getMetaServerPid();
   if(-1 == pid){
      response.setDataItem("msg", "服务器已经停止");
      return response;
   }
   kill(pid, SIGINT);
   response.setDataItem("msg", "服务器关闭成功");
   return response;
}

ServiceInvokeResponse DeploySystemRuntimeWrapper::restartMetaServer(const ServiceInvokeRequest &request)
{
   ServiceInvokeResponse response("ServerStatus/DeploySystemRuntime/startMetaServer", true);
   response.setSerial(request.getSerial());
   response.setIsFinal(true);
   int pid = getMetaServerPid();
   if(-1 != pid){
      kill(pid, SIGINT);
   }
   while(Filesystem::fileExist(m_metaServerPidFilename)){
      QThread::msleep(100);
   }
   if(!QProcess::startDetached(MS_SBIN_NAME, {"start"})){
      response.setDataItem("msg", "启动服务器失败, 请重新尝试");
      return response;
   }
   response.setDataItem("msg", "重新启动服务器成功");
   return response;
}

int DeploySystemRuntimeWrapper::getMetaServerPid()
{
   if(m_metaServerPidFilename.isEmpty()){
      QProcess process;
      QStringList args;
      args << "pidfilename";
      process.start(MS_SBIN_NAME, args);
      process.waitForFinished(-1);
      if(0 != process.exitCode()){
         return -1;
      }
      m_metaServerPidFilename = process.readAllStandardOutput();
   }
   if(!Filesystem::fileExist(m_metaServerPidFilename)){
      return -1;
   }
   return Filesystem::fileGetContents(m_metaServerPidFilename).toInt();
}

}//serverstatus
}//ummservice