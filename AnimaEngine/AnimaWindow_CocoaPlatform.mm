#include "AnimaWindow.h"
#include "AnimaEngine.h"

#include <string.h>
#include <crt_externs.h>
#include <stdlib.h>
#include <limits.h>

#include <IOKit/graphics/IOGraphicsLib.h>
#include <IOKit/graphics/IOGraphicsLib.h>
#include <CoreVideo/CVBase.h>
#include <CoreVideo/CVDisplayLink.h>
#include <ApplicationServices/ApplicationServices.h>
#include <sys/param.h>
#include <mach/mach_time.h>

BEGIN_ANIMA_ENGINE_NAMESPACE

static void centerCursor(AnimaEngineWindow_Base *window)
{
	int width, height;
	_AnimaEngineWindowPlatformGetWindowSize(window, &width, &height);
	_AnimaEngineWindowPlatformSetCursorPos(window, width / 2.0, height / 2.0);
}

static NSCursor* getModeCursor(AnimaEngineWindow_Base* window)
{
	if (window->GetCursorMode() == ANIMA_ENGINE_CURSOR_NORMAL)
	{
		if (window->GetCursor())
			return (NSCursor*) window->GetCursor()->ns._object;
		else
			return [NSCursor arrowCursor];
	}
	else
		return (NSCursor*) AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_WINDOW_STATE->_cursor;
}

static void updateModeCursor(AnimaEngineWindow_Base* window)
{
	// This is required for the cursor to update if it's inside the window
	[getModeCursor(window) set];
	
	// This is required for the cursor to update if it's outside the window
	[window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_object invalidateCursorRectsForView:window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_view];
}

static bool enterFullscreenMode(AnimaEngineWindow_Base* window)
{
	bool status;
	
	status = _AnimaEngineWindowSetVideoMode(window->GetMonitor(), window->GetVideoMode());
	
	// NOTE: The window is resized despite mode setting failure to make
	//       glfwSetWindowSize more robust
	[window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_object setFrame:[window->GetMonitor()->ns._screen frame] display:YES];
	
	return status;
}

static void leaveFullscreenMode(AnimaEngineWindow_Base* window)
{
	_AnimaEngineWindowRestoreVideoMode(window->GetMonitor());
}

static float transformY(float y)
{
	const float height = CGDisplayBounds(CGMainDisplayID()).size.height;
	return height - y;
}

static NSRect convertRectToBacking(AnimaEngineWindow_Base* window, NSRect contentRect)
{
#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
	if (floor(NSAppKitVersionNumber) > NSAppKitVersionNumber10_6)
		return [window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_view convertRectToBacking:contentRect];
	else
#endif
		return contentRect;
}

END_ANIMA_ENGINE_NAMESPACE

@interface AnimaEngineWindowWindowDelegate : NSObject
{
	Anima::AnimaEngineWindow_Base* window;
}

- (id)initWithAnimaEngineWindowWindow:(Anima::AnimaEngineWindow_Base *)initWindow;

@end

@implementation AnimaEngineWindowWindowDelegate

- (id)initWithAnimaEngineWindowWindow:(Anima::AnimaEngineWindow_Base*)initWindow
{
	self = [super init];
	if (self != nil)
		window = initWindow;
	
	return self;
}

- (BOOL)windowShouldClose:(id)sender
{
	if(window->GetCloseRequestCallback())
		window->GetCloseRequestCallback()(window);
	return NO;
}

- (void)windowDidResize:(NSNotification *)notification
{
	[window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_context update];
	
	const NSRect contentRect = [window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_view frame];
	const NSRect fbRect = convertRectToBacking(window, contentRect);

	if(window->GetFramebufferSizeCallback())
		window->GetFramebufferSizeCallback()(window, fbRect.size.width, fbRect.size.height);

	if(window->GetSizeCallback())
		window->GetSizeCallback()(window, contentRect.size.width, contentRect.size.height);

	if(window->GetDamageCallback())
		window->GetDamageCallback()(window);
	
	if (window->GetCursorMode()	== ANIMA_ENGINE_CURSOR_DISABLED)
		centerCursor(window);
}

- (void)windowDidMove:(NSNotification *)notification
{
	[window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_context update];
	
	int x, y;
	_AnimaEngineWindowPlatformGetWindowPos(window, &x, &y);
	
	if(window->GetPosCallback())
		window->GetPosCallback()(window, x, y);
	
	if (window->GetCursorMode() == ANIMA_ENGINE_CURSOR_DISABLED)
		centerCursor(window);
}

- (void)windowDidMiniaturize:(NSNotification *)notification
{
	if(window->GetIconifyCallback())
		window->GetIconifyCallback()(window, true);
}

- (void)windowDidDeminiaturize:(NSNotification *)notification
{
	if (window->GetMonitor())
		enterFullscreenMode(window);
	
	if(window->GetIconifyCallback())
		window->GetIconifyCallback()(window, false);
}

- (void)windowDidBecomeKey:(NSNotification *)notification
{
	if(window->GetFocusCallback())
		window->GetFocusCallback()(window, true);
	
	_AnimaEngineWindowPlatformApplyCursorMode(window);
}

- (void)windowDidResignKey:(NSNotification *)notification
{
	if(window->GetFocusCallback())
		window->GetFocusCallback()(window, false);
}

@end

@interface AnimaEngineWindowApplicationDelegate : NSObject
@end

@implementation AnimaEngineWindowApplicationDelegate

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender :(Anima::AnimaEngine*) engine
{
	Anima::AnimaEngineWindow_Base* window;
	
	for (window = engine->GetWindowListHead();  window;  window = window->GetNext())
	{
		if(window->GetCloseRequestCallback())
			window->GetCloseRequestCallback()(window);
	}
	
	return NSTerminateCancel;
}

- (void)applicationDidHide:(NSNotification *)notification :(Anima::AnimaEngine*) engine
{
	Anima::AnimaEngineWindow_Base* window;
	
	for (window = engine->GetWindowListHead();  window;  window = window->GetNext())
	{
		if(window->GetVisibilityCallback())
			window->GetVisibilityCallback()(window, false);
	}
}

- (void)applicationDidUnhide:(NSNotification *)notification :(Anima::AnimaEngine*) engine
{
	Anima::AnimaEngineWindow_Base* window;
	
	for (window = engine->GetWindowListHead();  window;  window = window->GetNext())
	{
		if ([window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_object isVisible])
		{
			if(window->GetVisibilityCallback())
				window->GetVisibilityCallback()(window, true);
		}
	}
}

- (void)applicationDidChangeScreenParameters:(NSNotification *) notification
{
	// TODO _glfwInputMonitorChange();
}

@end

BEGIN_ANIMA_ENGINE_NAMESPACE

static int translateFlags(NSUInteger flags)
{
	int mods = 0;
	
	if (flags & NSShiftKeyMask)
		mods |= ANIMA_ENGINE_MOD_SHIFT;
	if (flags & NSControlKeyMask)
		mods |= ANIMA_ENGINE_MOD_CONTROL;
	if (flags & NSAlternateKeyMask)
		mods |= ANIMA_ENGINE_MOD_ALT;
	if (flags & NSCommandKeyMask)
		mods |= ANIMA_ENGINE_MOD_SUPER;
	
	return mods;
}

static int translateKey(unsigned int key)
{
	// Keyboard symbol translation table
	static const unsigned int table[128] =
	{
		/* 00 */ ANIMA_ENGINE_KEY_A,
		/* 01 */ ANIMA_ENGINE_KEY_S,
		/* 02 */ ANIMA_ENGINE_KEY_D,
		/* 03 */ ANIMA_ENGINE_KEY_F,
		/* 04 */ ANIMA_ENGINE_KEY_H,
		/* 05 */ ANIMA_ENGINE_KEY_G,
		/* 06 */ ANIMA_ENGINE_KEY_Z,
		/* 07 */ ANIMA_ENGINE_KEY_X,
		/* 08 */ ANIMA_ENGINE_KEY_C,
		/* 09 */ ANIMA_ENGINE_KEY_V,
		/* 0a */ ANIMA_ENGINE_KEY_WORLD_1,
		/* 0b */ ANIMA_ENGINE_KEY_B,
		/* 0c */ ANIMA_ENGINE_KEY_Q,
		/* 0d */ ANIMA_ENGINE_KEY_W,
		/* 0e */ ANIMA_ENGINE_KEY_E,
		/* 0f */ ANIMA_ENGINE_KEY_R,
		/* 10 */ ANIMA_ENGINE_KEY_Y,
		/* 11 */ ANIMA_ENGINE_KEY_T,
		/* 12 */ ANIMA_ENGINE_KEY_1,
		/* 13 */ ANIMA_ENGINE_KEY_2,
		/* 14 */ ANIMA_ENGINE_KEY_3,
		/* 15 */ ANIMA_ENGINE_KEY_4,
		/* 16 */ ANIMA_ENGINE_KEY_6,
		/* 17 */ ANIMA_ENGINE_KEY_5,
		/* 18 */ ANIMA_ENGINE_KEY_EQUAL,
		/* 19 */ ANIMA_ENGINE_KEY_9,
		/* 1a */ ANIMA_ENGINE_KEY_7,
		/* 1b */ ANIMA_ENGINE_KEY_MINUS,
		/* 1c */ ANIMA_ENGINE_KEY_8,
		/* 1d */ ANIMA_ENGINE_KEY_0,
		/* 1e */ ANIMA_ENGINE_KEY_RIGHT_BRACKET,
		/* 1f */ ANIMA_ENGINE_KEY_O,
		/* 20 */ ANIMA_ENGINE_KEY_U,
		/* 21 */ ANIMA_ENGINE_KEY_LEFT_BRACKET,
		/* 22 */ ANIMA_ENGINE_KEY_I,
		/* 23 */ ANIMA_ENGINE_KEY_P,
		/* 24 */ ANIMA_ENGINE_KEY_ENTER,
		/* 25 */ ANIMA_ENGINE_KEY_L,
		/* 26 */ ANIMA_ENGINE_KEY_J,
		/* 27 */ ANIMA_ENGINE_KEY_APOSTROPHE,
		/* 28 */ ANIMA_ENGINE_KEY_K,
		/* 29 */ ANIMA_ENGINE_KEY_SEMICOLON,
		/* 2a */ ANIMA_ENGINE_KEY_BACKSLASH,
		/* 2b */ ANIMA_ENGINE_KEY_COMMA,
		/* 2c */ ANIMA_ENGINE_KEY_SLASH,
		/* 2d */ ANIMA_ENGINE_KEY_N,
		/* 2e */ ANIMA_ENGINE_KEY_M,
		/* 2f */ ANIMA_ENGINE_KEY_PERIOD,
		/* 30 */ ANIMA_ENGINE_KEY_TAB,
		/* 31 */ ANIMA_ENGINE_KEY_SPACE,
		/* 32 */ ANIMA_ENGINE_KEY_GRAVE_ACCENT,
		/* 33 */ ANIMA_ENGINE_KEY_BACKSPACE,
		/* 34 */ ANIMA_ENGINE_KEY_UNKNOWN,
		/* 35 */ ANIMA_ENGINE_KEY_ESCAPE,
		/* 36 */ ANIMA_ENGINE_KEY_RIGHT_SUPER,
		/* 37 */ ANIMA_ENGINE_KEY_LEFT_SUPER,
		/* 38 */ ANIMA_ENGINE_KEY_LEFT_SHIFT,
		/* 39 */ ANIMA_ENGINE_KEY_CAPS_LOCK,
		/* 3a */ ANIMA_ENGINE_KEY_LEFT_ALT,
		/* 3b */ ANIMA_ENGINE_KEY_LEFT_CONTROL,
		/* 3c */ ANIMA_ENGINE_KEY_RIGHT_SHIFT,
		/* 3d */ ANIMA_ENGINE_KEY_RIGHT_ALT,
		/* 3e */ ANIMA_ENGINE_KEY_RIGHT_CONTROL,
		/* 3f */ ANIMA_ENGINE_KEY_UNKNOWN, /* Function */
		/* 40 */ ANIMA_ENGINE_KEY_F17,
		/* 41 */ ANIMA_ENGINE_KEY_KP_DECIMAL,
		/* 42 */ ANIMA_ENGINE_KEY_UNKNOWN,
		/* 43 */ ANIMA_ENGINE_KEY_KP_MULTIPLY,
		/* 44 */ ANIMA_ENGINE_KEY_UNKNOWN,
		/* 45 */ ANIMA_ENGINE_KEY_KP_ADD,
		/* 46 */ ANIMA_ENGINE_KEY_UNKNOWN,
		/* 47 */ ANIMA_ENGINE_KEY_NUM_LOCK, /* Really KeypadClear... */
		/* 48 */ ANIMA_ENGINE_KEY_UNKNOWN, /* VolumeUp */
		/* 49 */ ANIMA_ENGINE_KEY_UNKNOWN, /* VolumeDown */
		/* 4a */ ANIMA_ENGINE_KEY_UNKNOWN, /* Mute */
		/* 4b */ ANIMA_ENGINE_KEY_KP_DIVIDE,
		/* 4c */ ANIMA_ENGINE_KEY_KP_ENTER,
		/* 4d */ ANIMA_ENGINE_KEY_UNKNOWN,
		/* 4e */ ANIMA_ENGINE_KEY_KP_SUBTRACT,
		/* 4f */ ANIMA_ENGINE_KEY_F18,
		/* 50 */ ANIMA_ENGINE_KEY_F19,
		/* 51 */ ANIMA_ENGINE_KEY_KP_EQUAL,
		/* 52 */ ANIMA_ENGINE_KEY_KP_0,
		/* 53 */ ANIMA_ENGINE_KEY_KP_1,
		/* 54 */ ANIMA_ENGINE_KEY_KP_2,
		/* 55 */ ANIMA_ENGINE_KEY_KP_3,
		/* 56 */ ANIMA_ENGINE_KEY_KP_4,
		/* 57 */ ANIMA_ENGINE_KEY_KP_5,
		/* 58 */ ANIMA_ENGINE_KEY_KP_6,
		/* 59 */ ANIMA_ENGINE_KEY_KP_7,
		/* 5a */ ANIMA_ENGINE_KEY_F20,
		/* 5b */ ANIMA_ENGINE_KEY_KP_8,
		/* 5c */ ANIMA_ENGINE_KEY_KP_9,
		/* 5d */ ANIMA_ENGINE_KEY_UNKNOWN,
		/* 5e */ ANIMA_ENGINE_KEY_UNKNOWN,
		/* 5f */ ANIMA_ENGINE_KEY_UNKNOWN,
		/* 60 */ ANIMA_ENGINE_KEY_F5,
		/* 61 */ ANIMA_ENGINE_KEY_F6,
		/* 62 */ ANIMA_ENGINE_KEY_F7,
		/* 63 */ ANIMA_ENGINE_KEY_F3,
		/* 64 */ ANIMA_ENGINE_KEY_F8,
		/* 65 */ ANIMA_ENGINE_KEY_F9,
		/* 66 */ ANIMA_ENGINE_KEY_UNKNOWN,
		/* 67 */ ANIMA_ENGINE_KEY_F11,
		/* 68 */ ANIMA_ENGINE_KEY_UNKNOWN,
		/* 69 */ ANIMA_ENGINE_KEY_F13,
		/* 6a */ ANIMA_ENGINE_KEY_F16,
		/* 6b */ ANIMA_ENGINE_KEY_F14,
		/* 6c */ ANIMA_ENGINE_KEY_UNKNOWN,
		/* 6d */ ANIMA_ENGINE_KEY_F10,
		/* 6e */ ANIMA_ENGINE_KEY_UNKNOWN,
		/* 6f */ ANIMA_ENGINE_KEY_F12,
		/* 70 */ ANIMA_ENGINE_KEY_UNKNOWN,
		/* 71 */ ANIMA_ENGINE_KEY_F15,
		/* 72 */ ANIMA_ENGINE_KEY_INSERT, /* Really Help... */
		/* 73 */ ANIMA_ENGINE_KEY_HOME,
		/* 74 */ ANIMA_ENGINE_KEY_PAGE_UP,
		/* 75 */ ANIMA_ENGINE_KEY_DELETE,
		/* 76 */ ANIMA_ENGINE_KEY_F4,
		/* 77 */ ANIMA_ENGINE_KEY_END,
		/* 78 */ ANIMA_ENGINE_KEY_F2,
		/* 79 */ ANIMA_ENGINE_KEY_PAGE_DOWN,
		/* 7a */ ANIMA_ENGINE_KEY_F1,
		/* 7b */ ANIMA_ENGINE_KEY_LEFT,
		/* 7c */ ANIMA_ENGINE_KEY_RIGHT,
		/* 7d */ ANIMA_ENGINE_KEY_DOWN,
		/* 7e */ ANIMA_ENGINE_KEY_UP,
		/* 7f */ ANIMA_ENGINE_KEY_UNKNOWN,
	};
	
	if (key >= 128)
		return ANIMA_ENGINE_KEY_UNKNOWN;
	
	return table[key];
}

END_ANIMA_ENGINE_NAMESPACE

@interface AnimaEngineWindowContentView : NSView
{
	Anima::AnimaEngineWindow_Base* window;
	NSTrackingArea* trackingArea;
}

- (id)initWithAnimaEngineWindowWindow:(Anima::AnimaEngineWindow_Base *)initWindow;

@end

@implementation AnimaEngineWindowContentView

+ (void)initialize
{
	if (self == [AnimaEngineWindowContentView class])
	{
		if (Anima::AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_WINDOW_STATE->_cursor == nil)
		{
			NSImage* data = [[NSImage alloc] initWithSize:NSMakeSize(16, 16)];
			
			id cursor = Anima::AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_WINDOW_STATE->_cursor;
			cursor = [[NSCursor alloc] initWithImage:data hotSpot:NSZeroPoint];
			[data release];
		}
	}
}

- (id)initWithAnimaEngineWindowWindow:(Anima::AnimaEngineWindow_Base *)initWindow
{
	self = [super init];
	if (self != nil)
	{
		window = initWindow;
		trackingArea = nil;
		
		[self updateTrackingAreas];
		[self registerForDraggedTypes:[NSArray arrayWithObjects: NSFilenamesPboardType, nil]];
	}
	
	return self;
}

-(void)dealloc
{
	[trackingArea release];
	[super dealloc];
}

- (BOOL)isOpaque
{
	return YES;
}

- (BOOL)canBecomeKeyView
{
	return YES;
}

- (BOOL)acceptsFirstResponder
{
	return YES;
}

- (void)cursorUpdate:(NSEvent *)event
{
	updateModeCursor(window);
}

- (void)mouseDown:(NSEvent *)event
{
	if(window->GetMouseClickCallback())
		window->GetMouseClickCallback()(window, ANIMA_ENGINE_MOUSE_BUTTON_LEFT, ANIMA_ENGINE_PRESS, Anima::translateFlags([event modifierFlags]));
}

- (void)mouseDragged:(NSEvent *)event
{
	[self mouseMoved:event];
}

- (void)mouseUp:(NSEvent *)event
{
	if(window->GetMouseClickCallback())
		window->GetMouseClickCallback()(window, ANIMA_ENGINE_MOUSE_BUTTON_LEFT, ANIMA_ENGINE_RELEASE, Anima::translateFlags([event modifierFlags]));
}

- (void)mouseMoved:(NSEvent *)event
{
	if (window->GetCursorMode()	== ANIMA_ENGINE_CURSOR_DISABLED)
	{
		if(window->GetCursorMotionCallback())
			window->GetCursorMotionCallback()(window, [event deltaX], [event deltaY]);
	}
	else
	{
		const NSRect contentRect = [window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_view frame];
		const NSPoint p = [event locationInWindow];
		
		if(window->GetCursorMotionCallback())
			window->GetCursorMotionCallback()(window, p.x, contentRect.size.height - p.y);
	}
}

- (void)rightMouseDown:(NSEvent *)event
{
	if(window->GetMouseClickCallback())
		window->GetMouseClickCallback()(window, ANIMA_ENGINE_MOUSE_BUTTON_RIGHT, ANIMA_ENGINE_PRESS, Anima::translateFlags([event modifierFlags]));
}

- (void)rightMouseDragged:(NSEvent *)event
{
	[self mouseMoved:event];
}

- (void)rightMouseUp:(NSEvent *)event
{
	if(window->GetMouseClickCallback())
		window->GetMouseClickCallback()(window, ANIMA_ENGINE_MOUSE_BUTTON_RIGHT, ANIMA_ENGINE_RELEASE, Anima::translateFlags([event modifierFlags]));
}

- (void)otherMouseDown:(NSEvent *)event
{
	if(window->GetMouseClickCallback())
		window->GetMouseClickCallback()(window, (int) [event buttonNumber], ANIMA_ENGINE_PRESS, Anima::translateFlags([event modifierFlags]));
}

- (void)otherMouseDragged:(NSEvent *)event
{
	[self mouseMoved:event];
}

- (void)otherMouseUp:(NSEvent *)event
{
	if(window->GetMouseClickCallback())
		window->GetMouseClickCallback()(window, (int) [event buttonNumber], ANIMA_ENGINE_RELEASE, Anima::translateFlags([event modifierFlags]));
}

- (void)mouseExited:(NSEvent *)event
{
	window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_cursorInside = false;
	
	if(window->GetCursorEnterCallback())
		window->GetCursorEnterCallback()(window, false);
}

- (void)mouseEntered:(NSEvent *)event
{
	window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_cursorInside = true;
	
	if(window->GetCursorEnterCallback())
		window->GetCursorEnterCallback()(window, true);
}

- (void)viewDidChangeBackingProperties
{
	const NSRect contentRect = [window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_view frame];
	const NSRect fbRect = convertRectToBacking(window, contentRect);
	
	if(window->GetFramebufferSizeCallback())
		window->GetFramebufferSizeCallback()(window, fbRect.size.width, fbRect.size.height);
}

- (void)updateTrackingAreas
{
	if (trackingArea != nil)
	{
		[self removeTrackingArea:trackingArea];
		[trackingArea release];
	}
	
	NSTrackingAreaOptions options = NSTrackingMouseEnteredAndExited | NSTrackingActiveInKeyWindow | NSTrackingCursorUpdate | NSTrackingInVisibleRect;
	
	trackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds] options:options owner:self userInfo:nil];
	
	[self addTrackingArea:trackingArea];
	[super updateTrackingAreas];
}

- (void)keyDown:(NSEvent *)event
{
	const int key = Anima::translateKey([event keyCode]);
	const int mods = Anima::translateFlags([event modifierFlags]);
	
	if(window->GetKeyCallback())
		window->GetKeyCallback()(window, key, [event keyCode], ANIMA_ENGINE_PRESS, mods);
	
	NSString* characters = [event characters];
	NSUInteger i, length = [characters length];
	const int plain = !(mods & ANIMA_ENGINE_MOD_SUPER);
	
	for (i = 0;  i < length;  i++)
	{
		if(window->GetCharCallback())
			window->GetCharCallback()(window, [characters characterAtIndex:i], mods, plain);
	}
}

- (void)flagsChanged:(NSEvent *)event
{
	int action;
	const unsigned int modifierFlags =
	[event modifierFlags] & NSDeviceIndependentModifierFlagsMask;
	const int key = Anima::translateKey([event keyCode]);
	const int mods = Anima::translateFlags(modifierFlags);
	
	if (modifierFlags == window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_modifierFlags)
	{
		if (window->GetKeys()[key] == ANIMA_ENGINE_PRESS)
			action = ANIMA_ENGINE_RELEASE;
		else
			action = ANIMA_ENGINE_PRESS;
	}
	else if (modifierFlags > window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_modifierFlags)
		action = ANIMA_ENGINE_PRESS;
	else
		action = ANIMA_ENGINE_RELEASE;
	
	window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_modifierFlags = modifierFlags;
	
	if(window->GetKeyCallback())
		window->GetKeyCallback()(window, key, [event keyCode], action, mods);
}

- (void)keyUp:(NSEvent *)event
{
	const int key = Anima::translateKey([event keyCode]);
	const int mods = Anima::translateFlags([event modifierFlags]);
	
	if(window->GetKeyCallback())
		window->GetKeyCallback()(window, key, [event keyCode], ANIMA_ENGINE_RELEASE, mods);
}

- (void)scrollWheel:(NSEvent *)event
{
	double deltaX, deltaY;
	
#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
	if (floor(NSAppKitVersionNumber) > NSAppKitVersionNumber10_6)
	{
		deltaX = [event scrollingDeltaX];
		deltaY = [event scrollingDeltaY];
		
		if ([event hasPreciseScrollingDeltas])
		{
			deltaX *= 0.1;
			deltaY *= 0.1;
		}
	}
	else
#endif
	{
		deltaX = [event deltaX];
		deltaY = [event deltaY];
	}
	
	if (fabs(deltaX) > 0.0 || fabs(deltaY) > 0.0)
	{
		if(window->GetScrollCallback())
			window->GetScrollCallback()(window, deltaX, deltaY);
	}
}

- (void)resetCursorRects
{
	[self addCursorRect:[self bounds] cursor:getModeCursor(window)];
}

- (NSDragOperation)draggingEntered:(id <NSDraggingInfo>)sender
{
	if ((NSDragOperationGeneric & [sender draggingSourceOperationMask]) == NSDragOperationGeneric)
	{
		[self setNeedsDisplay:YES];
		return NSDragOperationGeneric;
	}
	
	return NSDragOperationNone;
}

- (BOOL)prepareForDragOperation:(id <NSDraggingInfo>)sender
{
	[self setNeedsDisplay:YES];
	return YES;
}

- (BOOL)performDragOperation:(id <NSDraggingInfo>)sender
{
	NSPasteboard* pasteboard = [sender draggingPasteboard];
	NSArray* files = [pasteboard propertyListForType:NSFilenamesPboardType];
	
	int height;
	_AnimaEngineWindowPlatformGetWindowSize(window, NULL, &height);
	
	if(window->GetCursorMotionCallback())
		window->GetCursorMotionCallback()(window, [sender draggingLocation].x, height - [sender draggingLocation].y);
	
	const int count = [files count];
	if (count)
	{
		NSEnumerator* e = [files objectEnumerator];
		char** names = (char**)calloc(count, sizeof(char*));
		int i;
		
		for (i = 0;  i < count;  i++)
			names[i] = strdup([[e nextObject] UTF8String]);
		
		if(window->GetDropCallback())
			window->GetDropCallback()(window, count, (const char**) names);
		
		for (i = 0;  i < count;  i++)
			free(names[i]);
		free(names);
	}
	
	return YES;
}

- (void)concludeDragOperation:(id <NSDraggingInfo>)sender
{
	[self setNeedsDisplay:YES];
}

@end

@interface AnimaEngineWindowWindow : NSWindow {}
@end

@implementation AnimaEngineWindowWindow

- (BOOL)canBecomeKeyWindow
{
	// Required for NSBorderlessWindowMask windows
	return YES;
}

@end

@interface AnimaEngineWindowApplication : NSApplication
@end

@implementation AnimaEngineWindowApplication

// From http://cocoadev.com/index.pl?GameKeyboardHandlingAlmost
// This works around an AppKit bug, where key up events while holding
// down the command key don't get sent to the key window.
- (void)sendEvent:(NSEvent *)event
{
	if ([event type] == NSKeyUp && ([event modifierFlags] & NSCommandKeyMask))
		[[self keyWindow] sendEvent:event];
	else
		[super sendEvent:event];
}

@end

BEGIN_ANIMA_ENGINE_NAMESPACE

#if defined(_ANIMA_ENGINE_USE_MENUBAR)

static NSString* findAppName(void)
{
	size_t i;
	NSDictionary* infoDictionary = [[NSBundle mainBundle] infoDictionary];
	
	// Keys to search for as potential application names
	NSString* GLFWNameKeys[] =
	{
		@"CFBundleDisplayName",
		@"CFBundleName",
		@"CFBundleExecutable",
	};
	
	for (i = 0;  i < sizeof(GLFWNameKeys) / sizeof(GLFWNameKeys[0]);  i++)
	{
		id name = [infoDictionary objectForKey:GLFWNameKeys[i]];
		if (name &&
			[name isKindOfClass:[NSString class]] &&
			![name isEqualToString:@""])
		{
			return name;
		}
	}
	
	char** progname = _NSGetProgname();
	if (progname && *progname)
		return [NSString stringWithUTF8String:*progname];
	
	// Really shouldn't get here
	return @"AnimaEngine Application";
}

// Set up the menu bar (manually)
// This is nasty, nasty stuff -- calls to undocumented semi-private APIs that
// could go away at any moment, lots of stuff that really should be
// localize(d|able), etc.  Loading a nib would save us this horror, but that
// doesn't seem like a good thing to require of GLFW's clients.
//
static void createMenuBar(void)
{
	NSString* appName = findAppName();
	
	NSMenu* bar = [[NSMenu alloc] init];
	[NSApp setMainMenu:bar];
	
	NSMenuItem* appMenuItem =
	[bar addItemWithTitle:@"" action:NULL keyEquivalent:@""];
	NSMenu* appMenu = [[NSMenu alloc] init];
	[appMenuItem setSubmenu:appMenu];
	
	[appMenu addItemWithTitle:[NSString stringWithFormat:@"About %@", appName]
					   action:@selector(orderFrontStandardAboutPanel:)
				keyEquivalent:@""];
	[appMenu addItem:[NSMenuItem separatorItem]];
	NSMenu* servicesMenu = [[NSMenu alloc] init];
	[NSApp setServicesMenu:servicesMenu];
	[[appMenu addItemWithTitle:@"Services"
						action:NULL
				 keyEquivalent:@""] setSubmenu:servicesMenu];
	[appMenu addItem:[NSMenuItem separatorItem]];
	[appMenu addItemWithTitle:[NSString stringWithFormat:@"Hide %@", appName]
					   action:@selector(hide:)
				keyEquivalent:@"h"];
	[[appMenu addItemWithTitle:@"Hide Others"
						action:@selector(hideOtherApplications:)
				 keyEquivalent:@"h"]
	 setKeyEquivalentModifierMask:NSAlternateKeyMask | NSCommandKeyMask];
	[appMenu addItemWithTitle:@"Show All"
					   action:@selector(unhideAllApplications:)
				keyEquivalent:@""];
	[appMenu addItem:[NSMenuItem separatorItem]];
	[appMenu addItemWithTitle:[NSString stringWithFormat:@"Quit %@", appName]
					   action:@selector(terminate:)
				keyEquivalent:@"q"];
	
	NSMenuItem* windowMenuItem =
	[bar addItemWithTitle:@"" action:NULL keyEquivalent:@""];
	NSMenu* windowMenu = [[NSMenu alloc] initWithTitle:@"Window"];
	[NSApp setWindowsMenu:windowMenu];
	[windowMenuItem setSubmenu:windowMenu];
	
	[windowMenu addItemWithTitle:@"Minimize"
						  action:@selector(performMiniaturize:)
				   keyEquivalent:@"m"];
	[windowMenu addItemWithTitle:@"Zoom"
						  action:@selector(performZoom:)
				   keyEquivalent:@""];
	[windowMenu addItem:[NSMenuItem separatorItem]];
	[windowMenu addItemWithTitle:@"Bring All to Front"
						  action:@selector(arrangeInFront:)
				   keyEquivalent:@""];
	
#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
	if (floor(NSAppKitVersionNumber) > NSAppKitVersionNumber10_6)
	{
		// TODO: Make this appear at the bottom of the menu (for consistency)
		
		[windowMenu addItem:[NSMenuItem separatorItem]];
		[[windowMenu addItemWithTitle:@"Enter Full Screen"
							   action:@selector(toggleFullScreen:)
						keyEquivalent:@"f"]
		 setKeyEquivalentModifierMask:NSControlKeyMask | NSCommandKeyMask];
	}
#endif
	
	// Prior to Snow Leopard, we need to use this oddly-named semi-private API
	// to get the application menu working properly.
	SEL setAppleMenuSelector = NSSelectorFromString(@"setAppleMenu:");
	[NSApp performSelector:setAppleMenuSelector withObject:appMenu];
}

#endif

static bool initializeAppKit(void)
{
	if (NSApp)
		return true;
	
	// Implicitly create shared NSApplication instance
	[AnimaEngineWindowApplication sharedApplication];
	
	// In case we are unbundled, make us a proper UI application
	[NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
	
#if defined(_ANIMA_ENGINE_USE_MENUBAR)
	// Menu bar setup must go between sharedApplication above and
	// finishLaunching below, in order to properly emulate the behavior
	// of NSApplicationMain
	createMenuBar();
#endif
	
	[NSApp finishLaunching];
	
	return true;
}

static bool createWindow(AnimaEngineWindow_Base* window, const _AnimaEngineWindowwndconfig* wndconfig)
{
	_AnimaEngineWindowwindowNS* platformWindowState = window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE;
	platformWindowState->_delegate = [[AnimaEngineWindowWindowDelegate alloc] initWithAnimaEngineWindowWindow:window];
	if (platformWindowState->_delegate == nil)
	{
//		_glfwInputError(GLFW_PLATFORM_ERROR, "Cocoa: Failed to create window delegate");
		return false;
	}
	
	unsigned int styleMask = 0;
	
	if (wndconfig->_monitor || !wndconfig->_decorated)
		styleMask = NSBorderlessWindowMask;
	else
	{
		styleMask = NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask;
		
		if (wndconfig->_resizable)
			styleMask |= NSResizableWindowMask;
	}
	
	NSRect contentRect;
	
	if (wndconfig->_monitor)
		contentRect = [wndconfig->_monitor->ns._screen frame];
	else
		contentRect = NSMakeRect(0, 0, wndconfig->_width, wndconfig->_height);
	
	platformWindowState->_object = [[AnimaEngineWindowWindow alloc] initWithContentRect:contentRect styleMask:styleMask backing:NSBackingStoreBuffered defer:NO];
	
	if (platformWindowState->_object == nil)
	{
//		_glfwInputError(GLFW_PLATFORM_ERROR, "Cocoa: Failed to create window");
		return false;
	}
	
	platformWindowState->_view = [[AnimaEngineWindowContentView alloc] initWithAnimaEngineWindowWindow:window];
	
#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
	if (floor(NSAppKitVersionNumber) > NSAppKitVersionNumber10_6)
	{
#if defined(_ANIMA_ENGINE_USE_RETINA)
		[platformWindowState->_view setWantsBestResolutionOpenGLSurface:YES];
		window->SetResolutionMultiplier(2.0);
#endif
		
		if (wndconfig->_resizable)
			[platformWindowState->_object setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];
	}
#endif
	
	if (wndconfig->_monitor)
		[platformWindowState->_object setLevel:NSMainMenuWindowLevel + 1];
	else
	{
		[platformWindowState->_object center];
		
		if (wndconfig->_floating)
			[platformWindowState->_object setLevel:NSFloatingWindowLevel];
	}
	
	[platformWindowState->_object setTitle:[NSString stringWithUTF8String:wndconfig->_title]];
	[platformWindowState->_object setContentView:platformWindowState->_view];
	[platformWindowState->_object setDelegate:platformWindowState->_delegate];
	[platformWindowState->_object setAcceptsMouseMovedEvents:YES];
	[platformWindowState->_object setHidesOnDeactivate:YES];
	
#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
	if (floor(NSAppKitVersionNumber) > NSAppKitVersionNumber10_6)
		[platformWindowState->_object setRestorable:NO];
#endif
	
	return true;
}

bool _AnimaEngineWindowPlatformCreateWindow(AnimaEngineWindow_Base* window, const _AnimaEngineWindowwndconfig* wndconfig, const _AnimaEngineWindowctxconfig* ctxconfig, const _AnimaEngineWindowfbconfig* fbconfig)
{
	if (!initializeAppKit())
		return false;
	
	// There can only be one application delegate, but we allocate it the
	// first time a window is created to keep all window code in this file
	if (AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_WINDOW_STATE->_delegate == nil)
	{
		_AnimaEngineWindowlibraryNS* libraryWindowState = AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_WINDOW_STATE;
		libraryWindowState->_delegate = [[AnimaEngineWindowApplicationDelegate alloc] init];
		if (libraryWindowState->_delegate == nil)
		{
//			_glfwInputError(GLFW_PLATFORM_ERROR, "Cocoa: Failed to create application delegate");
			return false;
		}
		
		[NSApp setDelegate:libraryWindowState->_delegate];
	}
	
	if (!createWindow(window, wndconfig))
		return false;
	
	if (!_AnimaEngineWindowCreateContext(window, ctxconfig, fbconfig))
		return false;
	
	[window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_context setView:window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_view];
	
	if (wndconfig->_monitor)
	{
		_AnimaEngineWindowPlatformShowWindow(window);
		if (!enterFullscreenMode(window))
			return false;
	}
	
	return true;
}

void _AnimaEngineWindowPlatformDestroyWindow(AnimaEngineWindow_Base* window)
{
	[window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_object orderOut:nil];
	
	if (window->GetMonitor())
		leaveFullscreenMode(window);
	
	_AnimaEngineWindowDestroyContext(window);
	
	[window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_object setDelegate:nil];
	[window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_delegate release];
	window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_delegate = nil;
	
	[window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_view release];
	window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_view = nil;
	
	[window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_object close];
	window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_object = nil;
}

void _AnimaEngineWindowPlatformSetWindowTitle(AnimaEngineWindow_Base* window, const char *title)
{
	[window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_object setTitle:[NSString stringWithUTF8String:title]];
}

void _AnimaEngineWindowPlatformGetWindowPos(AnimaEngineWindow_Base* window, int* xpos, int* ypos)
{
	const NSRect contentRect = [window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_object contentRectForFrameRect:[window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_object frame]];
	
	if (xpos)
		*xpos = contentRect.origin.x;
	if (ypos)
		*ypos = transformY(contentRect.origin.y + contentRect.size.height);
}

void _AnimaEngineWindowPlatformSetWindowPos(AnimaEngineWindow_Base* window, int x, int y)
{
	const NSRect contentRect = [window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_view frame];
	const NSRect dummyRect = NSMakeRect(x, transformY(y + contentRect.size.height), 0, 0);
	const NSRect frameRect = [window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_object frameRectForContentRect:dummyRect];
	[window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_object setFrameOrigin:frameRect.origin];
}

void _AnimaEngineWindowPlatformGetWindowSize(AnimaEngineWindow_Base* window, int* width, int* height)
{
	const NSRect contentRect = [window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_view frame];
	
	if (width)
		*width = contentRect.size.width;
	if (height)
		*height = contentRect.size.height;
}

void _AnimaEngineWindowPlatformSetWindowSize(AnimaEngineWindow_Base* window, int width, int height)
{
	[window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_object setContentSize:NSMakeSize(width, height)];
}

void _AnimaEngineWindowPlatformGetFramebufferSize(AnimaEngineWindow_Base* window, int* width, int* height)
{
	const NSRect contentRect = [window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_view frame];
	const NSRect fbRect = convertRectToBacking(window, contentRect);
	
	if (width)
		*width = (int) fbRect.size.width;
	if (height)
		*height = (int) fbRect.size.height;
}

void _AnimaEngineWindowPlatformGetWindowFrameSize(AnimaEngineWindow_Base* window, int* left, int* top, int* right, int* bottom)
{
	const NSRect contentRect = [window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_view frame];
	const NSRect frameRect = [window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_object frameRectForContentRect:contentRect];
	
	if (left)
		*left = contentRect.origin.x - frameRect.origin.x;
	if (top)
		*top = frameRect.origin.y + frameRect.size.height -
		contentRect.origin.y - contentRect.size.height;
	if (right)
		*right = frameRect.origin.x + frameRect.size.width -
		contentRect.origin.x - contentRect.size.width;
	if (bottom)
		*bottom = contentRect.origin.y - frameRect.origin.y;
}

void _AnimaEngineWindowPlatformIconifyWindow(AnimaEngineWindow_Base* window)
{
	if (window->GetMonitor())
		leaveFullscreenMode(window);
	
	[window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_object miniaturize:nil];
}

void _AnimaEngineWindowPlatformRestoreWindow(AnimaEngineWindow_Base* window)
{
	[window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_object deminiaturize:nil];
}

void _AnimaEngineWindowPlatformShowWindow(AnimaEngineWindow_Base* window)
{
	// Make us the active application
	// HACK: This has been moved here from initializeAppKit to prevent
	//       applications using only hidden windows from being activated, but
	//       should probably not be done every time any window is shown
	[NSApp activateIgnoringOtherApps:YES];
	
	[window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_object makeKeyAndOrderFront:nil];
}

void _AnimaEngineWindowPlatformUnhideWindow(AnimaEngineWindow_Base* window)
{
	[window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_object orderFront:nil];
	if(window->GetVisibilityCallback())
		window->GetVisibilityCallback()(window, true);
}

void _AnimaEngineWindowPlatformHideWindow(AnimaEngineWindow_Base* window)
{
	[window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_object orderOut:nil];
	if(window->GetVisibilityCallback())
		window->GetVisibilityCallback()(window, false);
}

void _AnimaEngineWindowPlatformPollEvents(AnimaEngine*)
{
	for (;;)
	{
		NSEvent* event = [NSApp nextEventMatchingMask:NSAnyEventMask untilDate:[NSDate distantPast] inMode:NSDefaultRunLoopMode dequeue:YES];
		if (event == nil)
			break;
		
		[NSApp sendEvent:event];
	}
	
	[AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_WINDOW_STATE->_autoreleasePool drain];
	AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_WINDOW_STATE->_autoreleasePool = [[NSAutoreleasePool alloc] init];
}

void _AnimaEngineWindowPlatformWaitEvents(AnimaEngine* engine)
{
	// I wanted to pass NO to dequeue:, and rely on PollEvents to
	// dequeue and send.  For reasons not at all clear to me, passing
	// NO to dequeue: causes this method never to return.
	NSEvent *event = [NSApp nextEventMatchingMask:NSAnyEventMask untilDate:[NSDate distantFuture] inMode:NSDefaultRunLoopMode dequeue:YES];
	[NSApp sendEvent:event];
	
	_AnimaEngineWindowPlatformPollEvents(engine);
}

void _AnimaEngineWindowPlatformPostEmptyEvent(AnimaEngine*)
{
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	NSEvent* event = [NSEvent otherEventWithType:NSApplicationDefined
										location:NSMakePoint(0, 0)
								   modifierFlags:0
									   timestamp:0
									windowNumber:0
										 context:nil
										 subtype:0
										   data1:0
										   data2:0];
	[NSApp postEvent:event atStart:YES];
	[pool drain];
}

void _AnimaEngineWindowPlatformSetCursorPos(AnimaEngineWindow_Base* window, double x, double y)
{
	updateModeCursor(window);
	
	if (window->GetMonitor())
	{
		CGDisplayMoveCursorToPoint(window->GetMonitor()->ns._displayID, CGPointMake(x, y));
	}
	else
	{
		const NSRect contentRect = [window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_view frame];
		const NSPoint localPoint = NSMakePoint(x, contentRect.size.height - y - 1);
		const NSPoint globalPoint = [window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_object convertBaseToScreen:localPoint];
		
		CGWarpMouseCursorPosition(CGPointMake(globalPoint.x, transformY(globalPoint.y)));
	}
}

void _AnimaEngineWindowPlatformApplyCursorMode(AnimaEngineWindow_Base* window)
{
	updateModeCursor(window);
	
	if (window->GetCursorMode() == ANIMA_ENGINE_CURSOR_DISABLED)
	{
		CGAssociateMouseAndMouseCursorPosition(false);
		centerCursor(window);
	}
	else
		CGAssociateMouseAndMouseCursorPosition(true);
}

bool _AnimaEngineWindowPlatformCreateCursor(_AnimaEngineWindowcursor* cursor, const AnimaEngineWindowimage* image, int xhot, int yhot)
{
	NSImage* native;
	NSBitmapImageRep* rep;
	
	rep = [[NSBitmapImageRep alloc]
		   initWithBitmapDataPlanes:NULL
		   pixelsWide:image->_width
		   pixelsHigh:image->_height
		   bitsPerSample:8
		   samplesPerPixel:4
		   hasAlpha:YES
		   isPlanar:NO
		   colorSpaceName:NSCalibratedRGBColorSpace
		   bitmapFormat:NSAlphaNonpremultipliedBitmapFormat
		   bytesPerRow:image->_width * 4
		   bitsPerPixel:32];
	
	if (rep == nil)
		return false;
	
	memcpy([rep bitmapData], image->_pixels, image->_width * image->_height * 4);
	
	native = [[NSImage alloc] initWithSize:NSMakeSize(image->_width, image->_height)];
	[native addRepresentation: rep];
	
	cursor->ns._object = [[NSCursor alloc] initWithImage:native hotSpot:NSMakePoint(xhot, yhot)];
	
	[native release];
	[rep release];
	
	if (cursor->ns._object == nil)
		return false;
	
	return true;
}

void _AnimaEngineWindowPlatformDestroyCursor(_AnimaEngineWindowcursor* cursor)
{
	if (cursor->ns._object)
		[(NSCursor*) cursor->ns._object release];
}

void _AnimaEngineWindowPlatformSetCursor(AnimaEngineWindow_Base* window, _AnimaEngineWindowcursor* cursor)
{
	if (window->GetCursorMode() == ANIMA_ENGINE_CURSOR_NORMAL && window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_cursorInside)
	{
		if (cursor)
			[(NSCursor*) cursor->ns._object set];
		else
			[[NSCursor arrowCursor] set];
	}
}

void _AnimaEngineWindowPlatformSetClipboardString(AnimaEngineWindow_Base* window, const char* string)
{
	NSArray* types = [NSArray arrayWithObjects:NSStringPboardType, nil];
	
	NSPasteboard* pasteboard = [NSPasteboard generalPasteboard];
	[pasteboard declareTypes:types owner:nil];
	[pasteboard setString:[NSString stringWithUTF8String:string] forType:NSStringPboardType];
}

const char* _AnimaEngineWindowPlatformGetClipboardString(AnimaEngineWindow_Base* window)
{
	NSPasteboard* pasteboard = [NSPasteboard generalPasteboard];
	
	if (![[pasteboard types] containsObject:NSStringPboardType])
	{
//		_glfwInputError(GLFW_FORMAT_UNAVAILABLE, NULL);
		return NULL;
	}
	
	NSString* object = [pasteboard stringForType:NSStringPboardType];
	if (!object)
	{
//		_glfwInputError(GLFW_PLATFORM_ERROR, "Cocoa: Failed to retrieve object from pasteboard");
		return NULL;
	}
	
	free(AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_WINDOW_STATE->_clipboardString);
	AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_WINDOW_STATE->_clipboardString = strdup([object UTF8String]);
	
	return AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_WINDOW_STATE->_clipboardString;
}

id AnimaEngineWindowGetCocoaWindow(AnimaEngineWindow_Base* handle)
{
	AnimaEngineWindow_Base* window = (AnimaEngineWindow_Base*) handle;
	_ANIMA_ENGINE_REQUIRE_INIT_OR_RETURN(nil);
	return window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_object;
}

static char* getDisplayName(CGDirectDisplayID displayID)
{
	char* name;
	CFDictionaryRef info, names;
	CFStringRef value;
	CFIndex size;
	
	// NOTE: This uses a deprecated function because Apple has
	//       (as of September 2014) not provided any alternative
	info = IODisplayCreateInfoDictionary(CGDisplayIOServicePort(displayID), kIODisplayOnlyPreferredName);
	names = (CFDictionaryRef)CFDictionaryGetValue(info, CFSTR(kDisplayProductName));
	
	if (!names || !CFDictionaryGetValueIfPresent(names, CFSTR("en_US"), (const void**) &value))
	{
		// This may happen if a desktop Mac is running headless
//		_glfwInputError(GLFW_PLATFORM_ERROR, "Failed to retrieve display name");
		
		CFRelease(info);
		return strdup("Unknown");
	}
	
	size = CFStringGetMaximumSizeForEncoding(CFStringGetLength(value), kCFStringEncodingUTF8);
	name = (char*)calloc(size + 1, sizeof(char));
	CFStringGetCString(value, name, size, kCFStringEncodingUTF8);
	
	CFRelease(info);
	
	return name;
}

static bool modeIsGood(CGDisplayModeRef mode)
{
	uint32_t flags = CGDisplayModeGetIOFlags(mode);
	if (!(flags & kDisplayModeValidFlag) || !(flags & kDisplayModeSafeFlag))
		return false;
	
	if (flags & kDisplayModeInterlacedFlag)
		return false;
	
	if (flags & kDisplayModeStretchedFlag)
		return false;
	
	CFStringRef format = CGDisplayModeCopyPixelEncoding(mode);
	if (CFStringCompare(format, CFSTR(IO16BitDirectPixels), 0) && CFStringCompare(format, CFSTR(IO32BitDirectPixels), 0))
	{
		CFRelease(format);
		return false;
	}
	
	CFRelease(format);
	return true;
}

static AnimaEngineWindowvidmode vidmodeFromCGDisplayMode(CGDisplayModeRef mode, CVDisplayLinkRef link)
{
	AnimaEngineWindowvidmode result;
	result._width = (int) CGDisplayModeGetWidth(mode);
	result._height = (int) CGDisplayModeGetHeight(mode);
	result._refreshRate = (int) CGDisplayModeGetRefreshRate(mode);
	
	if (result._refreshRate == 0)
	{
		const CVTime time = CVDisplayLinkGetNominalOutputVideoRefreshPeriod(link);
		if (!(time.flags & kCVTimeIsIndefinite))
			result._refreshRate = (int) (time.timeScale / (double) time.timeValue);
	}
	
	CFStringRef format = CGDisplayModeCopyPixelEncoding(mode);
	
	if (CFStringCompare(format, CFSTR(IO16BitDirectPixels), 0) == 0)
	{
		result._redBits = 5;
		result._greenBits = 5;
		result._blueBits = 5;
	}
	else
	{
		result._redBits = 8;
		result._greenBits = 8;
		result._blueBits = 8;
	}
	
	CFRelease(format);
	return result;
}

static CGDisplayFadeReservationToken beginFadeReservation(void)
{
	CGDisplayFadeReservationToken token = kCGDisplayFadeReservationInvalidToken;
	
	if (CGAcquireDisplayFadeReservation(5, &token) == kCGErrorSuccess)
		CGDisplayFade(token, 0.3, kCGDisplayBlendNormal, kCGDisplayBlendSolidColor, 0.0, 0.0, 0.0, TRUE);
	
	return token;
}

static void endFadeReservation(CGDisplayFadeReservationToken token)
{
	if (token != kCGDisplayFadeReservationInvalidToken)
	{
		CGDisplayFade(token, 0.5, kCGDisplayBlendSolidColor, kCGDisplayBlendNormal, 0.0, 0.0, 0.0, FALSE);
		CGReleaseDisplayFadeReservation(token);
	}
}

bool _AnimaEngineWindowSetVideoMode(_AnimaEngineWindowmonitor* monitor, const AnimaEngineWindowvidmode* desired)
{
	CFArrayRef modes;
	CFIndex count, i;
	CVDisplayLinkRef link;
	CGDisplayModeRef native = NULL;
	AnimaEngineWindowvidmode current;
	const AnimaEngineWindowvidmode* best;
	
	best = _AnimaEngineWindowChooseVideoMode(monitor, desired);
	_AnimaEngineWindowPlatformGetVideoMode(monitor, &current);
	
	if (_AnimaEngineWindowCompareVideoModes(&current, best) == 0)
		return true;
	
	CVDisplayLinkCreateWithCGDisplay(monitor->ns._displayID, &link);
	
	modes = CGDisplayCopyAllDisplayModes(monitor->ns._displayID, NULL);
	count = CFArrayGetCount(modes);
	
	for (i = 0;  i < count;  i++)
	{
		CGDisplayModeRef dm = (CGDisplayModeRef) CFArrayGetValueAtIndex(modes, i);
		if (!modeIsGood(dm))
			continue;
		
		const AnimaEngineWindowvidmode mode = vidmodeFromCGDisplayMode(dm, link);
		if (_AnimaEngineWindowCompareVideoModes(best, &mode) == 0)
		{
			native = dm;
			break;
		}
	}
	
	if (native)
	{
		if (monitor->ns._previousMode == NULL)
			monitor->ns._previousMode = CGDisplayCopyDisplayMode(monitor->ns._displayID);
		
		CGDisplayFadeReservationToken token = beginFadeReservation();
		CGDisplayCapture(monitor->ns._displayID);
		CGDisplaySetDisplayMode(monitor->ns._displayID, native, NULL);
		endFadeReservation(token);
	}
	
	CFRelease(modes);
	CVDisplayLinkRelease(link);
	
	if (!native)
	{
//		_glfwInputError(GLFW_PLATFORM_ERROR, "Cocoa: Monitor mode list changed");
		return false;
	}
	
	return true;
}

void _AnimaEngineWindowRestoreVideoMode(_AnimaEngineWindowmonitor* monitor)
{
	if (monitor->ns._previousMode)
	{
		CGDisplayFadeReservationToken token = beginFadeReservation();
		CGDisplaySetDisplayMode(monitor->ns._displayID, monitor->ns._previousMode, NULL);
		CGDisplayRelease(monitor->ns._displayID);
		endFadeReservation(token);
		
		CGDisplayModeRelease(monitor->ns._previousMode);
		monitor->ns._previousMode = NULL;
	}
}

_AnimaEngineWindowmonitor** _AnimaEngineWindowPlatformGetMonitors(int* count)
{
	uint32_t i, found = 0, displayCount;
	_AnimaEngineWindowmonitor** monitors;
	CGDirectDisplayID* displays;
	
	*count = 0;
	
	CGGetOnlineDisplayList(0, NULL, &displayCount);
	
	displays = (CGDirectDisplayID*)calloc(displayCount, sizeof(CGDirectDisplayID));
	monitors = (_AnimaEngineWindowmonitor**)calloc(displayCount, sizeof(_AnimaEngineWindowmonitor*));
	
	CGGetOnlineDisplayList(displayCount, displays, &displayCount);
	
	NSArray* screens = [NSScreen screens];
	
	for (i = 0;  i < displayCount;  i++)
	{
		int j;
		
		if (CGDisplayIsAsleep(displays[i]))
			continue;
		
		CGDirectDisplayID screenDisplayID = CGDisplayMirrorsDisplay(displays[i]);
		if (screenDisplayID == kCGNullDirectDisplay)
			screenDisplayID = displays[i];
		
		const CGSize size = CGDisplayScreenSize(displays[i]);
		char* name = getDisplayName(displays[i]);
		
		monitors[found] = _AnimaEngineWindowAllocMonitor(name, size.width, size.height);
		monitors[found]->ns._displayID = displays[i];
		
		free(name);
		
		for (j = 0;  j < [screens count];  j++)
		{
			NSScreen* screen = [screens objectAtIndex:j];
			NSDictionary* dictionary = [screen deviceDescription];
			NSNumber* number = [dictionary objectForKey:@"NSScreenNumber"];
			
			if ([number unsignedIntegerValue] == screenDisplayID)
			{
				monitors[found]->ns._screen = screen;
				break;
			}
		}
		
		if (monitors[found]->ns._screen)
			found++;
		else
		{
			_AnimaEngineWindowFreeMonitor(monitors[found]);
			monitors[found] = NULL;
		}
	}
	
	free(displays);
	
	*count = found;
	return monitors;
}

bool _AnimaEngineWindowPlatformIsSameMonitor(_AnimaEngineWindowmonitor* first, _AnimaEngineWindowmonitor* second)
{
	return first->ns._displayID == second->ns._displayID;
}

void _AnimaEngineWindowPlatformGetMonitorPos(_AnimaEngineWindowmonitor* monitor, int* xpos, int* ypos)
{
	const CGRect bounds = CGDisplayBounds(monitor->ns._displayID);
	
	if (xpos)
		*xpos = (int) bounds.origin.x;
	if (ypos)
		*ypos = (int) bounds.origin.y;
}

AnimaEngineWindowvidmode* _AnimaEngineWindowPlatformGetVideoModes(_AnimaEngineWindowmonitor* monitor, int* found)
{
	CFArrayRef modes;
	CFIndex count, i, j;
	AnimaEngineWindowvidmode* result;
	CVDisplayLinkRef link;
	
	CVDisplayLinkCreateWithCGDisplay(monitor->ns._displayID, &link);
	
	modes = CGDisplayCopyAllDisplayModes(monitor->ns._displayID, NULL);
	count = CFArrayGetCount(modes);
	
	result = (AnimaEngineWindowvidmode*)calloc(count, sizeof(AnimaEngineWindowvidmode));
	*found = 0;
	
	for (i = 0;  i < count;  i++)
	{
		CGDisplayModeRef dm = (CGDisplayModeRef) CFArrayGetValueAtIndex(modes, i);
		if (!modeIsGood(dm))
			continue;
		
		const AnimaEngineWindowvidmode mode = vidmodeFromCGDisplayMode(dm, link);
		
		for (j = 0;  j < *found;  j++)
		{
			if (_AnimaEngineWindowCompareVideoModes(result + j, &mode) == 0)
				break;
		}
		
		if (i < *found)
		{
			// This is a duplicate, so skip it
			continue;
		}
		
		result[*found] = mode;
		(*found)++;
	}
	
	CFRelease(modes);
	
	CVDisplayLinkRelease(link);
	return result;
}

void _AnimaEngineWindowPlatformGetVideoMode(_AnimaEngineWindowmonitor* monitor, AnimaEngineWindowvidmode *mode)
{
	CGDisplayModeRef displayMode;
	CVDisplayLinkRef link;
	
	CVDisplayLinkCreateWithCGDisplay(monitor->ns._displayID, &link);
	
	displayMode = CGDisplayCopyDisplayMode(monitor->ns._displayID);
	*mode = vidmodeFromCGDisplayMode(displayMode, link);
	CGDisplayModeRelease(displayMode);
	
	CVDisplayLinkRelease(link);
}

void _AnimaEngineWindowPlatformGetGammaRamp(_AnimaEngineWindowmonitor* monitor, AnimaEngineWindowgammaramp* ramp)
{
	uint32_t i, size = CGDisplayGammaTableCapacity(monitor->ns._displayID);
	CGGammaValue* values = (CGGammaValue*)calloc(size * 3, sizeof(CGGammaValue));
	
	CGGetDisplayTransferByTable(monitor->ns._displayID, size, values, values + size, values + size * 2, &size);
	
	_AnimaEngineWindowAllocGammaArrays(ramp, size);
	
	for (i = 0; i < size; i++)
	{
		ramp->_red[i]   = (unsigned short) (values[i] * 65535);
		ramp->_green[i] = (unsigned short) (values[i + size] * 65535);
		ramp->_blue[i]  = (unsigned short) (values[i + size * 2] * 65535);
	}
	
	free(values);
}

void _AnimaEngineWindowPlatformSetGammaRamp(_AnimaEngineWindowmonitor* monitor, const AnimaEngineWindowgammaramp* ramp)
{
	int i;
	CGGammaValue* values = (CGGammaValue*)calloc(ramp->_size * 3, sizeof(CGGammaValue));
	
	for (i = 0;  i < ramp->_size;  i++)
	{
		values[i]                   = ramp->_red[i] / 65535.f;
		values[i + ramp->_size]     = ramp->_green[i] / 65535.f;
		values[i + ramp->_size * 2] = ramp->_blue[i] / 65535.f;
	}
	
	CGSetDisplayTransferByTable(monitor->ns._displayID, ramp->_size, values, values + ramp->_size, values + ramp->_size * 2);
	
	free(values);
}

CGDirectDisplayID AnimaEngineWindowGetCocoaMonitor(AnimaEngineWindowmonitor* handle)
{
	_AnimaEngineWindowmonitor* monitor = (_AnimaEngineWindowmonitor*) handle;
	_ANIMA_ENGINE_REQUIRE_INIT_OR_RETURN(kCGNullDirectDisplay);
	return monitor->ns._displayID;
}

#if defined(_ANIMA_ENGINE_USE_CHDIR)

static void changeToResourcesDirectory(void)
{
	char resourcesPath[MAXPATHLEN];
	
	CFBundleRef bundle = CFBundleGetMainBundle();
	if (!bundle)
		return;
	
	CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(bundle);
	
	CFStringRef last = CFURLCopyLastPathComponent(resourcesURL);
	if (CFStringCompare(CFSTR("Resources"), last, 0) != kCFCompareEqualTo)
	{
		CFRelease(last);
		CFRelease(resourcesURL);
		return;
	}
	
	CFRelease(last);
	
	if (!CFURLGetFileSystemRepresentation(resourcesURL, true, (UInt8*) resourcesPath, MAXPATHLEN))
	{
		CFRelease(resourcesURL);
		return;
	}
	
	CFRelease(resourcesURL);
	
	chdir(resourcesPath);
}

#endif

bool _AnimaEngineWindowPlatformInit(void)
{
	AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_WINDOW_STATE->_autoreleasePool = [[NSAutoreleasePool alloc] init];
	
#if defined(_GLFW_USE_CHDIR)
	changeToResourcesDirectory();
#endif
	
	AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_WINDOW_STATE->_eventSource = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);
	if (!AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_WINDOW_STATE->_eventSource)
		return false;
	
	CGEventSourceSetLocalEventsSuppressionInterval(AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_WINDOW_STATE->_eventSource, 0.0);
	
	if (!_AnimaEngineWindowInitContextAPI())
		return false;
	
	_AnimaEngineWindowInitTimer();
	_AnimaEngineWindowInitJoysticks();
	
	return true;
}

void _AnimaEngineWindowPlatformTerminate(void)
{
	if (AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_WINDOW_STATE->_eventSource)
	{
		CFRelease(AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_WINDOW_STATE->_eventSource);
		AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_WINDOW_STATE->_eventSource = NULL;
	}
	
	[NSApp setDelegate:nil];
	[AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_WINDOW_STATE->_delegate release];
	AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_WINDOW_STATE->_delegate = nil;
	
	[AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_WINDOW_STATE->_autoreleasePool release];
	AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_WINDOW_STATE->_autoreleasePool = nil;
	
	[AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_WINDOW_STATE->_cursor release];
	AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_WINDOW_STATE->_cursor = nil;
	
	free(AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_WINDOW_STATE->_clipboardString);
	
	_AnimaEngineWindowTerminateJoysticks();
	_AnimaEngineWindowTerminateContextAPI();
}

const char* _AnimaEngineWindowPlatformGetVersionString(void)
{
	const char* version = _ANIMA_ENGINE_VERSION_NUMBER " Cocoa"
#if defined(_ANIMA_ENGINE_NSGL)
	" NSGL"
#endif
#if defined(_ANIMA_ENGINE_USE_CHDIR)
	" chdir"
#endif
#if defined(_ANIMA_ENGINE_USE_MENUBAR)
	" menubar"
#endif
#if defined(_ANIMA_ENGINE_USE_RETINA)
	" retina"
#endif
#if defined(_ANIMA_ENGINE_BUILD_DLL)
	" dynamic"
#endif
	;
	
	return version;
}

static uint64_t getRawTime(void)
{
	return mach_absolute_time();
}

void _AnimaEngineWindowInitTimer(void)
{
	mach_timebase_info_data_t info;
	mach_timebase_info(&info);
	
	AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_TIME_STATE->_resolution = (double) info.numer / (info.denom * 1.0e9);
	AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_TIME_STATE->_base = getRawTime();
}

double _AnimaEngineWindowPlatformGetTime(void)
{
	return (double) (getRawTime() - AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_TIME_STATE->_base) * AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_TIME_STATE->_resolution;
}

void _AnimaEngineWindowPlatformSetTime(double time)
{
	AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_TIME_STATE->_base = getRawTime() - (uint64_t) (time / AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_TIME_STATE->_resolution);
}

END_ANIMA_ENGINE_NAMESPACE