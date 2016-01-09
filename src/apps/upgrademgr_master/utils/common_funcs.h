#ifndef UPGRADEMGR_MASTER_UTILS_COMMON_FUNCS_H
#define UPGRADEMGR_MASTER_UTILS_COMMON_FUNCS_H

#include <QLatin1String>

#include "application.h"

namespace upgrademgr{
namespace master{

using UpgrademgrMasterApplication = upgrademgr::master::Application;

UpgrademgrMasterApplication* get_app_ref();

QLatin1String get_upgrademgr_master_version();

QString get_application_filepath();

}//master
}//upgrademgr

#endif // UPGRADEMGR_MASTER_UTILS_COMMON_FUNCS_H
