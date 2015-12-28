#ifndef UPGRADEMGR_MASTER_API_REPO_INFO_H
#define UPGRADEMGR_MASTER_API_REPO_INFO_H

#include "api/macros.h"
#include "corelib/network/rpc/abstract_api.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "corelib/network/rpc/api_provider.h"

namespace upgrademgr{
namespace master{
namespace api{
namespace repo{

UMM_USING_API_NAMESPACES

class Info : public AbstractApi
{
public:
   Info(ApiProvider& provider);
   ApiInvokeResponse getVersion(const ApiInvokeRequest &request, ApiProvider &provider);
};

}//repo
}//api
}//master
}//upgrademgr

#endif // UPGRADEMGR_MASTER_API_REPO_INFO_H

