#ifndef UMM_LIB_KERNEL_STDDIR_H
#define UMM_LIB_KERNEL_STDDIR_H

#include <QString>

#include "ummlib/global/global.h"

#include "corelib/kernel/stddir.h"
#include "corelib/kernel/application.h"

namespace ummlib{
namespace kernel{

using BaseStdDir = sn::corelib::StdDir;
using sn::corelib::Application;

class UMM_LIB_EXPORT StdDir : public BaseStdDir
{
public:
   static QString getBaseDataDir();
   static QString getSoftwareRepoDir();
   static QString getMetaDir();
};

}//network
}//ummlib

#endif // UMM_LIB_KERNEL_STDDIR_H
