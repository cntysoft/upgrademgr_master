#include <QFileInfo>
#include <QDataStream>
#include <QBuffer>
#include <QVariant>
#include <QMap>
#include <QDebug>

#include "repo_info.h"

#include "ummlib/kernel/stddir.h"
#include "corelib/io/filesystem.h"

namespace ummservice{
namespace repo{

using ummlib::kernel::StdDir;
using sn::corelib::Filesystem;

InfoWrapper::InfoWrapper(ServiceProvider &provider)
   : AbstractService(provider),
     m_dataDir(StdDir::getBaseDataDir()+"/softwarerepo")
{
   if(!Filesystem::dirExist(m_dataDir)){
      Filesystem::createPath(m_dataDir);
   }
}

ServiceInvokeResponse InfoWrapper::lsSoftwareRepoDir(const ServiceInvokeRequest &request)
{
   int baseLength = m_dataDir.size()+1;
   QList<QVariant> ret;
   Filesystem::traverseFs(m_dataDir, 1, [&ret, baseLength](QFileInfo &fileInfo, int){
      QMap<QString, QVariant> items;
      items.insert("filename", fileInfo.absoluteFilePath().remove(0, baseLength));
      items.insert("filesize", fileInfo.size());
      ret.append(QVariant(items));
   });
   ServiceInvokeResponse response("Repo/Info/lsSoftwareRepoDir", true);
   response.setExtraData(encodeJsonObject(QVariant(ret)));
   response.setSerial(request.getSerial());
   response.setIsFinal(true);
   return response;
}

ServiceInvokeResponse InfoWrapper::removeSoftware(const ServiceInvokeRequest &request)
{
   QString filename = m_dataDir + QDir::separator() + request.getArg("filename").toString();
   if(Filesystem::fileExist(filename)){
      Filesystem::deleteFile(filename);
   }
   ServiceInvokeResponse response("Repo/Info/removeSoftware", true);
   response.setSerial(request.getSerial());
   response.setIsFinal(true);
   return response;
}

}//repo
}//ummservice

