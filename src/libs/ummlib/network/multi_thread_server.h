#ifndef UPGRADEMGR_MASTER_NETWORK_MULTI_THREAD_SERVER_H
#define UPGRADEMGR_MASTER_NETWORK_MULTI_THREAD_SERVER_H

#include <QMap>
#include <QByteArray>

QT_BEGIN_NAMESPACE
class QTcpSocket;
QT_END_NAMESPACE

#include "ummlib/global/global.h"

#include "corelib/network/abstract_multi_thread_server.h"
#include "corelib/kernel/application.h"
#include "corelib/network/rpc/api_provider.h"
#include "corelib/network/rpc/invoke_meta.h"

namespace ummlib{
namespace network{

using sn::corelib::Application;
using sn::corelib::network::AbstractMultiThreadServer;
using sn::corelib::network::ApiProvider;
using sn::corelib::network::ApiInvokeRequest;
using sn::corelib::network::ApiInvokeResponse;

class UM_MASTER_LIB_EXPORT MultiThreadServer : public AbstractMultiThreadServer
{
public:
   MultiThreadServer(Application& app, QObject* parent = nullptr);
   ApiProvider& getApiProvider();
protected:
   virtual void incomingConnection(qintptr socketDescriptor);
   void processRequest(const ApiInvokeRequest &request);
protected slots:
   void unboxRequest();
protected:
   QByteArray m_packageUnitBuffer;
   ApiProvider& m_apiProvider;
   static QMap<int, QTcpSocket*> m_connections;
};

}//network
}//ummlib

#endif // UPGRADEMGR_MASTER_NETWORK_MULTI_THREAD_SERVER_H
