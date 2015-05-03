#pragma once

#include <AnimaEngine.h>
#include <AnimaWindow.h>
#include <AnimaThirdPersonCamera.h>
#include <AnimaFirstPersonCamera.h>
#include <AnimaShaderProgram.h>
#include <AnimaTexture.h>
#include <AnimaMaterial.h>
#include <AnimaLight.h>
#include <AnimaRenderer.h>
#include <AnimaMesh.h>
#include <AnimaFPSTimer.h>
#include <AnimaBenchmarkTimer.h>

class Window : public Anima::AnimaWindow
{
public:
	Window();
	~Window();

	DECLARE_MESSAGE_MAP()

public:
	void DrawScene();
	void DrawCameraFrustum();
	void DrawLightFrustum();
	void Load();

	Anima::AnimaThirdPersonCamera* _tpcamera;
	Anima::AnimaFirstPersonCamera* _fpcamera;
	Anima::AnimaFPSTimer _timerFPS;
	Anima::AnimaBenchmarkTimer _timer;
	Anima::AnimaDirectionalLight* _directionalLight;
	Anima::AnimaScene* _scene;
	
private:
	ANIMA_WINDOW_EVENT void PaintCallback(Anima::AnimaWindow* window);
	ANIMA_WINDOW_EVENT void MouseMoveCallback(Anima::AnimaWindow* window, double x, double y);
	ANIMA_WINDOW_EVENT void MouseClickCallback(Anima::AnimaWindow* window, int button, int action, int mods);
	ANIMA_WINDOW_EVENT void KeyCallback(Anima::AnimaWindow* window, int key, int scancode, int action, int mods);
	ANIMA_WINDOW_EVENT void ScrollCallback(Anima::AnimaWindow* window, double x, double y);

	void BindAsRenderTarget();

	Anima::AnimaRenderer* renderingManager;
	
	bool projection;
	bool type;

	double _lastPTX, _lastPTY;

public:
	ANIMA_WINDOW_EVENT void FrameBufferResizeCallback(Anima::AnimaWindow* window, int w, int h);
};

