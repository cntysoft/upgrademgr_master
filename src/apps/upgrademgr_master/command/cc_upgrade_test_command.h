#ifndef UPGRADEMGR_MASTER_COMMAND_CC_UPGRADE_TEST_COMMAND_H
#define UPGRADEMGR_MASTER_COMMAND_CC_UPGRADE_TEST_COMMAND_H

#include <QSharedPointer>
#include "corelib/command/abstract_command.h"
#include "corelib/upgrade/upgradeenv.h"
#include "corelib/kernel/errorinfo.h"

namespace upgrademgr{
namespace master{
namespace command{

using sn::corelib::AbstractCommand;
using sn::corelib::AbstractCommandRunner;
using sn::corelib::CommandMeta;
using UpgradeEnvEngine = sn::corelib::upgrade::UpgradeEnv;
using sn::corelib::ErrorInfo;

class CcUpgradeTestCommand : public AbstractCommand
{
public:
   CcUpgradeTestCommand(AbstractCommandRunner& runner, const CommandMeta& invokeMeta);
public:
   virtual void exec();
protected:
   QSharedPointer<UpgradeEnvEngine> getUpgradeScriptEngine();
protected slots:
   void logMsgHandler(const QString &msg);
   void excpetionHandler(ErrorInfo errorInfo);
protected:
   QString m_upgradeDbName;
   QString m_upgradeDir;
   QString m_upgradeScript;
   QSharedPointer<UpgradeEnvEngine> m_upgradeScriptEngine;
};

}//command
}//master
}//upgrademgr

#endif // UPGRADEMGR_MASTER_COMMAND_CC_UPGRADE_TEST_COMMAND_H
