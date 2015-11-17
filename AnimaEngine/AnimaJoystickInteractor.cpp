//
//  AnimaJoystickInteractor.cpp
//  Anima
//
//  Created by Marco Zille on 03/11/15.
//
//

#include "AnimaJoystickInteractor.h"
#include "AnimaVertex.h"
#include "AnimaScene.h"
#include <boost/date_time/posix_time/posix_time.hpp>

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaJoystickInteractor AnimaJoystickInteractor::_instance;
AnimaJoystickDevice AnimaJoystickInteractor::_joysticks[];
bool AnimaJoystickInteractor::_joysticksInitialized = false;
HINSTANCE AnimaJoystickInteractor::_hWinmm = nullptr;
JOYGETDEVCAPS_T AnimaJoystickInteractor::_joyGetDevCaps = nullptr;
JOYGETPOS_T AnimaJoystickInteractor::_joyGetPos = nullptr;
JOYGETPOSEX_T AnimaJoystickInteractor::_joyGetPosEx = nullptr;
JOYGETNUMDEVS_T AnimaJoystickInteractor::_joyGetNumDevs = nullptr;

const AInt AnimaJoystickInteractor::AJ_XBOX_360_DEFAULT_A			= 0;
const AInt AnimaJoystickInteractor::AJ_XBOX_360_DEFAULT_B			= 1;
const AInt AnimaJoystickInteractor::AJ_XBOX_360_DEFAULT_X			= 2;
const AInt AnimaJoystickInteractor::AJ_XBOX_360_DEFAULT_Y			= 3;
const AInt AnimaJoystickInteractor::AJ_XBOX_360_DEFAULT_LB			= 4;
const AInt AnimaJoystickInteractor::AJ_XBOX_360_DEFAULT_RB			= 5;
const AInt AnimaJoystickInteractor::AJ_XBOX_360_DEFAULT_BACK		= 6;
const AInt AnimaJoystickInteractor::AJ_XBOX_360_DEFAULT_START		= 7;
const AInt AnimaJoystickInteractor::AJ_XBOX_360_DEFAULT_LS_CLICK	= 8;
const AInt AnimaJoystickInteractor::AJ_XBOX_360_DEFAULT_RS_CLICK	= 9;
const AInt AnimaJoystickInteractor::AJ_XBOX_360_DEFAULT_UP			= 10;
const AInt AnimaJoystickInteractor::AJ_XBOX_360_DEFAULT_RIGHT		= 11;
const AInt AnimaJoystickInteractor::AJ_XBOX_360_DEFAULT_DOWN		= 12;
const AInt AnimaJoystickInteractor::AJ_XBOX_360_DEFAULT_LEFT		= 13;
const AInt AnimaJoystickInteractor::AJ_XBOX_360_DEFAULT_XBOX		= 36;
const AInt AnimaJoystickInteractor::AJ_XBOX_360_DEFAULT_LS_X		= 0;
const AInt AnimaJoystickInteractor::AJ_XBOX_360_DEFAULT_LS_Y		= 1;
const AInt AnimaJoystickInteractor::AJ_XBOX_360_DEFAULT_LT			= 2;
const AInt AnimaJoystickInteractor::AJ_XBOX_360_DEFAULT_RT			= 2;
const AInt AnimaJoystickInteractor::AJ_XBOX_360_DEFAULT_RS_Y		= 3;
const AInt AnimaJoystickInteractor::AJ_XBOX_360_DEFAULT_RS_X		= 4;

void joystickPollThreadFunc(AnimaJoystickInteractor* interactor)
{
	try
	{
		while (true)
		{
			interactor->PollEvents();
			boost::this_thread::sleep(boost::posix_time::milliseconds(100));
		}
	}
	catch (boost::thread_interrupted const&)
	{
	}
}

AFloat NormalizeAxis(DWORD pos, DWORD min, DWORD max)
{
	AFloat fpos = (AFloat)pos;
	AFloat fmin = (AFloat)min;
	AFloat fmax = (AFloat)max;

	return (2.f * (fpos - fmin) / (fmax - fmin)) - 1.f;
}

AnimaJoystickDevice::AnimaJoystickDevice()
{
	_present = false;
	_name = "";
	_axis.resize(6, 0.0f);
	_buttons.resize(36, false);
}

AnimaJoystickDevice::~AnimaJoystickDevice()
{
	
}

AnimaJoystickDevice::AnimaJoystickDevice(const AnimaJoystickDevice& src)
{
	_present = src._present;
	_name = src._name;

	_axis = src._axis;
	_buttons = src._buttons;
}

AnimaJoystickDevice::AnimaJoystickDevice(AnimaJoystickDevice&& src)
{
	_present = src._present;
	_name = src._name;
		
	_axis = src._axis;
	_buttons = src._buttons;
}

AnimaJoystickDevice& AnimaJoystickDevice::operator=(const AnimaJoystickDevice& src)
{
	if(this != &src)
	{
		_present = src._present;
		_name = src._name;
		
		_axis = src._axis;
		_buttons = src._buttons;
	}
	return *this;
}

AnimaJoystickDevice& AnimaJoystickDevice::operator=(AnimaJoystickDevice&& src)
{
	if(this != &src)
	{
		_present = src._present;
		_name = src._name;
		
		_axis = src._axis;
		_buttons = src._buttons;
	}
	return *this;
}

AnimaJoystickInteractor* AnimaJoystickInteractor::GetInstance()
{
	return &_instance;
}

AnimaJoystickInteractor::AnimaJoystickInteractor()
{
	_engine = nullptr;
	_installed = false;
	_pollThread = nullptr;
	
	_zeroThreshold = 0.2f;
	_sensibility = 1.0f;
	
	_supportedEvents = {
		{"onButtonPressed",		""},
		{"onButtonReleased",	""},
		{"onAxisMoved",			""},
		{"onUpdateScene",		""}
	};

}

AnimaJoystickInteractor::~AnimaJoystickInteractor()
{
	Remove();
	
	if(_pollThread != nullptr)
	{
		delete _pollThread;
		_pollThread = nullptr;
	}
}

bool AnimaJoystickInteractor::Install(long windowId, AnimaEngine* engine)
{
	if(_installed)
		return true;
	
	if(InitializeJoysticks() == false)
		return false;
	
	_engine = engine;
	
	_pollThread = new boost::thread(joystickPollThreadFunc, this);

	_installed = true;
	return true;
}

bool AnimaJoystickInteractor::Remove()
{
	if(_installed == false)
		return false;
	
	TerminateJoysticks();
	
	if(_pollThread != nullptr)
	{
		_pollThread->interrupt();
		delete _pollThread;
		_pollThread = nullptr;
	}
	
	_installed = false;
	return true;
}

bool AnimaJoystickInteractor::SetEventHandler(const AnimaString& eventName, std::function<void (AnimaEventArgs* eventArgs)> handler)
{
	if(EventIsSupported(eventName))
	{
		_eventsMap[eventName] = handler;
		return true;
	}
	return false;
}

bool AnimaJoystickInteractor::LaunchEvent(const AnimaString& eventName, AnimaEventArgs* eventArgs) const
{
	auto func = _eventsMap.find(eventName);
	if(func != _eventsMap.end() && func->second != nullptr)
	{
		func->second(eventArgs);
		return true;
	}
	
	return false;
}

AnimaArray<AnimaEventInfo> AnimaJoystickInteractor::GetSupportedEvents() const
{
	return _supportedEvents;
}

bool AnimaJoystickInteractor::EventIsSupported(const AnimaString& eventName) const
{
	for(auto item : _supportedEvents)
	{
		if(item._eventName == eventName)
			return true;
	}
	
	return false;
}

void AnimaJoystickInteractor::PollEvents()
{
	if (_joysticksInitialized == false)
		return;

	if (_joyGetDevCaps == nullptr || _joyGetPos == nullptr || _joyGetPosEx == nullptr || _joyGetNumDevs == nullptr)
		return;

	_pollMutex.lock();
	
	for (AInt joy = 0; joy <= (AInt)AnimaJoystickID::AJI_LAST; joy++)
	{
		AnimaJoystickDevice* joystick = &_joysticks[joy];
		if (joystick->_present == false)
			continue;

		JOYCAPS jc;
		JOYINFOEX ji;
		if (_joyGetDevCaps(joy, &jc, sizeof(JOYCAPS)) != JOYERR_NOERROR)
			continue;

		ji.dwSize = sizeof(JOYINFOEX);
		ji.dwFlags = JOY_RETURNX | JOY_RETURNY | JOY_RETURNZ | JOY_RETURNR | JOY_RETURNU | JOY_RETURNV | JOY_RETURNBUTTONS | JOY_RETURNPOV;

		if (_joyGetPosEx(joy, &ji) != JOYERR_NOERROR)
			continue;

		AInt axisIndex = 0;
		joystick->_axis[axisIndex++] = NormalizeAxis(ji.dwXpos, jc.wXmin, jc.wXmax) * _sensibility;
		joystick->_axis[axisIndex++] = NormalizeAxis(ji.dwYpos, jc.wYmin, jc.wYmax) * _sensibility;

		if (jc.wCaps & JOYCAPS_HASZ)
			joystick->_axis[axisIndex++] = NormalizeAxis(ji.dwZpos, jc.wZmin, jc.wZmax) * _sensibility;

		if (jc.wCaps & JOYCAPS_HASR)
			joystick->_axis[axisIndex++] = NormalizeAxis(ji.dwRpos, jc.wRmin, jc.wRmax) * _sensibility;

		if (jc.wCaps & JOYCAPS_HASU)
			joystick->_axis[axisIndex++] = NormalizeAxis(ji.dwUpos, jc.wUmin, jc.wUmax) * _sensibility;

		if (jc.wCaps & JOYCAPS_HASV)
			joystick->_axis[axisIndex++] = NormalizeAxis(ji.dwVpos, jc.wVmin, jc.wVmax) * _sensibility;

		for (AInt na = 0; na < axisIndex; na++)
		{
			if (fabs(joystick->_axis[na]) <= _zeroThreshold)
				joystick->_axis[na] = 0.0f;

			if (joystick->_axis[na] != 0.0f)
			{
				AnimaJoystickAxisEventArgs* args = new AnimaJoystickAxisEventArgs(this, joystick->_name, joy, joystick->_buttons, joystick->_axis, na, joystick->_axis[na]);

				LaunchEvent("onAxisMoved", args);

				delete args;
				args = nullptr;
			}
		}

		AInt buttonIndex = 0;
		while (buttonIndex < (AInt)jc.wNumButtons)
		{
			bool pressed = ji.dwButtons & (1UL << buttonIndex);
			if (pressed)
			{
				if (joystick->_buttons[buttonIndex] == false)
				{
					joystick->_buttons[buttonIndex] = true;

					AnimaJoystickButtonEventArgs* args = new AnimaJoystickButtonEventArgs(this, joystick->_name, joy, joystick->_buttons, joystick->_axis, buttonIndex);

					LaunchEvent("onButtonPressed", args);

					delete args;
					args = nullptr;
				}
			}
			else if (joystick->_buttons[buttonIndex] == true)
			{
				joystick->_buttons[buttonIndex] = false;

				AnimaJoystickButtonEventArgs* args = new AnimaJoystickButtonEventArgs(this, joystick->_name, joy, joystick->_buttons, joystick->_axis, buttonIndex);

				LaunchEvent("onButtonReleased", args);

				delete args;
				args = nullptr;
			}

			buttonIndex++;
		}
		
		// Virtual buttons - Inject data from hats
		// Each hat is exposed as 4 buttons which exposes 8 directions with
		// concurrent button presses
		// NOTE: this API exposes only one hat
		if ((jc.wCaps & JOYCAPS_HASPOV) && (jc.wCaps & JOYCAPS_POV4DIR))
		{
			int i, value = ji.dwPOV / 100 / 45;

			// Bit fields of button presses for each direction, including nil
			const int directions[9] = { 1, 3, 2, 6, 4, 12, 8, 9, 0 };

			if (value < 0 || value > 8)
				value = 8;

			for (i = 0; i < 4; i++)
			{
				bool pressed = directions[value] & (1 << i);
				if (pressed)
				{
					if (joystick->_buttons[buttonIndex] == false)
					{
						joystick->_buttons[buttonIndex] = true;

						AnimaJoystickButtonEventArgs* args = new AnimaJoystickButtonEventArgs(this, joystick->_name, joy, joystick->_buttons, joystick->_axis, buttonIndex);

						LaunchEvent("onButtonPressed", args);

						delete args;
						args = nullptr;
					}
				}
				else if (joystick->_buttons[buttonIndex] == true)
				{
					joystick->_buttons[buttonIndex] = false;

					AnimaJoystickButtonEventArgs* args = new AnimaJoystickButtonEventArgs(this, joystick->_name, joy, joystick->_buttons, joystick->_axis, buttonIndex);

					LaunchEvent("onButtonReleased", args);

					delete args;
					args = nullptr;
				}

				buttonIndex++;
			}
		}
	}
	
	_pollMutex.unlock();
}

void AnimaJoystickInteractor::UpdateScene(AnimaScene* scene, AFloat elapsedTime)
{
	AnimaEventArgs* args = new AnimaUpdateSceneEventArgs(this, scene, elapsedTime);
	
	LaunchEvent("onUpdateScene", args);
	
	delete args;
	args = nullptr;
}

bool AnimaJoystickInteractor::InitializeJoysticks()
{
	if (_hWinmm == nullptr)
	{
		_hWinmm = LoadLibraryW(L"winmm.dll");

		if (_hWinmm == nullptr)
			return false;
	}

	if (_joyGetDevCaps == nullptr)
		_joyGetDevCaps = (JOYGETDEVCAPS_T)GetProcAddress(_hWinmm, "joyGetDevCapsA");

	if (_joyGetPos == nullptr)
		_joyGetPos = (JOYGETPOS_T)GetProcAddress(_hWinmm, "joyGetPos");

	if (_joyGetPosEx == nullptr)
		_joyGetPosEx = (JOYGETPOSEX_T)GetProcAddress(_hWinmm, "joyGetPosEx");

	if (_joyGetNumDevs == nullptr)
		_joyGetNumDevs = (JOYGETNUMDEVS_T)GetProcAddress(_hWinmm, "joyGetNumDevs");

	if (_joyGetDevCaps == nullptr || _joyGetPos == nullptr || _joyGetPosEx == nullptr || _joyGetNumDevs == nullptr)
		return false;

	UINT n = _joyGetNumDevs();

	for (AInt i = 0; i < (AInt)AnimaJoystickID::AJI_LAST + 1; i++)
	{
		JOYCAPS jc;
		MMRESULT res = _joyGetDevCaps(i, &jc, sizeof(JOYCAPS));
		if (res != JOYERR_NOERROR)
			continue;

		AnimaJoystickDevice* joystick = &_joysticks[i];
		joystick->_name = AnimaString(jc.szPname);
		joystick->_present = true;
	}

	_joysticksInitialized = true;
	return true;
}

void AnimaJoystickInteractor::TerminateJoysticks()
{
	for (AInt i = 0; i < (AInt)AnimaJoystickID::AJI_LAST + 1; i++)
	{
		AnimaJoystickDevice* joystick = &_joysticks[i];
		joystick->_name = "";
		joystick->_present = false;
	}

	if (_hWinmm)
	{
		FreeLibrary(_hWinmm);
		_hWinmm = nullptr;
	}

	_joysticksInitialized = false;
}

END_ANIMA_ENGINE_NAMESPACE