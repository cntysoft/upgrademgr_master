#include "initializers/initializer_cleanup_funcs.h"
#include "ummservicelib/service_repo.h"
#include <QDebug>
#include "corelib/network/rpc/service_provider.h"
#include "corelib/network/rpc/abstract_service.h"

namespace upgrademgr{
namespace master{

using sn::corelib::network::ServiceProvider;
using sn::corelib::network::AbstractService;

void init_service_provider()
{
   ServiceProvider& provider = ServiceProvider::instance();
   provider.addServiceToPool("Repo/Info", [](ServiceProvider& provider)-> AbstractService*{
                            return new ummservice::repo::InfoWrapper(provider);
                         });
   provider.addServiceToPool("ServerStatus/Info", [](ServiceProvider& provider)-> AbstractService*{
                            return new ummservice::serverstatus::InfoWrapper(provider);
                         });
   provider.addServiceToPool("Common/Uploader", [](ServiceProvider& provider)-> AbstractService*{
                            return new ummservice::common::UploaderWrapper(provider);
                         });
   provider.addServiceToPool("Common/DownloadServer", [](ServiceProvider& provider)-> AbstractService*{
                            return new ummservice::common::DownloadServerWrapper(provider);
                         });
   provider.addServiceToPool("Common/Filesystem", [](ServiceProvider& provider)-> AbstractService*{
                            return new ummservice::common::FilesystemWrapper(provider);
                         });
   provider.addServiceToPool("Upgrader/UpgradeCloudController", [](ServiceProvider& provider)-> AbstractService*{
                            return new ummservice::upgrader::UpgradeCloudControllerWrapper(provider);
                         });
   provider.addServiceToPool("Upgrader/UpgradeUpgradeMgrMaster", [](ServiceProvider& provider)-> AbstractService*{
                            return new ummservice::upgrader::UpgradeUpgradeMgrMasterWrapper(provider);
                         });
   provider.addServiceToPool("Upgrader/UpgradeUpgradeMgrSlave", [](ServiceProvider& provider)-> AbstractService*{
                            return new ummservice::upgrader::UpgradeUpgradeMgrSlaveWrapper(provider);
                         });
   provider.addServiceToPool("Upgrader/UpgradeMetaServer", [](ServiceProvider& provider)-> AbstractService*{
                            return new ummservice::upgrader::UpgradeMetaServerWrapper(provider);
                         });
   provider.addServiceToPool("Upgrader/UpgradeLuoXi", [](ServiceProvider& provider)-> AbstractService*{
                            return new ummservice::upgrader::UpgradeLuoXiWrapper(provider);
                         });
}

void cleanup_service_provider()
{
   ServiceProvider &provider = ServiceProvider::instance();
   delete &provider;
}

}//master
}//upgrademgr