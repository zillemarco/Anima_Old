//
//  AnimaEngineDemoView.h
//  Anima
//
//  Created by Marco Zille on 17/08/15.
//
//

#include <AnimaGC.h>
#import <Cocoa/Cocoa.h>

@interface AnimaEngineDemoView : NSView
{
	Anima::AnimaGC* gc;
}

- (id) initWithFrame: (NSRect)frameRect contextConfig: (Anima::AnimaGCContextConfig)context frameBufferConfig: (Anima::AnimaGCFrameBufferConfig)frameBuffer;
- (void) drawScene;
- (bool) prepareWithContextConfig: (Anima::AnimaGCContextConfig)context frameBufferConfig: (Anima::AnimaGCFrameBufferConfig)frameBuffer;

@end
