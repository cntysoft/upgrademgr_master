#include "common_funcs.h"

namespace upgrademgr{
namespace master{

UpgrademgrMasterApplication* get_app_ref()
{
   return static_cast<UpgrademgrMasterApplication*>(UpgrademgrMasterApplication::instance());
}

QLatin1String get_upgrademgr_master_version()
{
   return QLatin1String(UPGRADEMGR_MASTER_VERSION);
}

}//master
}//upgrademgr