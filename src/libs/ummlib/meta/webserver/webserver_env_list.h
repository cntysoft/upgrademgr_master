#ifndef UMM_LIB_META_WEBSERVER_WEBSERVER_ENV_LIST_H
#define UMM_LIB_META_WEBSERVER_WEBSERVER_ENV_LIST_H

#include "ummlib/global/global.h"

#include "kernel/stddir.h"

namespace ummlib{
namespace meta{
namespace webserver{

using ummlib::kernel::StdDir;

class UMM_LIB_EXPORT WebServerEnvList
{
public:
   static QString& getEnvRepoMetaFile();
   static QString& getEnvSoftwareRepoDir();   
};

}//webserver
}//meta
}//ummlib

#endif // UMM_LIB_META_WEBSERVER_WEBSERVER_ENV_LIST_H

