#include <QTcpSocket>
#include <QDataStream>
#include <QDebug>

#include "multi_thread_server.h"
#include "corelib/network/rpc/invoke_meta.h"

namespace ummlib{
namespace network{

using sn::corelib::network::ApiInvokeRequest;

MultiThreadServer::MultiThreadServer(Application& app,QObject *parent)
   : AbstractMultiThreadServer(app, parent),
     m_apiProvider(ApiProvider::instance())
{
}

void MultiThreadServer::incomingConnection(qintptr socketDescriptor)
{
   //这里暂时不进行加密处理
   //暂时也不进行多线程实现
   QTcpSocket* socket = new QTcpSocket(this);
   socket->setSocketDescriptor(socketDescriptor);
   connect(socket, &QTcpSocket::readyRead, this, &MultiThreadServer::unboxRequest);
   connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
}

void MultiThreadServer::unboxRequest()
{
   QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
   char byte;
   while(!socket->atEnd()){
      socket->read(&byte, 1);
      if('\r' == byte){
         char forward;
         if(socket->peek(&forward, 1) && '\n' == forward){
            //解压当前的包
            QDataStream stream(m_packageUnitBuffer);
            ApiInvokeRequest request;
            stream >> request;
            m_packageUnitBuffer.clear();
            socket->read(&forward, 1);
            continue;
         }
      }
      m_packageUnitBuffer.append(byte);
   }
}

}//network
}//ummlib