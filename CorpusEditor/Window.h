#pragma once

#include <AnimaEngine.h>
#include <AnimaWindow.h>
#include <AnimaThirdPersonCamera.h>
#include <AnimaFirstPersonCamera.h>
#include <AnimaShaderProgram.h>
#include <AnimaTexture.h>
#include <AnimaMaterial.h>

class Window : public Anima::AnimaWindow
{
public:
	Window();
	~Window();

	DECLARE_MESSAGE_MAP()

public:
	void DrawScene();
	void Load();


	Anima::AnimaThirdPersonCamera* _tpcamera;
	Anima::AnimaFirstPersonCamera* _fpcamera;

private:
	ANIMA_WINDOW_EVENT void PaintCallback(Anima::AnimaWindow* window);
	ANIMA_WINDOW_EVENT void MouseMoveCallback(Anima::AnimaWindow* window, double x, double y);
	ANIMA_WINDOW_EVENT void MouseClickCallback(Anima::AnimaWindow* window, int button, int action, int mods);
	ANIMA_WINDOW_EVENT void KeyCallback(Anima::AnimaWindow* window, int key, int scancode, int action, int mods);
	ANIMA_WINDOW_EVENT void ScrollCallback(Anima::AnimaWindow* window, double x, double y);

	Anima::AnimaShaderProgram* program;
	Anima::AnimaTexture* texture;

	int _matrixUniform;
	int _posAttr;

	Anima::AnimaMaterial* material;
	
	float rotX, rotY;
	float tx, ty, tz;

	bool projection;

	double _lastPTX, _lastPTY;

public:
	ANIMA_WINDOW_EVENT void FrameBufferResizeCallback(Anima::AnimaWindow* window, int w, int h);
};

