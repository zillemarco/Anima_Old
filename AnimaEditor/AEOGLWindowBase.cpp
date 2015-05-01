//
//  AEOGLWindowBase.cpp
//  Anima
//
//  Created by Marco Zille on 03/12/14.
//
//

#include <AnimaEngine.h>
#include "AEOGLWindowBase.h"
#include <QtCore/QCoreApplication>
#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <QPainter>
#include <qmessagebox>

AEOGLWindowBase::AEOGLWindowBase(Anima::AnimaEngine* engine, QWindow* parent)
: QWindow(parent)
, _animating(true)
, _updatePending(false)
, _context(nullptr)
, _device(nullptr)
, _engine(engine)
{
	setSurfaceType(QWindow::OpenGLSurface);
}

AEOGLWindowBase::~AEOGLWindowBase()
{
	delete _device;
}

void AEOGLWindowBase::Render(QPainter* painter)
{
	Q_UNUSED(painter);
}

void AEOGLWindowBase::Initialize()
{
}

void AEOGLWindowBase::Render()
{
	if(!_device)
		_device = new QOpenGLPaintDevice;
	
	_device->setSize(size());
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glGetError();
	
	QPainter painter(_device);
	Render(&painter);
}

void AEOGLWindowBase::RenderLater()
{
	if(!_updatePending)
	{
		_updatePending = true;
		QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
	}
}

bool AEOGLWindowBase::event(QEvent* event)
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

void AEOGLWindowBase::exposeEvent(QExposeEvent* event)
{
	Q_UNUSED(event);
	
	if(isExposed())
		RenderNow();
}

void AEOGLWindowBase::RenderNow()
{
	if (!isExposed())
		return;
	
	bool needsInitialize = false;
	
	if(!_context)
	{
		QSurfaceFormat format;
		format.setDepthBufferSize(24);
		format.setMajorVersion(4);
		format.setMinorVersion(2);
		format.setProfile(QSurfaceFormat::CompatibilityProfile);
		setFormat(format);
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