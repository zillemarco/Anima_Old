#include "AnimaWindow_Base.h"
#include "AnimaEngine.h"

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

bool _AnimaEngineWindowInitTLS(void)
{
	AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TLS_STATE->_context = TlsAlloc();
	if (AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TLS_STATE->_context == TLS_OUT_OF_INDEXES)
	{
		//_glfwInputError(GLFW_PLATFORM_ERROR, "Win32: Failed to allocate TLS index");
		return false;
	}

	AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TLS_STATE->_allocated = true;
	return true;
}

void _AnimaEngineWindowTerminateTLS(void)
{
	if (AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TLS_STATE->_allocated)
		TlsFree(AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TLS_STATE->_context);
}

void _AnimaEngineWindowSetCurrentContext(AnimaEngineWindow_Base* context)
{
	TlsSetValue(AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TLS_STATE->_context, context);
}

AnimaEngineWindow_Base* _AnimaEngineWindowPlatformGetCurrentContext(void)
{
	return (AnimaEngineWindow_Base*)TlsGetValue(AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TLS_STATE->_context);
}

END_ANIMA_ENGINE_CORE_NAMESPACE