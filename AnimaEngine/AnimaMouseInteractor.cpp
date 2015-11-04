//
//  AnimaMouseInteractor.cpp
//  Anima
//
//  Created by Marco Zille on 03/11/15.
//
//

#include "AnimaMouseInteractor.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaMouseInteractor::AnimaMouseInteractor()
{
	_windowId = 0;
}

AnimaMouseInteractor::AnimaMouseInteractor(const AnimaMouseInteractor& src)
{
	_windowId = src._windowId;
}

AnimaMouseInteractor::AnimaMouseInteractor(AnimaMouseInteractor&& src)
{
	_windowId = src._windowId;
}

AnimaMouseInteractor::~AnimaMouseInteractor()
{
	
}

AnimaMouseInteractor& AnimaMouseInteractor::operator=(const AnimaMouseInteractor& src)
{
	if(this != &src)
	{
		_windowId = src._windowId;
	}
	
	return *this;
}

AnimaMouseInteractor& AnimaMouseInteractor::operator=(AnimaMouseInteractor&& src)
{
	if(this != &src)
	{
		_windowId = src._windowId;
	}
	
	return *this;
}

END_ANIMA_ENGINE_NAMESPACE