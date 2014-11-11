//
//  AnimaEngineWS.h
//  Anima
//
//  Created by Marco Zille on 11/11/14.
//
//

#ifndef __Anima__AnimaEngineWS__
#define __Anima__AnimaEngineWS__

#if defined ANIMA_ENGINE_WS_EXPORT_ENABLED
#	if _MSC_VER
#		define ANIMA_ENGINE_WS_EXPORT __declspec(dllexport)
#	else
#		define ANIMA_ENGINE_WS_EXPORT __attribute__ ((visibility ("default")))
#	endif
#else
#	if _MSC_VER
#		define ANIMA_ENGINE_WS_EXPORT __declspec(dllimport)
#	else
#		define ANIMA_ENGINE_WS_EXPORT
#	endif
#endif

#define ANIMA_ENGINE_WS_NAMESPACE Anima
#define BEGIN_ANIMA_ENGINE_WS_NAMESPACE namespace Anima {
#define END_ANIMA_ENGINE_WS_NAMESPACE }

#endif