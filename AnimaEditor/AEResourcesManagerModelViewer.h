#pragma once

#include "AEOGLWindowBase.h"
#include <AnimaRenderer.h>
#include <AnimaModel.h>
#include <AnimaModelInstance.h>
#include <AnimaMesh.h>
#include <AnimaMeshInstance.h>
#include <AnimaTimer.h>

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

	void setSelectedObject(Anima::AnimaModel* model);
	void setSelectedObject(Anima::AnimaModelInstance* instance);
	void setSelectedObject(Anima::AnimaMesh* mesh);
	void setSelectedObject(Anima::AnimaMeshInstance* instance);
	
private:
	Anima::AnimaRenderer* _renderingManager;

	Anima::AnimaModel* _selectedModel;
	Anima::AnimaModelInstance* _selectedModelInstance;
	Anima::AnimaMesh* _selectedMesh;
	Anima::AnimaMeshInstance* _selectedMeshInstance;

	Anima::AnimaTimer _timer;

	int _frame;

	int _lastMouseXPos;
	int _lastMouseYPos;
};

