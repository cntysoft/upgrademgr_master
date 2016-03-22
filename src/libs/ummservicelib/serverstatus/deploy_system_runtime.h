#ifndef UMM_SERVICE_SERVER_STATUS_DEPLOY_SYSTEM_RUNTIME_H
#define UMM_SERVICE_SERVER_STATUS_DEPLOY_SYSTEM_RUNTIME_H

#include <QSharedPointer>
#include <QString>
#include <QMap>

#include "ummservicelib/global_defs.h"
#include "corelib/network/rpc/abstract_service.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "corelib/network/rpc/service_provider.h"
#include "corelib/network/rpc/service_invoker.h"

namespace ummservice{
namespace serverstatus{

using sn::corelib::network::ServiceInvoker;

UMM_USING_SERVICE_NAMESPACES

class UMM_SERVICE_EXPORT DeploySystemMetaServerRuntimeWrapper : public AbstractService
{
   Q_OBJECT
public:
   DeploySystemMetaServerRuntimeWrapper(ServiceProvider& provider);
   Q_INVOKABLE ServiceInvokeResponse startServer(const ServiceInvokeRequest &request);
   Q_INVOKABLE ServiceInvokeResponse stopServer(const ServiceInvokeRequest &request);
   Q_INVOKABLE ServiceInvokeResponse restartServer(const ServiceInvokeRequest &request);
protected:
   int getMetaServerPid();
protected:
   QString m_metaServerPidFilename;
};

class UMM_SERVICE_EXPORT DeploySystemLuoXiRuntimeWrapper : public AbstractService
{
   Q_OBJECT
   friend void deploy_system_luoxi_status_manage_handler(const ServiceInvokeResponse &response, void* args);
   struct StatusManageContext
   {
      ServiceInvokeRequest request;
      ServiceInvokeRequest operateRequest;
      ServiceInvokeResponse response;
      QString targetServerAddress;
      QSharedPointer<ServiceInvoker> serviceInvoker;
   };
public:
   DeploySystemLuoXiRuntimeWrapper(ServiceProvider& provider);
   Q_INVOKABLE ServiceInvokeResponse startServer(const ServiceInvokeRequest &request);
   Q_INVOKABLE ServiceInvokeResponse stopServer(const ServiceInvokeRequest &request);
   Q_INVOKABLE ServiceInvokeResponse restartServer(const ServiceInvokeRequest &request);
protected:
   ServiceInvokeResponse doOperate(const ServiceInvokeRequest &request, const ServiceInvokeRequest& operateRequest);
   void clearState();
protected slots:
   void connectToServerHandler();
   void connectToServerErrorHandler();
protected:
   bool m_isInAction = false;
   QSharedPointer<StatusManageContext> m_context;
};

}//serverstatus
}//ummservice


#endif // UMM_SERVICE_SERVER_STATUS_DEPLOY_SYSTEM_RUNTIME_H
