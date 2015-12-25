#ifndef UPGRADEMGR_MASTER_COMMAND_RUNNER_H
#define UPGRADEMGR_MASTER_COMMAND_RUNNER_H

#include "corelib/command/abstract_command_runner.h"

namespace upgrademgr{
namespace master{

using sn::corelib::AbstractCommandRunner;
class Application;

class CommandRunner : public AbstractCommandRunner
{
public:
   CommandRunner(Application &app);
private:
   void initCommandPool();
   void initRouteItems();
};

}//master
}//upgrademgr

#endif // UPGRADEMGR_MASTER_COMMAND_RUNNER_H
