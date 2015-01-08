//
//  CorpusOGLWindow.h
//  Anima
//
//  Created by Marco Zille on 03/12/14.
//
//

#ifndef __Anima__CorpusOGLWindow__
#define __Anima__CorpusOGLWindow__

#include <qwindow>
#include <qopenglfunctions>
#include <qevent>
#include <qexposeEvent>

namespace Anima {
	class AnimaEngine;
}

class CorpusOGLWindowBase : public QWindow, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	explicit CorpusOGLWindowBase(Anima::AnimaEngine* engine, QWindow* parent = nullptr);
	~CorpusOGLWindowBase();
	
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
	
private:
	bool _updatePending;
	bool _animating;
	
	QOpenGLContext* _context;
	QOpenGLPaintDevice* _device;

protected:
	Anima::AnimaEngine* _engine;
};

#endif /* defined(__Anima__CorpusOGLWindow__) */
