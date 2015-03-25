#include <stdio.h>
#include <QApplication>
#include <QStyleFactory>
#include <QDateTime>
#include <AnimaEngine.h>
#include <AnimaWindow.h>
#include "CorpusMainWindow.h"
#include "CorpusDocument.h"
#include <AnimaString.h>
#include <AnimaModelsManager.h>
#include "Window.h"
#include <AnimaMath.h>
#include <AnimaCamerasManager.h>
#include <AnimaBenchmarkTimer.h>
#include <AnimaMesh.h>
#include <AnimaScene.h>
#include <AnimaScenesManager.h>
#include <AnimaMaterial.h>
#include <AnimaMaterialsManager.h>
#include <AnimaArray.h>

#define _USE_MATH_DEFINES
#include <math.h>

void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	freopen(Anima::AnimaEngine::GetLogFilePath(), "a", stderr);
	
	QByteArray localMsg = msg.toLocal8Bit();
	switch (type) {
		case QtDebugMsg:
			fprintf(stderr, "Debug: %s\n\tFile: %s\n\tLine: %u\n\tFunction: %s\n", localMsg.constData(), context.file, context.line, context.function);
			break;
		case QtWarningMsg:
			fprintf(stderr, "Warning: %s\n\tFile: %s\n\tLine: %u\n\tFunction: %s\n", localMsg.constData(), context.file, context.line, context.function);
			break;
		case QtCriticalMsg:
			fprintf(stderr, "Critical: %s\n\tFile: %s\n\tLine: %u\n\tFunction: %s\n", localMsg.constData(), context.file, context.line, context.function);
			break;
		case QtFatalMsg:
			fprintf(stderr, "Fatal: %s\n\tFile: %s\n\tLine: %u\n\tFunction: %s\n", localMsg.constData(), context.file, context.line, context.function);
			abort();
	}
	
	fclose (stderr);
}

int main(int argc, char** argv)
{
	//Anima::AnimaEngine::SetUsedExternal();
	//
	//QDateTime dateTime = QDateTime::currentDateTime();
	//QString dateTimeString = dateTime.toString(QString("yyyyMMdd_HHmmss"));
	//QByteArray dateTimeByteArray = dateTimeString.toLocal8Bit();
	//
	//Anima::AChar tmpFileName[PATH_MAX];
	//sprintf(tmpFileName, "%s.log", dateTimeByteArray.constData());
	//Anima::AnimaEngine::SetLogFilePath(tmpFileName);
	//
	//FILE* f = fopen(Anima::AnimaEngine::GetLogFilePath(), "w");
	//fclose(f);
	//
	//qInstallMessageHandler(messageOutput);
	//
	//QApplication app(argc, argv);
	//
	////qApp->setStyle(QStyleFactory::create("fusion"));

	////QPalette palette;
	////palette.setColor(QPalette::Window, QColor(53, 53, 53));
	////palette.setColor(QPalette::WindowText, Qt::white);
	////palette.setColor(QPalette::Base, QColor(15, 15, 15));
	////palette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
	////palette.setColor(QPalette::ToolTipBase, Qt::white);
	////palette.setColor(QPalette::ToolTipText, Qt::white);
	////palette.setColor(QPalette::Text, Qt::white);
	////palette.setColor(QPalette::Disabled, QPalette::Text, QColor(255, 255, 255).darker());
	////palette.setColor(QPalette::Button, QColor(53, 53, 53));
	////palette.setColor(QPalette::ButtonText, Qt::white);
	////palette.setColor(QPalette::BrightText, Qt::red);
	////palette.setColor(QPalette::Light, QColor(15, 15, 15));

	////palette.setColor(QPalette::Highlight, QColor(0, 152, 217).lighter());
	////palette.setColor(QPalette::HighlightedText, Qt::black);
	////qApp->setPalette(palette);

	//CorpusMainWindow mainWindow;
	//
	//mainWindow.show();
	//return app.exec();
	
	Anima::AnimaEngine engine;
	engine.Initialize();

	Anima::AnimaScene* scene = engine.GetScenesManager()->CreateStage("test-scene");
	scene->Initialize();

	engine.SetWindowHint(ANIMA_ENGINE_CONTEXT_VERSION_MAJOR, 4);
	engine.SetWindowHint(ANIMA_ENGINE_CONTEXT_VERSION_MINOR, 1);
	engine.SetWindowHint(ANIMA_ENGINE_OPENGL_FORWARD_COMPAT, true);
	engine.SetWindowHint(ANIMA_ENGINE_OPENGL_CORE_PROFILE, false);
	engine.SetWindowHint(ANIMA_ENGINE_OPENGL_PROFILE, ANIMA_ENGINE_OPENGL_COMPAT_PROFILE);
	engine.SetWindowHint(ANIMA_ENGINE_RESIZABLE, true);
	engine.SetWindowHint(ANIMA_ENGINE_DECORATED, true);

#if defined WIN32
	Anima::AInt width = 1024;
	Anima::AInt height = 768;
#else
	Anima::AInt width = 512;
	Anima::AInt height = 512;
#endif

	Window* window = engine.CreateAnimaWindow<Window>(width, height, "AnimaEngine Custom Window", /*Anima::AnimaEngine::GetPrimaryMonitor()*/NULL, NULL);

	window->MakeCurrentContext();
	engine.SwapInterval(1);
	window->MakeCurrentContext();
	window->FrameBufferResizeCallback(window, (int)(width * window->GetResolutionMutiplier()), (int)(height * window->GetResolutionMutiplier()));

	Anima::AnimaString path(scene->GetStringAllocator());	
	Anima::AnimaModelsManager* manager = scene->GetModelsManager();
	Anima::AnimaMaterialsManager* matMgr = scene->GetMaterialsManager();
	
	Anima::AnimaMesh* mesh = nullptr;

#ifndef _DEBUG
	path = ANIMA_ENGINE_MODELS_PATH "sponza.obj";
	if ((mesh = manager->LoadModel(path, "sponza")) == nullptr)
		return 0;
	mesh->ComputeBoundingBox(true);

	Anima::AnimaCamerasManager* camMan = scene->GetCamerasManager();
	window->_tpcamera = camMan->CreateNewThirdPersonCamera("tp");
	window->_fpcamera = camMan->CreateNewFirstPersonCamera("fp");

	Anima::AnimaVertex3f pos(0.0f, 100.0f, 0.0f);
	Anima::AnimaVertex3f tar(0.0f, 0.0f, 0.0f);
	Anima::AnimaVertex3f forw(0.0f, 0.0f, -1.0f);

	window->_tpcamera->LookAt(pos, tar);
	window->_fpcamera->LookAt(pos, forw);

	window->_fpcamera->Activate();

#else
	//path = ANIMA_ENGINE_MODELS_PATH "scimmia.3ds";
	//if ((mesh = manager->LoadModel(path, "scimmia")) == nullptr)
	//	return 0;
	//mesh->GetTransformation()->Scale(10.0f, 10.0f, 10.0f);
	//mesh->ComputeBoundingBox(true);

	//path = ANIMA_ENGINE_MODELS_PATH "scimmia.3ds";
	//if ((mesh = manager->LoadModel(path, "scimmia2")) == nullptr)
	//	return 0;
	//mesh->GetTransformation()->SetTranslation(0.0f, 0.0f, 20.0f);
	//mesh->ComputeBoundingBox(true);

	path = ANIMA_ENGINE_MODELS_PATH "piano.3ds";
	if ((mesh = manager->LoadModel(path, "piano")) == nullptr)
		return 0;
	mesh->ComputeBoundingBox(true);
	mesh->GetTransformation()->Scale(100.0f, 100.0f, 100.0f);

	Anima::AnimaCamerasManager* camMan = scene->GetCamerasManager();
	window->_tpcamera = camMan->CreateNewThirdPersonCamera("tp");
	window->_fpcamera = camMan->CreateNewFirstPersonCamera("fp");

	Anima::AnimaVertex3f pos(0.0f, 0.0f, 10.0f);
	Anima::AnimaVertex3f tar(0.0f, 0.0f, 0.0f);
	Anima::AnimaVertex3f forw(0.0f, 0.0f, -1.0f);

	window->_tpcamera->LookAt(pos, tar);
	window->_fpcamera->LookAt(pos, forw);

	window->_tpcamera->Activate();

#endif

	window->FrameBufferResizeCallback(window, (int)(width * window->GetResolutionMutiplier()), (int)(height * window->GetResolutionMutiplier()));
	window->Load();
	
	while (!window->ShouldClose())
	{
		engine.PollEvents();		
		window->DrawScene();
	}
		
	return 0;
}