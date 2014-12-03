#include <stdio.h>
//#include <AnimaEngine.h>
//#include <AnimaModelsManager.h>
//#include <AnimaBenchmarkTimer.h>
//#include <AnimaString.h>
#include <QApplication>
#include "CorpusMainWindow.h"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	CorpusMainWindow mainWindow;
	
	mainWindow.show();
	return app.exec();
	
	
//	Anima::AnimaEngine engine;
//	engine.Initialize();
//	
//#if defined _MSC_VER
//	Anima::AnimaString path("D:/Modelli/Big_Dragon/Big_Dragon.fbx", &engine);
//#else
//	Anima::AnimaString path("/Users/marco/Documents/Modelli/ALDIUN/OBJ/alduin.obj", &engine);
//#endif
//	
//	Anima::AnimaModelsManager* manager = engine.GetModelsManager();
//	if(!manager->LoadModel(path))
//		return 0;
//	
//	engine.DumpMemory();

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
		
//	return 0;
}