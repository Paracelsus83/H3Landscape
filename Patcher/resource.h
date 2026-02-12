//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ generated include file.
// Used by Patcher.rc
//
#ifdef __MINGW32__
#include <winver.h>
#else
#include <VerRsrc.h>
#endif

#define LANG_NEUTRAL                    0x00
#define SUBLANG_NEUTRAL                 0x00

#define IDI_ICON                        101

#ifndef H3L_PATCHER_FLAGS
#	if   defined(_DEBUG)
#		define H3L_PATCHER_FLAGS VS_FF_DEBUG
#	elif defined(H3L_RELEASE)
#		define H3L_PATCHER_FLAGS 0x0L
#	elif defined(H3L_PROD_VERSION)
#		define H3L_PATCHER_FLAGS VS_FF_PRERELEASE
#	else
#		define H3L_PATCHER_FLAGS VS_FF_PRIVATEBUILD
#	endif
#endif

#ifndef H3L_PROD_VERSION
#	define H3L_PROD_VERSION	0,0,5,0
#endif
#ifndef H3L_PATCHER_VERSION
#	define H3L_PATCHER_VERSION H3L_PROD_VERSION
#endif
#ifndef H3LANDSCAPE_VERSION
#	define H3LANDSCAPE_VERSION unknown-{H3L_PROD_VERSION}
#endif

#define stringify(v) #v
#define STR(v) stringify(v)

// Next default values for new objects
// 
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        102
#define _APS_NEXT_COMMAND_VALUE         40001
#define _APS_NEXT_CONTROL_VALUE         1000
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif
