#include <QFileInfo>
#include <QDataStream>
#include <QBuffer>

#include "repo_info.h"

#include "ummlib/kernel/stddir.h"
#include "corelib/io/filesystem.h"

namespace upgrademgr{
namespace master{
namespace service{
namespace repo{

using ummlib::kernel::StdDir;
using sn::corelib::Filesystem;

Info::Info(ServiceProvider &provider)
   : AbstractService(provider),
     m_dataDir(StdDir::getBaseDataDir()+"/softwarerepo")
{
   if(!Filesystem::dirExist(m_dataDir)){
      Filesystem::createPath(m_dataDir);
   }
}

ServiceInvokeResponse Info::lsSoftwareRepoDir(const ServiceInvokeRequest &request)
{
   int baseLength = m_dataDir.size()+1;
   QStringList list;
   Filesystem::traverseFs(m_dataDir, 1, [&list, baseLength](QFileInfo &fileInfo, int){
      list << fileInfo.absoluteFilePath().remove(0, baseLength);
   });
   QBuffer buffer;
   buffer.open(QIODevice::WriteOnly);
   QDataStream out(&buffer);
   out << list;
   ServiceInvokeResponse response("Repo/Info/lsSoftwareRepoDir12", true);
   response.setExtraData(buffer.data());
   response.setSerial(request.getSerial());
   return response;
}

}//repo
}//service
}//master
}//upgrademgr
