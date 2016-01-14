#ifndef UPGRADEMGR_MASTER_SERVICE_SERVER_STATUS_INFO_H
#define UPGRADEMGR_MASTER_SERVICE_SERVER_STATUS_INFO_H

#include "service/macros.h"
#include "corelib/network/rpc/abstract_service.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "corelib/network/rpc/service_provider.h"

namespace upgrademgr{
namespace master{
namespace service{
namespace serverstatus{

UMM_USING_SERVICE_NAMESPACES

class Info : public AbstractService
{
   Q_OBJECT
public:
   Info(ServiceProvider& provider);
   Q_INVOKABLE ServiceInvokeResponse getVersionInfo(const ServiceInvokeRequest &request);
};

}//serverstatus
}//service
}//master
}//upgrademgr

#endif // UPGRADEMGR_MASTER_SERVICE_SERVER_STATUS_INFO_H
