#include <stdio.h>
#include <QApplication>
#include <QStyleFactory>
#include <QDateTime>
#include <qfile>
#include <qtextstream.h>
#include <AnimaEngine.h>
#include <AnimaWindow.h>
#include "AEMainWindow.h"
#include "AEDocument.h"
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
#ifndef _LOCAL_TEST_
	Anima::AnimaEngine::SetUsedExternal();
	
	//QDateTime dateTime = QDateTime::currentDateTime();
	//QString dateTimeString = dateTime.toString(QString("yyyyMMdd_HHmmss"));
	//QByteArray dateTimeByteArray = dateTimeString.toLocal8Bit();
	
	//Anima::AChar tmpFileName[PATH_MAX];
	//sprintf(tmpFileName, "aelog.log", dateTimeByteArray.constData());
	Anima::AnimaEngine::SetLogFilePath("aelog.log");
	
	qInstallMessageHandler(messageOutput);
	
	QApplication app(argc, argv);

	//QFile File(":/Res/dark_style3.qss");
	//if (File.open(QIODevice::ReadOnly | QIODevice::Text))
	//{
	//	QTextStream stream(&File);
	//	qApp->setStyleSheet(stream.readAll());
	//	File.close();
	//}

	AEMainWindow mainWindow;	
	mainWindow.show();
	return app.exec();
	
#else

	Anima::AnimaEngine engine;
	engine.Initialize();

	engine.SetWindowHint(ANIMA_ENGINE_CONTEXT_VERSION_MAJOR, 4);
	engine.SetWindowHint(ANIMA_ENGINE_CONTEXT_VERSION_MINOR, 1);
	engine.SetWindowHint(ANIMA_ENGINE_OPENGL_FORWARD_COMPAT, true);
	engine.SetWindowHint(ANIMA_ENGINE_OPENGL_CORE_PROFILE, false);
	engine.SetWindowHint(ANIMA_ENGINE_OPENGL_PROFILE, ANIMA_ENGINE_OPENGL_CORE_PROFILE);
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

	window->_scene = engine.GetScenesManager()->CreateScene("AnimaEditor");

	Anima::AnimaString path(engine.GetStringAllocator());
	Anima::AnimaModelsManager* manager = window->_scene->GetModelsManager();
	Anima::AnimaMaterialsManager* matMgr = window->_scene->GetMaterialsManager();
	
	Anima::AnimaModel* model = nullptr;

#if defined _DEBUG
	
	Anima::AnimaCamerasManager* camMan = window->_scene->GetCamerasManager();
	window->_tpcamera = camMan->CreateThirdPersonCamera("tp");
	window->_fpcamera = camMan->CreateFirstPersonCamera("fp");
	
	path = ANIMA_ENGINE_MODELS_PATH "sponza.obj";
	if ((model = manager->LoadModel(path, "sponza")) == nullptr)
		return 0;
	//model->ComputeBoundingBox(true);

	Anima::AnimaVertex3f pos(0.0f, 1000.0f, 1.0f);
	Anima::AnimaVertex3f tar(0.0f, 0.0f, 0.0f);
	Anima::AnimaVertex3f forw(0.0f, 0.0f, -1.0f);

	window->_tpcamera->LookAt(pos, tar);
	window->_fpcamera->LookAt(pos, forw);

	window->_fpcamera->Activate();

#else
	path = ANIMA_ENGINE_MODELS_PATH "scimmia.3ds";
	if ((model = manager->LoadModel(path, "scimmia")) == nullptr)
		return 0;
	model->GetTransformation()->Scale(5.0f, 5.0f, 5.0f);
	//model->ComputeBoundingBox(true);

	path = ANIMA_ENGINE_MODELS_PATH "scimmia.3ds";
	if ((model = manager->LoadModel(path, "scimmia2")) == nullptr)
		return 0;
	model->GetTransformation()->SetTranslation(0.0f, 0.0f, 20.0f);
	//model->ComputeBoundingBox(true);

	path = ANIMA_ENGINE_MODELS_PATH "piano.3ds";
	if ((model = manager->LoadModel(path, "piano")) == nullptr)
		return 0;
	//model->ComputeBoundingBox(true);
	model->GetTransformation()->Scale(100.0f, 100.0f, 100.0f);

	Anima::AnimaCamerasManager* camMan = window->_scene->GetCamerasManager();
	window->_tpcamera = camMan->CreateThirdPersonCamera("tp");
	window->_fpcamera = camMan->CreateFirstPersonCamera("fp");

	Anima::AnimaVertex3f pos(0.0f, 0.0f, 30.0f);
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
#endif
}