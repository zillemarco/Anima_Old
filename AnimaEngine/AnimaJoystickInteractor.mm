//
//  AnimaJoystickInteractor.mm
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

#if defined WIN32
#else
IOHIDManagerRef AnimaJoystickInteractor::_managerRef = nullptr;
AnimaJoystickDevice AnimaJoystickInteractor::_joysticks[];
#endif

AnimaJoystickInteractor AnimaJoystickInteractor::_instance;
bool AnimaJoystickInteractor::_joysticksInitialized = false;

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

AnimaJoystickDevice::AnimaJoystickDevice()
{
	_present = false;
	_name = "";
	_deviceRef = nullptr;
}

AnimaJoystickDevice::~AnimaJoystickDevice()
{
	
}

AnimaJoystickDevice::AnimaJoystickDevice(const AnimaJoystickDevice& src)
{
	_present = src._present;
	_name = src._name;
	
	_deviceRef = src._deviceRef;
	
	_axisElements = src._axisElements;
	_buttonElements = src._buttonElements;
	_hatElements = src._hatElements;
	
	_axis = src._axis;
	_buttons = src._buttons;
}

AnimaJoystickDevice::AnimaJoystickDevice(AnimaJoystickDevice&& src)
{
	_present = src._present;
	_name = src._name;
	
	_deviceRef = src._deviceRef;
	
	_axisElements = src._axisElements;
	_buttonElements = src._buttonElements;
	_hatElements = src._hatElements;
	
	_axis = src._axis;
	_buttons = src._buttons;
}

AnimaJoystickDevice& AnimaJoystickDevice::operator=(const AnimaJoystickDevice& src)
{
	if(this != &src)
	{
		_present = src._present;
		_name = src._name;
		
		_deviceRef = src._deviceRef;
		
		_axisElements = src._axisElements;
		_buttonElements = src._buttonElements;
		_hatElements = src._hatElements;
		
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
		
		_deviceRef = src._deviceRef;
		
		_axisElements = src._axisElements;
		_buttonElements = src._buttonElements;
		_hatElements = src._hatElements;
		
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
	_pollMutex.lock();
	
	for (AInt joy = 0; joy <= (AInt)AnimaJoystickID::AJI_LAST; joy++)
	{
		CFIndex i;
		AInt buttonIndex = 0;
		AnimaJoystickDevice* joystick = &_joysticks[joy];
		
		if (!joystick->_present)
			continue;
		
		for (i = 0; i < CFArrayGetCount(joystick->_buttonElements); i++)
		{
			AnimaJoystickElement* button = (AnimaJoystickElement*) CFArrayGetValueAtIndex(joystick->_buttonElements, i);
			
			if (GetElementValue(joystick, button))
			{
				if(joystick->_buttons[buttonIndex] == false)
				{
					joystick->_buttons[buttonIndex] = true;
				
					AnimaJoystickButtonEventArgs* args = new AnimaJoystickButtonEventArgs(this, joystick->_name, joy, joystick->_buttons, joystick->_axis, buttonIndex);

					LaunchEvent("onButtonPressed", args);
				
					delete args;
					args = nullptr;
				}
			}
			else if(joystick->_buttons[buttonIndex] == true)
			{
				joystick->_buttons[buttonIndex] = false;
				
				AnimaJoystickButtonEventArgs* args = new AnimaJoystickButtonEventArgs(this, joystick->_name, joy, joystick->_buttons, joystick->_axis, buttonIndex);
				
				LaunchEvent("onButtonReleased", args);
				
				delete args;
				args = nullptr;
			}
			
			buttonIndex++;
		}
		
		for (i = 0; i < CFArrayGetCount(joystick->_axisElements); i++)
		{
			AnimaJoystickElement* axis = (AnimaJoystickElement*) CFArrayGetValueAtIndex(joystick->_axisElements, i);
			
			long value = GetElementValue(joystick, axis);
			long readScale = axis->maxReport - axis->minReport;
			
			if (readScale == 0)
				joystick->_axis[i] = value;
			else
				joystick->_axis[i] = (2.f * (value - axis->minReport) / readScale) - 1.f;
			
			joystick->_axis[i] *= _sensibility;
			
			if(fabs(joystick->_axis[i]) <= _zeroThreshold)
				joystick->_axis[i] = 0.0f;
			
			if(joystick->_axis[i] != 0.0f)
			{				
				AnimaJoystickAxisEventArgs* args = new AnimaJoystickAxisEventArgs(this, joystick->_name, joy, joystick->_buttons, joystick->_axis, i, joystick->_axis[i]);
				
				LaunchEvent("onAxisMoved", args);
				
				delete args;
				args = nullptr;
			}
		}
		
		for (i = 0; i < CFArrayGetCount(joystick->_hatElements); i++)
		{
			AnimaJoystickElement* hat = (AnimaJoystickElement*) CFArrayGetValueAtIndex(joystick->_hatElements, i);
			
			// Bit fields of button presses for each direction, including nil
			const AInt directions[9] = { 1, 3, 2, 6, 4, 12, 8, 9, 0 };
			
			long j, value = GetElementValue(joystick, hat);
			if (value < 0 || value > 8)
				value = 8;
			
			for (j = 0;  j < 4;  j++)
			{
				if (directions[value] & (1 << j))
				{
					if(joystick->_buttons[buttonIndex] == false)
					{
						joystick->_buttons[buttonIndex] = true;
					
						AnimaJoystickButtonEventArgs* args = new AnimaJoystickButtonEventArgs(this, joystick->_name, joy, joystick->_buttons, joystick->_axis, buttonIndex);
					
						LaunchEvent("onButtonPressed", args);
					
						delete args;
						args = nullptr;
					}
				}
				else if(joystick->_buttons[buttonIndex] == true)
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
	CFMutableArrayRef matchingCFArrayRef;
	
	_managerRef = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
	
	matchingCFArrayRef = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);
	if (matchingCFArrayRef)
	{
		CFDictionaryRef matchingCFDictRef = CreateMatchingDictionary(kHIDPage_GenericDesktop, kHIDUsage_GD_Joystick);
		if (matchingCFDictRef)
		{
			CFArrayAppendValue(matchingCFArrayRef, matchingCFDictRef);
			CFRelease(matchingCFDictRef);
		}
		
		matchingCFDictRef = CreateMatchingDictionary(kHIDPage_GenericDesktop, kHIDUsage_GD_GamePad);
		if (matchingCFDictRef)
		{
			CFArrayAppendValue(matchingCFArrayRef, matchingCFDictRef);
			CFRelease(matchingCFDictRef);
		}
		
		matchingCFDictRef = CreateMatchingDictionary(kHIDPage_GenericDesktop, kHIDUsage_GD_MultiAxisController);
		if (matchingCFDictRef)
		{
			CFArrayAppendValue(matchingCFArrayRef, matchingCFDictRef);
			CFRelease(matchingCFDictRef);
		}
		
		IOHIDManagerSetDeviceMatchingMultiple(_managerRef, matchingCFArrayRef);
		CFRelease(matchingCFArrayRef);
	}
	
	IOHIDManagerRegisterDeviceMatchingCallback(_managerRef, &MatchCallback, nullptr);
	IOHIDManagerRegisterDeviceRemovalCallback(_managerRef, &RemoveCallback, nullptr);
	
	IOHIDManagerScheduleWithRunLoop(_managerRef, CFRunLoopGetMain(), kCFRunLoopDefaultMode);
	
	IOHIDManagerOpen(_managerRef, kIOHIDOptionsTypeNone);
	
	// Execute the run loop once in order to register any initially-attached
	// joysticks
	CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, false);
	
	return true;
}

void AnimaJoystickInteractor::TerminateJoysticks()
{
	for (AInt i = 0; i < (AInt)AnimaJoystickID::AJI_LAST + 1; i++)
	{
		AnimaJoystickDevice* joystick = &_joysticks[i];
		RemoveJoystick(joystick);
	}
	
	CFRelease(_managerRef);
	_managerRef = nullptr;
}

void AnimaJoystickInteractor::GetElementsCFArrayHandler(const void* value, void* parameter)
{
	if (CFGetTypeID(value) == IOHIDElementGetTypeID())
		AddJoystickElement((AnimaJoystickDevice*)parameter, (IOHIDElementRef)value);
}

void AnimaJoystickInteractor::AddJoystickElement(AnimaJoystickDevice* joystick, IOHIDElementRef elementRef)
{
	IOHIDElementType elementType;
	long usagePage, usage;
	CFMutableArrayRef elementsArray = nullptr;
	
	elementType = IOHIDElementGetType(elementRef);
	usagePage = IOHIDElementGetUsagePage(elementRef);
	usage = IOHIDElementGetUsage(elementRef);
	
	if ((elementType != kIOHIDElementTypeInput_Axis) && (elementType != kIOHIDElementTypeInput_Button) && (elementType != kIOHIDElementTypeInput_Misc))
		return;
	
	switch (usagePage)
	{
		case kHIDPage_GenericDesktop:
		{
			switch (usage)
			{
				case kHIDUsage_GD_X:
				case kHIDUsage_GD_Y:
				case kHIDUsage_GD_Z:
				case kHIDUsage_GD_Rx:
				case kHIDUsage_GD_Ry:
				case kHIDUsage_GD_Rz:
				case kHIDUsage_GD_Slider:
				case kHIDUsage_GD_Dial:
				case kHIDUsage_GD_Wheel:
					elementsArray = joystick->_axisElements;
					break;
				case kHIDUsage_GD_Hatswitch:
					elementsArray = joystick->_hatElements;
					break;
			}
			
			break;
		}
		case kHIDPage_Button:
			elementsArray = joystick->_buttonElements;
			break;
		default:
			break;
	}
	
	if (elementsArray)
	{
		AnimaJoystickElement* element = (AnimaJoystickElement*)calloc(1, sizeof(AnimaJoystickElement));
		
		CFArrayAppendValue(elementsArray, element);
		
		element->elementRef = elementRef;
		
		element->minReport = IOHIDElementGetLogicalMin(elementRef);
		element->maxReport = IOHIDElementGetLogicalMax(elementRef);
	}
}

void AnimaJoystickInteractor::RemoveCallback(void* context, IOReturn result, void* sender, IOHIDDeviceRef deviceRef)
{
	for (AInt joy = (AInt)AnimaJoystickID::AJI_1; joy <= (AInt)AnimaJoystickID::AJI_LAST; joy++)
	{
		AnimaJoystickDevice* joystick = &_joysticks[joy];
		if (joystick->_deviceRef == deviceRef)
		{
			RemoveJoystick(joystick);
			break;
		}
	}
}

void AnimaJoystickInteractor::RemoveJoystick(AnimaJoystickDevice* joystick)
{
	AInt i;
	if (!joystick->_present)
		return;
	
	for (i = 0;  i < CFArrayGetCount(joystick->_axisElements);  i++)
		free((void*) CFArrayGetValueAtIndex(joystick->_axisElements, i));
	CFArrayRemoveAllValues(joystick->_axisElements);
	CFRelease(joystick->_axisElements);
	
	for (i = 0;  i < CFArrayGetCount(joystick->_buttonElements);  i++)
		free((void*) CFArrayGetValueAtIndex(joystick->_buttonElements, i));
	CFArrayRemoveAllValues(joystick->_buttonElements);
	CFRelease(joystick->_buttonElements);
	
	for (i = 0;  i < CFArrayGetCount(joystick->_hatElements);  i++)
		free((void*) CFArrayGetValueAtIndex(joystick->_hatElements, i));
	CFArrayRemoveAllValues(joystick->_hatElements);
	CFRelease(joystick->_hatElements);
	
	joystick->_axis.clear();
	joystick->_buttons.clear();
	
	memset(joystick, 0, sizeof(AnimaJoystickDevice));
}

long AnimaJoystickInteractor::GetElementValue(AnimaJoystickDevice* joystick, AnimaJoystickElement* element)
{
	IOReturn result = kIOReturnSuccess;
	IOHIDValueRef valueRef;
	long value = 0;
	
	if (joystick && element && joystick->_deviceRef)
	{
		result = IOHIDDeviceGetValue(joystick->_deviceRef, element->elementRef, &valueRef);
		if (kIOReturnSuccess == result)
		{
			value = IOHIDValueGetIntegerValue(valueRef);
			
			// Record min and max for auto calibration
			if (value < element->minReport)
				element->minReport = value;
			if (value > element->maxReport)
				element->maxReport = value;
		}
	}
	
	// Auto user scale
	return value;
}

CFMutableDictionaryRef AnimaJoystickInteractor::CreateMatchingDictionary(long usagePage, long usage)
{
	CFMutableDictionaryRef result = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	
	if (result)
	{
		CFNumberRef pageRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberLongType, &usagePage);
		if (pageRef)
		{
			CFDictionarySetValue(result, CFSTR(kIOHIDDeviceUsagePageKey), pageRef);
			CFRelease(pageRef);
			
			CFNumberRef usageRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberLongType, &usage);
			if (usageRef)
			{
				CFDictionarySetValue(result, CFSTR(kIOHIDDeviceUsageKey), usageRef);
				CFRelease(usageRef);
			}
		}
	}
	
	return result;
}

void AnimaJoystickInteractor::MatchCallback(void* context, IOReturn result, void* sender, IOHIDDeviceRef deviceRef)
{
	AnimaJoystickDevice* joystick;
	AInt joy;
	for (joy = (AInt)AnimaJoystickID::AJI_1; joy <= (AInt)AnimaJoystickID::AJI_LAST; joy++)
	{
		joystick = &_joysticks[joy];
		
		if (!joystick->_present)
			continue;
		
		if (joystick->_deviceRef == deviceRef)
			return;
	}
	
	for (joy = (AInt)AnimaJoystickID::AJI_1; joy <= (AInt)AnimaJoystickID::AJI_LAST; joy++)
	{
		joystick = &_joysticks[joy];
		
		if (!joystick->_present)
			break;
	}
	
	if (joy > (AInt)AnimaJoystickID::AJI_LAST)
		return;
	
	joystick->_present = true;
	joystick->_deviceRef = deviceRef;
	
	CFStringRef name = (CFStringRef)IOHIDDeviceGetProperty(deviceRef, CFSTR(kIOHIDProductKey));
	NSString* tmpName = (NSString*)name;
	joystick->_name = [tmpName UTF8String];
	
	joystick->_axisElements = CFArrayCreateMutable(nullptr, 0, nullptr);
	joystick->_buttonElements = CFArrayCreateMutable(nullptr, 0, nullptr);
	joystick->_hatElements = CFArrayCreateMutable(nullptr, 0, nullptr);
	
	CFArrayRef arrayRef = IOHIDDeviceCopyMatchingElements(deviceRef, nullptr, kIOHIDOptionsTypeNone);
	CFRange range = { 0, CFArrayGetCount(arrayRef) };
	CFArrayApplyFunction(arrayRef, range, GetElementsCFArrayHandler, (void*)joystick);
	
	CFRelease(arrayRef);
	
	joystick->_axis.resize(CFArrayGetCount(joystick->_axisElements));
	joystick->_buttons.resize(CFArrayGetCount(joystick->_buttonElements) + CFArrayGetCount(joystick->_hatElements) * 4);
}

END_ANIMA_ENGINE_NAMESPACE