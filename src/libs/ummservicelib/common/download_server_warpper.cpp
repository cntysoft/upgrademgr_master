#include <QFile>
#include <QString>
#include <QVariant>
#include <QByteArray>

#include "download_server.h"
#include "ummlib/kernel/stddir.h"
#include "io/filesystem.h"
#include "corelib/global/common_funcs.h"
#include "corelib/kernel/errorinfo.h"

namespace ummservice{
namespace common{

using ummlib::kernel::StdDir;
using sn::corelib::Filesystem;
using sn::corelib::ErrorInfo;
using sn::corelib::throw_exception;

const int DownloadWrapper::E_FILE_NOT_EXIST = 10001;
const int DownloadWrapper::E_FILE_CANOT_OPEN = 10002;

//暂时只能从软件库下载
DownloadWrapper::DownloadWrapper(ServiceProvider &provider)
   : AbstractService(provider),
     m_baseDir(StdDir::getSoftwareRepoDir())
{}

ServiceInvokeResponse DownloadWrapper::init(const ServiceInvokeRequest &request)
{
   if(hasContextByRequest(request)){
      QSharedPointer<DownloadContext> context = getContextByRequest(request);
      if(context->step != DOWNLOAD_STEP_PREPARE){
         throw_exception(ErrorInfo("状态错误"), getErrorContext());
      }
   }
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
   QSharedPointer<DownloadContext> context(new DownloadContext);
   context->filename = filename;
   context->step = DOWNLOAD_STEP_PREPARE;
   context->targetFile= targetFile;
   response.setDataItem("fileSize", targetFile->size());
   response.setDataItem("serverStep", context->step);
   response.setDataItem("chunkSize", m_chunkSize);
   context->step = DOWNLOAD_STEP_META_SENT;
   m_contextPool.insert(request.getSocketNum(), context);
   response.setStatus(true);
   return response;
}

ServiceInvokeResponse DownloadWrapper::sendData(const ServiceInvokeRequest &request)
{
   bool stateOk = true;
   QSharedPointer<DownloadContext> context;
   if(!hasContextByRequest(request)){
      stateOk = false;
   }else{
      context = getContextByRequest(request);
      if(context->step != DOWNLOAD_STEP_META_SENT){
         stateOk = false;
      }
   }
   if(!stateOk){
      QSharedPointer<DownloadContext> context = getContextByRequest(request);
      if(context->step != DOWNLOAD_STEP_PREPARE){
         throw_exception(ErrorInfo("状态错误"), getErrorContext());
      }
   }
   const QMap<QString, QVariant> &args = request.getArgs();
   checkRequireFields(args, {"retrieveSize", "startPointer"});
   int retrieveSize = args.value("retrieveSize").toInt();
   //   int startPointer = args.value("startPointer").toInt();
   QByteArray data = context->targetFile->read(retrieveSize);
   ServiceInvokeResponse response("Common/Download/sendData", true);
   response.setDataItem("dataSize", data.size());
   response.setExtraData(data);
   response.setSerial(request.getSerial());
   return response;
}

ServiceInvokeResponse DownloadWrapper::notifyComplete(const ServiceInvokeRequest &request)
{
   
}

ServiceInvokeResponse DownloadWrapper::terminal(const ServiceInvokeRequest &request)
{
   
}

bool DownloadWrapper::hasContextByRequest(const ServiceInvokeRequest &request)
{
   return m_contextPool.contains(request.getSocketNum());
}

QSharedPointer<DownloadWrapper::DownloadContext> DownloadWrapper::getContextByRequest(const ServiceInvokeRequest &request)
{
   Q_ASSERT_X(m_contextPool.contains(request.getSocketNum()), "DownloadWrapper::getContextByRequest", "download context is not exist");
   return m_contextPool[request.getSocketNum()];
}

DownloadWrapper& DownloadWrapper::removeContextByRequestSocketId(int sid)
{
   if(m_contextPool.contains(sid)){
      QSharedPointer<DownloadContext> context = m_contextPool.value(sid);
      if(nullptr != context->targetFile){
         delete context->targetFile;
      }
      m_contextPool.remove(sid);
   }
   return *this;
}

DownloadWrapper::~DownloadWrapper()
{}

}//common
}//ummservice