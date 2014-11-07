#include "AnimaEngineLib.h"
#include <QApplication>
#include <stdio.h>

#if defined _MSC_VER
#include <Windows.h>
#else
#include <unistd.h>
#endif

void __init_anina_app__(int argc, char** argv)
{
	QApplication app(argc, argv);
}

bool __anima_get_working_dir(char* dest, int length)
{
#if defined _MSC_VER
	HMODULE hModule = GetModuleHandle(NULL);
	if(hModule == NULL)
		return false;

	if(GetModuleFileName(hModule, dest, length) == 0)
		return false;

	char* pch = strrchr(dest, '\\');	
	if (pch == NULL)
		return false;

	dest[pch - dest + 1] = '\0';
	return true;
#else
	char szTmp[32];
	sprintf(szTmp, "/proc/%d/exe", getpid());
	int bytes = MIN(readlink(szTmp, dest, length), length - 1);
	if (bytes >= 0)
		dest[bytes] = '\0';
	return true;
#endif
}