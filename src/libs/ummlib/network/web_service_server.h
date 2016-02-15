#ifndef UMM_LIB_NETWORK_WEB_SERVICE_SERVER_H
#define UMM_LIB_NETWORK_WEB_SERVICE_SERVER_H

#include <QMap>
#include <QByteArray>
#include <QtWebSockets/QWebSocketServer>

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

class UMM_LIB_EXPORT WebServiceServer : public QWebSocketServer
{
   Q_OBJECT
public:
   const static int SUPER_SERIAL_CODE = 0;
public:
   WebServiceServer(Application &app, const QString &serverName, QObject* parent = nullptr);
   virtual ~WebServiceServer();
   ServiceProvider& getServiceProvider();
   bool run();
   WebServiceServer& setHost(const QHostAddress& host);
   QHostAddress& getHost();
   WebServiceServer& setPort(quint16 port);
   quint16 getPort();
   WebServiceServer& setEnableSsl(bool flag);
   bool getEnableSsl();
protected:
   void processProtocolParseError(QWebSocket &socket, int errorCode, const QString &errorString);
   int getSocketKeyIndex();
protected slots:
   void unboxRequest(const QByteArray &message);
   void socketDisconnectedHandler();
   void newConnectionHandler();
protected:
   Application &m_app;
   bool m_enableSsl = false;
   QHostAddress m_host;
   quint16 m_port;
   ServiceProvider &m_serviceProvider;
   static QMap<int, QTcpSocket*> m_connections;
   static QStringList m_requirePackageKeys;
};

UMM_LIB_EXPORT WebServiceServer*& get_global_web_service_server();
UMM_LIB_EXPORT void set_global_web_service_server(WebServiceServer* server);

}//network
}//ummlib


#endif // UMM_LIB_NETWORK_WEB_SERVICE_SERVER_H
