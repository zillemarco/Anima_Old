//
//  AnimaLogger.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaLogger__
#define __Anima__AnimaLogger__

#include "AnimaEngineCore.h"
#include "AnimaEngine.h"
#include "AnimaString.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

enum AnimaLoggerTextColor
{
	ANIMA_LOGGER_TEXT_COLOR_WHITE,
	ANIMA_LOGGER_TEXT_COLOR_RED,
	ANIMA_LOGGER_TEXT_COLOR_YELLOW,
	ANIMA_LOGGER_TEXT_COLOR_GREEN,
	ANIMA_LOGGER_TEXT_COLOR_BLUE
};

class ANIMA_ENGINE_EXPORT AnimaLogger
{
public:
	static void LogMessage(const AnimaString& message, const AnimaLoggerTextColor& color = ANIMA_LOGGER_TEXT_COLOR_WHITE);
	static void LogMessageFormat(const AChar* message, ...);
	static void LogMessageFormatColor(const AnimaLoggerTextColor& color, const AChar* message, ...);

	static void InitGraphicsLogger();
	static void CheckGraphicErrors();

protected:
	static const char* OpenGLDebugSourceString(AUint source);
	static const char* OpenGLDebugTypeString(AUint type);
	static const char* OpenGLDebugSeverityString(AUint severity);
	static AnimaLoggerTextColor OpenGLDebugSeverityColor(AUint severity);
	static void OpenGLErrorCallback(AUint source, AUint type, AUint id, AUint severity, AInt length, const AChar* message, const void* userParam);
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaLogger__) */
