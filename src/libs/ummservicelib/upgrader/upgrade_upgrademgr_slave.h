#ifndef UMM_SERVICE_UPGRADER_UPGRADE_UPGRADEMGR_SLAVE_H
#define UMM_SERVICE_UPGRADER_UPGRADE_UPGRADEMGR_SLAVE_H

#include <QString>

#include "ummservicelib/global_defs.h"
#include "corelib/network/rpc/abstract_service.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "corelib/network/rpc/service_provider.h"
#include "corelib/network/rpc/service_invoker.h"

namespace ummservice{
namespace upgrader{

UMM_USING_SERVICE_NAMESPACES

using sn::corelib::network::ServiceInvoker;

class UMM_SERVICE_EXPORT UpgradeUpgradeMgrSlaveWrapper : public AbstractService
{
   Q_OBJECT
   friend void upgrade_upgrademgr_slave_handler(const ServiceInvokeResponse &response, void* args);
   struct UpgradeContext
   {
      ServiceInvokeRequest request;
      ServiceInvokeResponse response;
      QString targetVersion;
      QString targetServerAddress;
      QSharedPointer<ServiceInvoker> serviceInvoker;
   };
public:
   UpgradeUpgradeMgrSlaveWrapper(ServiceProvider &provider);
   Q_INVOKABLE ServiceInvokeResponse upgrade(const ServiceInvokeRequest &request);
protected:
   void clearState();
protected slots:
   void connectToServerHandler();
   void connectToServerErrorHandler();
protected:
   bool m_isInAction = false;
   QSharedPointer<UpgradeContext> m_context;
};

}//upgrader
}//ummservice

#endif // UMM_SERVICE_UPGRADER_UPGRADE_UPGRADEMGR_SLAVE_H
