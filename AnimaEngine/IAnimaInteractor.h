//
//  IAnimaInteractor.h
//  Anima
//
//  Created by Marco Zille on 03/11/15.
//
//
#pragma once

#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "IAnimaEvents.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaScene;

class ANIMA_ENGINE_EXPORT AnimaUpdateSceneEventArgs : public AnimaEventArgs
{
public:
	AnimaUpdateSceneEventArgs(IAnimaEvents* sourceEvent, AnimaScene* scene, AFloat elapsedTime)
		: AnimaEventArgs(sourceEvent)
	{
		_scene = scene;
		_elapsedTime = elapsedTime;
	}
	
public:
	AnimaScene* GetScene() { return _scene; }
	AFloat GetElapsedTime() const { return _elapsedTime; }
	
private:
	AnimaScene* _scene;
	AFloat _elapsedTime;
};


class ANIMA_ENGINE_EXPORT IAnimaInteractor
{
public:
	// L'override di questa funzione deve occuparsi di installare un hook sulla finestra passata in modo intercettare
	// il messaggio richiesto
	virtual bool Install(long windowId, AnimaEngine* engine) = 0;
	virtual bool Remove() = 0;
	virtual void UpdateScene(AnimaScene* scene, AFloat elapsedTime) = 0;
};

END_ANIMA_ENGINE_NAMESPACE
