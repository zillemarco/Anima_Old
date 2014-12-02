#ifndef _AnimaWindow_WindowsJoystick_h
#define _AnimaWindow_WindowsJoystick_h

#include "AnimaEngineCore.h"

#include <IOKit/IOKitLib.h>
#include <IOKit/IOCFPlugIn.h>
#include <IOKit/hid/IOHIDLib.h>
#include <IOKit/hid/IOHIDKeys.h>

#define _ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_JOYSTICK_STATE \
	_AnimaEngineWindowjoystickIOKit iokit_js[ANIMA_ENGINE_CORE_JOYSTICK_LAST + 1]

#define _GETD_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_JOYSTICK_STATE	_AnimaEngineWindowjoystickIOKit* GetPlatformLibraryJoystickState() { return iokit_js; }
#define _GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_JOYSTICK_STATE	GetPlatformLibraryJoystickState()

#define _INIT_STATIC_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_JOYSTICK_STATE(class) _AnimaEngineWindowjoystickIOKit class::iokit_js[];

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

typedef struct _AnimaEngineWindowjoystickIOKit
{
	bool _present;
	char _name[256];
	
	IOHIDDeviceInterface** _interface;
	
	CFMutableArrayRef _axisElements;
	CFMutableArrayRef _buttonElements;
	CFMutableArrayRef _hatElements;
	
	float*          _axes;
	unsigned char*  _buttons;
	
} _AnimaEngineWindowjoystickIOKit;


void _AnimaEngineWindowInitJoysticks(void);
void _AnimaEngineWindowTerminateJoysticks(void);

END_ANIMA_ENGINE_CORE_NAMESPACE

#endif