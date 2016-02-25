#ifndef UMM_SERVICE_COMMON_DOWNLOAD_SERVER_H
#define UMM_SERVICE_COMMON_DOWNLOAD_SERVER_H

#include <QFile>

#include "ummservicelib/global_defs.h"
#include "corelib/network/rpc/abstract_service.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "corelib/network/rpc/service_provider.h"

namespace ummservice{
namespace common{

using sn::corelib::network::ServiceInvokeRequest;
using sn::corelib::network::ServiceInvokeResponse;

UMM_USING_SERVICE_NAMESPACES

class UMM_SERVICE_EXPORT DownloadWrapper : public AbstractService
{
   Q_OBJECT
   const static int E_FILE_NOT_EXIST;
   const static int E_FILE_CANOT_OPEN;
   
   const static int DOWNLOAD_STEP_PREPARE = 0;
   const static int DOWNLOAD_STEP_META_SENT = 1;
   const static int DOWNLOAD_STEP_START = 2;
   const static int DOWNLOAD_STEP_PROCESS = 3;
   const static int DOWNLOAD_STEP_FINISH = 4;
   
   struct DownloadContext
   {
      QString filename;
      int step = DOWNLOAD_STEP_PREPARE;
      QFile *targetFile = nullptr;
   };
public:
   DownloadWrapper(ServiceProvider& provider);
   virtual ~DownloadWrapper();
   Q_INVOKABLE ServiceInvokeResponse init(const ServiceInvokeRequest &request);
protected:
   DownloadContext& getContextByRequest(const ServiceInvokeRequest &request);
   DownloadWrapper& removeContextByRequestSocketId(int sid);
protected:
   QMap<int, DownloadContext> m_contextPool;
   int m_cycleSize = 20;
   int m_chunkSize = 2048;
   int m_currentCycle = 0;
   QString m_baseDir;
};

}//common
}//ummservice

#endif // UMM_SERVICE_COMMON_DOWNLOAD_SERVER_H
