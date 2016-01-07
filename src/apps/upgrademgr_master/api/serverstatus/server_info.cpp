#include "utils/common_funcs.h"
#include "api/serverstatus/server_info.h"

namespace upgrademgr{
namespace master{
namespace api{
namespace serverstatus{

Info::Info(ApiProvider &provider)
   : AbstractApi(provider)
{
}

ApiInvokeResponse Info::getVersionInfo(const ApiInvokeRequest &request)
{
   ApiInvokeResponse response("ServerStatus/Info/getVersionInfo", true);
   response.setSerial(request.getSerial());
   response.addDataItem("version", upgrademgr::master::get_upgrademgr_master_version());
   return response;
}


}//serverstatus
}//api
}//master
}//upgrademgr