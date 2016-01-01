#include "info.h"


namespace upgrademgr{
namespace master{
namespace api{
namespace repo{

Info::Info(ApiProvider &provider)
   :AbstractApi(provider)
{
}

ApiInvokeResponse Info::lsSoftwareRepoDir(const ApiInvokeRequest &request)
{
   return ApiInvokeResponse("Repo/Info/lsSoftwareRepoDir", true);
}

}//repo
}//api
}//master
}//upgrademgr
