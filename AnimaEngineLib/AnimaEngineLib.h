#ifndef _ANIMA_ENGINE_LIB_H
#define _ANIMA_ENGINE_LIB_H

#if defined ANIMA_ENGINE_EXPORT_ENABLED
#	if _MSC_VER
#		define ANIMA_EXPORT __declspec(dllexport)
#	else
#		define ANIMA_EXPORT __attribute__ ((visibility ("default")))
#	endif
#else
#	if _MSC_VER
#		define ANIMA_EXPORT __declspec(dllimport)
#	else
#		define ANIMA_EXPORT
#	endif
#endif

#define BEGIN_NAMESPACE namespace Anima {
#define END_NAMESPACE }

#define ANIMA_ALLOCATOR_DEBUG

extern "C" ANIMA_EXPORT void __init_anina_app__(int argc, char** argv);
extern "C" ANIMA_EXPORT bool __anima_get_working_dir(char* dest, int length);

#define INIT_ANIMA_APP(argc, argv)		__init_anina_app__(argc, argv);
#define ANIMA_WORKING_DIR(dest, len)	__anima_get_working_dir(dest, len)

#endif //_ANIMA_ENGINE_LIB_H