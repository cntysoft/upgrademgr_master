#include <QDir>
#include <QList>
#include <QVariant>
#include <QStringList>
#include <QMap>
#include <QString>
#include <QDebug>
#include <QSharedPointer>
#include <QVariant>

#include "upgrade_cloudcontroller.h"
#include "corelib/network/rpc/service_invoker.h"
#include "corelib/io/filesystem.h"
#include "ummservicelib/serverstatus/server_info.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "global/const.h"

namespace ummservice{
namespace upgrader{

using sn::corelib::network::ServiceInvokeRequest;
using ummservice::serverstatus::Info;

void upgrade_cloudcontroller_handler(const ServiceInvokeResponse &response, void* args)
{
   //需要判断对错
   UpgradeCloudControllerWrapper *self = static_cast<UpgradeCloudControllerWrapper*>(args);
   if(response.getStatus()){
      self->m_context->response.setIsFinal(false);
      self->m_context->response.setDataItem("msg", response.getDataItem("msg"));
      self->m_context->response.setDataItem("step", response.getDataItem("step"));
      self->writeInterResponse(self->m_context->request, self->m_context->response);
      if(response.getDataItem("step").toInt() == UpgradeCloudControllerWrapper::STEP_FINISH){
         self->m_serviceInvoker->disconnectFromServer();
         self->clearState();
      }
   }else{
      //错误处理
      self->m_context->response.setStatus(false);
      self->m_context->response.setIsFinal(false);
      self->m_context->response.setError(response.getError());
      self->writeInterResponse(self->m_context->request, self->m_context->response);
      self->clearState();
      self->m_serviceInvoker->disconnectFromServer();
   }
}

UpgradeCloudControllerWrapper::UpgradeCloudControllerWrapper(ServiceProvider &provider)
   : AbstractService(provider)
{
}

ServiceInvokeResponse UpgradeCloudControllerWrapper::upgrade(const ServiceInvokeRequest &request)
{
   const QMap<QString, QVariant> &args = request.getArgs();
   checkRequireFields(args, {"fromVersion", "toVersion"});
   ServiceInvokeResponse response("Upgrader/UpgradeCloudController/upgrade", true);
   if(m_isInAction){
      response.setIsFinal(true);
      response.setStatus(false);
      response.setError({10000, "操作正在进行中"});
      return response;
   }
   m_isInAction = true;
   response.setSerial(request.getSerial());
   response.setIsFinal(false);
   m_context.reset(new UpgradeContext);
   m_context->request = request;
   m_context->response = response;
   response.setDataItem("msg", "开始获取云控制器服务器地址");
   writeInterResponse(request, response);
   Info info;
   QMap<QString, QString> meta = info.getServiceServerAddressMetaByKey("CloudController");
   response.setDataItem("msg", QString("目标更新服务器地址 %1").arg(meta.value("ip")));
   writeInterResponse(request, response);
   response.setDataItem("msg", "正在连接更新服务器");
   writeInterResponse(request, response);
   m_context->targetServerAddress = meta.value("ip");
   QSharedPointer<ServiceInvoker> invoker = getServiceInvoker(m_context->targetServerAddress, UMS_LISTEN_PORT);
   m_context->serviceInvoker = invoker;
   connect(invoker.data(), &ServiceInvoker::connectedToServerSignal, this, &UpgradeCloudControllerWrapper::connectToServerHandler);
   connect(invoker.data(), &ServiceInvoker::connectErrorSignal, this, &UpgradeCloudControllerWrapper::connectToServerErrorHandler);
   invoker->connectToServer();
   m_eventLoop.exec();
   response.setIsFinal(true);
   response.setDataItem("msg", "");
   return response;
}

void UpgradeCloudControllerWrapper::connectToServerHandler()
{
   m_context->response.setDataItem("msg", QString("连接服务器成功 [%1:%2]").arg(m_context->targetServerAddress).arg(UMS_LISTEN_PORT));
   writeInterResponse(m_context->request, m_context->response);
   m_context->response.setDataItem("msg", "向upgrademgr_slave服务器发送升级请求");
   writeInterResponse(m_context->request, m_context->response);
   ServiceInvokeRequest serviceRequest("Upgrader/UpgradeCloudController", "upgrade", {
                                          {"fromVersion", m_context->fromVersion}, 
                                          {"toVersion", m_context->toVersion}
                                       });
   m_context->serviceInvoker->request(serviceRequest, upgrade_cloudcontroller_handler, static_cast<void*>(this));
}

void UpgradeCloudControllerWrapper::connectToServerErrorHandler()
{
   m_context->response.setStatus(false);
   m_context->response.setError({-1, QString("连接服务器失败 [%1:%2]").arg(m_context->targetServerAddress).arg(UMS_LISTEN_PORT)});
   writeInterResponse(m_context->request, m_context->response);
   clearState();
   m_eventLoop.exit(0);
}

void UpgradeCloudControllerWrapper::clearState()
{
   m_isInAction= false;
   m_context.clear();
   m_eventLoop.exit(0);
   if(!m_serviceInvoker.isNull()){
      m_serviceInvoker->disconnect();
   }
}
//void UpgradeCloudControllerWrapper::notifySocketDisconnect(QWebSocket *socket)
//{

//}

}//upgrader
}//ummservice