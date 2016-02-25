#include <QFile>

#include "download_server.h"
#include "ummlib/kernel/stddir.h"
#include "io/filesystem.h"

namespace ummservice{
namespace common{

using ummlib::kernel::StdDir;
using sn::corelib::Filesystem;

const int DownloadWrapper::E_FILE_NOT_EXIST = 10001;
const int DownloadWrapper::E_FILE_CANOT_OPEN = 10002;

//暂时只能从软件库下载
DownloadWrapper::DownloadWrapper(ServiceProvider &provider)
   : AbstractService(provider),
     m_baseDir(StdDir::getSoftwareRepoDir())
{}

ServiceInvokeResponse DownloadWrapper::init(const ServiceInvokeRequest &request)
{
   QMap<QString, QVariant> args = request.getArgs();
   checkRequireFields(args, {"filename"});
   QString filename(StdDir::getSoftwareRepoDir()+'/'+args.value("filename").toString());
   ServiceInvokeResponse response("Common/Download/init", true);
   response.setSerial(request.getSerial());
   if(!Filesystem::fileExist(filename)){
      response.setStatus(false);
      response.setIsFinal(true);
      response.setError({E_FILE_NOT_EXIST, QString("文件: %1 不存在").arg(filename)});
      return response;
   }
   QFile *targetFile = new QFile(filename);
   if(!targetFile->open(QFile::ReadOnly)){
      response.setStatus(false);
      response.setIsFinal(true);
      response.setError({E_FILE_CANOT_OPEN, QString("文件 : %1 打开失败").arg(filename)});
      return response;
   }
   DownloadContext context;
   context.filename = filename;
   context.step = DOWNLOAD_STEP_PREPARE;
   context.targetFile= targetFile;
   response.setDataItem("size", targetFile->size());
   response.setDataItem("serverStep", context.step);
   context.step = DOWNLOAD_STEP_META_SENT;
   m_contextPool.insert(request.getSocketNum(), context);
   response.setStatus(true);
   return response;
}

DownloadWrapper::DownloadContext& DownloadWrapper::getContextByRequest(const ServiceInvokeRequest &request)
{
   Q_ASSERT_X(m_contextPool.contains(request.getSocketNum()), "DownloadWrapper::getContextByRequest", "upload context is not exist");
   return m_contextPool[request.getSocketNum()];
}

DownloadWrapper& DownloadWrapper::removeContextByRequestSocketId(int sid)
{
   if(m_contextPool.contains(sid)){
      DownloadContext context = m_contextPool.value(sid);
      if(nullptr != context.targetFile){
         delete context.targetFile;
      }
      m_contextPool.remove(sid);
   }
   return *this;
}

DownloadWrapper::~DownloadWrapper()
{}

}//common
}//ummservice