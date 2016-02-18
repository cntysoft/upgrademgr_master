#include "serverstatus/server_info.h"
#include "ummlib/global/common_funcs.h"
#include "ummlib/kernel/stddir.h"

namespace ummservice{
namespace serverstatus{

using ummlib::kernel::StdDir;

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


ServiceInvokeResponse Info::setServiceServerAddressMeta(const ServiceInvokeRequest &request)
{
   ServiceInvokeResponse response("ServerStatus/Info/setServiceServerAddressMeta", true);
   response.setSerial(request.getSerial());
   QString metaFilename = StdDir::getMetaDir() + '/' + "ServerAddress.json";
   
   return response;
}

}//serverstatus
}//ummservice