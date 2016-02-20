#include <QTimer>
#include <csignal>

#include "corelib/io/terminal.h"
#include "corelib/kernel/errorinfo.h"

#include "application.h"
#include "command_runner.h"

using UpgrademgrMasterApplication = upgrademgr::master::Application;
using CommandRunner = upgrademgr::master::CommandRunner;
using ErrorInfo = sn::corelib::ErrorInfo;
using Terminal = sn::corelib::Terminal;
using TerminalColor = sn::corelib::TerminalColor;
//全局更新函数
namespace upgrademgr{
namespace master{
void global_initializer();
void global_cleanup();
}//master
}//upgrademgr

int main(int argc, char *argv[])
{
   try{
      UpgrademgrMasterApplication app(argc, argv);
      qAddPreRoutine(upgrademgr::master::global_initializer);
      qAddPostRoutine(upgrademgr::master::global_cleanup);
      app.ensureImportantDir();
      app.watchUnixSignal(SIGINT, true);
      CommandRunner cmdrunner(app);
      QTimer::singleShot(0, Qt::PreciseTimer, [&cmdrunner]{
         cmdrunner.run();
      });
      return app.exec();
   }catch(const ErrorInfo& errorInfo){
      QString str(errorInfo.toString());
      if(str.size() > 0){
         str += '\n';
         Terminal::writeText(str.toLocal8Bit(), TerminalColor::Red);
      }
      return EXIT_FAILURE;
   }
}