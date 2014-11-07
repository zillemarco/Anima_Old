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

#endif //_ANIMA_ENGINE_LIB_H