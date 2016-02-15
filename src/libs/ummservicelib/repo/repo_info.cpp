#include <QFileInfo>
#include <QDataStream>
#include <QBuffer>
#include <QVariant>

#include "repo_info.h"

#include "ummlib/kernel/stddir.h"
#include "corelib/io/filesystem.h"

namespace ummservice{
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
   ServiceInvokeResponse response("Repo/Info/lsSoftwareRepoDir", true);
   response.setExtraData(encodeJsonObject(QVariant(list)));
   response.setSerial(request.getSerial());
   return response;
}

}//repo
}//ummservice

