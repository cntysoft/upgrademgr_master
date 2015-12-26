#include <QTcpSocket>
#include <QDebug>

#include "multi_thread_server.h"

namespace ummlib{
namespace network{

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
}

void MultiThreadServer::unboxRequest()
{
   
   qDebug() << "data";  
}

}//network
}//ummlib