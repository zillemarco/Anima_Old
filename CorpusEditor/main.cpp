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
	Anima::AnimaEngine::SetUsedExternal();
	
	QDateTime dateTime = QDateTime::currentDateTime();
	QString dateTimeString = dateTime.toString(QString("yyyyMMdd_HHmmss"));
	QByteArray dateTimeByteArray = dateTimeString.toLocal8Bit();
	
	Anima::AChar tmpFileName[PATH_MAX];
	sprintf(tmpFileName, "%s.log", dateTimeByteArray.constData());
	Anima::AnimaEngine::SetLogFilePath(tmpFileName);
	
	FILE* f = fopen(Anima::AnimaEngine::GetLogFilePath(), "w");
	fclose(f);
	
	qInstallMessageHandler(messageOutput);
	
	QApplication app(argc, argv);
	
	qApp->setStyle(QStyleFactory::create("fusion"));

	QPalette palette;
	palette.setColor(QPalette::Window, QColor(53, 53, 53));
	palette.setColor(QPalette::WindowText, Qt::white);
	palette.setColor(QPalette::Base, QColor(15, 15, 15));
	palette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
	palette.setColor(QPalette::ToolTipBase, Qt::white);
	palette.setColor(QPalette::ToolTipText, Qt::white);
	palette.setColor(QPalette::Text, Qt::white);
	palette.setColor(QPalette::Disabled, QPalette::Text, QColor(255, 255, 255).darker());
	palette.setColor(QPalette::Button, QColor(53, 53, 53));
	palette.setColor(QPalette::ButtonText, Qt::white);
	palette.setColor(QPalette::BrightText, Qt::red);
	palette.setColor(QPalette::Light, QColor(15, 15, 15));

	palette.setColor(QPalette::Highlight, QColor(0, 152, 217).lighter());
	palette.setColor(QPalette::HighlightedText, Qt::black);
	qApp->setPalette(palette);

	CorpusMainWindow mainWindow;
	
	mainWindow.show();
	return app.exec();
	
//	Anima::AnimaEngine engine;
//	engine.Initialize();
//			
//#if defined _MSC_VER
//	Anima::AnimaString path("D:/Modelli/ninja/ninja.3ds", &engine);
//	Anima::AChar tmpFileName[PATH_MAX];
//	sprintf(tmpFileName, "C:/Users/Marco/Desktop/output/log.log");
//	Anima::AnimaEngine::SetLogFilePath(tmpFileName);
//#else
//	Anima::AnimaString path("/Users/marco/Documents/Modelli/Big_Dragon/Big_Dragon.fbx", &engine);
//#endif
//	
//	engine.DumpMemory(nullptr, false);
//
//	Anima::AnimaModelsManager* manager = engine.GetModelsManager();
//	if(!manager->LoadModel(path))
//		return 0;
//	
//	engine.DumpMemory(nullptr, false);
//				
//	engine.SetWindowHint(ANIMA_ENGINE_CONTEXT_VERSION_MAJOR, 4);
//	engine.SetWindowHint(ANIMA_ENGINE_CONTEXT_VERSION_MINOR, 1);
//	engine.SetWindowHint(ANIMA_ENGINE_OPENGL_FORWARD_COMPAT, true);
//	engine.SetWindowHint(ANIMA_ENGINE_OPENGL_CORE_PROFILE, false);
//	engine.SetWindowHint(ANIMA_ENGINE_OPENGL_PROFILE, ANIMA_ENGINE_OPENGL_CORE_PROFILE);
//	engine.SetWindowHint(ANIMA_ENGINE_RESIZABLE, true);
//
//	Window* window = engine.CreateAnimaWindow<Window>(500, 500, "AnimaEngine Custom Window", NULL, NULL);
//	
//	engine.DumpMemory(nullptr, false);
//
//	Anima::AnimaCamerasManager* camMan = engine.GetCamerasManager();
//	window->_tpcamera = camMan->CreateNewThirdPersonCamera();
//	window->_fpcamera = camMan->CreateNewFirstPersonCamera();
//	
//	engine.DumpMemory(nullptr, false);
//
//	Anima::AnimaVertex3f pos(&engine);
//	pos[0] = 0;
//	pos[1] = 0;
//	pos[2] = -10;
//	
//	engine.DumpMemory(nullptr, false);
//
//	Anima::AnimaVertex3f tar(&engine);
//	tar[0] = 0;
//	tar[1] = 0;
//	tar[2] = 0;
//	
//	engine.DumpMemory(nullptr, false);
//
//	Anima::AnimaVertex3f forw(&engine);
//	forw[0] = 0;
//	forw[1] = 0;
//	forw[2] = 1.0;
//	
//	engine.DumpMemory(nullptr, false);
//
//	window->_tpcamera->LookAt(pos, tar);
//	engine.DumpMemory(nullptr, false);
//	window->_fpcamera->LookAt(pos, forw);
//	engine.DumpMemory(nullptr, false);
//
//	window->_tpcamera->Activate();
//	engine.DumpMemory(nullptr, false);
//
//	window->MakeCurrentContext();
//	engine.SwapInterval(1);
//	window->MakeCurrentContext();
//	engine.DumpMemory(nullptr, false);
//	window->FrameBufferResizeCallback(window, (int)(500 * window->GetResolutionMutiplier()), (int)(500 * window->GetResolutionMutiplier()));
//	engine.DumpMemory(nullptr, false);
//	
//	window->Load();
//	engine.DumpMemory(nullptr, false);
//
//	while (!window->ShouldClose())
//	{
//		engine.PollEvents();
//		
//		engine.DumpMemory(nullptr, false);
//		window->DrawScene();
//		engine.DumpMemory(nullptr, false);
//	}
//		
//	return 0;
}