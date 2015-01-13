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
	//qApp->setStyle(QStyleFactory::create("fusion"));

	//QPalette palette;
	//palette.setColor(QPalette::Window, QColor(53, 53, 53));
	//palette.setColor(QPalette::WindowText, Qt::white);
	//palette.setColor(QPalette::Base, QColor(15, 15, 15));
	//palette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
	//palette.setColor(QPalette::ToolTipBase, Qt::white);
	//palette.setColor(QPalette::ToolTipText, Qt::white);
	//palette.setColor(QPalette::Text, Qt::white);
	//palette.setColor(QPalette::Disabled, QPalette::Text, QColor(255, 255, 255).darker());
	//palette.setColor(QPalette::Button, QColor(53, 53, 53));
	//palette.setColor(QPalette::ButtonText, Qt::white);
	//palette.setColor(QPalette::BrightText, Qt::red);
	//palette.setColor(QPalette::Light, QColor(15, 15, 15));

	//palette.setColor(QPalette::Highlight, QColor(0, 152, 217).lighter());
	//palette.setColor(QPalette::HighlightedText, Qt::black);
	//qApp->setPalette(palette);

	//CorpusMainWindow mainWindow;
	//
	//mainWindow.show();
	//return app.exec();
	
	
	Anima::AnimaEngine engine;
	engine.Initialize();
		
#if defined _MSC_VER
	Anima::AnimaString path("D:/Modelli/ninja/ninja.3ds", &engine);
	Anima::AChar tmpFileName[PATH_MAX];
	sprintf(tmpFileName, "C:/Users/Marco/Desktop/output/log.log");
	Anima::AnimaEngine::SetLogFilePath(tmpFileName);
#else
	Anima::AnimaString path("/Users/marco/Documents/Modelli/Big_Dragon/Big_Dragon.fbx", &engine);
#endif
	
	Anima::AnimaModelsManager* manager = engine.GetModelsManager();
	if(!manager->LoadModel(path))
		return 0;
			
	engine.SetWindowHint(ANIMA_ENGINE_CONTEXT_VERSION_MAJOR, 4);
	engine.SetWindowHint(ANIMA_ENGINE_CONTEXT_VERSION_MINOR, 1);
	engine.SetWindowHint(ANIMA_ENGINE_OPENGL_FORWARD_COMPAT, true);
	engine.SetWindowHint(ANIMA_ENGINE_OPENGL_CORE_PROFILE, false);
	engine.SetWindowHint(ANIMA_ENGINE_OPENGL_PROFILE, ANIMA_ENGINE_OPENGL_CORE_PROFILE);
	engine.SetWindowHint(ANIMA_ENGINE_RESIZABLE, true);

	Window* window = engine.CreateAnimaWindow<Window>(500, 500, "AnimaEngine Custom Window", NULL, NULL);
	
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