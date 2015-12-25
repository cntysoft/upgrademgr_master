#ifndef UPGRADEMGR_MASTER_APPLICATION_H
#define UPGRADEMGR_MASTER_APPLICATION_H

#include "corelib/kernel/application.h"
#include "corelib/kernel/settings.h"

namespace upgrademgr{
namespace master{

using BaseApplication = sn::corelib::Application;
using sn::corelib::Settings;

class Application : public BaseApplication
{
public:
   Application(int &argc, char **argv);
public:
   virtual Settings::CfgInitializerFnType getDefaultCfgInitializerFn();
   virtual ~Application();
};

}//master
}//upgrademgr

#endif // UPGRADEMGR_MASTER_APPLICATION_H
