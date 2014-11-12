#include <stdio.h>
#include <AnimaEngine.h>

int main(int argc, char** argv)
{
	Anima::AnimaEngine engine;
	engine.Initialize();

	Anima::AnimaEngineWindow_Base* window = engine.CreateAnimaWindow(500, 300, "AnimaWindow", NULL, NULL);
	
	while (!window->ShouldClose())
	{ 
		;
	}

	return 0;
}