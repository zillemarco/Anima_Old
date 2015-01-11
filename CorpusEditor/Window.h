#pragma once

#include <AnimaEngine.h>
#include <AnimaWindow.h>
#include <AnimaShaderProgram.h>

class Window : public Anima::AnimaWindow
{
public:
	Window();
	~Window();

	DECLARE_MESSAGE_MAP()

public:
	void DrawScene();
	void Load();

private:
	ANIMA_WINDOW_EVENT void MouseClickCallback(Anima::AnimaWindow* window, int button, int action, int mods);
	ANIMA_WINDOW_EVENT void PaintCallback(Anima::AnimaWindow* window);
	ANIMA_WINDOW_EVENT void MouseMoveCallback(Anima::AnimaWindow* window, double x, double y);

	Anima::AnimaShaderProgram* _program;

	int _matrixUniform;
	int _posAttr;

public:
	ANIMA_WINDOW_EVENT void FrameBufferResizeCallback(Anima::AnimaWindow* window, int w, int h);
};

