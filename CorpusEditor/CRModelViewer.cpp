
#include <AnimaShadersManager.h>
#include <AnimaShaderProgram.h>
#include <AnimaShader.h>
#include <AnimaEngine.h>

#include "CRModelViewer.h"
#include <qscreen>
#include <qmessagebox>

static const char *vertexShaderSource =
"attribute vec4 posAttr;\n"
"attribute vec4 colAttr;\n"
"varying vec4 col;\n"
"uniform mat4 matrix;\n"
"void main() {\n"
"   col = colAttr;\n"
"   gl_Position = matrix * posAttr;\n"
"}\n";

static const char *fragmentShaderSource =
"varying vec4 col;\n"
"void main() {\n"
"   gl_FragColor = col;\n"
"}\n";

CRModelViewer::CRModelViewer(Anima::AnimaEngine* engine, QWindow* parent)
	: CorpusOGLWindowBase(engine, parent)
	, _program(nullptr)
	, _frame(0)
{
}

CRModelViewer::~CRModelViewer()
{
}

GLuint CRModelViewer::loadShader(GLenum type, const char *source)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, 0);
	glCompileShader(shader);
	return shader;
}

void CRModelViewer::Initialize()
{
	Anima::AnimaShader* vs = _engine->GetShadersManager()->LoadShader(vertexShaderSource, Anima::AnimaShader::VERTEX);
	Anima::AnimaShader* fs = _engine->GetShadersManager()->LoadShader(fragmentShaderSource, Anima::AnimaShader::FRAGMENT);

	_program = _engine->GetShadersManager()->CreateProgram();
	_program->Create();
	_program->AddShader(vs);
	_program->AddShader(fs);
	_program->Link();

	_matrixUniform = glGetUniformLocation(_program->GetID(), "matrix");
	_posAttr = glGetAttribLocation(_program->GetID(), "posAttr");
	_colAttr = glGetAttribLocation(_program->GetID(), "colAttr");

	//_program = new QOpenGLShaderProgram(this);
	//_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
	//_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
	//_program->link();
	//_posAttr = _program->attributeLocation("posAttr");
	//_colAttr = _program->attributeLocation("colAttr");
	//_matrixUniform = _program->uniformLocation("matrix");
}

void CRModelViewer::Render()
{
	const qreal retinaScale = devicePixelRatio();
	glViewport(0, 0, width() * retinaScale, height() * retinaScale);

	glClear(GL_COLOR_BUFFER_BIT);

	_program->Use();

	QMatrix4x4 matrix;
	matrix.perspective(60.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	matrix.translate(0, 0, -2);
	matrix.rotate(100.0f * _frame / screen()->refreshRate(), 0, 1, 0);
	
	_program->SetUniformValue(_matrixUniform, matrix.data());

	if (glGetError() != GL_NO_ERROR)
	{
		ANIMA_ASSERT(false);
	}

	GLfloat vertices[] = {
		0.0f, 0.707f,
		-0.5f, -0.5f,
		0.5f, -0.5f
	};

	GLfloat colors[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	glVertexAttribPointer(_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	//_program->release();

	++_frame;
}