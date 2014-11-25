#include <stdio.h>
#include <AnimaEngine.h>
#include <AnimaVertex.h>
#include <AnimaAllocators.h>
#include <AnimaModel.h>
#include <AnimaModelsManager.h>

#include "CustomWindow.h"

using namespace Anima::AnimaAllocatorNamespace;

int main(int argc, char** argv)
{		
#if defined _MSC_VER
	const char* path = "D:/Modelli/Big_Dragon/Big_Dragon.fbx";
#else
	const char* path = "/Users/marco/Documents/Modelli/ALDIUN/OBJ/alduin.obj";
#endif

	Anima::AnimaEngine engine;
	engine.Initialize();

	engine.DumpMemory();

	Anima::AnimaModelsManager* manager = engine.GetModelsManager();
	if(!manager->LoadModel(path))
		return 0;

	engine.DumpMemory();

	//Anima::AnimaEngine engine;
	//engine.Initialize();
	//
	////engine.SetWindowHint(ANIMA_ENGINE_CORE_CONTEXT_VERSION_MAJOR, 4);
	////engine.SetWindowHint(ANIMA_ENGINE_CORE_CONTEXT_VERSION_MINOR, 3);
	////engine.SetWindowHint(ANIMA_ENGINE_CORE_OPENGL_FORWARD_COMPAT, true);
	////engine.SetWindowHint(ANIMA_ENGINE_CORE_OPENGL_CORE_PROFILE, true);
	////engine.SetWindowHint(ANIMA_ENGINE_CORE_OPENGL_PROFILE, ANIMA_ENGINE_CORE_OPENGL_CORE_PROFILE);
	////engine.SetWindowHint(ANIMA_ENGINE_CORE_RESIZABLE, false);
	//CustomWindow* window = engine.CreateAnimaWindow<CustomWindow>(500, 500, "AnimaEngine Custom Window", NULL, NULL);
	//
	//window->MakeCurrentContext();
	//engine.SwapInterval(1);
	//window->MakeCurrentContext();
	//window->FrameBufferResizeCallback(window, (int)(500 * window->GetResolutionMutiplier()), (int)(500 * window->GetResolutionMutiplier()));
	//
	//if(!window->Load())
	//	return 0;
	//
	//while (!window->ShouldClose())
	//{
	//	engine.PollEvents();
	//	
	//	window->DrawScene();
	//}
		
	return 0;
}