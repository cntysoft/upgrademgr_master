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

InfoWrapper::InfoWrapper(ServiceProvider &provider)
   : AbstractService(provider),
     m_helper(new Info)
{
}

ServiceInvokeResponse InfoWrapper::getVersionInfo(const ServiceInvokeRequest &request)
{
   ServiceInvokeResponse response("ServerStatus/ServerInfo/getVersionInfo", true);
   response.setSerial(request.getSerial());
   response.setDataItem("version", ummlib::global::get_upgrademgr_master_version());
   response.setIsFinal(true);
   return response;
}

ServiceInvokeResponse InfoWrapper::setServiceServerAddressMeta(const ServiceInvokeRequest &request)
{
   const QMap<QString, QVariant> args = request.getArgs();
   checkRequireFields(args, {"servers"});
   ServiceInvokeResponse response("ServerStatus/ServerInfo/setServiceServerAddressMeta", true);
   response.setSerial(request.getSerial());
   m_helper->setServiceServerAddressMeta(args.value("servers"));
   response.setIsFinal(true);
   return response;
}

ServiceInvokeResponse InfoWrapper::getServiceServerAddressMeta(const ServiceInvokeRequest &request)
{
   ServiceInvokeResponse response("ServerStatus/ServerInfo/getServiceServerAddressMeta", true);
   response.setSerial(request.getSerial());
   QString metaFilename = m_helper->getAddressMetaFilename();
   if(!Filesystem::fileExist(metaFilename)){
      response.setExtraData(QByteArray("[]"));
   }else{
      response.setExtraData(QByteArray().append(Filesystem::fileGetContents(metaFilename)));
   }
   response.setIsFinal(true);
   return response;
}

}//serverstatus
}//ummservice