#pragma once

#include "CorpusOGLWindowBase.h"
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>

namespace Anima
{
	class AnimaShaderProgram;
};

class CRModelViewer : public CorpusOGLWindowBase
{
public:
	explicit CRModelViewer(Anima::AnimaEngine* engine, QWindow* parent = nullptr);
	~CRModelViewer();

	void Initialize() Q_DECL_OVERRIDE;
	void Render() Q_DECL_OVERRIDE;

private:
	GLuint loadShader(GLenum type, const char *source);

	GLuint _posAttr;
	GLuint _colAttr;
	GLuint _matrixUniform;

	Anima::AnimaShaderProgram* _program;
	int _frame;
};

