#ifndef UMM_SERVICE_SERVER_STATUS_INFO_H
#define UMM_SERVICE_SERVER_STATUS_INFO_H

#include <QSharedPointer>
#include <QString>
#include <QMap>

#include "ummservicelib/global_defs.h"
#include "corelib/network/rpc/abstract_service.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "corelib/network/rpc/service_provider.h"

namespace ummservice{
namespace serverstatus{

UMM_USING_SERVICE_NAMESPACES

class UMM_SERVICE_EXPORT Info
{
public:
   Info& setServiceServerAddressMeta(const QVariant &data);
   QMap<QString, QMap<QString, QString>> getServiceServerAddressMeta();
   QMap<QString, QString> getServiceServerAddressMetaByKey(const QString &key);
   QString getAddressMetaFilename()const;
};

class UMM_SERVICE_EXPORT InfoWrapper : public AbstractService
{
   Q_OBJECT
public:
   InfoWrapper(ServiceProvider& provider);
   Q_INVOKABLE ServiceInvokeResponse getVersionInfo(const ServiceInvokeRequest &request);
   Q_INVOKABLE ServiceInvokeResponse setServiceServerAddressMeta(const ServiceInvokeRequest &request);
   Q_INVOKABLE ServiceInvokeResponse getServiceServerAddressMeta(const ServiceInvokeRequest &request);
protected:
   QString getAddressMetaFilename()const;
   QSharedPointer<Info> m_helper;
};


}//serverstatus
}//ummservice

#endif // UMM_SERVICE_SERVER_STATUS_INFO_H
