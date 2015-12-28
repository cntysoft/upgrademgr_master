#ifndef UPGRADEMGR_MASTER_NETWORK_MULTI_THREAD_SERVER_H
#define UPGRADEMGR_MASTER_NETWORK_MULTI_THREAD_SERVER_H

#include <QMap>

QT_BEGIN_NAMESPACE
class QTcpSocket;
QT_END_NAMESPACE

#include "ummlib/global/global.h"

#include "corelib/network/abstract_multi_thread_server.h"
#include "corelib/kernel/application.h"
#include "corelib/network/rpc/api_provider.h"

namespace ummlib{
namespace network{

using sn::corelib::Application;
using sn::corelib::network::AbstractMultiThreadServer;
using sn::corelib::network::ApiProvider;

class UM_MASTER_LIB_EXPORT MultiThreadServer : public AbstractMultiThreadServer
{
public:
   MultiThreadServer(Application& app, QObject* parent = nullptr);
protected:
   virtual void incomingConnection(qintptr socketDescriptor);
protected slots:
   void unboxRequest();
protected:
   ApiProvider& m_apiProvider;
   static QMap<int, QTcpSocket*> m_connections;
};

}//network
}//ummlib

#endif // UPGRADEMGR_MASTER_NETWORK_MULTI_THREAD_SERVER_H
