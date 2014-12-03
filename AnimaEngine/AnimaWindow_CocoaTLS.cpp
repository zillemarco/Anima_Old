#include "AnimaWindow.h"
#include "AnimaEngine.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

bool _AnimaEngineWindowInitTLS(void)
{
	_AnimaEngineWindowtlsNS* tlsLibraryState = AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_TLS_STATE;
	
	if (pthread_key_create(&tlsLibraryState->_context, NULL) != 0)
	{
//		_glfwInputError(GLFW_PLATFORM_ERROR, "POSIX: Failed to create context TLS");
		return false;
	}
	
	return true;
}

void _AnimaEngineWindowTerminateTLS(void)
{
	pthread_key_delete(AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_TLS_STATE->_context);
}

void _AnimaEngineWindowSetCurrentContext(AnimaEngineWindow_Base* context)
{
	pthread_setspecific(AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_TLS_STATE->_context, context);
}

AnimaEngineWindow_Base* _AnimaEngineWindowPlatformGetCurrentContext(void)
{
	return (AnimaEngineWindow_Base*)pthread_getspecific(AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_TLS_STATE->_context);
}

END_ANIMA_ENGINE_NAMESPACE