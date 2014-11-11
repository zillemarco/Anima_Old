#ifndef _AnimaWindow_WindowsTLS_h
#define _AnimaWindow_WindowsTLS_h

#include "AnimaEngineCore.h"

#define _ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TLS_STATE _AnimaEngineWindowtlsWin32 win32_tls

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

typedef struct _AnimaEngineWindowtlsWin32
{
	bool	_allocated;
	DWORD	_context;

} _AnimaEngineWindowtlsWin32;

bool _AnimaEngineWindowInitTLS(void);
void _AnimaEngineWindowTerminateTLS(void);
void _AnimaEngineWindowSetCurrentContext(_AnimaEngineWindowwindow* context);

END_ANIMA_ENGINE_CORE_NAMESPACE

#endif