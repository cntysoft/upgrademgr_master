#include "stddir.h"

#include "ummlib/global/const.h"
#include "corelib/kernel/settings.h"

namespace ummlib{
namespace kernel{

using sn::corelib::Settings;

QString StdDir::getBaseDataDir()
{
   static QString dir;
   if(dir.isEmpty()){
      Settings& settings = Application::instance()->getSettings();
      dir = settings.getValue("baseDataDir", UMM_CFG_GROUP_GLOBAL, "/cntysoft/upgrademgr").toString();
   }
   return dir;
}

QString StdDir::getSoftwareRepoDir()
{
   return StdDir::getBaseDataDir()+"/"+"softwarerepo";
}

}//network
}//ummlib