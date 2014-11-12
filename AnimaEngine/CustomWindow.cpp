#include "CustomWindow.h"
#include <stdio.h>

BEGIN_MESSAGE_MAP(CustomWindow, Anima::AnimaEngineWindow_Base)
	ANIMA_WINDOW_MOUSE_CLICK_EVENT(MouseClickCallback)
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