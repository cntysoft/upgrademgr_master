#include <QWebSocket>
#include <QDataStream>
#include <QByteArray>
#include <QDebug>
#include <QChar>
#include <QString>
#include <QJsonDocument>
#include <QJsonParseError>

#include "web_service_server.h"

namespace ummlib{
namespace network{


static WebServiceServer *globalServer = nullptr;

WebServiceServer::WebServiceServer(Application& app, const QString &serverName, QObject *parent)
   : QWebSocketServer(serverName, QWebSocketServer::SslMode::NonSecureMode, parent),
     m_app(app),
     m_serviceProvider(ServiceProvider::instance())
{
   connect(this, SIGNAL(newConnection()), this, SLOT(newConnectionHandler()));
}

QStringList WebServiceServer::m_requirePackageKeys{
   "name", "method", "serial"
};

bool WebServiceServer::run()
{
   //暂时简单的监听
   bool ret = listen(m_host, m_port);
   return ret;
}

WebServiceServer& WebServiceServer::setHost(const QHostAddress &host)
{
   m_host = host;
   return *this;
}

QHostAddress& WebServiceServer::getHost()
{
   return m_host;
}

WebServiceServer& WebServiceServer::setPort(quint16 port)
{
   m_port = port;
   return *this;
}

quint16 WebServiceServer::getPort()
{
   return m_port;
}

WebServiceServer& WebServiceServer::setEnableSsl(bool flag)
{
   m_enableSsl = flag;
   return *this;
}

bool WebServiceServer::getEnableSsl()
{
   return m_enableSsl;
}

ServiceProvider& WebServiceServer::getServiceProvider()
{
   return m_serviceProvider;
}

void WebServiceServer::newConnectionHandler()
{
   //这里暂时不进行加密处理
   //暂时也不进行多线程实现
   QWebSocket *socket = nextPendingConnection();
   connect(socket, &QWebSocket::binaryMessageReceived, this, &WebServiceServer::unboxRequest);
   connect(socket, &QWebSocket::disconnected, this, &WebServiceServer::socketDisconnectedHandler);
   //   ServiceProvider& provider = ServiceProvider::instance();
   //   provider.setUnderlineSocket((int) socketDescriptor, socket);
}

void WebServiceServer::socketDisconnectedHandler()
{
   QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
   if (socket) {
      socket->deleteLater();
   }
}

void WebServiceServer::unboxRequest(const QByteArray &message)
{
   QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
   QByteArray unit = QByteArray::fromBase64(message);
   ServiceInvokeRequest request;
   QJsonParseError parserError;
   request.setIsWebSocket(true);
   QJsonDocument jsonDoc = QJsonDocument::fromJson(unit, &parserError);
   if(parserError.error == QJsonParseError::NoError){
      if(jsonDoc.isObject()){
         QJsonObject packageObject = jsonDoc.object();
         
      }else{
         //出错处理
      }
   }else{
      //记录日志等等
      //协议错误
      
      qDebug() << parserError.errorString();
   }
}

void WebServiceServer::processProtocolParseError(QWebSocket &socket, int errorCode, const QString &errorString)
{
   
}

WebServiceServer::~WebServiceServer()
{
}

void WebServiceServer::processRequest(const ServiceInvokeRequest &request)
{
   //m_serviceProvider.callService(request);
}

WebServiceServer*& get_global_web_service_server()
{
   return globalServer;
}

void set_global_web_service_server(WebServiceServer *server)
{
   globalServer = server;
}

}//network
}//ummlib