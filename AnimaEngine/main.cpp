#include <stdio.h>
#include <AnimaEngine.h>
#include "CustomWindow.h"

int main(int argc, char** argv)
{
//	FreeConsole();
	Anima::AnimaEngine engine;
	engine.Initialize();
	
	CustomWindow* window = engine.CreateAnimaWindow<CustomWindow>(500, 500, "Custom window", NULL, NULL);
	
	window->MakeCurrentContext();
	engine.SwapInterval(1);
	window->MakeCurrentContext();
	window->FrameBufferResizeCallback(window, 500, 500);
	
	while (!window->ShouldClose())
	{
		engine.PollEvents();
		
		window->DrawScene();
	}

	return 0;
}