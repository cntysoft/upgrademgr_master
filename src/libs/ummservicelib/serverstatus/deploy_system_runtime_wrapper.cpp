#include "deploy_system_runtime.h"

#include <QProcess>
#include <QThread>
#include <signal.h>

#include "corelib/io/filesystem.h"
#include "ummlib/global/const.h"

namespace ummservice{
namespace serverstatus{

using sn::corelib::Filesystem;

DeploySystemMetaServerRuntimeWrapper::DeploySystemMetaServerRuntimeWrapper(ServiceProvider &provider)
   : AbstractService(provider)
{
}


ServiceInvokeResponse DeploySystemMetaServerRuntimeWrapper::startServer(const ServiceInvokeRequest &request)
{
   ServiceInvokeResponse response("ServerStatus/DeploySystemMetaServerRuntime/startServer", true);
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

ServiceInvokeResponse DeploySystemMetaServerRuntimeWrapper::stopServer(const ServiceInvokeRequest &request)
{
   ServiceInvokeResponse response("ServerStatus/DeploySystemMetaServerRuntime/stopServer", true);
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

ServiceInvokeResponse DeploySystemMetaServerRuntimeWrapper::restartServer(const ServiceInvokeRequest &request)
{
   ServiceInvokeResponse response("ServerStatus/DeploySystemMetaServerRuntime/startServer", true);
   response.setSerial(request.getSerial());
   response.setIsFinal(true);
   int pid = getMetaServerPid();
   if(-1 != pid){
      kill(pid, SIGINT);
   }
   QThread::msleep(500);
   if(Filesystem::fileExist(m_metaServerPidFilename)){
      Filesystem::deleteFile(m_metaServerPidFilename);
   }
   if(!QProcess::startDetached(MS_SBIN_NAME, {"start"})){
      response.setDataItem("msg", "启动服务器失败, 请重新尝试");
      return response;
   }
   response.setDataItem("msg", "重新启动服务器成功");
   return response;
}

int DeploySystemMetaServerRuntimeWrapper::getMetaServerPid()
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

void deploy_system_luoxi_status_manage_handler(const ServiceInvokeResponse &response, void* args)
{
   //需要判断对错
   DeploySystemLuoXiRuntimeWrapper *self = static_cast<DeploySystemLuoXiRuntimeWrapper*>(args);
   if(response.getStatus()){
      self->m_context->response.setIsFinal(false);
      self->m_context->response.setDataItem("msg", response.getDataItem("msg"));
      self->m_context->response.setDataItem("isFinal", true);
      self->writeInterResponse(self->m_context->request, self->m_context->response);
   }else{
      //错误处理
      self->m_context->response.setStatus(false);
      self->m_context->response.setIsFinal(false);
      self->m_context->response.setError(response.getError());
      self->writeInterResponse(self->m_context->request, self->m_context->response);
      self->m_serviceInvoker->disconnectFromServer();
   }
   self->clearState();
}

DeploySystemLuoXiRuntimeWrapper::DeploySystemLuoXiRuntimeWrapper(ServiceProvider &provider)
   : AbstractService(provider)
{
}

ServiceInvokeResponse DeploySystemLuoXiRuntimeWrapper::startServer(const ServiceInvokeRequest &request)
{
   ServiceInvokeRequest operateRequest("ServerStatus/DeploySystemLuoXiRuntime", "startServer");
   ServiceInvokeResponse response = doOperate(request, operateRequest);
   response.setSignature("ServerStatus/DeploySystemLuoXiRuntime/startServer");
   return response;
}

ServiceInvokeResponse DeploySystemLuoXiRuntimeWrapper::restartServer(const ServiceInvokeRequest &request)
{
   ServiceInvokeRequest operateRequest("ServerStatus/DeploySystemLuoXiRuntime", "restartServer");
   ServiceInvokeResponse response = doOperate(request, operateRequest);
   response.setSignature("ServerStatus/DeploySystemLuoXiRuntime/restartServer");
   return response;
}

ServiceInvokeResponse DeploySystemLuoXiRuntimeWrapper::stopServer(const ServiceInvokeRequest &request)
{
   ServiceInvokeRequest operateRequest("ServerStatus/DeploySystemLuoXiRuntime", "stopServer");
   ServiceInvokeResponse response = doOperate(request, operateRequest);
   response.setSignature("ServerStatus/DeploySystemLuoXiRuntime/stopServer");
   return response;
}

ServiceInvokeResponse DeploySystemLuoXiRuntimeWrapper::doOperate(const ServiceInvokeRequest& request, const ServiceInvokeRequest& operateRequest)
{
   const QMap<QString, QVariant> &args = request.getArgs();
   checkRequireFields(args, {"serverAddress"});
   ServiceInvokeResponse response("ServerStatus/DeploySystemLuoXiRuntime/doOperate", true);
   response.setSerial(request.getSerial());
   if(m_isInAction){
      response.setIsFinal(true);
      response.setStatus(false);
      response.setError({10000, "操作正在进行中"});
      return response;
   }
   m_isInAction = true;
   response.setIsFinal(false);
   m_context.reset(new StatusManageContext);
   m_context->operateRequest = operateRequest;
   m_context->request = request;
   m_context->response = response;
   QString slaveServerAddress = args.value("serverAddress").toString();
   m_context->targetServerAddress = slaveServerAddress;
   response.setDataItem("msg", QString("正在连接UpgradeMgr slave服务器 %1").arg(slaveServerAddress));
   response.setDataItem("isFinal", false);
   QSharedPointer<ServiceInvoker> invoker = getServiceInvoker(slaveServerAddress, UMS_LISTEN_PORT);
   m_context->serviceInvoker = invoker;
   connect(invoker.data(), &ServiceInvoker::connectedToServerSignal, this, &DeploySystemLuoXiRuntimeWrapper::connectToServerHandler);
   connect(invoker.data(), &ServiceInvoker::connectErrorSignal, this, &DeploySystemLuoXiRuntimeWrapper::connectToServerErrorHandler);
   writeInterResponse(request, response);
   invoker->connectToServer();
   m_eventLoop.exec();
   response.setIsFinal(true);
   response.setDataItem("isFinal", true);
   response.setDataItem("msg", "");
   return response;
}

void DeploySystemLuoXiRuntimeWrapper::connectToServerHandler()
{
   m_context->response.setDataItem("msg", QString("连接服务器成功 [%1:%2]").arg(m_context->targetServerAddress).arg(UMS_LISTEN_PORT));
   m_context->response.setDataItem("isFinal", false);
   writeInterResponse(m_context->request, m_context->response);
   m_context->response.setDataItem("msg", "向upgrademgr_slave服务器发送操作请求");
   m_context->response.setDataItem("isFinal", false);
   writeInterResponse(m_context->request, m_context->response);
   m_context->serviceInvoker->request(m_context->operateRequest, deploy_system_luoxi_status_manage_handler, static_cast<void*>(this));
}

void DeploySystemLuoXiRuntimeWrapper::connectToServerErrorHandler()
{
   m_context->response.setStatus(false);
   m_context->response.setError({-1, QString("连接服务器失败 [%1:%2]").arg(m_context->targetServerAddress).arg(UMS_LISTEN_PORT)});
   writeInterResponse(m_context->request, m_context->response);
   clearState();
   m_eventLoop.exit(0);
}

void DeploySystemLuoXiRuntimeWrapper::clearState()
{
   m_isInAction= false;
   m_context.clear();
   m_eventLoop.exit(0);
   if(!m_serviceInvoker.isNull()){
      disconnect(m_serviceInvoker.data(), &ServiceInvoker::connectedToServerSignal, this, &DeploySystemLuoXiRuntimeWrapper::connectToServerHandler);
      disconnect(m_serviceInvoker.data(), &ServiceInvoker::connectErrorSignal, this, &DeploySystemLuoXiRuntimeWrapper::connectToServerErrorHandler);
      m_serviceInvoker->resetStatus();
   }
}



}//serverstatus
}//ummservice