#ifndef UMM_SERVICE_UPGRADE_CLOUDCONTROLLER_H
#define UMM_SERVICE_UPGRADE_CLOUDCONTROLLER_H

#include <QString>

#include "ummservicelib/global_defs.h"
#include "corelib/network/rpc/abstract_service.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "corelib/network/rpc/service_provider.h"

namespace ummservice{
namespace upgrader{

UMM_USING_SERVICE_NAMESPACES

class UMM_SERVICE_EXPORT UpgradeCloudControllerWrapper : public AbstractService
{
   Q_OBJECT
   friend void init_upgrade_handler(const ServiceInvokeResponse &response, void* args);
public:
   struct UpgradeContext
   {
      ServiceInvokeRequest request;
      ServiceInvokeResponse response;
      QString fromVersion;
      QString toVersion;
   };
public:
   UpgradeCloudControllerWrapper(ServiceProvider& provider);
   Q_INVOKABLE ServiceInvokeResponse upgrade(const ServiceInvokeRequest &request);
   
//protected:
//   virtual void notifySocketDisconnect(QWebSocket *socket);
protected:
   bool m_isInAction = false;
   UpgradeContext m_context;
};

}//upgrader
}//ummservice

#endif // UMM_SERVICE_UPGRADE_CLOUDCONTROLLER_H
