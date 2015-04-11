//
//  AnimaEditorOGLWindow.h
//  Anima
//
//  Created by Marco Zille on 03/12/14.
//
//

#ifndef __Anima__AnimaEditorOGLWindow__
#define __Anima__AnimaEditorOGLWindow__

#include <qwindow>
#include <qopenglfunctions>
#include <qevent>
#include <qexposeEvent>
#include <qopenglpaintdevice>

namespace Anima {
	class AnimaEngine;
}

class QOpenGLFunctions;

class AnimaEditorOGLWindowBase : public QWindow, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	explicit AnimaEditorOGLWindowBase(Anima::AnimaEngine* engine, QWindow* parent = nullptr);
	~AnimaEditorOGLWindowBase();
	
	virtual void Render(QPainter* painter);
	virtual void Render();
	
	virtual void Initialize();

	void SetAnimating(bool bSet) { _animating = bSet; }
	
public slots:
	void RenderLater();
	void RenderNow();
	
protected:
	bool event(QEvent* event);
	void exposeEvent(QExposeEvent* event);
	
protected:
	bool _animating;
	
private:
	bool _updatePending;
	
	QOpenGLContext* _context;
	QOpenGLPaintDevice* _device;

protected:
	Anima::AnimaEngine* _engine;
};

#endif /* defined(__Anima__AnimaEditorOGLWindow__) */
