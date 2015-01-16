
#include <AnimaShadersManager.h>
#include <AnimaShaderProgram.h>
#include <AnimaModelsManager.h>
#include <AnimaCamerasManager.h>
#include <AnimaModel.h>
#include <AnimaShader.h>
#include <AnimaEngine.h>
#include <QMouseEvent>

#include "CRModelViewer.h"
#include <qscreen>
#include <qmessagebox>

CRModelViewer::CRModelViewer(Anima::AnimaEngine* engine, QWindow* parent)
	: CorpusOGLWindowBase(engine, parent)
	, _program(nullptr)
	, _frame(0)
{
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

	char str[1024];
	sprintf(str, "%s", glGetString(GL_VERSION));
	
	GLenum error = glGetError();
	_program = _engine->GetShadersManager()->CreateProgram();
	_program->Create();
	_program->AddShader(vs);
	_program->AddShader(fs);
	_program->Link();
	error = glGetError();

	_matrixUniform = glGetUniformLocation(_program->GetID(), "gWorld");
	error = glGetError();
	_posAttr = glGetUniformLocation(_program->GetID(), "posAttr");
	error = glGetError();
	
	error = glGetError();
}

void CRModelViewer::Render()
{
	const qreal retinaScale = devicePixelRatio();
	int w = width() * retinaScale;
	int h = height() * retinaScale;
	
	GLenum error = glGetError();
	glViewport(0, 0, w, h);
	error = glGetError();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	error = glGetError();

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glFrontFace(GL_CW);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	_program->Use();

	Anima::AnimaMatrix projection(_engine);
	projection.Perspective(60.0f, w / h, 0.01f, 1000.0f);

	Anima::AnimaMatrix camera = _engine->GetCamerasManager()->GetActiveCamera()->GetViewMatrix();
	Anima::AnimaMatrix model(_engine);
	_program->SetUniformValue(_matrixUniform, (model * camera * projection).GetData());
	
	Anima::AnimaModelsManager* mgr = _engine->GetModelsManager();
	Anima::AnimaMatrix m(_engine);
	if (mgr->GetModelsNumber() > 0)
	{
		error = glGetError();
		mgr->GetPModel(0)->Draw(m);
		error = glGetError();
	}

	++_frame;
}

void CRModelViewer::mouseMoveEvent(QMouseEvent* mevent)
{
	if ((mevent->buttons() & Qt::LeftButton) == Qt::LeftButton)
	{
		double dx = _lastMouseXPos - mevent->x();
		double dy = _lastMouseYPos - mevent->y();

		_engine->GetCamerasManager()->GetActiveCamera()->RotateXDeg(dy);
		_engine->GetCamerasManager()->GetActiveCamera()->RotateYDeg(dx);
	}

	_lastMouseXPos = mevent->x();
	_lastMouseYPos = mevent->y();
}