//
//  AnimaBenchmarkTimer.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaBenchmarkTimer.h"
#include "AnimaEngine.h"

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
	char szBuff[1024];
	memset(szBuff, 0, 1024);
	
	sprintf(szBuff, "%f seconds\r\n", Elapsed());
	#if defined _MSC_VER
		OutputDebugStringA(szBuff);
	#else
		puts(szBuff);
	#endif
}

END_ANIMA_ENGINE_NAMESPACE
