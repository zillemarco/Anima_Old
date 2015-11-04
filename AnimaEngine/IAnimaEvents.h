//
//  IAnimaEvents.h
//  Anima
//
//  Created by Marco Zille on 03/11/15.
//
//
#pragma once

#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaArray.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class IAnimaEvents;

struct AnimaEventInfo
{
	AnimaString _eventName;
	AnimaString _eventDescription;
};

class ANIMA_ENGINE_EXPORT AnimaEventArgs
{
public:
	AnimaEventArgs(IAnimaEvents* sourceEvent)
	{
		ANIMA_ASSERT(sourceEvent != nullptr);
		_sourceEvent = sourceEvent;
	}
	
	IAnimaEvents* GetSourceEvent() { return _sourceEvent; }
	
private:
	IAnimaEvents* _sourceEvent;
};

class ANIMA_ENGINE_EXPORT IAnimaEvents
{
public:
	virtual bool SetEventHandler(const AnimaString& eventName, std::function<void (AnimaEventArgs* eventArgs)> handler) = 0;
	virtual bool LaunchEvent(const AnimaString& eventName, AnimaEventArgs* eventArgs) const = 0;
	virtual AnimaArray<AnimaEventInfo> GetSupportedEvents() const = 0;
	virtual bool EventIsSupported(const AnimaString& eventName) const = 0;
};

END_ANIMA_ENGINE_NAMESPACE
