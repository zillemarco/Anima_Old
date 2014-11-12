#include <stdio.h>
#include <AnimaEngine.h>
#include "CustomWindow.h"

int main(int argc, char** argv)
{
	FreeConsole();
	Anima::AnimaEngine engine;
	engine.Initialize();
	
	Anima::AnimaEngineWindow_Base* window1 = engine.CreateAnimaWindow<Anima::AnimaEngineWindow_Base>(500, 300, "AnimaWindow1", NULL, NULL);
	CustomWindow* window2 = engine.CreateAnimaWindow<CustomWindow>(500, 500, "Custom window", NULL, NULL);
	
	while (!window1->ShouldClose())
	{ 
		engine.PollEvents();
	}

	return 0;
}