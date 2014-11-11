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

#define ANIMA_ALLOCATOR_DEBUG

extern "C" ANIMA_ENGINE_CORE_EXPORT bool __anima_get_working_dir(char* dest, int length);
#define ANIMA_WORKING_DIR(dest, len)	__anima_get_working_dir(dest, len)

#endif //_ANIMA_ENGINE_LIB_H