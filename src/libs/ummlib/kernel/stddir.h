#ifndef UPGRADEMGR_MASTER_LIB_KERNEL_STDDIR_H
#define UPGRADEMGR_MASTER_LIB_KERNEL_STDDIR_H

#include <QString>

#include "ummlib/global/global.h"

#include "corelib/kernel/stddir.h"
#include "corelib/kernel/application.h"

namespace ummlib{
namespace network{

using BaseStdDir = sn::corelib::StdDir;
using sn::corelib::Application;

class UM_MASTER_LIB_EXPORT StdDir : public BaseStdDir
{
public:
   static QString getBaseDataDir();
};

}//network
}//ummlib

#endif // UPGRADEMGR_MASTER_LIB_KERNEL_STDDIR_H
