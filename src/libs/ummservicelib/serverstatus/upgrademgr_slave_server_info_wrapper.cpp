#include "upgrademgr_slave_server_info.h"

#include "ummlib/global/const.h"

namespace ummservice{
namespace serverstatus{

using sn::corelib::network::ServiceInvokeRequest;

void upgrademgr_slave_server_info_handler(const ServiceInvokeResponse &response, void* args)
{
   //需要判断对错
   UpgradeMgrSlaveServerInfoWrapper *self = static_cast<UpgradeMgrSlaveServerInfoWrapper*>(args);
   if(response.getStatus()){
      self->m_context->response->setDataItem("version", response.getDataItem("version"));
      self->writeInterResponse(self->m_context->request, *self->m_context->response);
      self->m_serviceInvoker->disconnectFromServer();
   }else{
      //错误处理
      self->m_context->response->setStatus(false);
      self->m_context->response->setIsFinal(false);
      self->m_context->response->setError(response.getError());
      self->writeInterResponse(self->m_context->request, *self->m_context->response);
      self->m_serviceInvoker->disconnectFromServer();
   }
   self->clearState();
}

UpgradeMgrSlaveServerInfoWrapper::UpgradeMgrSlaveServerInfoWrapper(ServiceProvider &provider)
   : AbstractService(provider)
{
}

ServiceInvokeResponse UpgradeMgrSlaveServerInfoWrapper::getServerVersion(const ServiceInvokeRequest &request)
{
   ServiceInvokeRequest operateRequest("ServerStatus/ServerInfo", "getVersionInfo");
   ServiceInvokeResponse response = doOperate(request, operateRequest);
   response.setSignature("ServerStatus/UpgradeMgrSlaveServerInfo/getServerVersion");
   return response;
}

ServiceInvokeResponse UpgradeMgrSlaveServerInfoWrapper::doOperate(const ServiceInvokeRequest& request, const ServiceInvokeRequest& operateRequest)
{
   const QMap<QString, QVariant> &args = request.getArgs();
   checkRequireFields(args, {"serverAddress"});
   ServiceInvokeResponse response("ServerStatus/UpgradeMgrSlaveServerInfo/doOperate", true);
   response.setSerial(request.getSerial());   
   if(m_isInAction){
      response.setIsFinal(true);
      response.setStatus(false);
      response.setError({10000, "操作正在进行中"});
      return response;
   }
   m_isInAction = true;
   response.setIsFinal(false);
   m_context.reset(new ServerInfoContext);
   m_context->request = request;
   m_context->operateRequest = operateRequest;
   m_context->response = &response;
   QString serverAddress = args.value("serverAddress").toString();
   m_context->serverAddress = serverAddress;
   response.setDataItem("msg", QString("正在连接luoxi deploy服务器 %1").arg(serverAddress));
   QSharedPointer<ServiceInvoker> invoker = getServiceInvoker(serverAddress, UMS_LISTEN_PORT);
   m_context->serviceInvoker = invoker;
   connect(invoker.data(), &ServiceInvoker::connectedToServerSignal, this, &UpgradeMgrSlaveServerInfoWrapper::connectToServerHandler);
   connect(invoker.data(), &ServiceInvoker::connectErrorSignal, this, &UpgradeMgrSlaveServerInfoWrapper::connectToServerErrorHandler);
   writeInterResponse(request, response);
   invoker->connectToServer();
   m_eventLoop.exec();
   response.setIsFinal(true);
   return response;
}

void UpgradeMgrSlaveServerInfoWrapper::connectToServerHandler()
{
   m_context->response->setDataItem("msg", QString("连接UpgrademgrSlave服务器成功 [%1:%2]").arg(m_context->serverAddress).arg(UMS_LISTEN_PORT));
   writeInterResponse(m_context->request, *m_context->response);
   m_context->response->setDataItem("msg", "向upgrademgr_slave服务器发送请求");
   writeInterResponse(m_context->request, *m_context->response);
   m_context->serviceInvoker->request(m_context->operateRequest, upgrademgr_slave_server_info_handler, static_cast<void*>(this));
}

void UpgradeMgrSlaveServerInfoWrapper::connectToServerErrorHandler()
{
   m_context->response->setStatus(false);
   m_context->response->setError({-1, QString("连接UpgrademgrSlave服务器失败 [%1:%2]").arg(m_context->serverAddress).arg(UMS_LISTEN_PORT)});
   writeInterResponse(m_context->request, *m_context->response);
   clearState();
}

void UpgradeMgrSlaveServerInfoWrapper::clearState()
{
   m_isInAction= false;
   m_context.clear();
   m_eventLoop.exit(0);
   if(!m_serviceInvoker.isNull()){
      disconnect(m_serviceInvoker.data(), &ServiceInvoker::connectedToServerSignal, this, &UpgradeMgrSlaveServerInfoWrapper::connectToServerHandler);
      disconnect(m_serviceInvoker.data(), &ServiceInvoker::connectErrorSignal, this, &UpgradeMgrSlaveServerInfoWrapper::connectToServerErrorHandler);
      m_serviceInvoker->resetStatus();
   }
}

}//serverstatus
}//ummservice