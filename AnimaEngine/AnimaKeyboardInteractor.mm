//
//  AnimaKeyboardInteractor.mm
//  Anima
//
//  Created by Marco Zille on 03/11/15.
//
//

#include "AnimaKeyboardInteractor.h"
#include "AnimaVertex.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaKeyboardInteractor::AnimaKeyboardInteractor()
{
	_windowId = 0;
	_engine = nullptr;
	_installed = false;
	
	_keyDownMonitor = nullptr;
	_keyUpMonitor = nullptr;
	
	_supportedEvents = {
		{"onKeyDown",	""},
		{"onKeyUp",		""}
	};
}

AnimaKeyboardInteractor::AnimaKeyboardInteractor(const AnimaKeyboardInteractor& src)
{
	_windowId = src._windowId;
	_engine = src._engine;
	_installed = src._installed;
	
	_keyDownMonitor = src._keyDownMonitor;
	_keyUpMonitor = src._keyUpMonitor;
}

AnimaKeyboardInteractor::AnimaKeyboardInteractor(AnimaKeyboardInteractor&& src)
{
	_windowId = src._windowId;
	_engine = src._engine;
	_installed = src._installed;
	
	_keyDownMonitor = src._keyDownMonitor;
	_keyUpMonitor = src._keyUpMonitor;
}

AnimaKeyboardInteractor::~AnimaKeyboardInteractor()
{
	Remove();
}

AnimaKeyboardInteractor& AnimaKeyboardInteractor::operator=(const AnimaKeyboardInteractor& src)
{
	if(this != &src)
	{
		_windowId = src._windowId;
		_engine = src._engine;
		_installed = src._installed;
		
		_keyDownMonitor = src._keyDownMonitor;
		_keyUpMonitor = src._keyUpMonitor;
	}
	
	return *this;
}

AnimaKeyboardInteractor& AnimaKeyboardInteractor::operator=(AnimaKeyboardInteractor&& src)
{
	if(this != &src)
	{
		_windowId = src._windowId;
		_engine = src._engine;
		_installed = src._installed;
		
		_keyDownMonitor = src._keyDownMonitor;
		_keyUpMonitor = src._keyUpMonitor;
	}
	
	return *this;
}

bool AnimaKeyboardInteractor::Install(long windowId, AnimaEngine* engine)
{
	if(_installed)
		return true;
	
	_windowId = windowId;
	_engine = engine;
	
	_keyDownMonitor = [NSEvent addLocalMonitorForEventsMatchingMask:NSKeyDownMask handler:^NSEvent* (NSEvent* event) {
		KeyDownCallback(this, event);
		return event;
	}];
	
	_keyUpMonitor = [NSEvent addLocalMonitorForEventsMatchingMask:NSKeyUpMask handler:^NSEvent* (NSEvent* event) {
		KeyUpCallback(this, event);
		return event;
	}];
	
	_installed = true;
	return true;
}

bool AnimaKeyboardInteractor::Remove()
{
	if(_installed == false)
		return false;
	
	[NSEvent removeMonitor:_keyDownMonitor];
	[NSEvent removeMonitor:_keyUpMonitor];
	
	_windowId = 0;
	_keyDownMonitor = nullptr;
	_keyUpMonitor = nullptr;
	
	_installed = false;
	return true;
}

bool AnimaKeyboardInteractor::SetEventHandler(const AnimaString& eventName, std::function<void (AnimaEventArgs* eventArgs)> handler)
{
	if(EventIsSupported(eventName))
	{
		_eventsMap[eventName] = handler;
		return true;
	}
	return false;
}

bool AnimaKeyboardInteractor::LaunchEvent(const AnimaString& eventName, AnimaEventArgs* eventArgs) const
{
	auto func = _eventsMap.find(eventName);
	if(func != _eventsMap.end() && func->second != nullptr)
	{
		func->second(eventArgs);
		return true;
	}
	
	return false;
}

AnimaArray<AnimaEventInfo> AnimaKeyboardInteractor::GetSupportedEvents() const
{
	return _supportedEvents;
}

bool AnimaKeyboardInteractor::EventIsSupported(const AnimaString& eventName) const
{
	for(auto item : _supportedEvents)
	{
		if(item._eventName == eventName)
			return true;
	}
	
	return false;
}

void AnimaKeyboardInteractor::KeyDownCallback(AnimaKeyboardInteractor* interactor, NSEvent* event)
{
	AnimaKeyboardKey key = TranslateKey([event keyCode]);
	AInt modifiers = TranslateFlags([event modifierFlags]);
	
	AnimaEventArgs* args = new AnimaKeyboardEventArgs(interactor, key, modifiers);
	interactor->LaunchEvent("onKeyDown", args);
	
	delete args;
	args = nullptr;
}

void AnimaKeyboardInteractor::KeyUpCallback(AnimaKeyboardInteractor* interactor, NSEvent* event)
{
	AnimaKeyboardKey key = TranslateKey([event keyCode]);
	AInt modifiers = TranslateFlags([event modifierFlags]);
	
	AnimaEventArgs* args = new AnimaKeyboardEventArgs(interactor, key, modifiers);
	interactor->LaunchEvent("onKeyUp", args);
	
	delete args;
	args = nullptr;
}

AInt TranslateFlags(NSUInteger flags)
{
	AInt mods = 0;
	
	if (flags & NSShiftKeyMask)
		mods |= (AInt)AnimaKeyboardModifier::AKM_SHIFT;
	if (flags & NSControlKeyMask)
		mods |= (AInt)AnimaKeyboardModifier::AKM_CTRL;
	if (flags & NSAlternateKeyMask)
		mods |= (AInt)AnimaKeyboardModifier::AKM_ALT;
	if (flags & NSCommandKeyMask)
		mods |= (AInt)AnimaKeyboardModifier::AKM_SUPER;
	
	return mods;
}

AnimaKeyboardKey TranslateKey(AUint key)
{
	// Keyboard symbol translation table
	static const AnimaKeyboardKey table[128] =
	{
		/* 00 */ AnimaKeyboardKey::AKK_A,
		/* 01 */ AnimaKeyboardKey::AKK_S,
		/* 02 */ AnimaKeyboardKey::AKK_D,
		/* 03 */ AnimaKeyboardKey::AKK_F,
		/* 04 */ AnimaKeyboardKey::AKK_H,
		/* 05 */ AnimaKeyboardKey::AKK_G,
		/* 06 */ AnimaKeyboardKey::AKK_Z,
		/* 07 */ AnimaKeyboardKey::AKK_X,
		/* 08 */ AnimaKeyboardKey::AKK_C,
		/* 09 */ AnimaKeyboardKey::AKK_V,
		/* 0a */ AnimaKeyboardKey::AKK_WORLD_1,
		/* 0b */ AnimaKeyboardKey::AKK_B,
		/* 0c */ AnimaKeyboardKey::AKK_Q,
		/* 0d */ AnimaKeyboardKey::AKK_W,
		/* 0e */ AnimaKeyboardKey::AKK_E,
		/* 0f */ AnimaKeyboardKey::AKK_R,
		/* 10 */ AnimaKeyboardKey::AKK_Y,
		/* 11 */ AnimaKeyboardKey::AKK_T,
		/* 12 */ AnimaKeyboardKey::AKK_1,
		/* 13 */ AnimaKeyboardKey::AKK_2,
		/* 14 */ AnimaKeyboardKey::AKK_3,
		/* 15 */ AnimaKeyboardKey::AKK_4,
		/* 16 */ AnimaKeyboardKey::AKK_6,
		/* 17 */ AnimaKeyboardKey::AKK_5,
		/* 18 */ AnimaKeyboardKey::AKK_EQUAL,
		/* 19 */ AnimaKeyboardKey::AKK_9,
		/* 1a */ AnimaKeyboardKey::AKK_7,
		/* 1b */ AnimaKeyboardKey::AKK_MINUS,
		/* 1c */ AnimaKeyboardKey::AKK_8,
		/* 1d */ AnimaKeyboardKey::AKK_0,
		/* 1e */ AnimaKeyboardKey::AKK_RIGHT_BRACKET,
		/* 1f */ AnimaKeyboardKey::AKK_O,
		/* 20 */ AnimaKeyboardKey::AKK_U,
		/* 21 */ AnimaKeyboardKey::AKK_LEFT_BRACKET,
		/* 22 */ AnimaKeyboardKey::AKK_I,
		/* 23 */ AnimaKeyboardKey::AKK_P,
		/* 24 */ AnimaKeyboardKey::AKK_ENTER,
		/* 25 */ AnimaKeyboardKey::AKK_L,
		/* 26 */ AnimaKeyboardKey::AKK_J,
		/* 27 */ AnimaKeyboardKey::AKK_APOSTROPHE,
		/* 28 */ AnimaKeyboardKey::AKK_K,
		/* 29 */ AnimaKeyboardKey::AKK_SEMICOLON,
		/* 2a */ AnimaKeyboardKey::AKK_BACKSLASH,
		/* 2b */ AnimaKeyboardKey::AKK_COMMA,
		/* 2c */ AnimaKeyboardKey::AKK_SLASH,
		/* 2d */ AnimaKeyboardKey::AKK_N,
		/* 2e */ AnimaKeyboardKey::AKK_M,
		/* 2f */ AnimaKeyboardKey::AKK_PERIOD,
		/* 30 */ AnimaKeyboardKey::AKK_TAB,
		/* 31 */ AnimaKeyboardKey::AKK_SPACE,
		/* 32 */ AnimaKeyboardKey::AKK_GRAVE_ACCENT,
		/* 33 */ AnimaKeyboardKey::AKK_BACKSPACE,
		/* 34 */ AnimaKeyboardKey::AKK_UNKNOWN,
		/* 35 */ AnimaKeyboardKey::AKK_ESCAPE,
		/* 36 */ AnimaKeyboardKey::AKK_RIGHT_SUPER,
		/* 37 */ AnimaKeyboardKey::AKK_LEFT_SUPER,
		/* 38 */ AnimaKeyboardKey::AKK_LEFT_SHIFT,
		/* 39 */ AnimaKeyboardKey::AKK_CAPS_LOCK,
		/* 3a */ AnimaKeyboardKey::AKK_LEFT_ALT,
		/* 3b */ AnimaKeyboardKey::AKK_LEFT_CONTROL,
		/* 3c */ AnimaKeyboardKey::AKK_RIGHT_SHIFT,
		/* 3d */ AnimaKeyboardKey::AKK_RIGHT_ALT,
		/* 3e */ AnimaKeyboardKey::AKK_RIGHT_CONTROL,
		/* 3f */ AnimaKeyboardKey::AKK_UNKNOWN,		/* Function */
		/* 40 */ AnimaKeyboardKey::AKK_F17,
		/* 41 */ AnimaKeyboardKey::AKK_KP_DECIMAL,
		/* 42 */ AnimaKeyboardKey::AKK_UNKNOWN,
		/* 43 */ AnimaKeyboardKey::AKK_KP_MULTIPLY,
		/* 44 */ AnimaKeyboardKey::AKK_UNKNOWN,
		/* 45 */ AnimaKeyboardKey::AKK_KP_ADD,
		/* 46 */ AnimaKeyboardKey::AKK_UNKNOWN,
		/* 47 */ AnimaKeyboardKey::AKK_NUM_LOCK,		/* Really KeypadClear... */
		/* 48 */ AnimaKeyboardKey::AKK_UNKNOWN,		/* VolumeUp */
		/* 49 */ AnimaKeyboardKey::AKK_UNKNOWN,		/* VolumeDown */
		/* 4a */ AnimaKeyboardKey::AKK_UNKNOWN,		/* Mute */
		/* 4b */ AnimaKeyboardKey::AKK_KP_DIVIDE,
		/* 4c */ AnimaKeyboardKey::AKK_KP_ENTER,
		/* 4d */ AnimaKeyboardKey::AKK_UNKNOWN,
		/* 4e */ AnimaKeyboardKey::AKK_KP_SUBTRACT,
		/* 4f */ AnimaKeyboardKey::AKK_F18,
		/* 50 */ AnimaKeyboardKey::AKK_F19,
		/* 51 */ AnimaKeyboardKey::AKK_KP_EQUAL,
		/* 52 */ AnimaKeyboardKey::AKK_KP_0,
		/* 53 */ AnimaKeyboardKey::AKK_KP_1,
		/* 54 */ AnimaKeyboardKey::AKK_KP_2,
		/* 55 */ AnimaKeyboardKey::AKK_KP_3,
		/* 56 */ AnimaKeyboardKey::AKK_KP_4,
		/* 57 */ AnimaKeyboardKey::AKK_KP_5,
		/* 58 */ AnimaKeyboardKey::AKK_KP_6,
		/* 59 */ AnimaKeyboardKey::AKK_KP_7,
		/* 5a */ AnimaKeyboardKey::AKK_F20,
		/* 5b */ AnimaKeyboardKey::AKK_KP_8,
		/* 5c */ AnimaKeyboardKey::AKK_KP_9,
		/* 5d */ AnimaKeyboardKey::AKK_UNKNOWN,
		/* 5e */ AnimaKeyboardKey::AKK_UNKNOWN,
		/* 5f */ AnimaKeyboardKey::AKK_UNKNOWN,
		/* 60 */ AnimaKeyboardKey::AKK_F5,
		/* 61 */ AnimaKeyboardKey::AKK_F6,
		/* 62 */ AnimaKeyboardKey::AKK_F7,
		/* 63 */ AnimaKeyboardKey::AKK_F3,
		/* 64 */ AnimaKeyboardKey::AKK_F8,
		/* 65 */ AnimaKeyboardKey::AKK_F9,
		/* 66 */ AnimaKeyboardKey::AKK_UNKNOWN,
		/* 67 */ AnimaKeyboardKey::AKK_F11,
		/* 68 */ AnimaKeyboardKey::AKK_UNKNOWN,
		/* 69 */ AnimaKeyboardKey::AKK_F13,
		/* 6a */ AnimaKeyboardKey::AKK_F16,
		/* 6b */ AnimaKeyboardKey::AKK_F14,
		/* 6c */ AnimaKeyboardKey::AKK_UNKNOWN,
		/* 6d */ AnimaKeyboardKey::AKK_F10,
		/* 6e */ AnimaKeyboardKey::AKK_UNKNOWN,
		/* 6f */ AnimaKeyboardKey::AKK_F12,
		/* 70 */ AnimaKeyboardKey::AKK_UNKNOWN,
		/* 71 */ AnimaKeyboardKey::AKK_F15,
		/* 72 */ AnimaKeyboardKey::AKK_INSERT,		/* Really Help... */
		/* 73 */ AnimaKeyboardKey::AKK_HOME,
		/* 74 */ AnimaKeyboardKey::AKK_PAGE_UP,
		/* 75 */ AnimaKeyboardKey::AKK_DELETE,
		/* 76 */ AnimaKeyboardKey::AKK_F4,
		/* 77 */ AnimaKeyboardKey::AKK_END,
		/* 78 */ AnimaKeyboardKey::AKK_F2,
		/* 79 */ AnimaKeyboardKey::AKK_PAGE_DOWN,
		/* 7a */ AnimaKeyboardKey::AKK_F1,
		/* 7b */ AnimaKeyboardKey::AKK_LEFT,
		/* 7c */ AnimaKeyboardKey::AKK_RIGHT,
		/* 7d */ AnimaKeyboardKey::AKK_DOWN,
		/* 7e */ AnimaKeyboardKey::AKK_UP,
		/* 7f */ AnimaKeyboardKey::AKK_UNKNOWN,
	};
	
	if (key >= 128)
		return AnimaKeyboardKey::AKK_UNKNOWN;
	
	return table[key];
}

END_ANIMA_ENGINE_NAMESPACE