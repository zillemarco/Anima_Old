//
//  AnimaFPSTimer.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaFPSTimer.h"
#include "AnimaEngine.h"

#ifdef WIN32
#	include <windows.h>
#	pragma comment(lib, "Winmm.lib")
#else
#endif

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaFPSTimer::AnimaFPSTimer()
{
	_fps = 0.0f;
	_time1 = 0.0f;
	_time2 = 0.0f;
	_diffTime = 0.0f;
	_framesElapsed = 0;
}

AnimaFPSTimer::~AnimaFPSTimer()
{
}

bool AnimaFPSTimer::Init()
{        
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&_frequency))
	{
		_performanceTimer = true;

		QueryPerformanceCounter((LARGE_INTEGER*)&_performanceTimerStart);

		_resolution = (float)(((double)1.0f) / ((double)_frequency));

		_performanceTimerElapsed = _performanceTimerStart;
	}
	else
	{
		_performanceTimer = false;

		_mmTimerStart = timeGetTime();
		_mmTimerElapsed = _mmTimerStart;
		_resolution = 1.0f / 1000.0f;
		_frequency = 1000;
	}

	return _performanceTimer;
}

void AnimaFPSTimer::Update()
{
	_framesElapsed++;

	if (_framesElapsed % 5 == 1)
		_time1 = GetTime() / 1000;

	else if (_framesElapsed % 5 == 0)
	{
		_time1 = _time2;
		_time2 = GetTime() / 1000;
		_diffTime = (float)fabs(_time2 - _time1);
	}

	_fps = 5.0f / (_diffTime);
}

float AnimaFPSTimer::GetTime()
{
	__int64 i64Time;

	if (_performanceTimer)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&i64Time);

		return ((float)(i64Time - _performanceTimerStart) * _resolution) * 1000.0f;
	}
	else
	{
		return ((float)(timeGetTime() - _mmTimerStart) * _resolution) * 1000.0f;
	}
}

float AnimaFPSTimer::GetElapsedSeconds()
{
	return _diffTime;
}

float AnimaFPSTimer::GetFPS()
{
	return _fps;
}

END_ANIMA_ENGINE_NAMESPACE
