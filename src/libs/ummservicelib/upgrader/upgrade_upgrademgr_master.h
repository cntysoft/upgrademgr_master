#ifndef UMM_SERVICE_UPGRADER_UPGRADER_H
#define UMM_SERVICE_UPGRADER_UPGRADER_H

#include <QString>

#include "ummservicelib/global_defs.h"
#include "corelib/network/rpc/abstract_service.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "corelib/network/rpc/service_provider.h"

namespace ummservice{
namespace upgrader{

UMM_USING_SERVICE_NAMESPACES

class UMM_SERVICE_EXPORT UpgradeUpgradeMgrMaster : public AbstractService
{
   Q_OBJECT
   const static QString RPM_FILENAME_TPL;
public:
   UpgradeUpgradeMgrMaster(ServiceProvider& provider);
   Q_INVOKABLE ServiceInvokeResponse upgrade(const ServiceInvokeRequest& request);
protected:
   int versionCompare(const QString &version1, const QString &version2);
   bool installRpmPackage(const QString &filename, QString &errorString);
protected:
   const QString m_softwareRepoDir;
};

}//upgrader
}//ummservice

#endif // UMM_SERVICE_UPGRADER_UPGRADER_H