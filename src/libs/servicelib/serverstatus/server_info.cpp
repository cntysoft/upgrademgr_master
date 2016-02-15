#include "serverstatus/server_info.h"
#include "ummlib/global/common_funcs.h"

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
   response.setDataItem("version", ummlib::global::get_upgrademgr_master_version());
   return response;
}

}//serverstatus
}//service
}//master
}//upgrademgr