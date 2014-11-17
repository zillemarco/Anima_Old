#include "CustomWindow.h"
#include <stdio.h>

BEGIN_MESSAGE_MAP(CustomWindow, Anima::AnimaEngineWindow_Base)
	ANIMA_WINDOW_MOUSE_CLICK_EVENT(MouseClickCallback)
	ANIMA_WINDOW_PAINT_EVENT(PaintCallback)
	ANIMA_WINDOW_FRAMEBUFFER_SIZE_CHANGED_EVENT(FrameBufferResizeCallback)
END_MESSAGE_MAP()

CustomWindow::CustomWindow()
{
}

CustomWindow::~CustomWindow()
{
}

void CustomWindow::MouseClickCallback(Anima::AnimaEngineWindow_Base* window, int button, int action, int mods)
{
	printf("\nMouse click da custom window");
}
void CustomWindow::PaintCallback(Anima::AnimaWindow* window)
{
	((CustomWindow*)window)->DrawScene();
}

void CustomWindow::DrawScene()
{
	MakeCurrentContext();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0, 0.0, 0.0, 1.0);
	
	glFlush();
	
	SwapBuffers();
}

void CustomWindow::FrameBufferResizeCallback(Anima::AnimaWindow* window, int w, int h)
{
	void* ctx = window->GetOpenGLContext();
	
	if(ctx)
		glViewport(0, 0, w, h);
}