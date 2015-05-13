//
//  AnimaTimer.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaTimer__
#define __Anima__AnimaTimer__

#include <iostream>
#include <chrono>
#include "AnimaEngineCore.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaTimer
{
public:
	AnimaTimer();
	
	void Reset();
	double Elapsed() const;
	void PrintElapsed();
	
private:
#pragma warning (disable: 4251) 
	typedef std::chrono::high_resolution_clock _clock;
	typedef std::chrono::duration<double, std::ratio<1> > _second;
	std::chrono::time_point<_clock> _beg;
#pragma warning (default: 4251) 
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaTimer__) */
