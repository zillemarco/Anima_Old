#include "AnimaEngineCore.h"
//#include <QApplication>
#include <stdio.h>
#include <string>

#if defined _MSC_VER
#include <Windows.h>
#else
#include <unistd.h>
#endif

#if (defined ANIMA_FORCE_GRAPHICS_DEBUG) || (defined ANIMA_ENGINE_USE_NVTOOLS)
	void AnimaFrameDebugPush(const char* title)	{ nvtxRangePushA(title);	}
	void AnimaFrameDebugPop()					{ nvtxRangePop();			}
#else
	void AnimaFrameDebugPush(const char* title)	{ }
	void AnimaFrameDebugPop()					{ }
#endif