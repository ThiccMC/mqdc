#ifndef PLIBSYS_HEADER_PLIBSYSCONFIG_H
#define PLIBSYS_HEADER_PLIBSYSCONFIG_H

#define PLIBSYS_VERSION_MAJOR 0
#define PLIBSYS_VERSION_MINOR 0
#define PLIBSYS_VERSION_PATCH 4
#define PLIBSYS_VERSION_STR "0.0.4"
#define PLIBSYS_VERSION 0x000004

/* #undef PLIBSYS_NEED_WINDOWS_H */
#define PLIBSYS_NEED_FLOAT_H
#define PLIBSYS_NEED_LIMITS_H
/* #undef PLIBSYS_NEED_VALUES_H */
/* #undef PLIBSYS_NEED_PTHREAD_NP_H */
/* #undef PLIBSYS_IS_BIGENDIAN */
#define PLIBSYS_SIZEOF_SAFAMILY_T 2
#define PLIBSYS_VA_COPY va_copy

#define PLIBSYS_NTDDI_VERSION_FROM_WIN32_WINNT2(ver)    ver##0000
#define PLIBSYS_NTDDI_VERSION_FROM_WIN32_WINNT(ver)     PLIBSYS_NTDDI_VERSION_FROM_WIN32_WINNT2(ver)

// #ifdef PLIBSYS_NEED_WINDOWS_H
// #  ifndef _WIN32_WINNT
// #    define _WIN32_WINNT 0x501
// #  endif
// #  ifndef NTDDI_VERSION
// #    define NTDDI_VERSION PLIBSYS_NTDDI_VERSION_FROM_WIN32_WINNT(_WIN32_WINNT)
// #  endif
// #  include <winsock2.h>
// #  include <ws2tcpip.h>
// #  include <windows.h>
// #endif

#include <pmacros.h>

#ifdef PLIBSYS_NEED_FLOAT_H
#  include <float.h>
#endif

#ifdef PLIBSYS_NEED_LIMITS_H
#  include <limits.h>
#endif

#ifdef PLIBSYS_NEED_VALUES_H
#  include <values.h>
#endif

P_BEGIN_DECLS

#define P_MINFLOAT    FLT_MIN
#define P_MAXFLOAT    FLT_MAX
#define P_MINDOUBLE   DBL_MIN
#define P_MAXDOUBLE   DBL_MAX
#define P_MINSHORT    SHRT_MIN
#define P_MAXSHORT    SHRT_MAX
#define P_MAXUSHORT   USHRT_MAX
#define P_MININT      INT_MIN
#define P_MAXINT      INT_MAX
#define P_MAXUINT     UINT_MAX
#define P_MINLONG     LONG_MIN
#define P_MAXLONG     LONG_MAX
#define P_MAXULONG    ULONG_MAX

#define PLIBSYS_SIZEOF_VOID_P 8
#define PLIBSYS_SIZEOF_SIZE_T 8
#define PLIBSYS_SIZEOF_LONG 8

#ifdef PLIBSYS_IS_BIGENDIAN
#  define P_BYTE_ORDER P_BIG_ENDIAN
#else
#  define P_BYTE_ORDER P_LITTLE_ENDIAN
#endif

P_END_DECLS

#endif /* PLIBSYS_HEADER_PLIBSYSCONFIG_H */
