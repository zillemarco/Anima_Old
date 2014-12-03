//
//  CorpusOGLWindow.h
//  Anima
//
//  Created by Marco Zille on 03/12/14.
//
//

#ifndef __Anima__CorpusOGLWindow__
#define __Anima__CorpusOGLWindow__

#include <QWindow>
#include <QOpenGLFunctions>
#include <QEvent>
#include <QExposeEvent>

class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;

class CorpusOGLWindow : public QWindow, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	explicit CorpusOGLWindow(QWindow* parent = nullptr);
	~CorpusOGLWindow();
	
	virtual void Render(QPainter* painter);
	virtual void Render();
	
	virtual void Initialize();
	
public slots:
	void RenderLater();
	void RenderNow();
	
protected:
	bool event(QEvent* event);
	void exposeEvent(QExposeEvent* event);
	
private:
	bool _updatePending;
	
	QOpenGLContext* _context;
	QOpenGLPaintDevice* _device;
};

#endif /* defined(__Anima__CorpusOGLWindow__) */
