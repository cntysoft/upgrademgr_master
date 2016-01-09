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

QString get_application_filepath()
{
#ifdef UPGRADEMGR_MASTER_DEBUG_BUILD
   return Application::applicationFilePath();
#else
   return QString("/usr/local/bin/upgrademgr_master");
#endif
}

}//master
}//upgrademgr