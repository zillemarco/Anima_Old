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

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT IAnimaInteractor
{
public:
	// L'override di questa funzione deve occuparsi di installare un hook sulla finestra passata in modo intercettare
	// il messaggio richiesto
	virtual bool Install(long windowId, AnimaEngine* engine) = 0;
	virtual bool Remove() = 0;
};

END_ANIMA_ENGINE_NAMESPACE
