//
//  IAnimaInteractor.cpp
//  Anima
//
//  Created by Marco Zille on 03/11/15.
//
//

#include "IAnimaInteractor.h"
#include "AnimaScene.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

#if defined WIN32

#if _MSC_VER >= 1300    // for VC 7.0
// from ATL 7.0 sources
#ifndef _delayimp_h
extern "C" IMAGE_DOS_HEADER __ImageBase;
#endif
#endif

HMODULE GetCurrentModule()
{
#if _MSC_VER < 1300    // earlier than .NET compiler (VC 6.0)

	// Here's a trick that will get you the handle of the module
	// you're running in without any a-priori knowledge:
	// http://www.dotnet247.com/247reference/msgs/13/65259.aspx

	MEMORY_BASIC_INFORMATION mbi;
	static int dummy;
	VirtualQuery(&dummy, &mbi, sizeof(mbi));

	return reinterpret_cast<HMODULE>(mbi.AllocationBase);

#else    // VC 7.0

	// from ATL 7.0 sources
	return reinterpret_cast<HMODULE>(&__ImageBase);
#endif
}

#endif

END_ANIMA_ENGINE_NAMESPACE