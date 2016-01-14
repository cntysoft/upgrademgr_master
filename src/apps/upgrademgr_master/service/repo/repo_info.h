#ifndef UPGRADEMGR_MASTER_SERVICE_REPO_INFO_H
#define UPGRADEMGR_MASTER_SERVICE_REPO_INFO_H

#include "service/macros.h"
#include "corelib/network/rpc/abstract_service.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "corelib/network/rpc/service_provider.h"

namespace upgrademgr{
namespace master{
namespace service{
namespace repo{

UMM_USING_SERVICE_NAMESPACES

class Info : public AbstractService
{
   Q_OBJECT
public:
   Info(ServiceProvider& provider);
   Q_INVOKABLE ServiceInvokeResponse lsSoftwareRepoDir(const ServiceInvokeRequest& request);
protected:
   QString m_dataDir;
};

}//repo
}//service
}//master
}//upgrademgr

#endif // UPGRADEMGR_MASTER_SERVICE_REPO_INFO_H
