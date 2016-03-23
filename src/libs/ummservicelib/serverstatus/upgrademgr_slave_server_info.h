#ifndef UMM_SERVICE_SERVER_STATUS_UPGRADEMGR_SLAVE_SERVER_INFO_H
#define UMM_SERVICE_SERVER_STATUS_UPGRADEMGR_SLAVE_SERVER_INFO_H

#include "ummservicelib/global_defs.h"
#include "corelib/network/rpc/abstract_service.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "corelib/network/rpc/service_provider.h"
#include "corelib/network/rpc/service_invoker.h"

namespace ummservice{
namespace serverstatus{

using sn::corelib::network::ServiceInvokeRequest;
using sn::corelib::network::ServiceInvokeResponse;

using sn::corelib::network::ServiceInvoker;

UMM_USING_SERVICE_NAMESPACES

class UMM_SERVICE_EXPORT UpgradeMgrSlaveServerInfoWrapper : public AbstractService
{
   Q_OBJECT
   friend void upgrademgr_slave_server_info_handler(const ServiceInvokeResponse &response, void* args);
   struct ServerInfoContext
   {
      ServiceInvokeRequest request;
      ServiceInvokeResponse *response;
      ServiceInvokeRequest operateRequest;
      QString serverAddress;
      QSharedPointer<ServiceInvoker> serviceInvoker;
   };
public:
   UpgradeMgrSlaveServerInfoWrapper(ServiceProvider &provider);
   Q_INVOKABLE ServiceInvokeResponse getServerVersion(const ServiceInvokeRequest &request);
protected:
   ServiceInvokeResponse doOperate(const ServiceInvokeRequest &request, const ServiceInvokeRequest& operateRequest);
   void clearState();
protected slots:
   void connectToServerHandler();
   void connectToServerErrorHandler();
protected:
   bool m_isInAction = false;
   QSharedPointer<ServerInfoContext> m_context;
};

}//serverstatus
}//ummservice

#endif // UMM_SERVICE_SERVER_STATUS_UPGRADEMGR_SLAVE_SERVER_INFO_H
