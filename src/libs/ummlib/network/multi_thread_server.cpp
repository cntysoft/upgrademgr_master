#include "multi_thread_server.h"

namespace ummlib{
namespace network{

MultiThreadServer::MultiThreadServer(Application& app,QObject *parent)
   : AbstractMultiThreadServer(app, parent)
{
}

void MultiThreadServer::incomingConnection(qintptr socketDescriptor)
{
   qDebug() << socketDescriptor;
}


}//network
}//ummlib