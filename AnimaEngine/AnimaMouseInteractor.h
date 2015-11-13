//
//  AnimaMouseInteractor.h
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

#if defined WIN32
#	include <Windows.h>
#	include <Windowsx.h>
#endif

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaScene;

class ANIMA_ENGINE_EXPORT AnimaMouseEventArgs : public AnimaEventArgs
{
public:
	AnimaMouseEventArgs(IAnimaEvents* sourceEvent, AnimaVertex2f point, AnimaVertex2f windowSize, AInt modifiers)
		: AnimaEventArgs(sourceEvent)
	{
		_point = point;
		_windowSize = windowSize;
		_modifiers = modifiers;
	}
	
public:
	AnimaVertex2f GetPoint() const { return _point; }
	AnimaVertex2f GetWindowSize() const { return _windowSize; }
	AInt GetModifiers() const { return _modifiers; }
	
private:
	AnimaVertex2f _point;
	AnimaVertex2f _windowSize;
	AInt _modifiers;
};

class ANIMA_ENGINE_EXPORT AnimaMouseDraggedEventArgs : public AnimaMouseEventArgs
{
public:
	AnimaMouseDraggedEventArgs(IAnimaEvents* sourceEvent, AnimaVertex2f point, AnimaVertex2f windowSize, AInt modifiers, AnimaVertex2f delta)
		: AnimaMouseEventArgs(sourceEvent, point, windowSize, modifiers)
	{
		_delta = delta;
	}
	
public:
	AnimaVertex2f GetDelta() const { return _delta; }
	
private:
	AnimaVertex2f _delta;
};

class ANIMA_ENGINE_EXPORT AnimaMouseInteractor : public IAnimaEvents, public IAnimaInteractor
{
public:
	AnimaMouseInteractor();
	AnimaMouseInteractor(const AnimaMouseInteractor& src);
	AnimaMouseInteractor(AnimaMouseInteractor&& src);
	virtual ~AnimaMouseInteractor();
	
	AnimaMouseInteractor& operator=(const AnimaMouseInteractor& src);
	AnimaMouseInteractor& operator=(AnimaMouseInteractor&& src);
	
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
	
private:
#if defined WIN32
	static LRESULT CALLBACK WindowsProcCallback(int nCode, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK MessageProcCallback(int nCode, WPARAM wParam, LPARAM lParam);
	static void HandleMessage(HWND hWnd, AUint message, WPARAM wParam, LPARAM lParam, AnimaMouseInteractor* interactor);
#else
	static void MouseMovedCallback(AnimaMouseInteractor* interactor, NSEvent* event);
	static void LeftMouseDraggedCallback(AnimaMouseInteractor* interactor, NSEvent* event);
	static void RightMouseDraggedCallback(AnimaMouseInteractor* interactor, NSEvent* event);
	static void LeftMouseDownCallback(AnimaMouseInteractor* interactor, NSEvent* event);
	static void RightMouseDownCallback(AnimaMouseInteractor* interactor, NSEvent* event);
	static void LeftMouseUpCallback(AnimaMouseInteractor* interactor, NSEvent* event);
	static void RightMouseUpCallback(AnimaMouseInteractor* interactor, NSEvent* event);
#endif
	
protected:
	bool _installed;
	long _windowId;
	AnimaEngine* _engine;
	
	bool _mouseMoved;
	AnimaVertex2f _lastMousePosition;
	
	AnimaArray<AnimaEventInfo> _supportedEvents;
	
#if defined WIN32
	static boost::unordered_map<long, AnimaMouseInteractor*> _installedInteractors;
	HHOOK _windowsProcHook;
	HHOOK _messageProcHook;
#else
	id _mouseMovedMonitor;
	id _leftMouseDraggedMonitor;
	id _rightMouseDraggedMonitor;
	id _leftMouseDownMonitor;
	id _rightMouseDownMonitor;
	id _leftMouseUpMonitor;
	id _rightMouseUpMonitor;
#endif
	
#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, std::function<void (AnimaEventArgs* eventArgs)>, AnimaStringHasher> _eventsMap;
#pragma warning (default: 4251)
};

END_ANIMA_ENGINE_NAMESPACE
