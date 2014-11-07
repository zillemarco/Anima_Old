#ifndef _ANIMA_ENGINE_LIB_H
#define _ANIMA_ENGINE_LIB_H

#if defined ANIMA_ENGINE_EXPORT_ENABLED
#	if _MSC_VER
#		define BEGIN_CLASS_EXPORT(className)						class __declspec(dllexport) className
#		define BEGIN_DERIVED_CLASS_EXPORT(className, baseClassName) class __declspec(dllexport) className : baseClassName
#	else
#		define BEGIN_CLASS_EXPORT(className)						extern "C" { class className
#		define BEGIN_DERIVED_CLASS_EXPORT(className, baseClassName) extern "C" { class className : baseClassName
#	endif
#else
#	if _MSC_VER
#		define BEGIN_CLASS_EXPORT(className)						class __declspec(dllimport) className
#		define BEGIN_DERIVED_CLASS_EXPORT(className, baseClassName) class __declspec(dllimport) className : baseClassName
#	else
#		define BEGIN_CLASS_EXPORT(className)						class className
#		define BEGIN_DERIVED_CLASS_EXPORT(className, baseClassName) class className : baseClassName
#	endif
#endif

#define BEGIN_NAMESPACE namespace Anima {
#define END_NAMESPACE }


#if defined ANIMA_ENGINE_EXPORT_ENABLED
#	if _MSC_VER
		__declspec(dllexport) void __init_anina_app__(int argc, char** argv);
		__declspec(dllexport) bool __anima_get_working_dir(char* dest, int length);
#	else
		extern "C" { void __init_anina_app__(int argc, char** argv); }
		extern "C" { bool __anima_get_working_dir(char* dest, int length); }
#	endif
#else
#	if _MSC_VER
		__declspec(dllimport) void __init_anina_app__(int argc, char** argv);
		__declspec(dllimport) bool __anima_get_working_dir(char* dest, int length);
#	else
		void __init_anina_app__(int argc, char** argv);
		bool __anima_get_working_dir(char* dest, int length);
#	endif
#endif

#define INIT_ANIMA_APP(argc, argv)		__init_anina_app__(argc, argv);
#define ANIMA_WORKING_DIR(dest, len)	__anima_get_working_dir(dest, len)

#endif //_ANIMA_ENGINE_LIB_H