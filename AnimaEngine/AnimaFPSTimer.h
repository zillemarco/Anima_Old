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
#include <iostream>
#include <chrono>

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaFPSTimer
{
public:
	AnimaFPSTimer(void);
	~AnimaFPSTimer(void);

public:
	void Init();
	void Update();
	double GetElapsedSeconds();
	double GetFPS();

private:
#pragma warning (disable: 4251)
	typedef std::chrono::high_resolution_clock _clock;
	typedef std::chrono::duration<double, std::ratio<1> > _second;

	std::chrono::time_point<_clock> _timerStart;
	std::chrono::time_point<_clock> _time1;
	std::chrono::time_point<_clock> _time2;
	std::chrono::duration<double>	_diffTime;
#pragma warning (default: 4251) 

	double	_fps;
	int		_framesElapsed;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaFPSTimer__) */
