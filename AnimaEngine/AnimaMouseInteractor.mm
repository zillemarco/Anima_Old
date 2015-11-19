//
//  AnimaMouseInteractor.mm
//  Anima
//
//  Created by Marco Zille on 03/11/15.
//
//

#include "AnimaMouseInteractor.h"
#include "AnimaKeyboardInteractor.h"
#include "AnimaVertex.h"
#include "AnimaScene.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaMouseInteractor::AnimaMouseInteractor()
{
	_windowId = 0;
	_engine = nullptr;
	_installed = false;
	_mouseMoved = false;
	
	_mouseMovedMonitor = nullptr;
	_leftMouseDraggedMonitor = nullptr;
	_rightMouseDraggedMonitor = nullptr;
	_leftMouseDownMonitor = nullptr;
	_rightMouseDownMonitor = nullptr;
	_leftMouseUpMonitor = nullptr;
	_rightMouseUpMonitor = nullptr;
	
	_supportedEvents = {
//		{"onMouseMoved",		""},
		{"onLeftMouseDragged",	""},
		{"onLeftMouseDown",		""},
		{"onLeftMouseUp",		""},
		{"onLeftMouseClick",	""},
		{"onRightMouseDragged", ""},
		{"onRightMouseDown",	""},
		{"onRightMouseUp",		""},
		{"onRightMouseClick",	""},
		{"onUpdateScene",		""}
	};
}

AnimaMouseInteractor::AnimaMouseInteractor(const AnimaMouseInteractor& src)
{
	_windowId = src._windowId;
	_engine = src._engine;
	_installed = src._installed;
	_mouseMoved = src._mouseMoved;
	_lastMousePosition = src._lastMousePosition;
	
	_mouseMovedMonitor = src._mouseMovedMonitor;
	_leftMouseDraggedMonitor = src._leftMouseDraggedMonitor;
	_rightMouseDraggedMonitor = src._rightMouseDraggedMonitor;
	_leftMouseDownMonitor = src._leftMouseDownMonitor;
	_rightMouseDownMonitor = src._rightMouseDownMonitor;
	_leftMouseUpMonitor = src._leftMouseUpMonitor;
	_rightMouseUpMonitor = src._rightMouseUpMonitor;
}

AnimaMouseInteractor::AnimaMouseInteractor(AnimaMouseInteractor&& src)
{
	_windowId = src._windowId;
	_engine = src._engine;
	_installed = src._installed;
	_mouseMoved = src._mouseMoved;
	_lastMousePosition = src._lastMousePosition;
	
	_mouseMovedMonitor = src._mouseMovedMonitor;
	_leftMouseDraggedMonitor = src._leftMouseDraggedMonitor;
	_rightMouseDraggedMonitor = src._rightMouseDraggedMonitor;
	_leftMouseDownMonitor = src._leftMouseDownMonitor;
	_rightMouseDownMonitor = src._rightMouseDownMonitor;
	_leftMouseUpMonitor = src._leftMouseUpMonitor;
	_rightMouseUpMonitor = src._rightMouseUpMonitor;
}

AnimaMouseInteractor::~AnimaMouseInteractor()
{
	Remove();
}

AnimaMouseInteractor& AnimaMouseInteractor::operator=(const AnimaMouseInteractor& src)
{
	if(this != &src)
	{
		_windowId = src._windowId;
		_engine = src._engine;
		_installed = src._installed;
		_mouseMoved = src._mouseMoved;
		_lastMousePosition = src._lastMousePosition;
		
		_mouseMovedMonitor = src._mouseMovedMonitor;
		_leftMouseDraggedMonitor = src._leftMouseDraggedMonitor;
		_rightMouseDraggedMonitor = src._rightMouseDraggedMonitor;
		_leftMouseDownMonitor = src._leftMouseDownMonitor;
		_rightMouseDownMonitor = src._rightMouseDownMonitor;
		_leftMouseUpMonitor = src._leftMouseUpMonitor;
		_rightMouseUpMonitor = src._rightMouseUpMonitor;
	}
	
	return *this;
}

AnimaMouseInteractor& AnimaMouseInteractor::operator=(AnimaMouseInteractor&& src)
{
	if(this != &src)
	{
		_windowId = src._windowId;
		_engine = src._engine;
		_installed = src._installed;
		_mouseMoved = src._mouseMoved;
		_lastMousePosition = src._lastMousePosition;
		
		_mouseMovedMonitor = src._mouseMovedMonitor;
		_leftMouseDraggedMonitor = src._leftMouseDraggedMonitor;
		_rightMouseDraggedMonitor = src._rightMouseDraggedMonitor;
		_leftMouseDownMonitor = src._leftMouseDownMonitor;
		_rightMouseDownMonitor = src._rightMouseDownMonitor;
		_leftMouseUpMonitor = src._leftMouseUpMonitor;
		_rightMouseUpMonitor = src._rightMouseUpMonitor;
	}
	
	return *this;
}

bool AnimaMouseInteractor::Install(long windowId, AnimaEngine* engine)
{
	AnimaLogger::LogMessage("Installing mouse interactor...");
	
	if(_installed)
	{
		AnimaLogger::LogMessage("Mouse interactor already installed");
		return true;
	}
	
	_windowId = windowId;
	_engine = engine;
	
	_mouseMovedMonitor = [NSEvent addLocalMonitorForEventsMatchingMask:NSMouseMovedMask handler:^NSEvent* (NSEvent* event) {
		MouseMovedCallback(this, event);
		return event;
	}];
	
	_leftMouseDraggedMonitor = [NSEvent addLocalMonitorForEventsMatchingMask:NSLeftMouseDraggedMask handler:^NSEvent* (NSEvent* event) {
		LeftMouseDraggedCallback(this, event);
		return event;
	}];
	
	_rightMouseDraggedMonitor = [NSEvent addLocalMonitorForEventsMatchingMask:NSRightMouseDraggedMask handler:^NSEvent* (NSEvent* event) {
		RightMouseDraggedCallback(this, event);
		return event;
	}];
	
	_leftMouseDownMonitor = [NSEvent addLocalMonitorForEventsMatchingMask:NSLeftMouseDownMask handler:^NSEvent* (NSEvent* event) {
		LeftMouseDownCallback(this, event);
		return event;
	}];
	
	_rightMouseDownMonitor = [NSEvent addLocalMonitorForEventsMatchingMask:NSRightMouseDownMask handler:^NSEvent* (NSEvent* event) {
		RightMouseDownCallback(this, event);
		return event;
	}];
	
	_leftMouseUpMonitor = [NSEvent addLocalMonitorForEventsMatchingMask:NSLeftMouseUpMask handler:^NSEvent* (NSEvent* event) {
		LeftMouseUpCallback(this, event);
		return event;
	}];
	
	_rightMouseUpMonitor = [NSEvent addLocalMonitorForEventsMatchingMask:NSRightMouseUpMask	handler:^NSEvent* (NSEvent* event) {
		RightMouseUpCallback(this, event);
		return event;
	}];
	
	AnimaLogger::LogMessage("Mouse interactor installed succesfully");
	
	_installed = true;
	return true;
}

bool AnimaMouseInteractor::Remove()
{
	AnimaLogger::LogMessage("Removing mouse interactor...");
	
	if(_installed == false)
	{
		AnimaLogger::LogMessage("Mouse interactor wasn't installed");
		return true;
	}
	
	[NSEvent removeMonitor:_mouseMovedMonitor];
	[NSEvent removeMonitor:_leftMouseDraggedMonitor];
	[NSEvent removeMonitor:_rightMouseDraggedMonitor];
	[NSEvent removeMonitor:_leftMouseDownMonitor];
	[NSEvent removeMonitor:_rightMouseDownMonitor];
	[NSEvent removeMonitor:_leftMouseUpMonitor];
	[NSEvent removeMonitor:_rightMouseUpMonitor];
	
	_windowId = 0;
	
	AnimaLogger::LogMessage("Mouse interactor removed succesfully");
	
	_installed = false;
	return true;
}

bool AnimaMouseInteractor::SetEventHandler(const AnimaString& eventName, std::function<void (AnimaEventArgs* eventArgs)> handler)
{
	if(EventIsSupported(eventName))
	{
		_eventsMap[eventName] = handler;
		return true;
	}
	return false;
}

bool AnimaMouseInteractor::LaunchEvent(const AnimaString& eventName, AnimaEventArgs* eventArgs) const
{
	auto func = _eventsMap.find(eventName);
	if(func != _eventsMap.end() && func->second != nullptr)
	{
		func->second(eventArgs);
		return true;
	}
	
	return false;
}

AnimaArray<AnimaEventInfo> AnimaMouseInteractor::GetSupportedEvents() const
{
	return _supportedEvents;
}

bool AnimaMouseInteractor::EventIsSupported(const AnimaString& eventName) const
{
	for(auto item : _supportedEvents)
	{
		if(item._eventName == eventName)
			return true;
	}
	
	return false;
}

void AnimaMouseInteractor::MouseMovedCallback(AnimaMouseInteractor* interactor, NSEvent* event)
{
//	if(interactor == nullptr || event == nullptr || interactor->_windowId == 0)
//		return;
//	
//	NSView* view = (NSView*)interactor->_windowId;
//	NSPoint point = [event locationInWindow];
//	NSRect rect = [view bounds];
//	NSRect frame = [view frame];
//	
//	point = [view convertPoint:point fromView:view];
//	
//	interactor->_mouseMoved = true;
//	
////	// Siccome dalla documentazione di NSEvent il punto ha la coordinata Y che parte da 1.0 invece che da 0.0
////	// sottraggo 1.0
////	point.y -= 1.0f;
//	
//	// Lancio il messaggio solo se in punto è dentro la finestra
//	if(NSPointInRect(point, rect))
//	{
//		AnimaEventArgs* args = new AnimaMouseEventArgs(interactor, AnimaVertex2f(point.x, point.y));
//		interactor->LaunchEvent("onMouseMoved", args);
//		
//		delete args;
//		args = nullptr;
//	}
}

void AnimaMouseInteractor::LeftMouseDraggedCallback(AnimaMouseInteractor* interactor, NSEvent* event)
{
	if(interactor == nullptr || event == nullptr || interactor->_windowId == 0)
		return;
	
	AInt modifiers = TranslateFlags([event modifierFlags]);
	NSView* view = (NSView*)interactor->_windowId;
	NSPoint point = [event locationInWindow];
	NSRect rect = [view bounds];
	point = [view convertPoint:point fromView:nil];
	
	AnimaVertex2f pt(point.x, point.y);
	AnimaVertex2f delta = interactor->_lastMousePosition - pt;
	AnimaVertex2f size(rect.size.width, rect.size.height);

	interactor->_lastMousePosition = pt;
	interactor->_mouseMoved = true;
	
	//	// Siccome dalla documentazione di NSEvent il punto ha la coordinata Y che parte da 1.0 invece che da 0.0
	//	// sottraggo 1.0
	//	point.y -= 1.0f;
	
	// Lancio il messaggio solo se in punto è dentro la finestra
	if(NSPointInRect(point, rect))
	{
		AnimaEventArgs* args = new AnimaMouseDraggedEventArgs(interactor, pt, size, modifiers, delta);
		interactor->LaunchEvent("onLeftMouseDragged", args);
		
		delete args;
		args = nullptr;
	}
}

void AnimaMouseInteractor::RightMouseDraggedCallback(AnimaMouseInteractor* interactor, NSEvent* event)
{
	if(interactor == nullptr || event == nullptr || interactor->_windowId == 0)
		return;
	
	AInt modifiers = TranslateFlags([event modifierFlags]);
	NSView* view = (NSView*)interactor->_windowId;
	NSPoint point = [event locationInWindow];
	NSRect rect = [view bounds];
	point = [view convertPoint:point fromView:nil];
	
	AnimaVertex2f pt(point.x, point.y);
	AnimaVertex2f delta = interactor->_lastMousePosition - pt;
	AnimaVertex2f size(rect.size.width, rect.size.height);
	
	interactor->_lastMousePosition = pt;
	interactor->_mouseMoved = true;
	
	//	// Siccome dalla documentazione di NSEvent il punto ha la coordinata Y che parte da 1.0 invece che da 0.0
	//	// sottraggo 1.0
	//	point.y -= 1.0f;
	
	// Lancio il messaggio solo se in punto è dentro la finestra
	if(NSPointInRect(point, rect))
	{
		AnimaEventArgs* args = new AnimaMouseDraggedEventArgs(interactor, pt, size, modifiers, delta);
		interactor->LaunchEvent("onRightMouseDragged", args);
		
		delete args;
		args = nullptr;
	}
}

void AnimaMouseInteractor::LeftMouseDownCallback(AnimaMouseInteractor* interactor, NSEvent* event)
{
	if(interactor == nullptr || event == nullptr || interactor->_windowId == 0)
		return;
	
	AInt modifiers = TranslateFlags([event modifierFlags]);
	NSView* view = (NSView*)interactor->_windowId;
	NSPoint point = [event locationInWindow];
	NSRect rect = [view bounds];
	point = [view convertPoint:point fromView:nil];
	
	AnimaVertex2f pt(point.x, point.y);
	AnimaVertex2f size(rect.size.width, rect.size.height);
	
	interactor->_lastMousePosition = pt;
	interactor->_mouseMoved = false;
	
	//	// Siccome dalla documentazione di NSEvent il punto ha la coordinata Y che parte da 1.0 invece che da 0.0
	//	// sottraggo 1.0
	//	point.y -= 1.0f;
	
	// Lancio il messaggio solo se in punto è dentro la finestra
	if(NSPointInRect(point, rect))
	{
		AnimaEventArgs* args = new AnimaMouseEventArgs(interactor, pt, size, modifiers);
		interactor->LaunchEvent("onLeftMouseDown", args);
		
		delete args;
		args = nullptr;
	}
}

void AnimaMouseInteractor::RightMouseDownCallback(AnimaMouseInteractor* interactor, NSEvent* event)
{
	if(interactor == nullptr || event == nullptr || interactor->_windowId == 0)
		return;
	
	AInt modifiers = TranslateFlags([event modifierFlags]);
	NSView* view = (NSView*)interactor->_windowId;
	NSPoint point = [event locationInWindow];
	NSRect rect = [view bounds];
	point = [view convertPoint:point fromView:nil];
	
	AnimaVertex2f pt(point.x, point.y);
	AnimaVertex2f size(rect.size.width, rect.size.height);
	
	interactor->_lastMousePosition = pt;
	interactor->_mouseMoved = false;
	
	//	// Siccome dalla documentazione di NSEvent il punto ha la coordinata Y che parte da 1.0 invece che da 0.0
	//	// sottraggo 1.0
	//	point.y -= 1.0f;
	
	// Lancio il messaggio solo se in punto è dentro la finestra
	if(NSPointInRect(point, rect))
	{
		AnimaEventArgs* args = new AnimaMouseEventArgs(interactor, pt, size, modifiers);
		interactor->LaunchEvent("onRightMouseDown", args);
		
		delete args;
		args = nullptr;
	}
}

void AnimaMouseInteractor::LeftMouseUpCallback(AnimaMouseInteractor* interactor, NSEvent* event)
{
	if(interactor == nullptr || event == nullptr || interactor->_windowId == 0)
		return;
	
	AInt modifiers = TranslateFlags([event modifierFlags]);
	NSView* view = (NSView*)interactor->_windowId;
	NSPoint point = [event locationInWindow];
	NSRect rect = [view bounds];
	point = [view convertPoint:point fromView:nil];
	
	AnimaVertex2f pt(point.x, point.y);
	AnimaVertex2f size(rect.size.width, rect.size.height);
	
	interactor->_lastMousePosition = pt;
	
	//	// Siccome dalla documentazione di NSEvent il punto ha la coordinata Y che parte da 1.0 invece che da 0.0
	//	// sottraggo 1.0
	//	point.y -= 1.0f;
	
	// Lancio il messaggio solo se in punto è dentro la finestra
	if(NSPointInRect(point, rect))
	{
		AnimaEventArgs* argsUp = new AnimaMouseEventArgs(interactor, pt, size, modifiers);
		interactor->LaunchEvent("onLeftMouseUp", argsUp);
		
		delete argsUp;
		argsUp = nullptr;
		
		if(interactor->_mouseMoved == false)
		{
			AnimaEventArgs* argsClick = new AnimaMouseEventArgs(interactor, pt, size, modifiers);
			interactor->LaunchEvent("onLeftMouseClick", argsClick);
			
			delete argsClick;
			argsClick = nullptr;
		}
	}
}

void AnimaMouseInteractor::RightMouseUpCallback(AnimaMouseInteractor* interactor, NSEvent* event)
{
	if(interactor == nullptr || event == nullptr || interactor->_windowId == 0)
		return;
	
	AInt modifiers = TranslateFlags([event modifierFlags]);
	NSView* view = (NSView*)interactor->_windowId;
	NSPoint point = [event locationInWindow];
	NSRect rect = [view bounds];
	point = [view convertPoint:point fromView:nil];
	
	AnimaVertex2f pt(point.x, point.y);
	AnimaVertex2f size(rect.size.width, rect.size.height);
	
	interactor->_lastMousePosition = pt;
	
	//	// Siccome dalla documentazione di NSEvent il punto ha la coordinata Y che parte da 1.0 invece che da 0.0
	//	// sottraggo 1.0
	//	point.y -= 1.0f;
	
	// Lancio il messaggio solo se in punto è dentro la finestra
	if(NSPointInRect(point, rect))
	{
		AnimaEventArgs* argsUp = new AnimaMouseEventArgs(interactor, pt, size, modifiers);
		interactor->LaunchEvent("onRightMouseUp", argsUp);
		
		delete argsUp;
		argsUp = nullptr;
		
		if(interactor->_mouseMoved == false)
		{
			AnimaEventArgs* argsClick = new AnimaMouseEventArgs(interactor, pt, size, modifiers);
			interactor->LaunchEvent("onRightMouseClick", argsClick);
			
			delete argsClick;
			argsClick = nullptr;
		}
	}
}

void AnimaMouseInteractor::UpdateScene(AnimaScene* scene, AFloat elapsedTime)
{
	AnimaEventArgs* args = new AnimaUpdateSceneEventArgs(this, scene, elapsedTime);
	
	LaunchEvent("onUpdateScene", args);
	
	delete args;
	args = nullptr;
}

END_ANIMA_ENGINE_NAMESPACE