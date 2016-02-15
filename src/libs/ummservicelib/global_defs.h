#ifndef UMM_SERVICE_GLOBAL_DEFS_H
#define UMM_SERVICE_GLOBAL_DEFS_H

#include <qglobal.h>

#ifdef UMM_SERVICE_STATIC_LIB
   #define UMM_SERVICE_EXPORT 
#else
   #ifdef UMM_SERVICE_LIBRARY
      #define UMM_SERVICE_EXPORT Q_DECL_EXPORT
   #else
      #define UMM_SERVICE_EXPORT Q_DECL_IMPORT
   #endif
#endif

#include "macros.h"

#endif // UMM_SERVICE_GLOBAL_DEFS_H
