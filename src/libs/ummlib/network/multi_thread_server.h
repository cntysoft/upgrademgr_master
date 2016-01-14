#ifndef UPGRADEMGR_MASTER_LIB_NETWORK_MULTI_THREAD_SERVER_H
#define UPGRADEMGR_MASTER_LIB_NETWORK_MULTI_THREAD_SERVER_H

#include <QMap>
#include <QByteArray>

QT_BEGIN_NAMESPACE
class QTcpSocket;
QT_END_NAMESPACE

#include "ummlib/global/global.h"

#include "corelib/network/abstract_multi_thread_server.h"
#include "corelib/kernel/application.h"
#include "corelib/network/rpc/service_provider.h"
#include "corelib/network/rpc/invoke_meta.h"

namespace ummlib{
namespace network{

using sn::corelib::Application;
using sn::corelib::network::AbstractMultiThreadServer;
using sn::corelib::network::ServiceProvider;
using sn::corelib::network::ServiceInvokeRequest;
using sn::corelib::network::ServiceInvokeResponse;

class UM_MASTER_LIB_EXPORT MultiThreadServer : public AbstractMultiThreadServer
{
public:
   MultiThreadServer(Application& app, QObject* parent = nullptr);
   virtual ~MultiThreadServer();
   ServiceProvider& getServiceProvider();
protected:
   virtual void incomingConnection(qintptr socketDescriptor);
   void processRequest(const ServiceInvokeRequest &request);
protected slots:
   void unboxRequest();
protected:
   QByteArray m_packageUnitBuffer;
   ServiceProvider& m_serviceProvider;
   static QMap<int, QTcpSocket*> m_connections;
};

UM_MASTER_LIB_EXPORT MultiThreadServer*& get_global_server();
UM_MASTER_LIB_EXPORT void set_global_server(MultiThreadServer* server);

}//network
}//ummlib

#endif // UPGRADEMGR_MASTER_LIB_NETWORK_MULTI_THREAD_SERVER_H
