#include "initializers/initializer_cleanup_funcs.h"

namespace upgrademgr{
namespace master{

void global_initializer()
{
   init_api_provider();
}

void global_cleanup()
{
   cleanup_api_provider();   
}

}//master
}//upgrademgr
