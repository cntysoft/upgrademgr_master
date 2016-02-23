#include "download_server.h"
#include "ummlib/kernel/stddir.h"

namespace ummservice{
namespace common{

using ummlib::kernel::StdDir;
//暂时只能从软件库下载
DownloadWrapper::DownloadWrapper(ServiceProvider &provider)
   : AbstractService(provider),
     m_baseDir(StdDir::getSoftwareRepoDir())
{}

ServiceInvokeResponse DownloadWrapper::init(const ServiceInvokeRequest &request)
{
   
}

DownloadWrapper::DownloadContext& DownloadWrapper::getContextByRequest(const ServiceInvokeRequest &request)
{
   Q_ASSERT_X(m_context.contains(request.getSocketNum()), "DownloadWrapper::getContextByRequest", "upload context is not exist");
   return m_context[request.getSocketNum()];
}

DownloadWrapper& DownloadWrapper::removeContextByRequestSocketId(int sid)
{
   if(m_context.contains(sid)){
      DownloadContext context = m_context.value(sid);
      if(nullptr != context.targetFile){
         delete context.targetFile;
      }
      m_context.remove(sid);
   }
   return *this;
}

DownloadWrapper::~DownloadWrapper()
{}

}//common
}//ummservice