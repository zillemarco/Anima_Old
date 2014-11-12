#ifndef _CustomWindow_h
#define _CustomWindow_h

#include <AnimaEngine.h>

class CustomWindow : public Anima::AnimaEngineWindow_Base
{
public:
	CustomWindow();
	~CustomWindow();

	DECLARE_MESSAGE_MAP()

private:
	ANIMA_WINDOW_EVENT void MouseClickCallback(Anima::AnimaEngineWindow_Base* window, int button, int action, int mods);
};

#endif