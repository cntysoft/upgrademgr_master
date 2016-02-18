#include <QMap>
#include <QString>
#include <QVariant>

#include "serverstatus/server_info.h"
#include "ummlib/global/common_funcs.h"
#include "ummlib/kernel/stddir.h"
#include "corelib/io/filesystem.h"

namespace ummservice{
namespace serverstatus{

using ummlib::kernel::StdDir;
using sn::corelib::Filesystem;

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
   const QMap<QString, QVariant> args = request.getArgs();
   checkRequireFields(args, {"servers"});
   ServiceInvokeResponse response("ServerStatus/Info/setServiceServerAddressMeta", true);
   response.setSerial(request.getSerial());
   QString jsonStr(encodeJsonObject(args.value("servers")));
   Filesystem::filePutContents(getAddressMetaFilename(), jsonStr);
   return response;
}

ServiceInvokeResponse Info::getServiceServerAddressMeta(const ServiceInvokeRequest &request)
{
   ServiceInvokeResponse response("ServerStatus/Info/getServiceServerAddressMeta", true);
   response.setSerial(request.getSerial());
   QString metaFilename = getAddressMetaFilename();
   if(!Filesystem::fileExist(metaFilename)){
      response.setExtraData(QByteArray("[]"));
   }else{
      response.setExtraData(QByteArray().append(Filesystem::fileGetContents(metaFilename)));
   }
   return response;
}

QString Info::getAddressMetaFilename()const
{
   return StdDir::getMetaDir() + '/' + "ServerAddress.json";
}

}//serverstatus
}//ummservice