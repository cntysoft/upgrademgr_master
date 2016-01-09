#ifndef UPGRADEMGR_MASTER_LIB_META_WEBSERVER_WEBSERVER_ENV_LIST_H
#define UPGRADEMGR_MASTER_LIB_META_WEBSERVER_WEBSERVER_ENV_LIST_H

#include "ummlib/global/global.h"

#include "kernel/stddir.h"

namespace ummlib{
namespace meta{
namespace webserver{

using ummlib::kernel::StdDir;

class UM_MASTER_LIB_EXPORT WebServerEnvList
{
public:
   static QString& getEnvRepoMetaFile();
   static QString& getEnvSoftwareRepoDir();   
};

}//webserver
}//meta
}//ummlib

#endif // UPGRADEMGR_MASTER_LIB_META_WEBSERVER_WEBSERVER_ENV_LIST_H

