#ifndef UPGRADEMGR_MASTER_API_UPGRADER_UPGRADER_H
#define UPGRADEMGR_MASTER_API_UPGRADER_UPGRADER_H

#include "api/macros.h"
#include "corelib/network/rpc/abstract_api.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "corelib/network/rpc/api_provider.h"

namespace upgrademgr{
namespace master{
namespace api{
namespace upgrader{

UMM_USING_API_NAMESPACES

class UpgradeUpgradeMgrMaster : public AbstractApi
{
   Q_OBJECT
public:
   UpgradeUpgradeMgrMaster(ApiProvider& provider);
   Q_INVOKABLE ApiInvokeResponse upgrade(const ApiInvokeRequest& request);
};

}//upgrader
}//api
}//master
}//upgrademgr

#endif // UPGRADEMGR_MASTER_API_UPGRADER_UPGRADER_H
