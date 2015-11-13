//
//  AnimaKeyboardInteractor.h
//  Anima
//
//  Created by Marco Zille on 03/11/15.
//
//
#pragma once

#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "IAnimaInteractor.h"
#include "IAnimaEvents.h"
#include "AnimaString.h"
#include "AnimaVertex.h"

#include <boost/unordered_map.hpp>

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaScene;

enum class AnimaKeyboardModifier : AInt
{
	AKM_SHIFT	= 1,
	AKM_CTRL	= 2,
	AKM_ALT		= 4,
	AKM_SUPER	= 8
};

enum class AnimaKeyboardKey : AInt
{
	AKK_UNKNOWN			= -1,
	AKK_SPACE			= 32,
	AKK_APOSTROPHE		= 39,  /* ' */
	AKK_COMMA			= 44,  /* , */
	AKK_MINUS			= 45,  /* - */
	AKK_PERIOD			= 46,  /* . */
	AKK_SLASH			= 47,  /* / */
	AKK_0 				= 48,
	AKK_1 				= 49,
	AKK_2 				= 50,
	AKK_3 				= 51,
	AKK_4 				= 52,
	AKK_5 				= 53,
	AKK_6 				= 54,
	AKK_7 				= 55,
	AKK_8				= 56,
	AKK_9				= 57,
	AKK_SEMICOLON		= 59,	/* ; */
	AKK_EQUAL			= 61,	/* = */
	AKK_A				= 65,
	AKK_B				= 66,
	AKK_C				= 67,
	AKK_D				= 68,
	AKK_E				= 69,
	AKK_F				= 70,
	AKK_G				= 71,
	AKK_H				= 72,
	AKK_I				= 73,
	AKK_J				= 74,
	AKK_K				= 75,
	AKK_L				= 76,
	AKK_M				= 77,
	AKK_N				= 78,
	AKK_O				= 79,
	AKK_P				= 80,
	AKK_Q				= 81,
	AKK_R				= 82,
	AKK_S				= 83,
	AKK_T				= 84,
	AKK_U				= 85,
	AKK_V				= 86,
	AKK_W				= 87,
	AKK_X				= 88,
	AKK_Y				= 89,
	AKK_Z				= 90,
	AKK_LEFT_BRACKET	= 91,	/* [ */
	AKK_BACKSLASH		= 92,	/* \ */
	AKK_RIGHT_BRACKET	= 93,	/* ] */
	AKK_GRAVE_ACCENT	= 96,	/* ` */
	AKK_WORLD_1			= 161,	/* non-US #1 */
	AKK_WORLD_2			= 162,	/* non-US #2 */
	// Function keys
	AKK_ESCAPE			= 256,
	AKK_ENTER			= 257,
	AKK_TAB				= 258,
	AKK_BACKSPACE		= 259,
	AKK_INSERT			= 260,
	AKK_DELETE			= 261,
	AKK_RIGHT			= 262,
	AKK_LEFT			= 263,
	AKK_DOWN			= 264,
	AKK_UP				= 265,
	AKK_PAGE_UP			= 266,
	AKK_PAGE_DOWN		= 267,
	AKK_HOME			= 268,
	AKK_END				= 269,
	AKK_CAPS_LOCK		= 280,
	AKK_SCROLL_LOCK		= 281,
	AKK_NUM_LOCK		= 282,
	AKK_PRINT_SCREEN	= 283,
	AKK_PAUSE			= 284,
	AKK_F1				= 290,
	AKK_F2				= 291,
	AKK_F3				= 292,
 	AKK_F4				= 293,
	AKK_F5				= 294,
	AKK_F6				= 295,
	AKK_F7				= 296,
	AKK_F8				= 297,
	AKK_F9				= 298,
	AKK_F10				= 299,
	AKK_F11				= 300,
	AKK_F12				= 301,
	AKK_F13				= 302,
	AKK_F14				= 303,
	AKK_F15				= 304,
	AKK_F16				= 305,
	AKK_F17				= 306,
	AKK_F18				= 307,
	AKK_F19				= 308,
	AKK_F20				= 309,
	AKK_F21				= 310,
	AKK_F22				= 311,
	AKK_F23				= 312,
	AKK_F24				= 313,
	AKK_F25				= 314,
	AKK_KP_0			= 320,
	AKK_KP_1			= 321,
	AKK_KP_2			= 322,
	AKK_KP_3			= 323,
	AKK_KP_4			= 324,
	AKK_KP_5			= 325,
	AKK_KP_6			= 326,
	AKK_KP_7			= 327,
	AKK_KP_8			= 328,
	AKK_KP_9			= 329,
	AKK_KP_DECIMAL		= 330,
	AKK_KP_DIVIDE		= 331,
	AKK_KP_MULTIPLY		= 332,
	AKK_KP_SUBTRACT		= 333,
	AKK_KP_ADD			= 334,
	AKK_KP_ENTER		= 335,
	AKK_KP_EQUAL		= 336,
	AKK_LEFT_SHIFT		= 340,
	AKK_LEFT_CONTROL	= 341,
	AKK_LEFT_ALT		= 342,
	AKK_LEFT_SUPER		= 343,
	AKK_RIGHT_SHIFT		= 344,
	AKK_RIGHT_CONTROL	= 345,
	AKK_RIGHT_ALT		= 346,
	AKK_RIGHT_SUPER		= 347,
	AKK_MENU			= 348,
	AKK_LAST			= AKK_MENU
};

#if defined WIN32
	AInt TranslateFlags();
	AnimaKeyboardKey TranslateKey(WPARAM wParam, LPARAM lParam);
#else
	AInt TranslateFlags(AUint flags);
	AnimaKeyboardKey TranslateKey(AUint key);
#endif

typedef boost::unordered_map<AnimaKeyboardKey, bool> AnimaKeyboardStatusMap;

class ANIMA_ENGINE_EXPORT AnimaKeyboardEventArgs : public AnimaEventArgs
{
public:
	AnimaKeyboardEventArgs(IAnimaEvents* sourceEvent, AnimaKeyboardKey key, AInt modifiers, const AnimaKeyboardStatusMap& keyboardStatus)
		: AnimaEventArgs(sourceEvent)
	{
		_key = key;
		_modifiers = modifiers;
		_keyboardStatus = keyboardStatus;
	}
	
public:
	const AnimaKeyboardKey& GetKey() const { return _key; }
	const AInt& GetModifiers() const { return _modifiers; }
	const AnimaKeyboardStatusMap& GetKeyboardStatus() const { return _keyboardStatus; }
	
private:
	AnimaKeyboardKey _key;
	AInt _modifiers;
	AnimaKeyboardStatusMap _keyboardStatus;
};

class ANIMA_ENGINE_EXPORT AnimaKeyboardInteractor : public IAnimaEvents, public IAnimaInteractor
{
public:
	AnimaKeyboardInteractor();
	AnimaKeyboardInteractor(const AnimaKeyboardInteractor& src);
	AnimaKeyboardInteractor(AnimaKeyboardInteractor&& src);
	virtual ~AnimaKeyboardInteractor();
	
	AnimaKeyboardInteractor& operator=(const AnimaKeyboardInteractor& src);
	AnimaKeyboardInteractor& operator=(AnimaKeyboardInteractor&& src);
	
public:
	bool Install(long windowId, AnimaEngine* engine) override;
	bool Remove() override;
	void UpdateScene(AnimaScene* scene, AFloat elapsedTime) override;
	
	bool SetEventHandler(const AnimaString& eventName, std::function<void (AnimaEventArgs* eventArgs)> handler) override;
	bool LaunchEvent(const AnimaString& eventName, AnimaEventArgs* eventArgs) const override;
	AnimaArray<AnimaEventInfo> GetSupportedEvents() const override;
	bool EventIsSupported(const AnimaString& eventName) const override;
	
	long GetWindowId() const { return _windowId; }
	AnimaEngine* GetEngine() { return _engine; }
	const AnimaKeyboardStatusMap& GetKeyboardStatus() const { return _keyboardStatusMap; }
	
private:
#if defined WIN32
	static LRESULT CALLBACK WindowsProcCallback(int nCode, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK MessageProcCallback(int nCode, WPARAM wParam, LPARAM lParam);
	static void HandleMessage(HWND hWnd, AUint message, WPARAM wParam, LPARAM lParam, AnimaKeyboardInteractor* interactor);
#else
	static void KeyDownCallback(AnimaKeyboardInteractor* interactor, NSEvent* event);
	static void KeyUpCallback(AnimaKeyboardInteractor* interactor, NSEvent* event);
#endif
	
protected:
	bool _installed;
	long _windowId;
	AnimaEngine* _engine;
	
	AnimaArray<AnimaEventInfo> _supportedEvents;
	
#if defined WIN32
	static boost::unordered_map<long, AnimaKeyboardInteractor*> _installedInteractors;
	HHOOK _windowsProcHook;
	HHOOK _messageProcHook;
#else
	id _keyDownMonitor;
	id _keyUpMonitor;
#endif
	
#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, std::function<void (AnimaEventArgs* eventArgs)>, AnimaStringHasher> _eventsMap;
	AnimaKeyboardStatusMap _keyboardStatusMap;
#pragma warning (default: 4251)
};

END_ANIMA_ENGINE_NAMESPACE
