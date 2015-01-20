#pragma once

#include "CorpusOGLWindowBase.h"

class QMouseEvent;
class QWheelEvent;

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

	void mouseMoveEvent(QMouseEvent* mEvent);
	void wheelEvent(QWheelEvent* wEvent);

private:
	GLuint loadShader(GLenum type, const char *source);

	GLuint _posAttr;
	GLuint _colAttr;
	GLuint _matrixUniform;

	Anima::AnimaShaderProgram* _program;
	int _frame;

	int _lastMouseXPos;
	int _lastMouseYPos;
};

