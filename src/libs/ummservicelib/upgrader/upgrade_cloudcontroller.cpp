#include "upgrade_cloudcontroller.h"

#include <QDir>
#include <QList>
#include <QVariant>
#include <QStringList>
#include <QMap>
#include <QString>
#include <QDebug>
#include <QSharedPointer>

#include "corelib/network/rpc/service_invoker.h"
#include "corelib/io/filesystem.h"
#include "ummservicelib/serverstatus/server_info.h"
#include "corelib/network/rpc/invoke_meta.h"

namespace ummservice{
namespace upgrader{

using sn::corelib::network::ServiceInvoker;
using sn::corelib::network::ServiceInvokeRequest;
using ummservice::serverstatus::Info;

UpgradeCloudControllerWrapper::UpgradeCloudControllerWrapper(ServiceProvider &provider)
   : AbstractService(provider)
{}

ServiceInvokeResponse UpgradeCloudControllerWrapper::upgrade(const ServiceInvokeRequest &request)
{
   ServiceInvokeResponse response("Upgrade/UpgradeCloudController/upgrade", true);
   if(m_isInAction){
      response.setIsFinal(true);
      response.setStatus(false);
      response.setError({10000, "操作正在进行中"});
      return response;
   }
   m_isInAction = true;
   m_context.request = request;
   m_context.response = response;
   response.setSerial(request.getSerial());
   response.setIsFinal(false);
   response.setDataItem("msg", "开始获取云控制器服务器地址");
   writeInterResponse(request, response);
   Info info;
   QMap<QString, QString> meta = info.getServiceServerAddressMetaByKey("CloudController");
   response.setDataItem("msg", QString("目标更新服务器地址 %1").arg(meta.value("ip")));
   writeInterResponse(request, response);
   response.setDataItem("msg", "正在连接更新服务器");
   writeInterResponse(request, response);
   QSharedPointer<ServiceInvoker> invoker = getServiceInvoker(meta.value("ip"), UPGRADEMGR_SLAVE_LISTEN_PORT);
   connect(invoker.data(), &ServiceInvoker::connectedToServerSignal, this, [&](){
      response.setDataItem("msg", QString("连接服务器成功 [%1:%2]").arg(meta.value("ip")).arg(UPGRADEMGR_SLAVE_LISTEN_PORT));
      writeInterResponse(request, response);
   }, Qt::DirectConnection);
   connect(invoker.data(), &ServiceInvoker::connectErrorSignal, this, [&](const QString&){
      response.setDataItem("msg", QString("连接服务器失败 [%1:%2]").arg(meta.value("ip")).arg(UPGRADEMGR_SLAVE_LISTEN_PORT));
      writeInterResponse(request, response);
   });
   invoker->connectToServer();
//   writeInterResponse(request, response);
//   response.setDataItem("msg", "开始下载软件包");
//   response.setIsFinal(true);
   m_eventLoop.exec();
   return response;
}

//void UpgradeCloudControllerWrapper::notifySocketDisconnect(QWebSocket *socket)
//{
   
//}

}//upgrader
}//ummservice