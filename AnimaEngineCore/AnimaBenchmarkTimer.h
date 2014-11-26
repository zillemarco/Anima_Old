//
//  AnimaBenchmarkTimer.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaBenchmarkTimer__
#define __Anima__AnimaBenchmarkTimer__

#include <iostream>
#include <chrono>
#include "AnimaEngineCore.h"

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

class ANIMA_ENGINE_CORE_EXPORT AnimaBenchmarkTimer
{
public:
	AnimaBenchmarkTimer();
	
	void Reset();
	double Elapsed() const;
	void PrintElapsed();
	
private:
	typedef std::chrono::high_resolution_clock _clock;
	typedef std::chrono::duration<double, std::ratio<1> > _second;
	std::chrono::time_point<_clock> _beg;
};

END_ANIMA_ENGINE_CORE_NAMESPACE

#endif /* defined(__Anima__AnimaBenchmarkTimer__) */
