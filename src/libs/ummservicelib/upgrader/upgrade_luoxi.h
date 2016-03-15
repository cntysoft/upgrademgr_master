#ifndef UMM_SERVICE_UPGRADE_LUOXI_H
#define UMM_SERVICE_UPGRADE_LUOXI_H

#include <QString>
#include <QSharedPointer>

#include "ummservicelib/global_defs.h"
#include "corelib/network/rpc/abstract_service.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "corelib/network/rpc/service_provider.h"
#include "corelib/network/rpc/service_invoker.h"

namespace ummservice{
namespace upgrader{

UMM_USING_SERVICE_NAMESPACES

using sn::corelib::network::ServiceInvoker;

class UMM_SERVICE_EXPORT UpgradeLuoXiWrapper : public AbstractService
{
   Q_OBJECT
   friend void upgrade_luoxi_handler(const ServiceInvokeResponse &response, void* args);
   const static int STEP_PREPARE = -1;
   const static int STEP_INIT_CONTEXT = 0;
   const static int STEP_DOWNLOAD_PKG = 1;
   const static int STEP_DOWNLOAD_COMPLETE = 2;
   const static int STEP_INSTALL_RPM = 3;
   const static int STEP_RESTART_SERVER = 4;
   const static int STEP_FINISH = 5;
   const static int STEP_ERROR = 6;
   struct UpgradeContext
   {
      ServiceInvokeRequest request;
      ServiceInvokeResponse response;
      QString targetVersion;
      QString targetServerAddress;
      QSharedPointer<ServiceInvoker> serviceInvoker;
   };
public:
   UpgradeLuoXiWrapper(ServiceProvider &provider);
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

#endif // UMM_SERVICE_UPGRADE_LUOXI_H
