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
#include <AnimaStage.h>
#include <AnimaStagesManager.h>

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

	Anima::AnimaStage* stage = engine.GetStagesManager()->CreateStage("test-stage");
	stage->Initialize();

	Anima::AnimaString path(ANIMA_ENGINE_MODELS_PATH "scimmiaO.3ds", stage->GetStringAllocator());
	Anima::AnimaString pathPlane(ANIMA_ENGINE_MODELS_PATH "piano.3ds", stage->GetStringAllocator());

	Anima::AnimaModelsManager* manager = stage->GetModelsManager();
	if (!manager->LoadModel(path, "scimmia"))
		return 0;
	if (!manager->LoadModel(pathPlane, "piano"))
		return 0;
	
	engine.SetWindowHint(ANIMA_ENGINE_CONTEXT_VERSION_MAJOR, 4);
	engine.SetWindowHint(ANIMA_ENGINE_CONTEXT_VERSION_MINOR, 1);
	engine.SetWindowHint(ANIMA_ENGINE_OPENGL_FORWARD_COMPAT, true);
	engine.SetWindowHint(ANIMA_ENGINE_OPENGL_CORE_PROFILE, false);
	engine.SetWindowHint(ANIMA_ENGINE_OPENGL_PROFILE, ANIMA_ENGINE_OPENGL_CORE_PROFILE);
	engine.SetWindowHint(ANIMA_ENGINE_RESIZABLE, true);

	Window* window = engine.CreateAnimaWindow<Window>(500, 500, "AnimaEngine Custom Window", NULL, NULL);
	
	Anima::AnimaCamerasManager* camMan = stage->GetCamerasManager();
	window->_tpcamera = camMan->CreateNewThirdPersonCamera("tp");
	window->_fpcamera = camMan->CreateNewFirstPersonCamera("fp");
	
	Anima::AnimaVertex3f pos(0, 0.5, 10);	
	Anima::AnimaVertex3f tar(0, 0, 0);
	Anima::AnimaVertex3f forw(0, 0, -1);

	window->_tpcamera->LookAt(pos, tar);
	window->_fpcamera->LookAt(pos, forw);
	
	window->_tpcamera->Activate();
	
	window->MakeCurrentContext();
	engine.SwapInterval(1);
	window->MakeCurrentContext();
	window->FrameBufferResizeCallback(window, (int)(500 * window->GetResolutionMutiplier()), (int)(500 * window->GetResolutionMutiplier()));
	
	window->Load();
	
	while (!window->ShouldClose())
	{
		engine.PollEvents();		
		window->DrawScene();
	}
		
	return 0;
}