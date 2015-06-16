#include "AnimaEngineCore.h"
//#include <QApplication>
#include <stdio.h>
#include <string>

#if defined _MSC_VER
#include <Windows.h>
#else
#include <unistd.h>
#endif

//void __init_anina_app__(int argc, char** argv)
//{
//	QApplication app(argc, argv);
//}

bool __anima_get_working_dir(char* dest, int length)
{
#if defined _MSC_VER
	//HMODULE hModule = GetModuleHandle(NULL);
	//if(hModule == NULL)
	//	return false;

	//if(GetModuleFileName(hModule, dest, length) == 0)
	//	return false;

	//char* pch = strrchr(dest, '\\');	
	//if (pch == NULL)
	//	return false;

	//dest[pch - dest + 1] = '\0';
	return true;
#else
	char* cwd;
	if((cwd = getcwd(dest, length)) == NULL)
		return false;
	
	int cwdLen = strlen(cwd);
	
	if(cwdLen == length)
		return false;
	
	dest[cwdLen] = '/';
	dest[cwdLen + 1] = '\0';
	
	return true;
#endif
}

#if (defined ANIMA_FORCE_GRAPHICS_DEBUG) || (defined _DEBUG && ANIMA_ENGINE_USE_NVTOOLS)
	void AnimaFrameDebugPush(const char* title)	{ nvtxRangePushA(title);	}
	void AnimaFrameDebugPop()					{ nvtxRangePop();			}
#else
	void AnimaFrameDebugPush(const char* title)	{ }
	void AnimaFrameDebugPop()					{ }
#endif