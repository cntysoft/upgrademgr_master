#include <QWebSocket>
#include <QDataStream>
#include <QByteArray>
#include <QDebug>
#include <QChar>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>

#include <ctime>

#include "web_service_server.h"
#include "global/error_code.h"

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
}

int WebServiceServer::getSocketKeyIndex()
{
   return static_cast<int>(std::time(nullptr));
}

void WebServiceServer::socketDisconnectedHandler()
{
   QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
   qDebug() << "disconnected";
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
   ServiceProvider& provider = ServiceProvider::instance();
   int index = getSocketKeyIndex();
   request.setSocketNum(index);
   provider.setUnderlineSocket(index, socket);
   QJsonDocument jsonDoc = QJsonDocument::fromJson(unit, &parserError);
   if(parserError.error == QJsonParseError::NoError){
      if(jsonDoc.isObject()){
         QJsonObject packageObject = jsonDoc.object();
         QStringList topLevelKeys = packageObject.keys();
         QStringList leakKeys;
         for(int i = 0; i < WebServiceServer::m_requirePackageKeys.length(); i++){
            if(!topLevelKeys.contains(WebServiceServer::m_requirePackageKeys.at(i))){
               leakKeys.append(WebServiceServer::m_requirePackageKeys.at(i));
            }
         }
         if(!leakKeys.isEmpty()){
            processProtocolParseError(*socket, ummlib::E_PROTOCOL_ILL_FORMAT, QString("缺少协议必要字段: %1").arg(leakKeys.join(", ")));
            return;
         }
         request.setName(packageObject.value("name").toString());
         request.setMethod(packageObject.value("method").toString());
         request.setSerial(packageObject.value("serial").toInt());
         if(packageObject.contains("extraData")){
            QByteArray extraData;
            extraData.append(packageObject.value("extraData").toString());
            request.setExtraData(extraData);
         }
         if(packageObject.contains("args")){
            QJsonObject args = packageObject.value("args").toObject();
            QJsonObject::const_iterator it = args.constBegin();
            while(it != args.constEnd()){
               request.setArg(it.key(), it.value().toVariant());
               it++;
            }
         }
         m_serviceProvider.callService(request);
      }else{
         //出错处理
         processProtocolParseError(*socket, ummlib::E_PROTOCOL_ILL_FORMAT, "协议json根元素不为对象");
      }
   }else{
      //记录日志等等
      //协议错误
      processProtocolParseError(*socket, ummlib::E_PROTOCOL_ILL_FORMAT, parserError.errorString());
   }
}

void WebServiceServer::processProtocolParseError(QWebSocket &socket, int errorCode, const QString &errorString)
{
   QJsonDocument responseJsonDoc;
   QJsonObject packageObject;
   packageObject.insert("signature", "kernel/protocolParser");
   packageObject.insert("status", false);
   packageObject.insert("errorCode", errorCode);
   packageObject.insert("errorString", errorString);
   packageObject.insert("isFinal", true);
   packageObject.insert("serial", QJsonValue(WebServiceServer::SUPER_SERIAL_CODE));
   responseJsonDoc.setObject(packageObject);
   socket.sendTextMessage(responseJsonDoc.toJson().toBase64());
   socket.flush();
}

WebServiceServer::~WebServiceServer()
{
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