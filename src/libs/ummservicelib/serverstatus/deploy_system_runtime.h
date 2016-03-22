#ifndef UMM_SERVICE_SERVER_STATUS_DEPLOY_SYSTEM_RUNTIME_H
#define UMM_SERVICE_SERVER_STATUS_DEPLOY_SYSTEM_RUNTIME_H

#include <QSharedPointer>
#include <QString>
#include <QMap>

#include "ummservicelib/global_defs.h"
#include "corelib/network/rpc/abstract_service.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "corelib/network/rpc/service_provider.h"

namespace ummservice{
namespace serverstatus{

UMM_USING_SERVICE_NAMESPACES

class UMM_SERVICE_EXPORT DeploySystemRuntimeWrapper : public AbstractService
{
   Q_OBJECT
public:
   DeploySystemRuntimeWrapper(ServiceProvider& provider);
   Q_INVOKABLE ServiceInvokeResponse startMetaServer(const ServiceInvokeRequest &request);
   Q_INVOKABLE ServiceInvokeResponse stopMetaServer(const ServiceInvokeRequest &request);
   Q_INVOKABLE ServiceInvokeResponse restartMetaServer(const ServiceInvokeRequest &request);
protected:
   int getMetaServerPid();
protected:
   QString m_metaServerPidFilename;
};

}//serverstatus
}//ummservice


#endif // UMM_SERVICE_SERVER_STATUS_DEPLOY_SYSTEM_RUNTIME_H
