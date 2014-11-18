#ifndef _CustomWindow_h
#define _CustomWindow_h

#include <AnimaEngine.h>

class CustomWindow : public Anima::AnimaWindow
{
public:
	CustomWindow();
	~CustomWindow();

	DECLARE_MESSAGE_MAP()

public:
	void DrawScene();
	
private:
	ANIMA_WINDOW_EVENT void MouseClickCallback(Anima::AnimaWindow* window, int button, int action, int mods);
	ANIMA_WINDOW_EVENT void PaintCallback(Anima::AnimaWindow* window);
	ANIMA_WINDOW_EVENT void MouseMoveCallback(Anima::AnimaWindow* window, double x, double y);
public:
	ANIMA_WINDOW_EVENT void FrameBufferResizeCallback(Anima::AnimaWindow* window, int w, int h);
};

#endif