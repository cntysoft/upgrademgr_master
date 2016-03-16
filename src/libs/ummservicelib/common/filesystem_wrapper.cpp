#include <QMap>
#include <QVariant>
#include <QString>
#include <QFileInfo>
#include <QDateTime>

#include "filesystem.h"
#include "ummlib/kernel/stddir.h"
#include "corelib/io/filesystem.h"

namespace ummservice{
namespace common{

using ummlib::kernel::StdDir;
using sn::corelib::Filesystem;

QStringList FilesystemWrapper::sm_allowPath = {
   "/softwarerepo"
};

QStringList FilesystemWrapper::sm_protectedList = {
   "/softwarerepo"
};


FilesystemWrapper::FilesystemWrapper(ServiceProvider &provider)
   : AbstractService(provider),
     m_baseDir(StdDir::getBaseDataDir())
{}

ServiceInvokeResponse FilesystemWrapper::ls(const ServiceInvokeRequest &request)
{
   QMap<QString, QVariant> args = request.getArgs();
   checkRequireFields(args, {"path"});
   QString path = args.value("path").toString().trimmed();
   QMap<QString, QVariant> ret{
      {"path", path}
   };
   ServiceInvokeResponse response("Common/Filesystem/ls", true);
   response.setSerial(request.getSerial());
   response.setIsFinal(true);
   if(path.size() == 0){
      ret.insert("entries", QVariantList());
      response.setExtraData(encodeJsonObject(QVariant(ret)));
      return response;
   }
   QVariantList entries;
   path = m_baseDir + path;
   QFileInfoList fileInfoList = Filesystem::ls(path);
   for(int i = 0; i < fileInfoList.size(); i++){
      QFileInfo fileInfo = fileInfoList[i];
      QMap<QString, QVariant> item{
         {"name", fileInfo.fileName()},
         {"type", fileInfo.isDir() ? "dir" : !fileInfo.suffix().isEmpty() ? fileInfo.suffix() : "txt"},
            {"cTime", fileInfo.created()}
      };
      entries.append(item);
   }
   
}

ServiceInvokeResponse FilesystemWrapper::getStartDirPaths(const ServiceInvokeRequest &request)
{
   QMap<QString, QVariant> args = request.getArgs();
   checkRequireFields(args, {"startPaths"});
   QStringList startPaths = args.value("startPaths").toStringList();
   QMap<QString, QVariant> ret{
      {"path", ""},
      {"entries", QVariantList()}
   };
   ServiceInvokeResponse response("Common/Filesystem/getStartDirPaths", true);
   response.setSerial(request.getSerial());
   response.setIsFinal(true);
   if(startPaths.empty()){
      response.setExtraData(encodeJsonObject(QVariant(ret)));
      return response;
   }else if(startPaths.size() == 1){
      ServiceInvokeRequest lsRequest(request);
      lsRequest.setArgs({{"path", startPaths.first()}});
      return ls(lsRequest);
   }
   
}

}//common
}//ummservice
