#pragma once

#include "CorpusOGLWindowBase.h"
#include <AnimaRenderingManager.h>

class QMouseEvent;
class QWheelEvent;

class CRModelViewer : public CorpusOGLWindowBase
{
public:
	explicit CRModelViewer(Anima::AnimaEngine* engine, QWindow* parent = nullptr);
	~CRModelViewer();

	void Initialize() Q_DECL_OVERRIDE;
	void Render() Q_DECL_OVERRIDE;

	void mouseMoveEvent(QMouseEvent* mEvent);
	void wheelEvent(QWheelEvent* wEvent);

	void setSelectedModel(Anima::AnimaModel* model);

private:
	Anima::AnimaRenderingManager* _renderingManager;
	Anima::AnimaModel* _selectedModel;

	int _frame;

	int _lastMouseXPos;
	int _lastMouseYPos;
};

