#include "upgrade_upgrademgr_slave.h"

#include <QDebug>
#include <QProcess>
#include <QDir>
#include <QList>
#include <QVariant>
#include <QStringList>

#include "ummlib/network/multi_thread_server.h"
#include "ummlib/kernel/stddir.h"
#include "ummlib/global/common_funcs.h"
#include "global/const.h"

#include "corelib/io/filesystem.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "corelib/kernel/application.h"
#include "corelib/utils/version.h"

namespace ummservice{
namespace upgrader{

using ummlib::kernel::StdDir;

using sn::corelib::Filesystem;
using sn::corelib::utils::Version;
using sn::corelib::Application;
using sn::corelib::network::ServiceInvokeRequest;

void upgrade_upgrademgr_slave_handler(const ServiceInvokeResponse &response, void* args)
{
   //需要判断对错
   UpgradeUpgradeMgrSlaveWrapper *self = static_cast<UpgradeUpgradeMgrSlaveWrapper*>(args);
   if(response.getStatus()){
      self->m_context->response.setIsFinal(false);
      self->m_context->response.setDataItem("msg", response.getDataItem("msg"));
      self->m_context->response.setDataItem("step", response.getDataItem("step"));
      self->writeInterResponse(self->m_context->request, self->m_context->response);
//      if(response.getDataItem("step").toInt() == UpgradeCloudControllerWrapper::STEP_FINISH){
//         self->m_serviceInvoker->disconnectFromServer();
//         self->clearState();
//      }
   }else{
      //错误处理
      self->m_context->response.setStatus(false);
      self->m_context->response.setIsFinal(false);
      self->m_context->response.setError(response.getError());
      self->writeInterResponse(self->m_context->request, self->m_context->response);
      self->clearState();
      self->m_serviceInvoker->disconnectFromServer();
   }
}

UpgradeUpgradeMgrSlaveWrapper::UpgradeUpgradeMgrSlaveWrapper(sn::corelib::network::ServiceProvider &provider)
   : AbstractService(provider)
{
   
}

ServiceInvokeResponse UpgradeUpgradeMgrSlaveWrapper::upgrade(const ServiceInvokeRequest& request)
{
   const QMap<QString, QVariant> &args = request.getArgs();
   checkRequireFields(args, {"targetVersion", "slaveServerAddress"});
   ServiceInvokeResponse response("Upgrade/UpgradeUpgradeMgrSlave/upgrade", true);
   response.setSerial(request.getSerial());
   if(m_isInAction){
      response.setIsFinal(true);
      response.setStatus(false);
      response.setError({10000, "操作正在进行中"});
      return response;
   }
   m_isInAction = true;
   response.setIsFinal(false);
   m_context.reset(new UpgradeContext);
   m_context->request = request;
   m_context->response = response;
   m_context->targetVersion = args.value("targetVersion").toString();
   QString slaveServerAddress = args.value("slaveServerAddress").toString();
   m_context->targetServerAddress = slaveServerAddress;
   response.setDataItem("msg", QString("正在连接UpgradeMgr slave服务器 %1").arg(slaveServerAddress));
   QSharedPointer<ServiceInvoker> invoker = getServiceInvoker(slaveServerAddress, UMS_LISTEN_PORT);
   m_context->serviceInvoker = invoker;
   connect(invoker.data(), &ServiceInvoker::connectedToServerSignal, this, &UpgradeUpgradeMgrSlaveWrapper::connectToServerHandler);
   connect(invoker.data(), &ServiceInvoker::connectErrorSignal, this, &UpgradeUpgradeMgrSlaveWrapper::connectToServerErrorHandler);
   writeInterResponse(request, response);
   invoker->connectToServer();
   m_eventLoop.exec();
   response.setIsFinal(true);
   response.setDataItem("msg", "");
   return response;
}

void UpgradeUpgradeMgrSlaveWrapper::connectToServerHandler()
{
   m_context->response.setDataItem("msg", QString("连接服务器成功 [%1:%2]").arg(m_context->targetServerAddress).arg(UMS_LISTEN_PORT));
   writeInterResponse(m_context->request, m_context->response);
   m_context->response.setDataItem("msg", "向upgrademgr_slave服务器发送升级请求");
   writeInterResponse(m_context->request, m_context->response);
   ServiceInvokeRequest serviceRequest("Upgrader/UpgradeUpgradeMgrSlave", "upgrade", {{"targetVersion", m_context->targetVersion}});
   m_context->serviceInvoker->request(serviceRequest, upgrade_upgrademgr_slave_handler, static_cast<void*>(this));
}

void UpgradeUpgradeMgrSlaveWrapper::connectToServerErrorHandler()
{
   m_context->response.setStatus(false);
   m_context->response.setError({-1, QString("连接服务器失败 [%1:%2]").arg(m_context->targetServerAddress).arg(UMS_LISTEN_PORT)});
   writeInterResponse(m_context->request, m_context->response);
   clearState();
   m_eventLoop.exit(0);
}

void UpgradeUpgradeMgrSlaveWrapper::clearState()
{
   m_isInAction= false;
   m_context.clear();
   m_eventLoop.exit(0);
   if(!m_serviceInvoker.isNull()){
      disconnect(m_serviceInvoker.data(), &ServiceInvoker::connectedToServerSignal, this, &UpgradeUpgradeMgrSlaveWrapper::connectToServerHandler);
      disconnect(m_serviceInvoker.data(), &ServiceInvoker::connectErrorSignal, this, &UpgradeUpgradeMgrSlaveWrapper::connectToServerErrorHandler);
      m_serviceInvoker->resetStatus();
   }
}


}//upgrader
}//ummservice