//
//  AnimaBenchmarkTimer.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaBenchmarkTimer.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaBenchmarkTimer::AnimaBenchmarkTimer()
{
	Reset();
}

void AnimaBenchmarkTimer::Reset()
{
	_beg = _clock::now();
}

double AnimaBenchmarkTimer::Elapsed() const
{
	return std::chrono::duration_cast<_second>(_clock::now() - _beg).count();
}

void AnimaBenchmarkTimer::PrintElapsed()
{
	printf("%f seconds", Elapsed());
}

END_ANIMA_ENGINE_NAMESPACE
