
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

#include "AEResourcesManagerModelViewer.h"
#include <qscreen>
#include <qmessagebox>
#include <QCoreApplication>

#define _USE_MATH_DEFINES
#include <math.h>

#define UPDATE_VIEW QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));

AEResourcesManagerModelViewer::AEResourcesManagerModelViewer(Anima::AnimaEngine* engine, QWindow* parent)
	: AEOGLWindowBase(engine, parent)
	, _frame(0)
{
	_renderingManager = nullptr;
	_lastMouseXPos = 0;
	_lastMouseYPos = 0;
	_selectedModel = nullptr;
}

AEResourcesManagerModelViewer::~AEResourcesManagerModelViewer()
{
	if (_renderingManager != nullptr)
	{
		delete _renderingManager;
		_renderingManager = nullptr;
	}
}

void AEResourcesManagerModelViewer::Initialize()
{
	_renderingManager = new Anima::AnimaRenderingManager(_engine->GetScenesManager()->GetScene("AnimaEditor"), _engine->GetGenericAllocator());

	const qreal retinaScale = devicePixelRatio();
	int w = width() * retinaScale;
	int h = height() * retinaScale;

	_renderingManager->InitRenderingUtilities(w, h);
	_renderingManager->InitRenderingTargets(w, h);

	Anima::AnimaShadersManager* mgr = _engine->GetScenesManager()->GetScene("AnimaEditor")->GetShadersManager();

	mgr->CreateProgram("deferred-prepare");
	mgr->GetProgramFromName("deferred-prepare")->Create();
	mgr->GetProgramFromName("deferred-prepare")->AddShader(mgr->LoadShaderFromFile("deferred-prepare-vs", ANIMA_ENGINE_SHADERS_PATH "Deferred/deferred-prepare-vs.glsl", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("deferred-prepare")->AddShader(mgr->LoadShaderFromFile("deferred-prepare-fs", ANIMA_ENGINE_SHADERS_PATH "Deferred/deferred-prepare-fs.glsl", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("deferred-prepare")->Link();

	mgr->CreateProgram("deferred-shadowMap");
	mgr->GetProgramFromName("deferred-shadowMap")->Create();
	mgr->GetProgramFromName("deferred-shadowMap")->AddShader(mgr->LoadShaderFromFile("deferred-shadowMap-vs", ANIMA_ENGINE_SHADERS_PATH "Deferred/deferred-shadowMap-vs.glsl", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("deferred-shadowMap")->AddShader(mgr->LoadShaderFromFile("deferred-shadowMap-fs", ANIMA_ENGINE_SHADERS_PATH "Deferred/deferred-shadowMap-fs.glsl", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("deferred-shadowMap")->Link();

	mgr->CreateProgram("deferred-combine");
	mgr->GetProgramFromName("deferred-combine")->Create();
	mgr->GetProgramFromName("deferred-combine")->AddShader(mgr->LoadShaderFromFile("deferred-combine-vs", ANIMA_ENGINE_SHADERS_PATH "Deferred/deferred-combine-vs.glsl", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("deferred-combine")->AddShader(mgr->LoadShaderFromFile("deferred-combine-fs", ANIMA_ENGINE_SHADERS_PATH "Deferred/deferred-combine-fs.glsl", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("deferred-combine")->Link();

	mgr->CreateProgram("primitive-draw");
	mgr->GetProgramFromName("primitive-draw")->Create();
	mgr->GetProgramFromName("primitive-draw")->AddShader(mgr->LoadShaderFromFile("primitive-draw-vs", ANIMA_ENGINE_SHADERS_PATH "Primitive/primitive-vs.glsl", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("primitive-draw")->AddShader(mgr->LoadShaderFromFile("primitive-draw-fs", ANIMA_ENGINE_SHADERS_PATH "Primitive/primitive-fs.glsl", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("primitive-draw")->Link();

	mgr->CreateProgram("primitive-combine");
	mgr->GetProgramFromName("primitive-combine")->Create();
	mgr->GetProgramFromName("primitive-combine")->AddShader(mgr->LoadShaderFromFile("primitive-combine-vs", ANIMA_ENGINE_SHADERS_PATH "Primitive/combine-vs.glsl", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("primitive-combine")->AddShader(mgr->LoadShaderFromFile("primitive-combine-fs", ANIMA_ENGINE_SHADERS_PATH "Primitive/combine-fs.glsl", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("primitive-combine")->Link();

	mgr->CreateProgram("fxaaFilter");
	mgr->GetProgramFromName("fxaaFilter")->Create();
	mgr->GetProgramFromName("fxaaFilter")->AddShader(mgr->LoadShaderFromFile("fxaaFilter-vs", ANIMA_ENGINE_SHADERS_PATH "Filters/fxaaFilter-vs.glsl", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("fxaaFilter")->AddShader(mgr->LoadShaderFromFile("fxaaFilter-fs", ANIMA_ENGINE_SHADERS_PATH "Filters/fxaaFilter-fs.glsl", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("fxaaFilter")->Link();

	mgr->CreateProgram("gaussBlur7x1Filter");
	mgr->GetProgramFromName("gaussBlur7x1Filter")->Create();
	mgr->GetProgramFromName("gaussBlur7x1Filter")->AddShader(mgr->LoadShaderFromFile("gaussBlur7x1Filter-vs", ANIMA_ENGINE_SHADERS_PATH "Filters/gaussBlur7x1Filter-vs.glsl", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("gaussBlur7x1Filter")->AddShader(mgr->LoadShaderFromFile("gaussBlur7x1Filter-fs", ANIMA_ENGINE_SHADERS_PATH "Filters/gaussBlur7x1Filter-fs.glsl", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("gaussBlur7x1Filter")->Link();

	mgr->CreateProgram("ssao");
	mgr->GetProgramFromName("ssao")->Create();
	mgr->GetProgramFromName("ssao")->AddShader(mgr->LoadShaderFromFile("ssao-vs", ANIMA_ENGINE_SHADERS_PATH "Deferred/ssao-vs.glsl", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("ssao")->AddShader(mgr->LoadShaderFromFile("ssao-fs", ANIMA_ENGINE_SHADERS_PATH "Deferred/ssao-fs.glsl", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("ssao")->Link();
}

void AEResourcesManagerModelViewer::Render()
{
	const qreal retinaScale = devicePixelRatio();
	int w = width() * retinaScale;
	int h = height() * retinaScale;

	_renderingManager->InitRenderingTargets(w, h);

	glViewport(0, 0, w, h);

	_engine->GetScenesManager()->GetScene("AnimaEditor")->GetCamerasManager()->UpdatePerspectiveCameras(60.0f, w / h, 0.1f, 1000.0f);
	_engine->GetScenesManager()->GetScene("AnimaEditor")->GetDataGeneratorsManager()->UpdateValues();

	_renderingManager->Start(_engine->GetScenesManager()->GetScene("AnimaEditor"));

	_renderingManager->DeferredDrawAll(_engine->GetScenesManager()->GetScene("AnimaEditor"));

	//if (_selectedModel != nullptr)
	//	_renderingManager->DeferredDrawModel(_engine->GetScenesManager()->GetScene("AnimaEditor"), _selectedModel);

	_renderingManager->Finish(_engine->GetScenesManager()->GetScene("AnimaEditor"));

	++_frame;
}

void AEResourcesManagerModelViewer::mouseMoveEvent(QMouseEvent* mEvent)
{
	const float translateDivisor = -50.0f;
	if ((mEvent->buttons() & Qt::MiddleButton) == Qt::MiddleButton)
	{
		if ((mEvent->modifiers() & Qt::ControlModifier) == Qt::ControlModifier)
		{
			double dx = _lastMouseXPos - mEvent->x();
			double dy = _lastMouseYPos - mEvent->y();

			_engine->GetScenesManager()->GetScene("AnimaEditor")->GetCamerasManager()->GetActiveCamera()->Move(1.0f, 0.0f, 0.0f, dx / translateDivisor);
			_engine->GetScenesManager()->GetScene("AnimaEditor")->GetCamerasManager()->GetActiveCamera()->Move(0.0f, 1.0f, 0.0f, dy / translateDivisor);

			UPDATE_VIEW;
		}
		else
		{
			double dx = _lastMouseXPos - mEvent->x();
			double dy = _lastMouseYPos - mEvent->y();

			_engine->GetScenesManager()->GetScene("AnimaEditor")->GetCamerasManager()->GetActiveCamera()->RotateXDeg(dy);
			_engine->GetScenesManager()->GetScene("AnimaEditor")->GetCamerasManager()->GetActiveCamera()->RotateYDeg(dx);

			UPDATE_VIEW;
		}
	}
	else if ((mEvent->buttons() & Qt::LeftButton) == Qt::LeftButton)
	{
		if ((mEvent->modifiers() & Qt::ControlModifier) == Qt::ControlModifier)
		{
			double dx = _lastMouseXPos - mEvent->x();
			double dy = _lastMouseYPos - mEvent->y();

			_engine->GetScenesManager()->GetScene("AnimaEditor")->GetCamerasManager()->GetActiveCamera()->Move(1.0f, 0.0f, 0.0f, dx / translateDivisor);
			_engine->GetScenesManager()->GetScene("AnimaEditor")->GetCamerasManager()->GetActiveCamera()->Move(0.0f, 1.0f, 0.0f, dy / translateDivisor);

			UPDATE_VIEW;
		}
		else if ((mEvent->modifiers() & Qt::AltModifier) == Qt::AltModifier)
		{
			double dx = _lastMouseXPos - mEvent->x();
			double dy = _lastMouseYPos - mEvent->y();

			_engine->GetScenesManager()->GetScene("AnimaEditor")->GetCamerasManager()->GetActiveCamera()->RotateXDeg(dy);
			_engine->GetScenesManager()->GetScene("AnimaEditor")->GetCamerasManager()->GetActiveCamera()->RotateYDeg(dx);

			UPDATE_VIEW;
		}
	}

	_lastMouseXPos = mEvent->x();
	_lastMouseYPos = mEvent->y();
}

void AEResourcesManagerModelViewer::wheelEvent(QWheelEvent* wEvent)
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

	_engine->GetScenesManager()->GetScene("AnimaEditor")->GetCamerasManager()->GetActiveCamera()->Zoom(zoomAmount);
}

void AEResourcesManagerModelViewer::setSelectedModel(Anima::AnimaModel* model)
{
	_selectedModel = model;

	if (_selectedModel != nullptr)
	{
		// _selectedModel->ComputeBoundingBox(true);
		// Anima::AnimaVertex3f min = _selectedModel->GetBoundingBoxMin();
		// Anima::AnimaVertex3f max = _selectedModel->GetBoundingBoxMax();
		// 
		// float boxL = fabs(max.x - min.x);
		// float boxH = fabs(max.y - min.y);
		// float boxP = fabs(max.z - min.z);
		// 
		// Anima::AnimaVertex3f center(max.x - (boxL / 2.0f), max.y - (boxH / 2.0f), max.z - (boxP / 2.0f));
		// Anima::AnimaVertex3f pos = center;
		// pos.z += (boxH / 2.0f) / sinf(60.0f * (float)M_PI / 180.0f);
		// 
		// _engine->GetScenesManager()->GetScene("AnimaEditor")->GetCamerasManager()->GetActiveCamera()->LookAt(pos, center);
	}
}