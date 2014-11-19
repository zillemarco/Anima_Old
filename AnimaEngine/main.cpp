#include <stdio.h>
#include <AnimaEngine.h>
#include "CustomWindow.h"
#include <AnimaVertex.hpp>
#include <AnimaAllocators.h>

#define MEM_SIZE 2097152

int main(int argc, char** argv)
{
#if _MSC_VER
	FreeConsole();
#endif
	void* pMem = malloc(MEM_SIZE);
	
	Anima::AnimaFreeListAllocator allocator(MEM_SIZE, pMem);
	printf("Memory used: %lu\n", allocator.GetUsedMemory());
	
	Anima::AnimaVertex4d* vector1 = new Anima::AnimaVertex<double, 4>(&allocator);
	printf("Memory used: %lu\n", allocator.GetUsedMemory());
	
	Anima::AnimaVertex<double, 4>* vector2 = new Anima::AnimaVertex<double, 4>(&allocator);
	printf("Memory used: %lu\n", allocator.GetUsedMemory());
	
	Anima::AnimaVertex<int, 3>* vector3 = new Anima::AnimaVertex<int, 3>(&allocator);
	printf("Memory used: %lu\n", allocator.GetUsedMemory());
	
	(*vector1)[0] = -1.0;
	(*vector1)[1] = 1.0;
	(*vector1)[2] = 56.0;
	(*vector1)[3] = -30.0;
	
	(*vector2) = (*vector1);
	
	(*vector3)[0] = -1;
	(*vector3)[1] = 50.0;
	(*vector3)[2] = 99;
	
	printf("Memory used: %lu\n", allocator.GetUsedMemory());
	
	delete vector2;
	printf("Memory used: %lu\n", allocator.GetUsedMemory());
	
	delete vector1;
	printf("Memory used: %lu\n", allocator.GetUsedMemory());
	
	delete vector3;
	printf("Memory used: %lu\n", allocator.GetUsedMemory());
	
	Anima::AnimaEngine engine;
	engine.Initialize();
	
//	engine.SetWindowHint(ANIMA_ENGINE_CORE_CONTEXT_VERSION_MAJOR, 4);
//	engine.SetWindowHint(ANIMA_ENGINE_CORE_CONTEXT_VERSION_MINOR, 1);
//	engine.SetWindowHint(ANIMA_ENGINE_CORE_OPENGL_FORWARD_COMPAT, true);
//	engine.SetWindowHint(ANIMA_ENGINE_CORE_OPENGL_CORE_PROFILE, true);
//	engine.SetWindowHint(ANIMA_ENGINE_CORE_OPENGL_PROFILE, ANIMA_ENGINE_CORE_OPENGL_CORE_PROFILE);
	CustomWindow* window = engine.CreateAnimaWindow<CustomWindow>(500, 500, "AnimaEngine Custom Window", NULL, NULL);
	
	window->MakeCurrentContext();
	engine.SwapInterval(1);
	window->MakeCurrentContext();
	window->FrameBufferResizeCallback(window, 500 * window->GetResolutionMutiplier(), 500 * window->GetResolutionMutiplier());
	
	if(!window->Load())
		return 0;
	
	while (!window->ShouldClose())
	{
		engine.PollEvents();
		
		window->DrawScene();
	}

	return 0;
}