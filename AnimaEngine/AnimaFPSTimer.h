//
//  AnimaFPSTimer.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaFPSTimer__
#define __Anima__AnimaFPSTimer__

#include "AnimaEngineCore.h"
#include "AnimaTypes.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaFPSTimer
{
public:
	AnimaFPSTimer(void);
	~AnimaFPSTimer(void);

public:
	bool Init();
	void Update();
	float GetTime();
	float GetElapsedSeconds();
	float GetFPS();

private:
#ifdef WIN32
	__int64	_performanceTimerStart;
	__int64	_performanceTimerElapsed;

	unsigned long _mmTimerElapsed;
	unsigned long _mmTimerStart;

	__int64	_frequency;
	float	_resolution;
	bool	_performanceTimer;

	float	_time1;
	float	_time2;
	float	_diffTime;
#else
#endif

	float	_fps;
	int		_framesElapsed;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaFPSTimer__) */
