#include "info.h"


namespace upgrademgr{
namespace master{
namespace api{
namespace repo{

Info::Info(ApiProvider &provider)
   :AbstractApi(provider)
{
}

ApiInvokeResponse Info::getVersion(const ApiInvokeRequest &request, ApiProvider &provider)
{
   return ApiInvokeResponse();
}

}//repo
}//api
}//master
}//upgrademgr
