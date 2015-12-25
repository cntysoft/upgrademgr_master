#include "common_funcs.h"

namespace upgrademgr{
namespace master{

UpgrademgrMasterApplication* get_app_ref()
{
   return static_cast<UpgrademgrMasterApplication*>(UpgrademgrMasterApplication::instance());
}

}//master
}//upgrademgr