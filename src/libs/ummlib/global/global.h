#ifndef UPGRADEMGR_MASTER_LIB_GLOBAL_H
#define UPGRADEMGR_MASTER_LIB_GLOBAL_H

#include "corelib/global/global.h"

#ifdef UM_MASTER_STATIC_LIB
   #define UM_MASTER_LIB_EXPORT 
#else
   #ifdef UM_MASTER_LIBRARY
      #define UM_MASTER_LIB_EXPORT Q_DECL_EXPORT
   #else
      #define UM_MASTER_LIB_EXPORT Q_DECL_IMPORT
   #endif
#endif

#endif // UPGRADEMGR_MASTER_LIB_GLOBAL_H
