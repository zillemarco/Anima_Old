//
//  CorpusOGLWindowBase.cpp
//  Anima
//
//  Created by Marco Zille on 03/12/14.
//
//

#include <AnimaEngine.h>
#include "CorpusOGLWindowBase.h"
#include <QtCore/QCoreApplication>
#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <QPainter>
#include <qmessagebox>

CorpusOGLWindowBase::CorpusOGLWindowBase(Anima::AnimaEngine* engine, QWindow* parent)
: QWindow(parent)
, _animating(true)
, _updatePending(false)
, _context(nullptr)
, _device(nullptr)
, _engine(engine)
{
	setSurfaceType(QWindow::OpenGLSurface);
}

CorpusOGLWindowBase::~CorpusOGLWindowBase()
{
	delete _device;
}

void CorpusOGLWindowBase::Render(QPainter* painter)
{
	Q_UNUSED(painter);
}

void CorpusOGLWindowBase::Initialize()
{
}

void CorpusOGLWindowBase::Render()
{
	if(!_device)
		_device = new QOpenGLPaintDevice;
	
	_device->setSize(size());
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glGetError();
	
	QPainter painter(_device);
	Render(&painter);
}

void CorpusOGLWindowBase::RenderLater()
{
	if(!_updatePending)
	{
		_updatePending = true;
		QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
	}
}

bool CorpusOGLWindowBase::event(QEvent* event)
{
	switch (event->type()) {
		case QEvent::UpdateRequest:
			_updatePending = false;
			RenderNow();
			return true;
			
		default:
			return QWindow::event(event);
	}
}

void CorpusOGLWindowBase::exposeEvent(QExposeEvent* event)
{
	Q_UNUSED(event);
	
	if(isExposed())
		RenderNow();
}

void CorpusOGLWindowBase::RenderNow()
{
	if (!isExposed())
		return;
	
	bool needsInitialize = false;
	
	if(!_context)
	{
		QSurfaceFormat format;
		format.setDepthBufferSize( 24 );
		format.setMajorVersion( 4 );
		format.setMinorVersion( 1 );
		format.setSamples( 4 );
		format.setProfile( QSurfaceFormat::CoreProfile );
		setFormat( format );
		create();
		
		_context = new QOpenGLContext(this);
		_context->setFormat(format);

		if(!_context->create())
		{
			int i = 0;
			i++;
		}
		
		needsInitialize = true;
	}
	
	_context->makeCurrent(this);
	
	if(needsInitialize)
	{
		initializeOpenGLFunctions();
		
		GLenum error = glGetError();
		Anima::AnimaEngine::InitializeGlewExtensions();
		error = glGetError();

		Initialize();
	}
	
	Render();
	
	_context->swapBuffers(this);

	if (_animating)
		RenderLater();
}