#ifndef _ANIMA_ENGINE_LIB_H
#define _ANIMA_ENGINE_LIB_H

#if defined ANIMA_ENGINE_EXPORT_ENABLED

	#define BEGIN_CLASS_EXPORT(className) extern "C" {
	#define END_CLASS_EXPORT }

#else

	#define BEGIN_CLASS_EXPORT(className)
	#define END_CLASS_EXPORT

#endif

#define BEGIN_NAMESPACE namespace Anima {
#define END_NAMESPACE }

#endif //_ANIMA_ENGINE_LIB_H