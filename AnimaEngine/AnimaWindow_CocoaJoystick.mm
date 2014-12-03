#include "AnimaWindow.h"
#include "AnimaEngine.h"

#include <unistd.h>
#include <ctype.h>

#include <mach/mach.h>
#include <mach/mach_error.h>

#include <CoreFoundation/CoreFoundation.h>
#include <Kernel/IOKit/hidsystem/IOHIDUsageTables.h>

BEGIN_ANIMA_ENGINE_NAMESPACE

typedef struct
{
	IOHIDElementCookie cookie;
	
	long min;
	long max;
	
	long minReport;
	long maxReport;
	
} _AnimaEngineWindowjoyelement;

static void getElementsCFArrayHandler(const void* value, void* parameter);


static void addJoystickElement(_AnimaEngineWindowjoystickIOKit* joystick, CFTypeRef elementRef)
{
	long elementType, usagePage, usage;
	CFMutableArrayRef elementsArray = NULL;
	
	CFNumberGetValue((CFNumberRef)CFDictionaryGetValue((CFDictionaryRef)elementRef, CFSTR(kIOHIDElementTypeKey)), kCFNumberLongType, &elementType);
	CFNumberGetValue((CFNumberRef)CFDictionaryGetValue((CFDictionaryRef)elementRef, CFSTR(kIOHIDElementUsagePageKey)), kCFNumberLongType, &usagePage);
	CFNumberGetValue((CFNumberRef)CFDictionaryGetValue((CFDictionaryRef)elementRef, CFSTR(kIOHIDElementUsageKey)), kCFNumberLongType, &usage);
	
	if ((elementType == kIOHIDElementTypeInput_Axis) || (elementType == kIOHIDElementTypeInput_Button) || (elementType == kIOHIDElementTypeInput_Misc))
	{
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
			long number;
			CFTypeRef numberRef;
			_AnimaEngineWindowjoyelement* element = (_AnimaEngineWindowjoyelement*)calloc(1, sizeof(_AnimaEngineWindowjoyelement));
			
			CFArrayAppendValue(elementsArray, element);
			
			numberRef = CFDictionaryGetValue((CFDictionaryRef)elementRef, CFSTR(kIOHIDElementCookieKey));
			if (numberRef && CFNumberGetValue((CFNumberRef)numberRef, kCFNumberLongType, &number))
				element->cookie = (IOHIDElementCookie) number;
			
			numberRef = CFDictionaryGetValue((CFDictionaryRef)elementRef, CFSTR(kIOHIDElementMinKey));
			if (numberRef && CFNumberGetValue((CFNumberRef)numberRef, kCFNumberLongType, &number))
				element->minReport = element->min = number;
			
			numberRef = CFDictionaryGetValue((CFDictionaryRef)elementRef, CFSTR(kIOHIDElementMaxKey));
			if (numberRef && CFNumberGetValue((CFNumberRef)numberRef, kCFNumberLongType, &number))
				element->maxReport = element->max = number;
		}
	}
	else
	{
		CFTypeRef array = CFDictionaryGetValue((CFDictionaryRef)elementRef, CFSTR(kIOHIDElementKey));
		if (array)
		{
			if (CFGetTypeID(array) == CFArrayGetTypeID())
			{
				CFRange range = { 0, CFArrayGetCount((CFArrayRef)array) };
				CFArrayApplyFunction((CFArrayRef)array, range, getElementsCFArrayHandler, joystick);
			}
		}
	}
}

static void getElementsCFArrayHandler(const void* value, void* parameter)
{
	if (CFGetTypeID(value) == CFDictionaryGetTypeID())
		addJoystickElement((_AnimaEngineWindowjoystickIOKit*) parameter, (CFTypeRef) value);
}

static long getElementValue(_AnimaEngineWindowjoystickIOKit* joystick, _AnimaEngineWindowjoyelement* element)
{
	IOReturn result = kIOReturnSuccess;
	IOHIDEventStruct hidEvent;
	hidEvent.value = 0;
	
	if (joystick && element && joystick->_interface)
	{
		result = (*(joystick->_interface))->getElementValue(joystick->_interface, element->cookie, &hidEvent);
		if (kIOReturnSuccess == result)
		{
			// Record min and max for auto calibration
			if (hidEvent.value < element->minReport)
				element->minReport = hidEvent.value;
			if (hidEvent.value > element->maxReport)
				element->maxReport = hidEvent.value;
		}
	}
	
	// Auto user scale
	return (long) hidEvent.value;
}

static void removeJoystick(_AnimaEngineWindowjoystickIOKit* joystick)
{
	int i;
	
	if (!joystick->_present)
		return;
	
	for (i = 0;  i < CFArrayGetCount(joystick->_axisElements);  i++)
		free((void*) CFArrayGetValueAtIndex(joystick->_axisElements, i));
	CFArrayRemoveAllValues(joystick->_axisElements);
	
	for (i = 0;  i < CFArrayGetCount(joystick->_buttonElements);  i++)
		free((void*) CFArrayGetValueAtIndex(joystick->_buttonElements, i));
	CFArrayRemoveAllValues(joystick->_buttonElements);
	
	for (i = 0;  i < CFArrayGetCount(joystick->_hatElements);  i++)
		free((void*) CFArrayGetValueAtIndex(joystick->_hatElements, i));
	CFArrayRemoveAllValues(joystick->_hatElements);
	
	free(joystick->_axes);
	free(joystick->_buttons);
	
	(*(joystick->_interface))->close(joystick->_interface);
	(*(joystick->_interface))->Release(joystick->_interface);
	
	memset(joystick, 0, sizeof(_AnimaEngineWindowjoystickIOKit));
}

static void removalCallback(void* target, IOReturn result, void* refcon, void* sender)
{
	removeJoystick((_AnimaEngineWindowjoystickIOKit*) refcon);
}

static void pollJoystickEvents(void)
{
	int joy;
	
	for (joy = 0;  joy <= ANIMA_ENGINE_JOYSTICK_LAST;  joy++)
	{
		CFIndex i;
		int buttonIndex = 0;
		_AnimaEngineWindowjoystickIOKit* joystick = AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_JOYSTICK_STATE + joy;
		
		if (!joystick->_present)
			continue;
		
		for (i = 0;  i < CFArrayGetCount(joystick->_buttonElements);  i++)
		{
			_AnimaEngineWindowjoyelement* button = (_AnimaEngineWindowjoyelement*) CFArrayGetValueAtIndex(joystick->_buttonElements, i);
			
			if (getElementValue(joystick, button))
				joystick->_buttons[buttonIndex++] = ANIMA_ENGINE_PRESS;
			else
				joystick->_buttons[buttonIndex++] = ANIMA_ENGINE_RELEASE;
		}
		
		for (i = 0;  i < CFArrayGetCount(joystick->_axisElements); i++)
		{
			_AnimaEngineWindowjoyelement* axis = (_AnimaEngineWindowjoyelement*) CFArrayGetValueAtIndex(joystick->_axisElements, i);
			
			long value = getElementValue(joystick, axis);
			long readScale = axis->maxReport - axis->minReport;
			
			if (readScale == 0)
				joystick->_axes[i] = value;
			else
				joystick->_axes[i] = (2.f * (value - axis->minReport) / readScale) - 1.f;
		}
		
		for (i = 0;  i < CFArrayGetCount(joystick->_hatElements);  i++)
		{
			_AnimaEngineWindowjoyelement* hat = (_AnimaEngineWindowjoyelement*) CFArrayGetValueAtIndex(joystick->_hatElements, i);
			
			// Bit fields of button presses for each direction, including nil
			const int directions[9] = { 1, 3, 2, 6, 4, 12, 8, 9, 0 };
			
			long j, value = getElementValue(joystick, hat);
			if (value < 0 || value > 8)
				value = 8;
			
			for (j = 0;  j < 4;  j++)
			{
				if (directions[value] & (1 << j))
					joystick->_buttons[buttonIndex++] = ANIMA_ENGINE_PRESS;
				else
					joystick->_buttons[buttonIndex++] = ANIMA_ENGINE_RELEASE;
			}
		}
	}
}

void _AnimaEngineWindowInitJoysticks(void)
{
	int joy = 0;
	IOReturn result = kIOReturnSuccess;
	mach_port_t masterPort = 0;
	io_iterator_t objectIterator = 0;
	CFMutableDictionaryRef hidMatchDictionary = NULL;
	io_object_t ioHIDDeviceObject = 0;
	
	result = IOMasterPort(bootstrap_port, &masterPort);
	hidMatchDictionary = IOServiceMatching(kIOHIDDeviceKey);
	if (kIOReturnSuccess != result || !hidMatchDictionary)
	{
		if (hidMatchDictionary)
			CFRelease(hidMatchDictionary);
		
		return;
	}
	
	result = IOServiceGetMatchingServices(masterPort, hidMatchDictionary, &objectIterator);
	if (result != kIOReturnSuccess)
		return;
	
	if (!objectIterator)
	{
		// There are no joysticks
		return;
	}
	
	while ((ioHIDDeviceObject = IOIteratorNext(objectIterator)))
	{
		CFMutableDictionaryRef propsRef = NULL;
		CFTypeRef valueRef = NULL;
		kern_return_t result;
		
		IOCFPlugInInterface** ppPlugInInterface = NULL;
		HRESULT plugInResult = S_OK;
		SInt32 score = 0;
		
		long usagePage = 0;
		long usage = 0;
		
		valueRef = IORegistryEntryCreateCFProperty(ioHIDDeviceObject, CFSTR(kIOHIDPrimaryUsagePageKey), kCFAllocatorDefault, kNilOptions);
		if (valueRef)
		{
			CFNumberGetValue((CFNumberRef)valueRef, kCFNumberLongType, &usagePage);
			CFRelease(valueRef);
		}
		
		valueRef = IORegistryEntryCreateCFProperty(ioHIDDeviceObject, CFSTR(kIOHIDPrimaryUsageKey), kCFAllocatorDefault, kNilOptions);
		if (valueRef)
		{
			CFNumberGetValue((CFNumberRef)valueRef, kCFNumberLongType, &usage);
			CFRelease(valueRef);
		}
		
		if (usagePage != kHIDPage_GenericDesktop)
		{
			// This device is not relevant to GLFW
			continue;
		}
		
		if ((usage != kHIDUsage_GD_Joystick && usage != kHIDUsage_GD_GamePad && usage != kHIDUsage_GD_MultiAxisController))
		{
			// This device is not relevant to GLFW
			continue;
		}
		
		result = IORegistryEntryCreateCFProperties(ioHIDDeviceObject, &propsRef, kCFAllocatorDefault, kNilOptions);
		
		if (result != kIOReturnSuccess)
			continue;
		
		_AnimaEngineWindowjoystickIOKit* joystick = AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_JOYSTICK_STATE + joy;
		joystick->_present = true;
		
		result = IOCreatePlugInInterfaceForService(ioHIDDeviceObject, kIOHIDDeviceUserClientTypeID, kIOCFPlugInInterfaceID, &ppPlugInInterface, &score);
		
		if (kIOReturnSuccess != result)
		{
			CFRelease(propsRef);
			return;
		}
		
		// TODO da controllare bene
		plugInResult = (*ppPlugInInterface)->QueryInterface(ppPlugInInterface, CFUUIDGetUUIDBytes(kIOHIDDeviceInterfaceID), (void **)&(joystick->_interface));
		
		if (plugInResult != S_OK)
		{
			CFRelease(propsRef);
			return;
		}
		
		(*ppPlugInInterface)->Release(ppPlugInInterface);
		
		(*(joystick->_interface))->open(joystick->_interface, 0);
		(*(joystick->_interface))->setRemovalCallback(joystick->_interface, removalCallback, joystick, joystick);
		
		// Get product string
		valueRef = CFDictionaryGetValue(propsRef, CFSTR(kIOHIDProductKey));
		if (valueRef)
		{
			CFStringGetCString((CFStringRef)valueRef, joystick->_name, sizeof(joystick->_name), kCFStringEncodingUTF8);
		}
		
		joystick->_axisElements = CFArrayCreateMutable(NULL, 0, NULL);
		joystick->_buttonElements = CFArrayCreateMutable(NULL, 0, NULL);
		joystick->_hatElements = CFArrayCreateMutable(NULL, 0, NULL);
		
		valueRef = CFDictionaryGetValue(propsRef, CFSTR(kIOHIDElementKey));
		if (CFGetTypeID(valueRef) == CFArrayGetTypeID())
		{
			CFRange range = { 0, CFArrayGetCount((CFArrayRef)valueRef) };
			CFArrayApplyFunction((CFArrayRef)valueRef, range, getElementsCFArrayHandler, (void*) joystick);
		}
		
		CFRelease(propsRef);
		
		joystick->_axes = (float*)calloc(CFArrayGetCount(joystick->_axisElements), sizeof(float));
		joystick->_buttons = (unsigned char*)calloc(CFArrayGetCount(joystick->_buttonElements) + CFArrayGetCount(joystick->_hatElements) * 4, 1);
		
		joy++;
		if (joy > ANIMA_ENGINE_JOYSTICK_LAST)
			break;
	}
}

void _AnimaEngineWindowTerminateJoysticks(void)
{
	int i;
	
	for (i = 0;  i < ANIMA_ENGINE_JOYSTICK_LAST + 1;  i++)
	{
		_AnimaEngineWindowjoystickIOKit* joystick = &AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_JOYSTICK_STATE[i];
		removeJoystick(joystick);
		
		if (joystick->_axisElements)
			CFRelease(joystick->_axisElements);
		if (joystick->_buttonElements)
			CFRelease(joystick->_buttonElements);
		if (joystick->_hatElements)
			CFRelease(joystick->_hatElements);
	}
}

bool _AnimaEngineWindowPlatformJoystickPresent(int joy)
{
	pollJoystickEvents();
	
	return AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_JOYSTICK_STATE[joy]._present;
}

const float* _AnimaEngineWindowPlatformGetJoystickAxes(int joy, int* count)
{
	_AnimaEngineWindowjoystickIOKit* joystick = AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_JOYSTICK_STATE + joy;
	
	pollJoystickEvents();
	
	if (!joystick->_present)
		return NULL;
	
	*count = (int) CFArrayGetCount(joystick->_axisElements);
	return joystick->_axes;
}

const unsigned char* _AnimaEngineWindowPlatformGetJoystickButtons(int joy, int* count)
{
	_AnimaEngineWindowjoystickIOKit* joystick = AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_JOYSTICK_STATE + joy;
	
	pollJoystickEvents();
	
	if (!joystick->_present)
		return NULL;
	
	*count = (int) CFArrayGetCount(joystick->_buttonElements) +
	(int) CFArrayGetCount(joystick->_hatElements) * 4;
	return joystick->_buttons;
}

const char* _AnimaEngineWindowPlatformGetJoystickName(int joy)
{
	pollJoystickEvents();
	
	return AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_JOYSTICK_STATE[joy]._name;
}

END_ANIMA_ENGINE_NAMESPACE