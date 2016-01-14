#include "utils/common_funcs.h"
#include "service/serverstatus/server_info.h"

namespace upgrademgr{
namespace master{
namespace service{
namespace serverstatus{

Info::Info(ServiceProvider &provider)
   : AbstractService(provider)
{
}

ServiceInvokeResponse Info::getVersionInfo(const ServiceInvokeRequest &request)
{
   ServiceInvokeResponse response("ServerStatus/Info/getVersionInfo", true);
   response.setSerial(request.getSerial());
   response.setDataItem("version", upgrademgr::master::get_upgrademgr_master_version());
   return response;
}

}//serverstatus
}//service
}//master
}//upgrademgr