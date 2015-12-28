#include <QTimer>

#include "corelib/io/terminal.h"
#include "corelib/kernel/errorinfo.h"

#include "application.h"
#include "command_runner.h"

using CloudControllerApplication = upgrademgr::master::Application;
using CommandRunner = upgrademgr::master::CommandRunner;
using ErrorInfo = sn::corelib::ErrorInfo;
using Terminal = sn::corelib::Terminal;
using TerminalColor = sn::corelib::TerminalColor;
//全局更新函数
namespace upgrademgr{
namespace master{
void global_initializer();
}//master
}//upgrademgr

int main(int argc, char *argv[])
{
   try{
      qAddPreRoutine(upgrademgr::master::global_initializer);
      CloudControllerApplication app(argc, argv);
      app.ensureImportantDir();
      CommandRunner cmdrunner(app);
      QTimer::singleShot(0, Qt::PreciseTimer, [&cmdrunner]{
         cmdrunner.run();
      });
      return app.exec();
   }catch(const ErrorInfo& errorInfo){
      QString str(errorInfo.toString());
      if(str.size() > 0){
         str += '\n';
         Terminal::writeText(str.toLatin1(), TerminalColor::Red);
      }
      return EXIT_FAILURE;
   }
}