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

	//float data[16] = {	2.0, 1.0, 5.0, 4.0,
	//					16.0, 5.0, 8.0, -1.0,
	//					-5.0, 4.0, -1.0, 0.0, 
	//					10.0, -5.0, 0.0, 4.0 };

	//Anima::AnimaMatrix trasp(data), traspSSE(data);
	//Anima::AnimaMatrix mul0(data), mul1(data), mulSSE0(data), mulSSE1(data);
	//Anima::AnimaMatrix inv(data), invSSE(data);

	//Anima::AnimaBenchmarkTimer timer;
	//double t;

	//char szBuff[1024];

	//OutputDebugStringA("TEST TRASPOSTA\n");
	//OutputDebugStringA("trasp:\n");
	//trasp.DumpMemory();
	//OutputDebugStringA("traspSSE:\n");
	//traspSSE.DumpMemory();

	//timer.Reset();
	//trasp.Transpose();
	//t = timer.Elapsed();
	//sprintf(szBuff, "trasp trasposta in: %.20f\n", t);
	//OutputDebugStringA(szBuff);
	//trasp.DumpMemory();

	//timer.Reset();
	//traspSSE.TransposeSSE();
	//t = timer.Elapsed();
	//sprintf(szBuff, "traspSSE trasposta in: %.20f\n", t);
	//OutputDebugStringA(szBuff);
	//traspSSE.DumpMemory();

	//OutputDebugStringA("TEST MOLTIPLICAZIONE\n");
	//OutputDebugStringA("mul0:\n");
	//mul0.DumpMemory();
	//OutputDebugStringA("mul1:\n");
	//mul1.DumpMemory();
	//OutputDebugStringA("mulSSE0:\n");
	//mulSSE0.DumpMemory();
	//OutputDebugStringA("mulSSE1:\n");
	//mulSSE1.DumpMemory();

	//timer.Reset();
	//mul0.MultiplyMatrix(mul1);
	//t = timer.Elapsed();
	//sprintf(szBuff, "mul0 moltiplicata in: %.20f\n", t);
	//OutputDebugStringA(szBuff);
	//mul0.DumpMemory();

	//timer.Reset();
	//mulSSE0.MultiplyMatrixSSE(mulSSE1);
	//t = timer.Elapsed();
	//sprintf(szBuff, "mul0SSE moltiplicata in: %.20f\n", t);
	//OutputDebugStringA(szBuff);
	//mulSSE0.DumpMemory();

	//OutputDebugStringA("TEST INVERSA\n");
	//OutputDebugStringA("inv:\n");
	//inv.DumpMemory();
	//OutputDebugStringA("invSSE:\n");
	//invSSE.DumpMemory();

	//timer.Reset();
	//inv.Inverse();
	//t = timer.Elapsed();
	//sprintf(szBuff, "inv inversa in: %.20f\n", t);
	//OutputDebugStringA(szBuff);
	//inv.DumpMemory();

	//timer.Reset();
	//invSSE.InverseSSE();
	//t = timer.Elapsed();
	//sprintf(szBuff, "invSSE inversa in: %.20f\n", t);
	//OutputDebugStringA(szBuff);
	//invSSE.DumpMemory();
			
#if defined _MSC_VER
	Anima::AnimaString path1("D:/Git/AnimaEngine/AnimaEngine/data/models/piano.3ds", &engine);
	Anima::AnimaString path2("D:/Git/AnimaEngine/AnimaEngine/data/models/scimmiaO.3ds", &engine);
	//Anima::AnimaString path2("D:/Git/AnimaEngine/AnimaEngine/data/models/LeePerrySmith.obj", &engine);
	Anima::AnimaString path3("D:/Git/AnimaEngine/AnimaEngine/data/models/x.3ds", &engine);
	Anima::AnimaString path4("D:/Git/AnimaEngine/AnimaEngine/data/models/y.3ds", &engine);
	Anima::AnimaString path5("D:/Git/AnimaEngine/AnimaEngine/data/models/z.3ds", &engine);
	//Anima::AnimaString path2("D:/Git/AnimaEngine/AnimaEngine/data/models/nurse.obj", &engine);
	Anima::AChar tmpFileName[PATH_MAX];
	sprintf(tmpFileName, "log.log");
	Anima::AnimaEngine::SetLogFilePath(tmpFileName);
#else
	Anima::AnimaString path("/Users/marco/Documents/Modelli/Big_Dragon/Big_Dragon.fbx", &engine);
#endif
	
	Anima::AnimaModelsManager* manager = engine.GetModelsManager();
	//if(!manager->LoadModel(path1))
	//	return 0;
	if (!manager->LoadModel(path2, "origine"))
		return 0;
	if (!manager->LoadModel(path3, "x-cubo"))
		return 0;
	if (!manager->LoadModel(path4, "y-sfera"))
		return 0;
	if (!manager->LoadModel(path5, "z-toro"))
		return 0;
				
	engine.SetWindowHint(ANIMA_ENGINE_CONTEXT_VERSION_MAJOR, 4);
	engine.SetWindowHint(ANIMA_ENGINE_CONTEXT_VERSION_MINOR, 1);
	engine.SetWindowHint(ANIMA_ENGINE_OPENGL_FORWARD_COMPAT, true);
	engine.SetWindowHint(ANIMA_ENGINE_OPENGL_CORE_PROFILE, false);
	engine.SetWindowHint(ANIMA_ENGINE_OPENGL_PROFILE, ANIMA_ENGINE_OPENGL_CORE_PROFILE);
	engine.SetWindowHint(ANIMA_ENGINE_RESIZABLE, true);

	Window* window = engine.CreateAnimaWindow<Window>(500, 500, "AnimaEngine Custom Window", NULL, NULL);
	
	Anima::AnimaCamerasManager* camMan = engine.GetCamerasManager();
	window->_tpcamera = camMan->CreateNewThirdPersonCamera();
	window->_fpcamera = camMan->CreateNewFirstPersonCamera();
	
	Anima::AnimaVertex3f pos(0, 0, 10);	
	Anima::AnimaVertex3f tar(0, 0, 0);
	Anima::AnimaVertex3f forw(0, 0, -1);

	window->_tpcamera->LookAt(pos, tar);
	window->_fpcamera->LookAt(pos, forw);
	
	window->_tpcamera->Activate();
	
	GLenum error = glGetError();

	window->MakeCurrentContext();
	error = glGetError();
	engine.SwapInterval(1);
	error = glGetError();
	window->MakeCurrentContext();
	error = glGetError();
	window->FrameBufferResizeCallback(window, (int)(500 * window->GetResolutionMutiplier()), (int)(500 * window->GetResolutionMutiplier()));
	error = glGetError();
	
	window->Load();
	error = glGetError();
	
	while (!window->ShouldClose())
	{
		engine.PollEvents();
		
		engine.DumpMemory(nullptr, false);
		window->DrawScene();
		engine.DumpMemory(nullptr, false);
	}
		
	return 0;
}