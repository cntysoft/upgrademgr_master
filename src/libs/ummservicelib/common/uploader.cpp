#include <QList>
#include <QVariant>
#include <QThread>
#include <QFile>
#include <QByteArray>
#include <QCryptographicHash>
#include <QString>
#include <QMap>

#include "ummlib/kernel/stddir.h"
#include "uploader.h"
#include "corelib/kernel/errorinfo.h"
#include "corelib/io/filesystem.h"

namespace ummservice{
namespace common{

using ummlib::kernel::StdDir;
using sn::corelib::ErrorInfo;
using sn::corelib::Filesystem;

Uploader::Uploader(ServiceProvider& provider)
   : AbstractService(provider)
{
}

ServiceInvokeResponse Uploader::init(const ServiceInvokeRequest &request)
{
   try{
//      QMap<QString, QVariant> args = request.getArgs();
//      QString baseDir = args.at(0).toString();
//      if(baseDir.isEmpty()){
//         baseDir = StdDir::getSoftwareRepoDir();
//      }
//      if(!Filesystem::dirExist(baseDir)){
//         Filesystem::createPath(baseDir);
//      }
//      QString filename = args.at(1).toString();
//      QFileInfo fileInfo(filename);
//      filename = baseDir+"/"+fileInfo.fileName();
//      UploadContext context;
//      context.baseDir = baseDir;
//      context.filename = filename;
//      context.md5 = args.at(2).toString();
//      context.total = args.at(3).toInt();
//      context.uploaded = 0;
//      context.step = UPLOAD_STEP_PREPARE;
//      context.cycleSize = args.at(4).toInt();
//      QFile *file = new QFile(filename);
//      file->open(QIODevice::Truncate | QIODevice::WriteOnly);
//      context.targetFile = file;
//      m_context[request.getSocketNum()] = context;
//      ServiceInvokeResponse response("Common/Uploader/init", true);
//      response.setSerial(request.getSerial());
//      return response;
   }catch(ErrorInfo errorInfo){
      ServiceInvokeResponse response("Common/Uploader/init", false);
      response.setError({0, errorInfo.toString()});
      response.setSerial(request.getSerial());
      removeContextByRequestSocketId(request.getSocketNum());
      return response;
   }
}

ServiceInvokeResponse Uploader::receiveData(const ServiceInvokeRequest &request)
{
   try{
      UploadContext &context = getContextByRequest(request);
      if(context.step != UPLOAD_STEP_PREPARE && context.step != UPLOAD_STEP_PROCESS){
         throw ErrorInfo("上下文状态错误");  
      }
      ServiceInvokeResponse response("Common/Uploader/receiveData", true);
      response.setSerial(request.getSerial());
      QByteArray unit = QByteArray::fromBase64(request.getExtraData());
      context.targetFile->write(unit);
      context.uploaded += unit.size();
      context.currentCycle++;
      QMap<QString, QVariant> data{
         {"receivedCycleSize", QVariant(unit.size())}
      };
      if(0 == (context.currentCycle % context.cycleSize)){
         data.insert("cycleComplete", QVariant(true));
      }
      if(context.uploaded == context.total){
        data.insert("lastReceived", QVariant(true));
      }
      context.step = UPLOAD_STEP_PROCESS;
      response.setData(data);
      return response;
   }catch(ErrorInfo errorInfo){
      ServiceInvokeResponse response("Common/Uploader/receiveData", false);
      response.setError({0, errorInfo.toString()});
      response.setSerial(request.getSerial());
      removeContextByRequestSocketId(request.getSocketNum());
      return response;
   }
}

ServiceInvokeResponse Uploader::checkUploadResult(const ServiceInvokeRequest &request)
{
   UploadContext &context = getContextByRequest(request);
   try{
      if(context.step != UPLOAD_STEP_PROCESS){
         throw ErrorInfo("上下文状态错误");  
      }
      context.step = UPLOAD_STEP_CHECKSUM;
      ServiceInvokeResponse response("Common/Uploader/checkUploadResult", true);
      response.setSerial(request.getSerial());
      context.targetFile->close();
      QFile file(context.filename);
      file.open(QIODevice::ReadOnly);
      QByteArray fileContent;
      while(!file.atEnd()){
         fileContent.append(file.read(2048));
      }
      file.close();
      QByteArray md5(QCryptographicHash::hash(fileContent, QCryptographicHash::Md5).toHex());
      if(md5 != context.md5){
         response.setStatus(false);
         response.setError({1, "md5校验失败"});
      }
      context.step = UPLOAD_STEP_FINISH;
      removeContextByRequestSocketId(request.getSocketNum());
      return response;
   }catch(ErrorInfo errorInfo){
      ServiceInvokeResponse response("Common/Uploader/checkUploadResult", false);
      response.setError({0, errorInfo.toString()});
      response.setSerial(request.getSerial());
      QString filename = context.filename;
      if(Filesystem::fileExist(filename)){
         Filesystem::deleteFile(filename);
      }
      removeContextByRequestSocketId(request.getSocketNum());
      return response;
   }
}

Uploader::UploadContext& Uploader::getContextByRequest(const ServiceInvokeRequest &request)
{
   Q_ASSERT_X(m_context.contains(request.getSocketNum()), "Uploader::getContextByRequest", "upload context is not exist");
   return m_context[request.getSocketNum()];
}

Uploader& Uploader::removeContextByRequestSocketId(int sid)
{
   if(m_context.contains(sid)){
      UploadContext context = m_context.value(sid);
      if(nullptr != context.targetFile){
         delete context.targetFile;
      }
      m_context.remove(sid);
   }
   return *this;
}

Uploader::~Uploader()
{
   QMap<int, UploadContext>::const_iterator it = m_context.cbegin();
   while(it != m_context.cend()){
      delete (*it).targetFile;
   }
}

}//common
}//ummservice