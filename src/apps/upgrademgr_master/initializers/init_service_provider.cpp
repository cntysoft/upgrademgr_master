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
                            return new ummservice::repo::Info(provider);
                         });
   provider.addServiceToPool("ServerStatus/Info", [](ServiceProvider& provider)-> AbstractService*{
                            return new ummservice::serverstatus::Info(provider);
                         });
   provider.addServiceToPool("Common/Uploader", [](ServiceProvider& provider)-> AbstractService*{
                            return new ummservice::common::Uploader(provider);
                         });
//   provider.addServiceToPool("Upgrader/UpgradeUpgrademgrMaster", [](ServiceProvider& provider)-> AbstractService*{
//                            return new upgrademgr::master::service::upgrader::UpgradeUpgradeMgrMaster(provider);
//                         });
}

void cleanup_service_provider()
{
   ServiceProvider &provider = ServiceProvider::instance();
   delete &provider;
}

}//master
}//upgrademgr