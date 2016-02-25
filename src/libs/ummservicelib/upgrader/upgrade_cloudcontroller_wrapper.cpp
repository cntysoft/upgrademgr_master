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

using sn::corelib::network::ServiceInvoker;
using sn::corelib::network::ServiceInvokeRequest;
using ummservice::serverstatus::Info;

void init_upgrade_handler(const ServiceInvokeResponse &response, void* args)
{
   //需要判断对错
   UpgradeCloudControllerWrapper *self = static_cast<UpgradeCloudControllerWrapper*>(args);
   if(response.getStatus()){
      self->m_context.response.setIsFinal(false);
      self->m_context.response.setDataItem("msg", response.getDataItem("msg"));
      self->writeInterResponse(self->m_context.request, self->m_context.response);
   }else{
      //错误处理
      self->m_context.response.setStatus(false);
      self->m_context.response.setIsFinal(false);
      self->m_context.response.setError(response.getError());
      self->writeInterResponse(self->m_context.request, self->m_context.response);
      self->m_isInAction = false;
      self->m_eventLoop.exit(0);
      self->m_context.response.setStatus(false);
      self->m_context.response.setError({-1, "升级失败"});
   }
}

UpgradeCloudControllerWrapper::UpgradeCloudControllerWrapper(ServiceProvider &provider)
   : AbstractService(provider)
{}

ServiceInvokeResponse UpgradeCloudControllerWrapper::upgrade(const ServiceInvokeRequest &request)
{
   const QMap<QString, QVariant> &args = request.getArgs();
   checkRequireFields(args, {"fromVersion", "toVersion"});
   ServiceInvokeResponse response("Upgrade/UpgradeCloudController/upgrade", true);
   if(m_isInAction){
      response.setIsFinal(true);
      response.setStatus(false);
      response.setError({10000, "操作正在进行中"});
      return response;
   }
   m_isInAction = true;
   response.setSerial(request.getSerial());
   response.setIsFinal(false);
   m_context.request = request;
   m_context.response = response;
   response.setDataItem("msg", "开始获取云控制器服务器地址");
   writeInterResponse(request, response);
   Info info;
   QMap<QString, QString> meta = info.getServiceServerAddressMetaByKey("CloudController");
   response.setDataItem("msg", QString("目标更新服务器地址 %1").arg(meta.value("ip")));
   writeInterResponse(request, response);
   response.setDataItem("msg", "正在连接更新服务器");
   writeInterResponse(request, response);
   QSharedPointer<ServiceInvoker> invoker = getServiceInvoker(meta.value("ip"), UMS_LISTEN_PORT);
   connect(invoker.data(), &ServiceInvoker::connectedToServerSignal, this, [&](){
      response.setDataItem("msg", QString("连接服务器成功 [%1:%2]").arg(meta.value("ip")).arg(UMS_LISTEN_PORT));
      writeInterResponse(request, response);
      response.setDataItem("msg", "向upgrademgr_slave服务器发送升级请求");
      writeInterResponse(request, response);
      ServiceInvokeRequest serviceRequest("Upgrade/UpgradeCloudController", "init", args);
      invoker->request(serviceRequest, init_upgrade_handler, static_cast<void*>(this));
   });
   connect(invoker.data(), &ServiceInvoker::connectErrorSignal, this, [&](const QString&){
      response.setDataItem("msg", QString("连接服务器失败 [%1:%2]").arg(meta.value("ip")).arg(UMS_LISTEN_PORT));
      writeInterResponse(request, response);
      m_eventLoop.exit(0);
   });
   invoker->connectToServer();
   m_eventLoop.exec();
   return m_context.response;
}

//void UpgradeCloudControllerWrapper::notifySocketDisconnect(QWebSocket *socket)
//{

//}

}//upgrader
}//ummservice