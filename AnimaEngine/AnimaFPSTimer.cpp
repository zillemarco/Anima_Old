//
//  AnimaFPSTimer.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaFPSTimer.h"
#include "AnimaEngine.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaFPSTimer::AnimaFPSTimer()
{
	_fps = 0.0;
	_framesElapsed = 0;
}

AnimaFPSTimer::~AnimaFPSTimer()
{
}

void AnimaFPSTimer::Init()
{
	_time1 = _time2 = _timerStart = _clock::now();
}

void AnimaFPSTimer::Update()
{
	_framesElapsed++;

	_time2 = _clock::now();
	_diffTime = _time2 - _time1;
	
	_fps = 1.0f / _diffTime.count();

	_time1 = _clock::now();
}

double AnimaFPSTimer::GetElapsedSeconds()
{
	return _diffTime.count();
}

double AnimaFPSTimer::GetFPS()
{
	return _fps;
}

END_ANIMA_ENGINE_NAMESPACE
