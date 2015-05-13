//
//  AnimaTimer.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaTimer.h"
#include "AnimaEngine.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaTimer::AnimaTimer()
{
	Reset();
}

void AnimaTimer::Reset()
{
	_beg = _clock::now();
}

double AnimaTimer::Elapsed() const
{
	return std::chrono::duration_cast<_second>(_clock::now() - _beg).count();
}

void AnimaTimer::PrintElapsed()
{
	char szBuff[1024];
	memset(szBuff, 0, 1024);
	
	sprintf(szBuff, "%.20f seconds\r\n", Elapsed());
	#if defined _MSC_VER
		OutputDebugStringA(szBuff);
	#else
		puts(szBuff);
	#endif
}

END_ANIMA_ENGINE_NAMESPACE
