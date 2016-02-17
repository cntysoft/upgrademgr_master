#ifndef UMM_SERVICE_REPO_INFO_H
#define UMM_SERVICE_REPO_INFO_H

#include "ummservicelib/global_defs.h"
#include "corelib/network/rpc/abstract_service.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "corelib/network/rpc/service_provider.h"

namespace ummservice{
namespace repo{

UMM_USING_SERVICE_NAMESPACES

class UMM_SERVICE_EXPORT Info : public AbstractService
{
   Q_OBJECT
public:
   Info(ServiceProvider& provider);
   Q_INVOKABLE ServiceInvokeResponse lsSoftwareRepoDir(const ServiceInvokeRequest &request);
   Q_INVOKABLE ServiceInvokeResponse removeSoftware(const ServiceInvokeRequest &request);
protected:
   QString m_dataDir;
};

}//repo
}//ummservice

#endif // UMM_SERVICE_REPO_INFO_H
