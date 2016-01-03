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
      int total = args.at(2).toInt();
      UploadContext context;
      context.baseDir = baseDir;
      context.filename = filename;
      context.total = total;
      context.uploaded = 0;
      context.step = UPLOAD_STEP_PREPARE;
      m_context[request.getSocketNum()] = context;
      ApiInvokeResponse response("Common/Uploader/init", true);
      response.setSerial(request.getSerial());
      return response;
   }catch(ErrorInfo errorInfo){
      ApiInvokeResponse response("Common/Uploader/init", false);
      response.setError({0, errorInfo.toString()});
      response.setSerial(request.getSerial());
      return response;
   }
}

}//common
}//api
}//master
}//upgrademgr