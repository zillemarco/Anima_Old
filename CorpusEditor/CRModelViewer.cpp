
#include <AnimaShadersManager.h>
#include <AnimaShaderProgram.h>
#include <AnimaModelsManager.h>
#include <AnimaCamerasManager.h>
#include <AnimaDataGeneratorsManager.h>
#include <AnimaMesh.h>
#include <AnimaShader.h>
#include <AnimaEngine.h>
#include <AnimaScene.h>
#include <AnimaScenesManager.h>
#include <QMouseEvent>
#include <QWheelEvent>

#include "CRModelViewer.h"
#include <qscreen>
#include <qmessagebox>
#include <QCoreApplication>

#define _USE_MATH_DEFINES
#include <math.h>

#define UPDATE_VIEW QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));

CRModelViewer::CRModelViewer(Anima::AnimaEngine* engine, QWindow* parent)
	: CorpusOGLWindowBase(engine, parent)
	, _frame(0)
{
	_renderingManager = new Anima::AnimaRenderingManager(_engine->GetSharedMemoryAllocator());
	_lastMouseXPos = 0;
	_lastMouseYPos = 0;
	_selectedModel = nullptr;
}

CRModelViewer::~CRModelViewer()
{
}

void CRModelViewer::Initialize()
{
	Anima::AnimaShadersManager* mgr = _engine->GetScenesManager()->GetStage("test-scene")->GetShadersManager();

	mgr->CreateProgram("phong");
	mgr->GetProgramFromName("phong")->Create();
	mgr->GetProgramFromName("phong")->AddShader(mgr->LoadShaderFromFile("phong-vs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Phong/phong.vs", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("phong")->AddShader(mgr->LoadShaderFromFile("phong-fs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Phong/phong.fs", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("phong")->Link();

	mgr->CreateProgram("forward-ambient");
	mgr->GetProgramFromName("forward-ambient")->Create();
	mgr->GetProgramFromName("forward-ambient")->AddShader(mgr->LoadShaderFromFile("forward-ambient-vs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Forward/forward-ambient.vs", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("forward-ambient")->AddShader(mgr->LoadShaderFromFile("forward-ambient-fs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Forward/forward-ambient.fs", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("forward-ambient")->Link();

	mgr->CreateProgram("forward-directional");
	mgr->GetProgramFromName("forward-directional")->Create();
	mgr->GetProgramFromName("forward-directional")->AddShader(mgr->LoadShaderFromFile("forward-directional-vs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Forward/forward-directional.vs", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("forward-directional")->AddShader(mgr->LoadShaderFromFile("forward-directional-fs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Forward/forward-directional.fs", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("forward-directional")->Link();

	mgr->CreateProgram("forward-point");
	mgr->GetProgramFromName("forward-point")->Create();
	mgr->GetProgramFromName("forward-point")->AddShader(mgr->LoadShaderFromFile("forward-point-vs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Forward/forward-point.vs", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("forward-point")->AddShader(mgr->LoadShaderFromFile("forward-point-fs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Forward/forward-point.fs", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("forward-point")->Link();

	mgr->CreateProgram("forward-spot");
	mgr->GetProgramFromName("forward-spot")->Create();
	mgr->GetProgramFromName("forward-spot")->AddShader(mgr->LoadShaderFromFile("forward-spot-vs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Forward/forward-spot.vs", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("forward-spot")->AddShader(mgr->LoadShaderFromFile("forward-spot-fs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Forward/forward-spot.fs", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("forward-spot")->Link();
}

void CRModelViewer::Render()
{
	const qreal retinaScale = devicePixelRatio();
	int w = width() * retinaScale;
	int h = height() * retinaScale;
	
	glViewport(0, 0, w, h);

	_engine->GetScenesManager()->GetStage("test-scene")->GetCamerasManager()->UpdatePerspectiveCameras(60.0f, w / h, 0.1f, 1000.0f);
	_engine->GetScenesManager()->GetStage("test-scene")->GetDataGeneratorsManager()->UpdateValues();

	_renderingManager->Start(_engine->GetScenesManager()->GetStage("test-scene"));

	if (_selectedModel != nullptr)
		_renderingManager->ForwardDrawSingleModel(_engine->GetScenesManager()->GetStage("test-scene"), _selectedModel);

	_renderingManager->Finish(_engine->GetScenesManager()->GetStage("test-scene"));

	++_frame;
}

void CRModelViewer::mouseMoveEvent(QMouseEvent* mEvent)
{
	const float translateDivisor = -50.0f;
	if ((mEvent->buttons() & Qt::MiddleButton) == Qt::MiddleButton)
	{
		if ((mEvent->modifiers() & Qt::ControlModifier) == Qt::ControlModifier)
		{
			double dx = _lastMouseXPos - mEvent->x();
			double dy = _lastMouseYPos - mEvent->y();

			_engine->GetScenesManager()->GetStage("test-scene")->GetCamerasManager()->GetActiveCamera()->Move(1.0f, 0.0f, 0.0f, dx / translateDivisor);
			_engine->GetScenesManager()->GetStage("test-scene")->GetCamerasManager()->GetActiveCamera()->Move(0.0f, 1.0f, 0.0f, dy / translateDivisor);

			UPDATE_VIEW;
		}
		else
		{
			double dx = _lastMouseXPos - mEvent->x();
			double dy = _lastMouseYPos - mEvent->y();

			_engine->GetScenesManager()->GetStage("test-scene")->GetCamerasManager()->GetActiveCamera()->RotateXDeg(dy);
			_engine->GetScenesManager()->GetStage("test-scene")->GetCamerasManager()->GetActiveCamera()->RotateYDeg(dx);

			UPDATE_VIEW;
		}
	}
	else if ((mEvent->buttons() & Qt::LeftButton) == Qt::LeftButton)
	{
		if ((mEvent->modifiers() & Qt::ControlModifier) == Qt::ControlModifier)
		{
			double dx = _lastMouseXPos - mEvent->x();
			double dy = _lastMouseYPos - mEvent->y();

			_engine->GetScenesManager()->GetStage("test-scene")->GetCamerasManager()->GetActiveCamera()->Move(1.0f, 0.0f, 0.0f, dx / translateDivisor);
			_engine->GetScenesManager()->GetStage("test-scene")->GetCamerasManager()->GetActiveCamera()->Move(0.0f, 1.0f, 0.0f, dy / translateDivisor);

			UPDATE_VIEW;
		}
		else if ((mEvent->modifiers() & Qt::AltModifier) == Qt::AltModifier)
		{
			double dx = _lastMouseXPos - mEvent->x();
			double dy = _lastMouseYPos - mEvent->y();

			_engine->GetScenesManager()->GetStage("test-scene")->GetCamerasManager()->GetActiveCamera()->RotateXDeg(dy);
			_engine->GetScenesManager()->GetStage("test-scene")->GetCamerasManager()->GetActiveCamera()->RotateYDeg(dx);

			UPDATE_VIEW;
		}
	}

	_lastMouseXPos = mEvent->x();
	_lastMouseYPos = mEvent->y();
}

void CRModelViewer::wheelEvent(QWheelEvent* wEvent)
{
	QPoint numPixels = wEvent->pixelDelta();
	QPoint numDegrees = wEvent->angleDelta() / 8;

	float zoomAmount = 0.0f;

	if (!numPixels.isNull()) 
	{
		zoomAmount = (float)numPixels.y();
	}
	else if (!numDegrees.isNull()) 
	{
		QPoint numSteps = numDegrees / 15;
		zoomAmount = (float)numSteps.y();
	}

	_engine->GetScenesManager()->GetStage("test-scene")->GetCamerasManager()->GetActiveCamera()->Zoom(zoomAmount);
}

void CRModelViewer::setSelectedModel(Anima::AnimaMesh* model)
{
	_selectedModel = model;

	if (_selectedModel != nullptr)
	{
		_selectedModel->ComputeBoundingBox(true);
		Anima::AnimaVertex3f min = _selectedModel->GetBoundingBoxMin();
		Anima::AnimaVertex3f max = _selectedModel->GetBoundingBoxMax();

		float boxL = fabs(max.x - min.x);
		float boxH = fabs(max.y - min.y);
		float boxP = fabs(max.z - min.z);

		Anima::AnimaVertex3f center(max.x - (boxL / 2.0f), max.y - (boxH / 2.0f), max.z - (boxP / 2.0f));
		Anima::AnimaVertex3f pos = center;
		pos.z += (boxH / 2.0f) / sinf(60.0f * (float)M_PI / 180.0f);

		_engine->GetScenesManager()->GetStage("test-scene")->GetCamerasManager()->GetActiveCamera()->LookAt(pos, center);
	}
}