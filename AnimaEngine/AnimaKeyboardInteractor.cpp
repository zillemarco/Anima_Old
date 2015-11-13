//
//  AnimaKeyboardInteractor.mm
//  Anima
//
//  Created by Marco Zille on 03/11/15.
//
//

#include "AnimaKeyboardInteractor.h"
#include "AnimaVertex.h"
#include "AnimaScene.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

boost::unordered_map<long, AnimaKeyboardInteractor*> AnimaKeyboardInteractor::_installedInteractors;

AnimaKeyboardInteractor::AnimaKeyboardInteractor()
{
	_windowId = 0;
	_engine = nullptr;
	_installed = false;

	_windowsProcHook = NULL;
	_messageProcHook = NULL;
	
	_supportedEvents = {
		{"onKeyDown",		""},
		{"onKeyUp",			""},
		{"onUpdateScene",	""}
	};
	
	_keyboardStatusMap[AnimaKeyboardKey::AKK_UNKNOWN		] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_SPACE			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_APOSTROPHE		] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_COMMA			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_MINUS			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_PERIOD			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_SLASH			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_0 				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_1 				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_2 				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_3 				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_4 				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_5 				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_6 				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_7 				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_8				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_9				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_SEMICOLON		] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_EQUAL			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_A				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_B				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_C				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_D				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_E				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_F				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_G				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_H				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_I				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_J				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_K				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_L				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_M				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_N				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_O				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_P				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_Q				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_R				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_S				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_T				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_U				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_V				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_W				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_X				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_Y				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_Z				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_LEFT_BRACKET	] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_BACKSLASH		] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_RIGHT_BRACKET	] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_GRAVE_ACCENT	] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_WORLD_1		] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_WORLD_2		] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_ESCAPE			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_ENTER			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_TAB			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_BACKSPACE		] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_INSERT			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_DELETE			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_RIGHT			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_LEFT			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_DOWN			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_UP				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_PAGE_UP		] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_PAGE_DOWN		] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_HOME			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_END			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_CAPS_LOCK		] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_SCROLL_LOCK	] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_NUM_LOCK		] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_PRINT_SCREEN	] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_PAUSE			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_F1				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_F2				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_F3				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_F4				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_F5				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_F6				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_F7				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_F8				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_F9				] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_F10			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_F11			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_F12			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_F13			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_F14			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_F15			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_F16			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_F17			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_F18			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_F19			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_F20			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_F21			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_F22			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_F23			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_F24			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_F25			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_KP_0			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_KP_1			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_KP_2			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_KP_3			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_KP_4			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_KP_5			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_KP_6			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_KP_7			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_KP_8			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_KP_9			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_KP_DECIMAL		] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_KP_DIVIDE		] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_KP_MULTIPLY	] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_KP_SUBTRACT	] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_KP_ADD			] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_KP_ENTER		] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_KP_EQUAL		] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_LEFT_SHIFT		] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_LEFT_CONTROL	] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_LEFT_ALT		] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_LEFT_SUPER		] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_RIGHT_SHIFT	] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_RIGHT_CONTROL	] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_RIGHT_ALT		] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_RIGHT_SUPER	] = false;
	_keyboardStatusMap[AnimaKeyboardKey::AKK_MENU			] = false;
}

AnimaKeyboardInteractor::AnimaKeyboardInteractor(const AnimaKeyboardInteractor& src)
{
	_windowId = src._windowId;
	_engine = src._engine;
	_installed = src._installed;

	_windowsProcHook = src._windowsProcHook;
	_messageProcHook = src._messageProcHook;
	
	_keyboardStatusMap = src._keyboardStatusMap;
}

AnimaKeyboardInteractor::AnimaKeyboardInteractor(AnimaKeyboardInteractor&& src)
{
	_windowId = src._windowId;
	_engine = src._engine;
	_installed = src._installed;
	
	_windowsProcHook = src._windowsProcHook;
	_messageProcHook = src._messageProcHook;
	
	_keyboardStatusMap = src._keyboardStatusMap;
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

		_windowsProcHook = src._windowsProcHook;
		_messageProcHook = src._messageProcHook;
		
		_keyboardStatusMap = src._keyboardStatusMap;
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

		_windowsProcHook = src._windowsProcHook;
		_messageProcHook = src._messageProcHook;
		
		_keyboardStatusMap = src._keyboardStatusMap;
	}
	
	return *this;
}

bool AnimaKeyboardInteractor::Install(long windowId, AnimaEngine* engine)
{
	AnimaLogger::LogMessage("Installing keyboard interactor...");

	if (_installed)
	{
		AnimaLogger::LogMessage("Keyboard interactor already installed");
		return true;
	}

	_windowId = windowId;
	_engine = engine;

	HMODULE module = GetCurrentModule();
	_windowsProcHook = SetWindowsHookEx(WH_CALLWNDPROC, WindowsProcCallback, module, NULL);
	if (_windowsProcHook == NULL)
	{
		AnimaLogger::LogMessage("Unable to install keyboard interactor");
		return false;
	}
	_messageProcHook = SetWindowsHookEx(WH_GETMESSAGE, MessageProcCallback, module, NULL);
	if (_messageProcHook == NULL)
	{
		AnimaLogger::LogMessage("Unable to install keyboard interactor");
		return false;
	}

	// Aggingo questo interactor alla lista di interactor installati
	_installedInteractors[windowId] = this;

	_installed = true;

	AnimaLogger::LogMessage("Keyboard interactor installed succesfully");

	return true;
}

bool AnimaKeyboardInteractor::Remove()
{
	if (_installed == false)
		return false;

	// Rimuovo questo interactor dalla lista di interactor installati
	auto interactor = _installedInteractors.find(_windowId);
	if (interactor != _installedInteractors.end())
		_installedInteractors.erase(interactor);

	UnhookWindowsHook(WH_CALLWNDPROC, WindowsProcCallback);
	UnhookWindowsHook(WH_GETMESSAGE, MessageProcCallback);

	_windowId = 0;
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

LRESULT CALLBACK AnimaKeyboardInteractor::WindowsProcCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	CWPSTRUCT* pWndProcStruct = reinterpret_cast<CWPSTRUCT*>(lParam);
	if (pWndProcStruct == nullptr)
		return 0;

	auto pair = _installedInteractors.find((long)pWndProcStruct->hwnd);
	if (pair == _installedInteractors.end())
		return 0;

	AnimaKeyboardInteractor* pKeyboardInteractor = pair->second;
	if (pKeyboardInteractor == nullptr)
		return 0;

	if (nCode == HC_ACTION)
	{
		HWND wndProcWnd = pWndProcStruct->hwnd;
		AUint wndProcMessage = pWndProcStruct->message;
		WPARAM wndProcWparam = pWndProcStruct->wParam;
		WPARAM wndProcLparam = pWndProcStruct->lParam;

		HandleMessage(wndProcWnd, wndProcMessage, wndProcWparam, wndProcLparam, pKeyboardInteractor);
	}

	return CallNextHookEx(pKeyboardInteractor->_windowsProcHook, nCode, wParam, lParam);
}

LRESULT CALLBACK AnimaKeyboardInteractor::MessageProcCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	MSG* pMessageProcStruct = reinterpret_cast<MSG*>(lParam);
	if (pMessageProcStruct == nullptr)
		return 0;

	auto pair = _installedInteractors.find((long)pMessageProcStruct->hwnd);
	if (pair == _installedInteractors.end())
		return 0;

	AnimaKeyboardInteractor* pKeyboardInteractor = pair->second;
	if (pKeyboardInteractor == nullptr)
		return 0;

	if (nCode == HC_ACTION)
	{
		HWND wndProcWnd = pMessageProcStruct->hwnd;
		AUint wndProcMessage = pMessageProcStruct->message;
		WPARAM wndProcWparam = pMessageProcStruct->wParam;
		WPARAM wndProcLparam = pMessageProcStruct->lParam;

		HandleMessage(wndProcWnd, wndProcMessage, wndProcWparam, wndProcLparam, pKeyboardInteractor);
	}

	return CallNextHookEx(pKeyboardInteractor->_windowsProcHook, nCode, wParam, lParam);
}

void AnimaKeyboardInteractor::HandleMessage(HWND hWnd, AUint message, WPARAM wParam, LPARAM lParam, AnimaKeyboardInteractor* interactor)
{
	switch (message)
	{
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	{
		const AInt mods = TranslateFlags();
		const AnimaKeyboardKey key = TranslateKey(wParam, lParam);

		interactor->_keyboardStatusMap[key] = true;

		AnimaEventArgs* argsUp = new AnimaKeyboardEventArgs(interactor, key, mods, interactor->_keyboardStatusMap);
		interactor->LaunchEvent("onKeyDown", argsUp);

		delete argsUp;
		argsUp = nullptr;

		break;
	}
	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		const AInt mods = TranslateFlags();
		const AnimaKeyboardKey key = TranslateKey(wParam, lParam);
		
		if (wParam == VK_SHIFT)
		{
			interactor->_keyboardStatusMap[AnimaKeyboardKey::AKK_LEFT_SHIFT] = false;

			// Release both Shift keys on Shift up event, as only one event
			// is sent even if both keys are released
			AnimaEventArgs* argsLeft = new AnimaKeyboardEventArgs(interactor, AnimaKeyboardKey::AKK_LEFT_SHIFT, mods, interactor->_keyboardStatusMap);
			interactor->LaunchEvent("onKeyUp", argsLeft);

			delete argsLeft;
			argsLeft = nullptr;

			interactor->_keyboardStatusMap[AnimaKeyboardKey::AKK_RIGHT_SHIFT] = false;

			AnimaEventArgs* argsRight = new AnimaKeyboardEventArgs(interactor, AnimaKeyboardKey::AKK_RIGHT_SHIFT, mods, interactor->_keyboardStatusMap);
			interactor->LaunchEvent("onKeyUp", argsRight);

			delete argsRight;
			argsRight = nullptr;
		}
		else if (wParam == VK_SNAPSHOT)
		{
			interactor->_keyboardStatusMap[key] = true;

			// Key down is not reported for the print screen key
			AnimaEventArgs* argsDown = new AnimaKeyboardEventArgs(interactor, key, mods, interactor->_keyboardStatusMap);
			interactor->LaunchEvent("onKeyDown", argsDown);

			delete argsDown;
			argsDown = nullptr;

			interactor->_keyboardStatusMap[key] = false;

			AnimaEventArgs* argsUp = new AnimaKeyboardEventArgs(interactor, key, mods, interactor->_keyboardStatusMap);
			interactor->LaunchEvent("onKeyUp", argsUp);

			delete argsUp;
			argsUp = nullptr;
		}
		else
		{
			interactor->_keyboardStatusMap[key] = false;

			AnimaEventArgs* argsUp = new AnimaKeyboardEventArgs(interactor, key, mods, interactor->_keyboardStatusMap);
			interactor->LaunchEvent("onKeyUp", argsUp);

			delete argsUp;
			argsUp = nullptr;
		}

		break;
	}
	}
}

AInt TranslateFlags()
{
	AInt mods = 0;

	if (GetAsyncKeyState(VK_SHIFT) & (1 << 31))
		mods |= (AInt)AnimaKeyboardModifier::AKM_SHIFT;
	if (GetAsyncKeyState(VK_CONTROL) & (1 << 31))
		mods |= (AInt)AnimaKeyboardModifier::AKM_CTRL;
	if (GetAsyncKeyState(VK_MENU) & (1 << 31))
		mods |= (AInt)AnimaKeyboardModifier::AKM_ALT;
	if ((GetAsyncKeyState(VK_LWIN) | GetAsyncKeyState(VK_RWIN)) & (1 << 31))
		mods |= (AInt)AnimaKeyboardModifier::AKM_SUPER;

	return mods;
}

AnimaKeyboardKey TranslateKey(WPARAM wParam, LPARAM lParam)
{
	// Check for numeric keypad keys
	// NOTE: This way we always force "NumLock = ON", which is intentional since
	//       the returned key code should correspond to a physical location.
	if ((HIWORD(lParam) & 0x100) == 0)
	{
		switch (MapVirtualKey(HIWORD(lParam) & 0xFF, 1))
		{
		case VK_INSERT:   return AnimaKeyboardKey::AKK_KP_0;
		case VK_END:      return AnimaKeyboardKey::AKK_KP_1;
		case VK_DOWN:     return AnimaKeyboardKey::AKK_KP_2;
		case VK_NEXT:     return AnimaKeyboardKey::AKK_KP_3;
		case VK_LEFT:     return AnimaKeyboardKey::AKK_KP_4;
		case VK_CLEAR:    return AnimaKeyboardKey::AKK_KP_5;
		case VK_RIGHT:    return AnimaKeyboardKey::AKK_KP_6;
		case VK_HOME:     return AnimaKeyboardKey::AKK_KP_7;
		case VK_UP:       return AnimaKeyboardKey::AKK_KP_8;
		case VK_PRIOR:    return AnimaKeyboardKey::AKK_KP_9;
		case VK_DIVIDE:   return AnimaKeyboardKey::AKK_KP_DIVIDE;
		case VK_MULTIPLY: return AnimaKeyboardKey::AKK_KP_MULTIPLY;
		case VK_SUBTRACT: return AnimaKeyboardKey::AKK_KP_SUBTRACT;
		case VK_ADD:      return AnimaKeyboardKey::AKK_KP_ADD;
		case VK_DELETE:   return AnimaKeyboardKey::AKK_KP_DECIMAL;
		default:          break;
		}
	}

	switch (HIWORD(lParam) & 0xFF)
	{
		// handle printable chars except space in a language independent way,
		// using scancodes rather than virtual keys
		// as virtual keys are language dependent.
		// Printable keys are mapped according to US layout.

		// Row 0:
	case 0x29:             return AnimaKeyboardKey::AKK_GRAVE_ACCENT;
	case 0x02:             return AnimaKeyboardKey::AKK_1;
	case 0x03:             return AnimaKeyboardKey::AKK_2;
	case 0x04:             return AnimaKeyboardKey::AKK_3;
	case 0x05:             return AnimaKeyboardKey::AKK_4;
	case 0x06:             return AnimaKeyboardKey::AKK_5;
	case 0x07:             return AnimaKeyboardKey::AKK_6;
	case 0x08:             return AnimaKeyboardKey::AKK_7;
	case 0x09:             return AnimaKeyboardKey::AKK_8;
	case 0x0A:             return AnimaKeyboardKey::AKK_9;
	case 0x0B:             return AnimaKeyboardKey::AKK_0;
	case 0x0C:             return AnimaKeyboardKey::AKK_MINUS;
	case 0x0D:             return AnimaKeyboardKey::AKK_EQUAL;

		// Row 1:
	case 0x10:             return AnimaKeyboardKey::AKK_Q;
	case 0x11:             return AnimaKeyboardKey::AKK_W;
	case 0x12:             return AnimaKeyboardKey::AKK_E;
	case 0x13:             return AnimaKeyboardKey::AKK_R;
	case 0x14:             return AnimaKeyboardKey::AKK_T;
	case 0x15:             return AnimaKeyboardKey::AKK_Y;
	case 0x16:             return AnimaKeyboardKey::AKK_U;
	case 0x17:             return AnimaKeyboardKey::AKK_I;
	case 0x18:             return AnimaKeyboardKey::AKK_O;
	case 0x19:             return AnimaKeyboardKey::AKK_P;
	case 0x1A:             return AnimaKeyboardKey::AKK_LEFT_BRACKET;
	case 0x1B:             return AnimaKeyboardKey::AKK_RIGHT_BRACKET;
		// We do not map 0x2B as this is only on US - use vKeys for this to prevent confusion with 0x56

		// Row 2:
	case 0x1E:             return AnimaKeyboardKey::AKK_A;
	case 0x1F:             return AnimaKeyboardKey::AKK_S;
	case 0x20:             return AnimaKeyboardKey::AKK_D;
	case 0x21:             return AnimaKeyboardKey::AKK_F;
	case 0x22:             return AnimaKeyboardKey::AKK_G;
	case 0x23:             return AnimaKeyboardKey::AKK_H;
	case 0x24:             return AnimaKeyboardKey::AKK_J;
	case 0x25:             return AnimaKeyboardKey::AKK_K;
	case 0x26:             return AnimaKeyboardKey::AKK_L;
	case 0x27:             return AnimaKeyboardKey::AKK_SEMICOLON;
	case 0x28:             return AnimaKeyboardKey::AKK_APOSTROPHE;

		// Row 3:
	case 0x2C:             return AnimaKeyboardKey::AKK_Z;
	case 0x2D:             return AnimaKeyboardKey::AKK_X;
	case 0x2E:             return AnimaKeyboardKey::AKK_C;
	case 0x2F:             return AnimaKeyboardKey::AKK_V;
	case 0x30:             return AnimaKeyboardKey::AKK_B;
	case 0x31:             return AnimaKeyboardKey::AKK_N;
	case 0x32:             return AnimaKeyboardKey::AKK_M;
	case 0x33:             return AnimaKeyboardKey::AKK_COMMA;
	case 0x34:             return AnimaKeyboardKey::AKK_PERIOD;
	case 0x35:             return AnimaKeyboardKey::AKK_SLASH;
	default:               break;
	}

	// Check which key was pressed or released
	switch (wParam)
	{
		// The SHIFT keys require special handling
	case VK_SHIFT:
	{
		// Compare scan code for this key with that of VK_RSHIFT in
		// order to determine which shift key was pressed (left or
		// right)
		const DWORD scancode = MapVirtualKey(VK_RSHIFT, 0);
		if ((DWORD)((lParam & 0x01ff0000) >> 16) == scancode)
			return AnimaKeyboardKey::AKK_RIGHT_SHIFT;

		return AnimaKeyboardKey::AKK_LEFT_SHIFT;
	}

		// The CTRL keys require special handling
	case VK_CONTROL:
	{
		MSG next;
		DWORD time;

		// Is this an extended key (i.e. right key)?
		if (lParam & 0x01000000)
			return AnimaKeyboardKey::AKK_RIGHT_CONTROL;

		// Here is a trick: "Alt Gr" sends LCTRL, then RALT. We only
		// want the RALT message, so we try to see if the next message
		// is a RALT message. In that case, this is a false LCTRL!
		time = GetMessageTime();

		if (PeekMessageW(&next, NULL, 0, 0, PM_NOREMOVE))
		{
			if (next.message == WM_KEYDOWN || next.message == WM_SYSKEYDOWN || next.message == WM_KEYUP || next.message == WM_SYSKEYUP)
			{
				if (next.wParam == VK_MENU && (next.lParam & 0x01000000) && next.time == time)
				{
					// Next message is a RALT down message, which
					// means that this is not a proper LCTRL message
					return AnimaKeyboardKey::AKK_UNKNOWN;
				}
			}
		}

		return AnimaKeyboardKey::AKK_LEFT_CONTROL;
	}

		// The ALT keys require special handling
	case VK_MENU:
	{
		// Is this an extended key (i.e. right key)?
		if (lParam & 0x01000000)
			return AnimaKeyboardKey::AKK_RIGHT_ALT;

		return AnimaKeyboardKey::AKK_LEFT_ALT;
	}

		// The ENTER keys require special handling
	case VK_RETURN:
	{
		// Is this an extended key (i.e. right key)?
		if (lParam & 0x01000000)
			return AnimaKeyboardKey::AKK_KP_ENTER;

		return AnimaKeyboardKey::AKK_ENTER;
	}

		// Funcion keys (non-printable keys)
	case VK_ESCAPE:        return AnimaKeyboardKey::AKK_ESCAPE;
	case VK_TAB:           return AnimaKeyboardKey::AKK_TAB;
	case VK_BACK:          return AnimaKeyboardKey::AKK_BACKSPACE;
	case VK_HOME:          return AnimaKeyboardKey::AKK_HOME;
	case VK_END:           return AnimaKeyboardKey::AKK_END;
	case VK_PRIOR:         return AnimaKeyboardKey::AKK_PAGE_UP;
	case VK_NEXT:          return AnimaKeyboardKey::AKK_PAGE_DOWN;
	case VK_INSERT:        return AnimaKeyboardKey::AKK_INSERT;
	case VK_DELETE:        return AnimaKeyboardKey::AKK_DELETE;
	case VK_LEFT:          return AnimaKeyboardKey::AKK_LEFT;
	case VK_UP:            return AnimaKeyboardKey::AKK_UP;
	case VK_RIGHT:         return AnimaKeyboardKey::AKK_RIGHT;
	case VK_DOWN:          return AnimaKeyboardKey::AKK_DOWN;
	case VK_F1:            return AnimaKeyboardKey::AKK_F1;
	case VK_F2:            return AnimaKeyboardKey::AKK_F2;
	case VK_F3:            return AnimaKeyboardKey::AKK_F3;
	case VK_F4:            return AnimaKeyboardKey::AKK_F4;
	case VK_F5:            return AnimaKeyboardKey::AKK_F5;
	case VK_F6:            return AnimaKeyboardKey::AKK_F6;
	case VK_F7:            return AnimaKeyboardKey::AKK_F7;
	case VK_F8:            return AnimaKeyboardKey::AKK_F8;
	case VK_F9:            return AnimaKeyboardKey::AKK_F9;
	case VK_F10:           return AnimaKeyboardKey::AKK_F10;
	case VK_F11:           return AnimaKeyboardKey::AKK_F11;
	case VK_F12:           return AnimaKeyboardKey::AKK_F12;
	case VK_F13:           return AnimaKeyboardKey::AKK_F13;
	case VK_F14:           return AnimaKeyboardKey::AKK_F14;
	case VK_F15:           return AnimaKeyboardKey::AKK_F15;
	case VK_F16:           return AnimaKeyboardKey::AKK_F16;
	case VK_F17:           return AnimaKeyboardKey::AKK_F17;
	case VK_F18:           return AnimaKeyboardKey::AKK_F18;
	case VK_F19:           return AnimaKeyboardKey::AKK_F19;
	case VK_F20:           return AnimaKeyboardKey::AKK_F20;
	case VK_F21:           return AnimaKeyboardKey::AKK_F21;
	case VK_F22:           return AnimaKeyboardKey::AKK_F22;
	case VK_F23:           return AnimaKeyboardKey::AKK_F23;
	case VK_F24:           return AnimaKeyboardKey::AKK_F24;
	case VK_NUMLOCK:       return AnimaKeyboardKey::AKK_NUM_LOCK;
	case VK_CAPITAL:       return AnimaKeyboardKey::AKK_CAPS_LOCK;
	case VK_SNAPSHOT:      return AnimaKeyboardKey::AKK_PRINT_SCREEN;
	case VK_SCROLL:        return AnimaKeyboardKey::AKK_SCROLL_LOCK;
	case VK_PAUSE:         return AnimaKeyboardKey::AKK_PAUSE;
	case VK_LWIN:          return AnimaKeyboardKey::AKK_LEFT_SUPER;
	case VK_RWIN:          return AnimaKeyboardKey::AKK_RIGHT_SUPER;
	case VK_APPS:          return AnimaKeyboardKey::AKK_MENU;

		// Numeric keypad
	case VK_NUMPAD0:       return AnimaKeyboardKey::AKK_KP_0;
	case VK_NUMPAD1:       return AnimaKeyboardKey::AKK_KP_1;
	case VK_NUMPAD2:       return AnimaKeyboardKey::AKK_KP_2;
	case VK_NUMPAD3:       return AnimaKeyboardKey::AKK_KP_3;
	case VK_NUMPAD4:       return AnimaKeyboardKey::AKK_KP_4;
	case VK_NUMPAD5:       return AnimaKeyboardKey::AKK_KP_5;
	case VK_NUMPAD6:       return AnimaKeyboardKey::AKK_KP_6;
	case VK_NUMPAD7:       return AnimaKeyboardKey::AKK_KP_7;
	case VK_NUMPAD8:       return AnimaKeyboardKey::AKK_KP_8;
	case VK_NUMPAD9:       return AnimaKeyboardKey::AKK_KP_9;
	case VK_DIVIDE:        return AnimaKeyboardKey::AKK_KP_DIVIDE;
	case VK_MULTIPLY:      return AnimaKeyboardKey::AKK_KP_MULTIPLY;
	case VK_SUBTRACT:      return AnimaKeyboardKey::AKK_KP_SUBTRACT;
	case VK_ADD:           return AnimaKeyboardKey::AKK_KP_ADD;
	case VK_DECIMAL:       return AnimaKeyboardKey::AKK_KP_DECIMAL;

		// Printable keys are mapped according to US layout
	case VK_SPACE:         return AnimaKeyboardKey::AKK_SPACE;
	case 0xDC:             return AnimaKeyboardKey::AKK_BACKSLASH;
	case 0xDF:             return AnimaKeyboardKey::AKK_WORLD_1;
	case 0xE2:             return AnimaKeyboardKey::AKK_WORLD_2;
	default:               break;
	}

	// No matching translation was found
	return AnimaKeyboardKey::AKK_UNKNOWN;
}

void AnimaKeyboardInteractor::UpdateScene(AnimaScene* scene, AFloat elapsedTime)
{
	AnimaEventArgs* args = new AnimaUpdateSceneEventArgs(this, scene, elapsedTime);
	
	LaunchEvent("onUpdateScene", args);
	
	delete args;
	args = nullptr;
}

END_ANIMA_ENGINE_NAMESPACE