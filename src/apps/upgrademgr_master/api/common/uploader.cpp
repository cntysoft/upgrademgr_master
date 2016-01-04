#include <QList>
#include <QVariant>

#include "ummlib/kernel/stddir.h"
#include "uploader.h"
#include "corelib/kernel/errorinfo.h"
#include "corelib/io/filesystem.h"

namespace upgrademgr{
namespace master{
namespace api{
namespace common{

using ummlib::kernel::StdDir;
using sn::corelib::ErrorInfo;
using sn::corelib::Filesystem;

Uploader::Uploader(ApiProvider& provider)
   : AbstractApi(provider)
{
}

ApiInvokeResponse Uploader::init(const ApiInvokeRequest &request)
{
   try{
      QList<QVariant> args = request.getArgs();
      QString baseDir = args.at(0).toString();
      if(baseDir.isEmpty()){
         baseDir = StdDir::getSoftwareRepoDir();
      }
      if(!Filesystem::dirExist(baseDir)){
         Filesystem::createPath(baseDir);
      }
      QString filename = args.at(1).toString();
      QFileInfo fileInfo(filename);
      filename = baseDir+"/"+fileInfo.fileName();
      QString md5 = args.at(2).toString();
      int total = args.at(3).toInt();
      UploadContext context;
      context.baseDir = baseDir;
      context.filename = filename;
      context.md5 = md5;
      context.total = total;
      context.uploaded = 0;
      context.step = UPLOAD_STEP_PREPARE;
      QFile *file = new QFile(filename);
      file->open(QIODevice::Truncate | QIODevice::WriteOnly);
      m_context[request.getSocketNum()] = context;
      ApiInvokeResponse response("Common/Uploader/init", true);
      response.setSerial(request.getSerial());
      return response;
   }catch(ErrorInfo errorInfo){
      ApiInvokeResponse response("Common/Uploader/init", false);
      response.setError({0, errorInfo.toString()});
      response.setSerial(request.getSerial());
      removeContextByRequestSocketId(request.getSocketNum());
      return response;
   }
}

ApiInvokeResponse Uploader::receiveData(const ApiInvokeRequest &request)
{
   try{
      UploadContext &context = getContextByRequest(request);
      if(context.step != UPLOAD_STEP_PREPARE){
         throw ErrorInfo("上下文状态错误");  
      }
      qDebug() << 1;
      return ApiInvokeResponse("Common/Uploader/receiveData", true);
   }catch(ErrorInfo errorInfo){
      ApiInvokeResponse response("Common/Uploader/receiveData", false);
      response.setError({0, errorInfo.toString()});
      response.setSerial(request.getSerial());
      removeContextByRequestSocketId(request.getSocketNum());
      return response;
   }
}

Uploader::UploadContext& Uploader::getContextByRequest(const ApiInvokeRequest &request)
{
   Q_ASSERT_X(m_context.contains(request.getSocketNum()), "Uploader::getContextByRequest", "upload context is not exist");
   return m_context[request.getSocketNum()];
}

Uploader& Uploader::removeContextByRequestSocketId(int sid)
{
   if(m_context.contains(sid)){
      UploadContext context = m_context.value(sid);
      
      delete context.targetFile;
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
}//api
}//master
}//upgrademgr