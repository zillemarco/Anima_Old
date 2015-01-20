
#include <AnimaShadersManager.h>
#include <AnimaShaderProgram.h>
#include <AnimaModelsManager.h>
#include <AnimaCamerasManager.h>
#include <AnimaModel.h>
#include <AnimaShader.h>
#include <AnimaEngine.h>
#include <QMouseEvent>
#include <QWheelEvent>

#include "CRModelViewer.h"
#include <qscreen>
#include <qmessagebox>
#include <QCoreApplication>

#define UPDATE_VIEW QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));

CRModelViewer::CRModelViewer(Anima::AnimaEngine* engine, QWindow* parent)
	: CorpusOGLWindowBase(engine, parent)
	, _program(nullptr)
	, _frame(0)
{
//	_animating = false;
	_lastMouseXPos = 0;
	_lastMouseYPos = 0;
}

CRModelViewer::~CRModelViewer()
{
}

GLuint CRModelViewer::loadShader(GLenum type, const char *source)
{
	return 0;
}

void CRModelViewer::Initialize()
{
#if defined _MSC_VER
	Anima::AnimaShader* vs = _engine->GetShadersManager()->LoadShaderFromFile("D:/Git/AnimaEngine/AnimaEngine/data/shaders/test/shader.vs", Anima::AnimaShader::VERTEX);
	Anima::AnimaShader* fs = _engine->GetShadersManager()->LoadShaderFromFile("D:/Git/AnimaEngine/AnimaEngine/data/shaders/test/shader.fs", Anima::AnimaShader::FRAGMENT);
#else
	Anima::AnimaShader* vs = _engine->GetShadersManager()->LoadShaderFromFile("/Users/marco/Documents/Progetti/Repository/AnimaEngine/AnimaEngine/data/shaders/test/shader.vs", Anima::AnimaShader::VERTEX);
	Anima::AnimaShader* fs = _engine->GetShadersManager()->LoadShaderFromFile("/Users/marco/Documents/Progetti/Repository/AnimaEngine/AnimaEngine/data/shaders/test/shader.fs", Anima::AnimaShader::FRAGMENT);
#endif
	
	_program = _engine->GetShadersManager()->CreateProgram();
	_program->Create();
	_program->AddShader(vs);
	_program->AddShader(fs);
	_program->Link();

	_program->AddUniform("gWorld");
	_program->AddUniform("uniformColor");

	//_matrixUniform = glGetUniformLocation(_program->GetID(), "gWorld");
	//_posAttr = glGetUniformLocation(_program->GetID(), "posAttr");
}

void CRModelViewer::Render()
{
	const qreal retinaScale = devicePixelRatio();
	int w = width() * retinaScale;
	int h = height() * retinaScale;
	
	glViewport(0, 0, w, h);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
	glFrontFace(GL_CW);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	_program->Use();

	Anima::AnimaMatrix projection(_engine);
	projection.Perspective(60.0f, w / h, 0.01f, 1000.0f);

	Anima::AnimaMatrix camera = _engine->GetCamerasManager()->GetActiveCamera()->GetViewMatrix();
	Anima::AnimaMatrix model(_engine);
	_program->SetUniform("gWorld", model * camera * projection);
	_program->SetUniform("uniformColor", 0.0, 1.0, 1.0);
	
	Anima::AnimaModelsManager* mgr = _engine->GetModelsManager();
	Anima::AnimaMatrix m(_engine);
	if (mgr->GetModelsNumber() > 0)
	{
		mgr->GetPModel(0)->Draw(m);
	}

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

			_engine->GetCamerasManager()->GetActiveCamera()->Move(1.0f, 0.0f, 0.0f, dx / translateDivisor);
			_engine->GetCamerasManager()->GetActiveCamera()->Move(0.0f, 1.0f, 0.0f, dy / translateDivisor);

			UPDATE_VIEW;
		}
		else
		{
			double dx = _lastMouseXPos - mEvent->x();
			double dy = _lastMouseYPos - mEvent->y();

			_engine->GetCamerasManager()->GetActiveCamera()->RotateXDeg(dy);
			_engine->GetCamerasManager()->GetActiveCamera()->RotateYDeg(dx);

			UPDATE_VIEW;
		}
	}
	else if ((mEvent->buttons() & Qt::LeftButton) == Qt::LeftButton)
	{
		if ((mEvent->modifiers() & Qt::ControlModifier) == Qt::ControlModifier)
		{
			double dx = _lastMouseXPos - mEvent->x();
			double dy = _lastMouseYPos - mEvent->y();

			_engine->GetCamerasManager()->GetActiveCamera()->Move(1.0f, 0.0f, 0.0f, dx / translateDivisor);
			_engine->GetCamerasManager()->GetActiveCamera()->Move(0.0f, 1.0f, 0.0f, dy / translateDivisor);

			UPDATE_VIEW;
		}
		else if ((mEvent->modifiers() & Qt::AltModifier) == Qt::AltModifier)
		{
			double dx = _lastMouseXPos - mEvent->x();
			double dy = _lastMouseYPos - mEvent->y();

			_engine->GetCamerasManager()->GetActiveCamera()->RotateXDeg(dy);
			_engine->GetCamerasManager()->GetActiveCamera()->RotateYDeg(dx);

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

	_engine->GetCamerasManager()->GetActiveCamera()->Zoom(zoomAmount);
}