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
#include <mutex>

#include <boost/unordered_map.hpp>
#include <boost/thread.hpp>

#if defined WIN32
#	include <Windows.h>
#	include <mmsystem.h>

	typedef MMRESULT(WINAPI * JOYGETDEVCAPS_T)(UINT_PTR, LPJOYCAPS, UINT);
	typedef MMRESULT(WINAPI * JOYGETPOS_T)(UINT, LPJOYINFO);
	typedef MMRESULT(WINAPI * JOYGETPOSEX_T)(UINT, LPJOYINFOEX);
	typedef UINT(WINAPI * JOYGETNUMDEVS_T)(void);
#else
#	include <IOKit/IOKitLib.h>
#	include <IOKit/IOCFPlugIn.h>
#	include <IOKit/hid/IOHIDLib.h>
#	include <IOKit/hid/IOHIDKeys.h>
#endif

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaScene;

enum class AnimaJoystickID : AInt
{
	AJI_1		= 0,
	AJI_2		= 1,
	AJI_3		= 2,
	AJI_4		= 3,
	AJI_5		= 4,
	AJI_6		= 5,
	AJI_7		= 6,
	AJI_8		= 7,
	AJI_9		= 8,
	AJI_10		= 9,
	AJI_11		= 10,
	AJI_12		= 11,
	AJI_13		= 12,
	AJI_14		= 13,
	AJI_15		= 14,
	AJI_16		= 15,
	AJI_LAST	= AJI_16
};

class ANIMA_ENGINE_EXPORT AnimaJoystickDevice
{
	friend class AnimaJoystickInteractor;
	
public:
	AnimaJoystickDevice();
	~AnimaJoystickDevice();
	
	AnimaJoystickDevice(const AnimaJoystickDevice& src);
	AnimaJoystickDevice(AnimaJoystickDevice&& src);
	
	AnimaJoystickDevice& operator=(const AnimaJoystickDevice& src);
	AnimaJoystickDevice& operator=(AnimaJoystickDevice&& src);
	
	bool IsPresent() const { return _present; }
	const AnimaString& GetName() const { return _name; }
	const AnimaArray<AFloat>& GetAxis() const { return _axis; }
	const AnimaArray<bool>& GetButtons() const { return _buttons; }
	
private:
	bool _present;
	AnimaString _name;

#if !defined WIN32
	IOHIDDeviceRef _deviceRef;
	
	CFMutableArrayRef _axisElements;
	CFMutableArrayRef _buttonElements;
	CFMutableArrayRef _hatElements;
#endif
	
	AnimaArray<AFloat>	_axis;
	AnimaArray<bool>	_buttons;
};

#if !defined WIN32
typedef struct
{
	IOHIDElementRef elementRef;
	
	long min;
	long max;
	
	long minReport;
	long maxReport;
	
} AnimaJoystickElement;
#endif

class ANIMA_ENGINE_EXPORT AnimaJoystickEventArgs : public AnimaEventArgs
{
public:
	AnimaJoystickEventArgs(IAnimaEvents* sourceEvent, const AnimaString& joystickName, AInt joystickId, const AnimaArray<bool>& buttonsState, const AnimaArray<AFloat>& axisState)
		: AnimaEventArgs(sourceEvent)
	{
		_joystickName = joystickName;
		_joystickId = joystickId;
		_buttonsState = buttonsState;
		_axisState = axisState;
	}
	
public:
	const AnimaString& GetJoystickName() const { return _joystickName; }
	const AInt& GetJoystickId() const { return _joystickId; }
	const AnimaArray<bool>& GetButtonsState() const { return _buttonsState; }
	const AnimaArray<AFloat>& GetAxisState() const { return _axisState; }
	
private:
	AnimaString _joystickName;
	AInt _joystickId;
	AnimaArray<bool> _buttonsState;
	AnimaArray<AFloat> _axisState;
};

class ANIMA_ENGINE_EXPORT AnimaJoystickButtonEventArgs : public AnimaJoystickEventArgs
{
public:
	AnimaJoystickButtonEventArgs(IAnimaEvents* sourceEvent, const AnimaString& joystickName, AInt joystickId, const AnimaArray<bool>& buttonsState, const AnimaArray<AFloat>& axisState, AInt buttonId)
	: AnimaJoystickEventArgs(sourceEvent, joystickName, joystickId, buttonsState, axisState)
	{
		_buttonId = buttonId;
	}
	
public:
	const AInt& GetButtonId() const { return _buttonId; }
	
private:
	AInt _buttonId;
};

class ANIMA_ENGINE_EXPORT AnimaJoystickAxisEventArgs : public AnimaJoystickEventArgs
{
public:
	AnimaJoystickAxisEventArgs(IAnimaEvents* sourceEvent, const AnimaString& joystickName, AInt joystickId, const AnimaArray<bool>& buttonsState, const AnimaArray<AFloat>& axisState, AInt axisId, AFloat axisValue)
	: AnimaJoystickEventArgs(sourceEvent, joystickName, joystickId, buttonsState, axisState)
	{
		_axisId = axisId;
		_axisValue = axisValue;
	}
	
public:
	const AInt& GetAxisId() const { return _axisId; }
	const AFloat& GetAxisValue() const { return _axisValue; }
	
private:
	AInt _axisId;
	AFloat _axisValue;
};

class ANIMA_ENGINE_EXPORT AnimaJoystickInteractor : public IAnimaEvents, public IAnimaInteractor
{
public:
	static AnimaJoystickInteractor* GetInstance();
	
protected:
	AnimaJoystickInteractor();
	virtual ~AnimaJoystickInteractor();
	
public:
	bool Install(long windowId, AnimaEngine* engine) override;
	bool Remove() override;
	void UpdateScene(AnimaScene* scene, AFloat elapsedTime) override;
	
	bool SetEventHandler(const AnimaString& eventName, std::function<void (AnimaEventArgs* eventArgs)> handler) override;
	bool LaunchEvent(const AnimaString& eventName, AnimaEventArgs* eventArgs) const override;
	AnimaArray<AnimaEventInfo> GetSupportedEvents() const override;
	bool EventIsSupported(const AnimaString& eventName) const override;
	
	void SetZeroThreshold(AFloat zeroThreshold) { _zeroThreshold = zeroThreshold; }
	AFloat GetZeroThreshold() const { return _zeroThreshold; }
	
	void SetSensibility(AFloat sensibility) { _sensibility = sensibility; }
	AFloat GetSensibility() { return _sensibility; }
	
	AnimaEngine* GetEngine() { return _engine; }
	
	void PollEvents();
	
	AnimaJoystickDevice* GetJoystick(AInt joystickId) const { return (joystickId >= 0 && joystickId <= (AInt)AnimaJoystickID::AJI_LAST) ? &_joysticks[joystickId] : nullptr; }
	
private:
#if defined WIN32
#else
	static void AddJoystickElement(AnimaJoystickDevice* joystick, IOHIDElementRef elementRef);
	static void GetElementsCFArrayHandler(const void* value, void* parameter);
	static void RemoveCallback(void* context, IOReturn result, void* sender, IOHIDDeviceRef deviceRef);
	static void RemoveJoystick(AnimaJoystickDevice* joystick);
	static CFMutableDictionaryRef CreateMatchingDictionary(long usagePage, long usage);
	static void MatchCallback(void* context, IOReturn result, void* sender, IOHIDDeviceRef deviceRef);
	
	long GetElementValue(AnimaJoystickDevice* joystick, AnimaJoystickElement* element);
#endif
	
	static bool InitializeJoysticks();
	static void TerminateJoysticks();
	
protected:
	bool _installed;
	AnimaEngine* _engine;
	AFloat _zeroThreshold;
	AFloat _sensibility;
	
	std::mutex _pollMutex;
	boost::thread* _pollThread;
	
	AnimaArray<AnimaEventInfo> _supportedEvents;

	static AnimaJoystickDevice _joysticks[(AInt)AnimaJoystickID::AJI_LAST + 1];
	static bool _joysticksInitialized;
	
	static AnimaJoystickInteractor _instance;
	
#if defined WIN32
	static HINSTANCE _hWinmm;
	static JOYGETDEVCAPS_T _joyGetDevCaps;
	static JOYGETPOS_T _joyGetPos;
	static JOYGETPOSEX_T _joyGetPosEx;
	static JOYGETNUMDEVS_T _joyGetNumDevs;
#else
	static IOHIDManagerRef _managerRef;
#endif
	
#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, std::function<void (AnimaEventArgs* eventArgs)>, AnimaStringHasher> _eventsMap;
#pragma warning (default: 4251)

public:
	static const AInt AJ_XBOX_360_DEFAULT_A;
	static const AInt AJ_XBOX_360_DEFAULT_B;
	static const AInt AJ_XBOX_360_DEFAULT_X;
	static const AInt AJ_XBOX_360_DEFAULT_Y;
	static const AInt AJ_XBOX_360_DEFAULT_UP;
	static const AInt AJ_XBOX_360_DEFAULT_DOWN;
	static const AInt AJ_XBOX_360_DEFAULT_LEFT;
	static const AInt AJ_XBOX_360_DEFAULT_RIGHT;
	static const AInt AJ_XBOX_360_DEFAULT_BACK;
	static const AInt AJ_XBOX_360_DEFAULT_START;
	static const AInt AJ_XBOX_360_DEFAULT_LS_X;
	static const AInt AJ_XBOX_360_DEFAULT_LS_Y;
	static const AInt AJ_XBOX_360_DEFAULT_RS_X;
	static const AInt AJ_XBOX_360_DEFAULT_RS_Y;
	static const AInt AJ_XBOX_360_DEFAULT_LS_CLICK;
	static const AInt AJ_XBOX_360_DEFAULT_RS_CLICK;
	static const AInt AJ_XBOX_360_DEFAULT_LB;
	static const AInt AJ_XBOX_360_DEFAULT_RB;
	static const AInt AJ_XBOX_360_DEFAULT_LT;
	static const AInt AJ_XBOX_360_DEFAULT_RT;
	static const AInt AJ_XBOX_360_DEFAULT_XBOX;
};

END_ANIMA_ENGINE_NAMESPACE
