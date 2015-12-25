#ifndef UPGRADEMGR_MASTER_NETWORK_MULTI_THREAD_SERVER_H
#define UPGRADEMGR_MASTER_NETWORK_MULTI_THREAD_SERVER_H

#include "corelib/network/abstract_multi_thread_server.h"
#include "corelib/kernel/application.h"

#include "ummlib/global/global.h"

namespace ummlib{
namespace network{

using sn::corelib::Application;
using sn::network::AbstractMultiThreadServer;

class UM_MASTER_LIB_EXPORT MultiThreadServer : public AbstractMultiThreadServer
{
public:
   MultiThreadServer(Application& app, QObject* parent = nullptr);
protected:
   virtual void incomingConnection(qintptr socketDescriptor);
};

}//network
}//ummlib

#endif // UPGRADEMGR_MASTER_NETWORK_MULTI_THREAD_SERVER_H
