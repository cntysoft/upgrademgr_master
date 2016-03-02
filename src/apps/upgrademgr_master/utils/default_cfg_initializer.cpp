#include <QDir>

#include "ummlib/global/const.h"
#include "kernel/settings.h"

namespace upgrademgr{
namespace master{

using sn::corelib::Settings;

static void init_global_cfg(Settings &settings);

void init_defualt_cfg(Settings& settings)
{
   init_global_cfg(settings);
}

static void init_global_cfg(Settings& settings)
{
   QString runtimeDir = QDir::tempPath()+QDir::separator()+"upgrademgrmaster";
   settings.setValue("runtimeDir", runtimeDir, UMM_CFG_GROUP_GLOBAL);
   settings.setValue("listenPort", UMM_LISTEN_PORT, UMM_CFG_GROUP_GLOBAL);
   settings.setValue("baseDataDir", "/cntysoft/upgrademgr", UMM_CFG_GROUP_GLOBAL);
   settings.setValue("dbHost", "localhost", UMM_CFG_GROUP_GLOBAL);
   settings.setValue("dbUser", "root", UMM_CFG_GROUP_GLOBAL);
   settings.setValue("dbPassword", "cntysoft", UMM_CFG_GROUP_GLOBAL);
   settings.setValue("dbName", "upgrademgrmaster", UMM_CFG_GROUP_GLOBAL);
   settings.setValue("upgradeTestDbName", "cloudcontroller_upgrade", UMM_CFG_GROUP_GLOBAL);
   settings.setValue("upgradeTestDir", "/srv/www/cloudcontrollerupgrade", UMM_CFG_GROUP_GLOBAL);
}


}//master
}//upgrademgr