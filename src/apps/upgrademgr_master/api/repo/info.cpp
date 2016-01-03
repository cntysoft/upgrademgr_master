#include <QFileInfo>
#include <QDataStream>
#include <QBuffer>

#include "info.h"

#include "ummlib/kernel/stddir.h"
#include "corelib/io/filesystem.h"

namespace upgrademgr{
namespace master{
namespace api{
namespace repo{

using ummlib::kernel::StdDir;
using sn::corelib::Filesystem;

Info::Info(ApiProvider &provider)
   : AbstractApi(provider),
     m_dataDir(StdDir::getBaseDataDir()+"/softwarerepo")
{
   if(!Filesystem::dirExist(m_dataDir)){
      Filesystem::createPath(m_dataDir);
   }
}

ApiInvokeResponse Info::lsSoftwareRepoDir(const ApiInvokeRequest &request)
{
   QStringList list;
   Filesystem::traverseFs(m_dataDir, 1, [&list](QFileInfo &fileInfo, int){
      list << fileInfo.baseName();
   });
   QBuffer buffer;
   buffer.open(QIODevice::WriteOnly);
   QDataStream out(&buffer);
   out << list;
   ApiInvokeResponse response("Repo/Info/lsSoftwareRepoDir", false);
   response.setError({1, "asdasdasdas"});
   response.setExtraData(buffer.data());
   response.setSerial(request.getSerial());
   return response;
}

}//repo
}//api
}//master
}//upgrademgr
