#ifndef _AnimaWindow_WindowsJoystick_h
#define _AnimaWindow_WindowsJoystick_h

#include "AnimaEngineCore.h"

#define _ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_JOYSTICK_STATE \
    _AnimaEngineWindowjoystickWinMM winmm_js[ANIMA_ENGINE_CORE_JOYSTICK_LAST + 1]

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

typedef struct _AnimaEngineWindowjoystickWinMM
{
	float           axes[6];
	unsigned char   buttons[36]; // 32 buttons plus one hat
	char*           name;
} _AnimaEngineWindowjoystickWinMM;


void _AnimaEngineWindowInitJoysticks(void);
void _AnimaEngineWindowTerminateJoysticks(void);

END_ANIMA_ENGINE_CORE_NAMESPACE

#endif