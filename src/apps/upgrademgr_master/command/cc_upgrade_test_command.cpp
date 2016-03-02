#include <QJSEngine>
#include <QJSValue>

#include "cc_upgrade_test_command.h"
#include "corelib/kernel/settings.h"
#include "corelib/kernel/application.h"
#include "corelib/io/filesystem.h"
#include "ummlib/global/const.h"

#include <QDebug>

namespace upgrademgr{
namespace master{
namespace command{

using sn::corelib::Application;
using sn::corelib::Settings;
using sn::corelib::Filesystem;


CcUpgradeTestCommand::CcUpgradeTestCommand(AbstractCommandRunner &runner, const CommandMeta &invokeMeta)
   : AbstractCommand(runner, invokeMeta)
{
   Settings& settings = Application::instance()->getSettings();
   m_upgradeDir = settings.getValue("upgradeTestDir", UMM_CFG_GROUP_GLOBAL).toString();
   m_upgradeDbName = settings.getValue("upgradeTestDbName", UMM_CFG_GROUP_GLOBAL).toString();
}

void CcUpgradeTestCommand::exec()
{
   const CommandMeta::CmdArgType &args = m_invokeMeta.getCmdArgs();
   m_upgradeScript = args["script"];
   if(args.contains("upgradedb")){
      m_upgradeDbName = args.value("upgradedb");
   }
   if(args.contains("upgradedir")){
      m_upgradeDir = args.value("upgradedir");
   }
   if(!Filesystem::fileExist(m_upgradeScript)){
      throw ErrorInfo(QString("升级脚本 : %1 不存在").arg(m_upgradeScript));
   }
   printConsoleMsg("开始执行升级脚本\n");
   QSharedPointer<UpgradeEnvEngine> scriptEngine = getUpgradeScriptEngine();
   scriptEngine->exec(m_upgradeScript);
   exit(EXIT_SUCCESS);
}

QSharedPointer<UpgradeEnvEngine> CcUpgradeTestCommand::getUpgradeScriptEngine()
{
   if(m_upgradeScriptEngine.isNull()){
      Settings& settings = Application::instance()->getSettings();
      m_upgradeScriptEngine.reset(new UpgradeEnvEngine(settings.getValue("dbHost").toString(), settings.getValue("dbUser").toString(), 
                                                       settings.getValue("dbPassword").toString(), m_upgradeDbName));
      QJSEngine &engine = m_upgradeScriptEngine->getJsEngine();
      QJSValue env = engine.newObject();
      env.setProperty("deployDir", m_upgradeDir);
      env.setProperty("backupDir", "testenv/backupDir");
      env.setProperty("upgradeDir", "testenv/upgradeDir");
      m_upgradeScriptEngine->registerContextObject("UpgradeMeta", env, true);
      connect(m_upgradeScriptEngine.data(), &UpgradeEnvEngine::excpetionSignal, this, &CcUpgradeTestCommand::excpetionHandler);
      connect(m_upgradeScriptEngine.data(), &UpgradeEnvEngine::logMsgSignal, this, &CcUpgradeTestCommand::logMsgHandler);
   }
   return m_upgradeScriptEngine;
}

void CcUpgradeTestCommand::logMsgHandler(const QString &msg)
{
   printConsoleMsg(msg+'\n');
}

void CcUpgradeTestCommand::excpetionHandler(ErrorInfo errorInfo)
{
   printConsoleMsg(errorInfo.toString()+"\n");
}

}//command
}//master
}//upgrademgr