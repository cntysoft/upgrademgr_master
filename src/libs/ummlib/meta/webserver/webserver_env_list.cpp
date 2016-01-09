#include <QString>

#include "webserver_env_list.h"

namespace ummlib{
namespace meta{
namespace webserver{

QString& WebServerEnvList::getEnvRepoMetaFile()
{
   static QString filename(StdDir::getMetaDir()+"/webserver/envsoftwarelist.json");
   return filename;
}

}//webserver
}//meta
}//ummlib
