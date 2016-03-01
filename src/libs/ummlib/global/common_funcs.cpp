#include "common_funcs.h"

namespace ummlib{
namespace global{

QString get_upgrademgr_master_version()
{
   return QLatin1String(UMM_VERSION);
}

QString get_application_filepath()
{
#ifdef UPGRADEMGR_MASTER_DEBUG_BUILD
   return Application::applicationFilePath();
#else
   return QString("/usr/local/bin/upgrademgr_master");
#endif
}

}//global
}//ummlib