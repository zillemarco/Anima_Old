#include <stdio.h>
#include <AnimaEngine.h>
#include <AnimaVertex.h>
#include <AnimaAllocators.h>
#include <AnimaModel.h>

#include "CustomWindow.h"

#define MEM_SIZE 2097152

int main(int argc, char** argv)
{
#if _MSC_VER
	//FreeConsole();
#endif

	void* mainMem = malloc(MEM_SIZE);
	Anima::AnimaFreeListAllocator allocator(MEM_SIZE, mainMem);
	printf("Used memory: %lu\n", allocator.GetUsedMemory());
	
	Anima::AnimaVertex4f v(&allocator);
	v[0] = 1.0;
	v[1] = 2.0;
	v[2] = -1.0;
	v[3] = -10.0;
	printf("Used memory: %lu\n", allocator.GetUsedMemory());
	allocator.Deallocate(&v);
	printf("Used memory: %lu\n", allocator.GetUsedMemory());
	
	Anima::AnimaModel* model = (Anima::AnimaModel*)allocator.Allocate(sizeof(Anima::AnimaModel), ANIMA_ENGINE_CORE_ALIGN_OF(Anima::AnimaModel));
	printf("Used memory: %lu\n", allocator.GetUsedMemory());
	
	model->SetVertices(&v, 1);
	printf("Used memory: %lu\n", allocator.GetUsedMemory());
	
	
	allocator.Deallocate(model);
	
	
	
	Anima::AnimaEngine engine;
	engine.Initialize();
	
	//engine.SetWindowHint(ANIMA_ENGINE_CORE_CONTEXT_VERSION_MAJOR, 4);
	//engine.SetWindowHint(ANIMA_ENGINE_CORE_CONTEXT_VERSION_MINOR, 3);
	//engine.SetWindowHint(ANIMA_ENGINE_CORE_OPENGL_FORWARD_COMPAT, true);
	//engine.SetWindowHint(ANIMA_ENGINE_CORE_OPENGL_CORE_PROFILE, true);
	//engine.SetWindowHint(ANIMA_ENGINE_CORE_OPENGL_PROFILE, ANIMA_ENGINE_CORE_OPENGL_CORE_PROFILE);
	//engine.SetWindowHint(ANIMA_ENGINE_CORE_RESIZABLE, false);
	CustomWindow* window = engine.CreateAnimaWindow<CustomWindow>(500, 500, "AnimaEngine Custom Window", NULL, NULL);
	
	window->MakeCurrentContext();
	engine.SwapInterval(1);
	window->MakeCurrentContext();
	window->FrameBufferResizeCallback(window, (int)(500 * window->GetResolutionMutiplier()), (int)(500 * window->GetResolutionMutiplier()));
	
	if(!window->Load())
		return 0;
	
	while (!window->ShouldClose())
	{
		engine.PollEvents();
		
		window->DrawScene();
	}
	
	free(mainMem);

	return 0;
}