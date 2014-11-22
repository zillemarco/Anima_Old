#ifndef _ANIMA_ENGINE_LIB_H
#define _ANIMA_ENGINE_LIB_H

#if defined ANIMA_ENGINE_CORE_EXPORT_ENABLED
#	if _MSC_VER
#		define ANIMA_ENGINE_CORE_EXPORT __declspec(dllexport)
#	else
#		define ANIMA_ENGINE_CORE_EXPORT __attribute__ ((visibility ("default")))
#	endif
#else
#	if _MSC_VER
#		define ANIMA_ENGINE_CORE_EXPORT __declspec(dllimport)
#	else
#		define ANIMA_ENGINE_CORE_EXPORT
#	endif
#endif

#define ANIMA_ENGINE_CORE_NAMESPACE Anima
#define BEGIN_ANIMA_ENGINE_CORE_NAMESPACE namespace Anima {
#define END_ANIMA_ENGINE_CORE_NAMESPACE }

extern "C" ANIMA_ENGINE_CORE_EXPORT bool __anima_get_working_dir(char* dest, int length);
#define ANIMA_WORKING_DIR(dest, len)	__anima_get_working_dir(dest, len)

#if defined _MSC_VER
#	define ANIMA_ENGINE_CORE_ALIGN_OF(a)	__alignof(a)
#else
#	define ANIMA_ENGINE_CORE_ALIGN_OF(a)	__alignof__(a)
#endif

#define ANIMA_ALLOCATOR_DEBUG

#if defined _MSC_VER
#	define _ANIMA_ENGINE_CORE_WIN32
#	define _ANIMA_ENGINE_CORE_WGL
#else
#	define _ANIMA_ENGINE_CORE_COCOA
#	define _ANIMA_ENGINE_CORE_NSGL
#	define _ANIMA_ENGINE_CORE_USE_MENUBAR
#	define _ANIMA_ENGINE_CORE_USE_RETINA
#endif

#define _ANIMA_ENGINE_CORE_USE_OPENGL

#define _ANIMA_ENGINE_CORE_VERSION_MAJOR          0
#define _ANIMA_ENGINE_CORE_VERSION_MINOR          0
#define _ANIMA_ENGINE_CORE_VERSION_REVISION       0
#define _ANIMA_ENGINE_CORE_VERSION_NUMBER "0.0.0"

#include <vector>

#endif //_ANIMA_ENGINE_LIB_H