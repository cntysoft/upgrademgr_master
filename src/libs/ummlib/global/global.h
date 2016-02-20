#ifndef UMM_LIB_GLOBAL_GLOBAL_H
#define UMM_LIB_GLOBAL_GLOBAL_H

#include "corelib/global/global.h"

#ifdef UMM_STATIC_LIB
   #define UMM_LIB_EXPORT 
#else
   #ifdef UMM_LIBRARY
      #define UMM_LIB_EXPORT Q_DECL_EXPORT
   #else
      #define UMM_LIB_EXPORT Q_DECL_IMPORT
   #endif
#endif

#define UPGRADEMGR_SLAVE_LISTEN_PORT 7770

#endif // UMM_LIB_GLOBAL_GLOBAL_H
