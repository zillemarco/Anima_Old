#ifndef _AnimaWindow_WindowsJoystick_h
#define _AnimaWindow_WindowsJoystick_h

#include "AnimaEngineCore.h"

#define _ANIMA_ENGINE_PLATFORM_LIBRARY_JOYSTICK_STATE \
    _AnimaEngineWindowjoystickWinMM winmm_js[ANIMA_ENGINE_JOYSTICK_LAST + 1]

#define _GETD_ANIMA_ENGINE_PLATFORM_LIBRARY_JOYSTICK_STATE	_AnimaEngineWindowjoystickWinMM* GetPlatformLibraryJoystickState() { return winmm_js; }
#define _GET_ANIMA_ENGINE_PLATFORM_LIBRARY_JOYSTICK_STATE	GetPlatformLibraryJoystickState()

#define _INIT_STATIC_ANIMA_ENGINE_PLATFORM_LIBRARY_JOYSTICK_STATE(class) _AnimaEngineWindowjoystickWinMM class::winmm_js[];

BEGIN_ANIMA_ENGINE_NAMESPACE

typedef struct _AnimaEngineWindowjoystickWinMM
{
	float           axes[6];
	unsigned char   buttons[36]; // 32 buttons plus one hat
	char*           name;
} _AnimaEngineWindowjoystickWinMM;


extern "C" ANIMA_ENGINE_EXPORT void _AnimaEngineWindowInitJoysticks(void);
extern "C" ANIMA_ENGINE_EXPORT void _AnimaEngineWindowTerminateJoysticks(void);

END_ANIMA_ENGINE_NAMESPACE

#endif