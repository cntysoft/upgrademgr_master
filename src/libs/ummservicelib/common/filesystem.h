#ifndef UMM_SERVICE_COMMON_FILESYSTEM_H
#define UMM_SERVICE_COMMON_FILESYSTEM_H

#include <QFile>
#include <QSharedPointer>
#include <QStringList>

#include "ummservicelib/global_defs.h"
#include "corelib/network/rpc/abstract_service.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "corelib/network/rpc/service_provider.h"

namespace ummservice{
namespace common{

using sn::corelib::network::ServiceInvokeRequest;
using sn::corelib::network::ServiceInvokeResponse;

UMM_USING_SERVICE_NAMESPACES

class UMM_SERVICE_EXPORT FilesystemWrapper : public AbstractService
{
   Q_OBJECT
public:
   FilesystemWrapper(ServiceProvider& provider);
public:
   Q_INVOKABLE ServiceInvokeResponse ls(const ServiceInvokeRequest &request);
   Q_INVOKABLE ServiceInvokeResponse getStartDirPaths(const ServiceInvokeRequest &request);
   Q_INVOKABLE ServiceInvokeResponse createDir(const ServiceInvokeRequest &request);
   Q_INVOKABLE ServiceInvokeResponse deleteDir(const ServiceInvokeRequest &request);
   Q_INVOKABLE ServiceInvokeResponse deleteDirs(const ServiceInvokeRequest &request);
   Q_INVOKABLE ServiceInvokeResponse deleteFile(const ServiceInvokeRequest &request);
   Q_INVOKABLE ServiceInvokeResponse deleteFiles(const ServiceInvokeRequest &request);
   Q_INVOKABLE ServiceInvokeResponse rename(const ServiceInvokeRequest &request);
   Q_INVOKABLE ServiceInvokeResponse cat(const ServiceInvokeRequest &request);
   Q_INVOKABLE ServiceInvokeResponse save(const ServiceInvokeRequest &request);
   Q_INVOKABLE ServiceInvokeResponse paste(const ServiceInvokeRequest &request);
protected:
   void checkFileOrPathIsAllowed(const QString &path);
   bool isValidPath(const QString &path);
protected:
   const QString m_baseDir; 
   static QStringList sm_allowPath;
};

}//common
}//ummservice

#endif // UMM_SERVICE_COMMON_FILESYSTEM_H
