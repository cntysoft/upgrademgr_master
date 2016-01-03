#ifndef UPGRADEMGR_MASTER_API_COMMON_UPLOADER_H
#define UPGRADEMGR_MASTER_API_COMMON_UPLOADER_H

#include <QMap>
#include <QString>

#include "api/macros.h"
#include "corelib/network/rpc/abstract_api.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "corelib/network/rpc/api_provider.h"

namespace upgrademgr{
namespace master{
namespace api{
namespace common{

using sn::corelib::network::ApiInvokeRequest;
using sn::corelib::network::ApiInvokeResponse;

UMM_USING_API_NAMESPACES

class Uploader : public AbstractApi
{
   Q_OBJECT
   const static int UPLOAD_STEP_PREPARE = 0;
   const static int UPLOAD_STEP_START = 1;
   const static int UPLOAD_STEP_PROCESS = 2;
   const static int UPLOAD_STEP_FINISH = 3;
   struct UploadContext
   {
      QString baseDir;
      QString filename;
      int total = 0;
      int uploaded = 0;
      int step = UPLOAD_STEP_PREPARE;
   };
public:
   Uploader(ApiProvider& provider);
   Q_INVOKABLE ApiInvokeResponse init(const ApiInvokeRequest &request);
protected:
   UploadContext& getContextByRequest(const ApiInvokeRequest &request);
protected:
   QMap<int, UploadContext> m_context;
};

}//common
}//api
}//master
}//upgrademgr

#endif // UPGRADEMGR_MASTER_API_COMMON_UPLOADER_H