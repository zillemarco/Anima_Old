#pragma once

#include "AEOGLWindowBase.h"
#include <AnimaRenderingManager.h>
#include <AnimaModel.h>

class QMouseEvent;
class QWheelEvent;

class AEResourcesManagerModelViewer : public AEOGLWindowBase
{
public:
	explicit AEResourcesManagerModelViewer(Anima::AnimaEngine* engine, QWindow* parent = nullptr);
	~AEResourcesManagerModelViewer();

	void Initialize() Q_DECL_OVERRIDE;
	void Render() Q_DECL_OVERRIDE;

	void mouseMoveEvent(QMouseEvent* mEvent);
	void wheelEvent(QWheelEvent* wEvent);

	void setSelectedModel(Anima::AnimaModel* model);

	void InitRender();
	void DeinitRender();

private:
	Anima::AnimaRenderingManager* _renderingManager;
	Anima::AnimaModel* _selectedModel;

	int _frame;

	int _lastMouseXPos;
	int _lastMouseYPos;
};

