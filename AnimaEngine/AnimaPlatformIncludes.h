//
//  AnimaPlatformIncludes.hpp
//  Anima
//
//  Created by Marco Zille on 30/11/15.
//
//

#ifndef AnimaPlatformIncludes_h
#define AnimaPlatformIncludes_h

#if defined _WIN32
#	include <GL/wglew.h>
#	include <windows.h>
#else
#	if defined __OBJC__
#		import <Cocoa/Cocoa.h>
#	else
#		include <ApplicationServices/ApplicationServices.h>
typedef void* id;
#	endif
#endif

#endif /* AnimaPlatformIncludes_h */
