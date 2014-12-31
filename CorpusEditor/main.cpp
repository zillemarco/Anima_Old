#include <stdio.h>
#include <AnimaEngine.h>
//#include <AnimaModelsManager.h>
#include <AnimaBenchmarkTimer.h>
#include <AnimaMatrix.h>
//#include <AnimaString.h>
#include <QApplication>
#include <QStyleFactory>
#include <QDateTime>
#include "CorpusMainWindow.h"

#ifndef _MSC_VER
#include <sys/syslimits.h>
#endif

#if defined _MSC_VER
#define PATH_MAX MAX_PATH
#endif

#include <stdio.h>
#include <stdlib.h>

char outputFileName[PATH_MAX];

void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	freopen (outputFileName, "a", stderr);
	
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
	Anima::AnimaEngine engine;
	engine.Initialize();
	
	float data1[16] = {	1, 9, 0, 3,
						0, 1, 0, 0,
						8, 9, 6, 1,
						0, 0, 0, 1};
	
	float data2[16] = {	6, 8, 3, 0,
						1, 5, -4, 6,
						2, 3, 0, 9,
						0, 1, 0, 0};
	
	Anima::AnimaMatrix matrix1(&engine, data1);
	Anima::AnimaMatrix matrix2(&engine, data2);
	
	matrix1.DumpMemory();
	matrix2.DumpMemory();
	
	Anima::AnimaMatrix matrix3 = matrix1 * matrix2;
	
	matrix3.DumpMemory();
	
	matrix1 = matrix1.Inverse();
	matrix1.DumpMemory();
	
	return 0;
	
	Anima::AnimaEngine::SetUsedExternal();
	
	QDateTime dateTime = QDateTime::currentDateTime();
	QString dateTimeString = dateTime.toString(QString("yyyyMMdd_HHmmss"));
	QByteArray dateTimeByteArray = dateTimeString.toLocal8Bit();
	
	sprintf(outputFileName, "%s.log", dateTimeByteArray.constData());
	
	FILE* f = fopen(outputFileName, "w");
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