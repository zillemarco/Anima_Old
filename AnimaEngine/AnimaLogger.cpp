//
//  AnimaLogger.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaLogger.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

#define COLOR_WHITE		"\033[22;0m"
#define COLOR_RED		"\033[22;32m"
#define COLOR_YELLOW	"\033[22;33m"
#define COLOR_GREEN		"\033[22;32m"
#define COLOR_BLUE		"\033[22;34m"

void AnimaLogger::LogMessageFormat(const char* message, ...)
{
	va_list ap;
	va_start(ap, message);
	AnimaString str = FormatString(message, ap);
	va_end(ap);

	LogMessage(str);
}

void AnimaLogger::LogMessageFormatColor(const AnimaLoggerTextColor& color, const AChar* message, ...)
{
	va_list ap;
	va_start(ap, message);
	AnimaString str = FormatString(message, ap);
	va_end(ap);

	LogMessage(str, color);
}

void AnimaLogger::LogMessage(const AnimaString& message, const AnimaLoggerTextColor& color)
{
	AnimaString colorStr = COLOR_WHITE;

	switch (color)
	{
	case ANIMA_LOGGER_TEXT_COLOR_RED:		colorStr = COLOR_RED;		break;
	case ANIMA_LOGGER_TEXT_COLOR_YELLOW:	colorStr = COLOR_YELLOW;	break;
	case ANIMA_LOGGER_TEXT_COLOR_GREEN:		colorStr = COLOR_GREEN;		break;
	case ANIMA_LOGGER_TEXT_COLOR_BLUE:		colorStr = COLOR_BLUE;		break;
	case ANIMA_LOGGER_TEXT_COLOR_WHITE:
	default:								colorStr = COLOR_WHITE;		break;
	}
	
	//printf("%s%s%s\n", colorStr.c_str(), message.c_str(), COLOR_WHITE);
	printf("%s\n", message.c_str());
}

void AnimaLogger::CheckGraphicErrors()
{
	AUint error = glGetError();
	if (error != GL_NO_ERROR)
	{
		switch (error)
		{
		case GL_INVALID_ENUM:
			LogMessage("OpenGL error: invalid enum");
			break;
		case GL_INVALID_VALUE:
			LogMessage("OpenGL error: invalid value");
			break;
		case GL_INVALID_OPERATION:
			LogMessage("OpenGL error: invalid operation");
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			LogMessage("OpenGL error: invalid framebuffer operation");
			break;
		case GL_OUT_OF_MEMORY:
			LogMessage("OpenGL error: out of memory");
			break;
		case GL_STACK_UNDERFLOW:
			LogMessage("OpenGL error: stack underflow");
			break;
		case GL_STACK_OVERFLOW:
			LogMessage("OpenGL error: stack overflow");
			break;
		default:
			LogMessage("OpenGL error: unknown error");
			break;
		}
	}
}

void AnimaLogger::InitGraphicsLogger()
{
	if (glDebugMessageControl != NULL)
	{
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		glDebugMessageCallback((GLDEBUGPROC)OpenGLErrorCallback, nullptr);
	}
}

void AnimaLogger::OpenGLErrorCallback(AUint source, AUint type, AUint id, AUint severity, AInt length, const AChar* message, const void* userParam)
{
//#if defined _DEBUG
//	if (severity != GL_DEBUG_SEVERITY_NOTIFICATION)
//	{
//		const AnimaLoggerTextColor textColor = OpenGLDebugSeverityColor(severity);
//		LogMessage("OpenGL Error:", textColor);
//		LogMessage("=============", textColor);
//		LogMessageFormatColor(textColor, " Object ID: %d", id);
//		LogMessageFormatColor(textColor, " Severity:  %s", OpenGLDebugSeverityString(severity));
//		LogMessageFormatColor(textColor, " Type:      %s", OpenGLDebugTypeString(type));
//		LogMessageFormatColor(textColor, " Source:    %s", OpenGLDebugSourceString(source));
//		LogMessageFormatColor(textColor, " Message:   %s", message);
//
////#ifdef _MSC_VER
////		if(IsDebuggerPresent()) __debugbreak();
////#else
////		asm("int $3");
////#endif
//	}
//#endif
}

const char* AnimaLogger::OpenGLDebugSourceString(AUint source)
{
	static const char* sources[] = { "API", "Window System", "Shader Compiler", "Third Party", "Application", "Other", "Unknown" };

	int str_idx = fmin(source - GL_DEBUG_SOURCE_API, sizeof(sources) / sizeof(const char *));

	return sources[str_idx];
}

const char* AnimaLogger::OpenGLDebugTypeString(AUint type)
{
	static const char* types[] = { "Error", "Deprecated Behavior", "Undefined Behavior", "Portability", "Performance", "Other", "Marker", "Push group", "Pop group" };

	int str_idx = fmin(type - GL_DEBUG_TYPE_ERROR, sizeof(types) / sizeof(const char *));

	return types[str_idx];
}

const char* AnimaLogger::OpenGLDebugSeverityString(AUint severity)
{
	static const char* severities[] = { "High", "Medium", "Low", "Unknown", "Notification", "Unknown" };

	int str_idx = fmin(severity - GL_DEBUG_SEVERITY_HIGH, sizeof(severities) / sizeof(const char *));

	return severities[str_idx];
}

AnimaLoggerTextColor AnimaLogger::OpenGLDebugSeverityColor(AUint severity)
{
	static AnimaLoggerTextColor severities[] = {
		ANIMA_LOGGER_TEXT_COLOR_RED,	// High (Red)
		ANIMA_LOGGER_TEXT_COLOR_YELLOW, // Med  (Yellow)
		ANIMA_LOGGER_TEXT_COLOR_GREEN,	// Low  (Green)
		ANIMA_LOGGER_TEXT_COLOR_WHITE	// ???  (White)
	};

	int col_idx = fmin(severity - GL_DEBUG_SEVERITY_HIGH, sizeof(severities) / sizeof(AnimaLoggerTextColor));

	return severities[col_idx];
}

END_ANIMA_ENGINE_NAMESPACE
