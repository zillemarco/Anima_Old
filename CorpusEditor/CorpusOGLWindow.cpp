//
//  CorpusOGLWindow.cpp
//  Anima
//
//  Created by Marco Zille on 03/12/14.
//
//

#include "CorpusOGLWindow.h"
#include <QtCore/QCoreApplication>
#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <QPainter>

CorpusOGLWindow::CorpusOGLWindow(QWindow* parent)
: QWindow(parent)
, _updatePending(false)
, _context(nullptr)
, _device(nullptr)
{
	setSurfaceType(QWindow::OpenGLSurface);
}

CorpusOGLWindow::~CorpusOGLWindow()
{
	delete _device;
}

void CorpusOGLWindow::Render(QPainter* painter)
{
	Q_UNUSED(painter);
}

void CorpusOGLWindow::Initialize()
{
}

void CorpusOGLWindow::Render()
{
	if(!_device)
		_device = new QOpenGLPaintDevice;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	_device->setSize(size());
	
	QPainter painter(_device);
	Render(&painter);
}

void CorpusOGLWindow::RenderLater()
{
	if(!_updatePending)
	{
		_updatePending = true;
		QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
	}
}

bool CorpusOGLWindow::event(QEvent* event)
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

void CorpusOGLWindow::exposeEvent(QExposeEvent* event)
{
	Q_UNUSED(event);
	
	if(isExposed())
		RenderNow();
}

void CorpusOGLWindow::RenderNow()
{
	if (!isExposed())
		return;
	
	bool needsInitialize = false;
	
	if(!_context)
	{
		_context = new QOpenGLContext(this);
		_context->setFormat(requestedFormat());
		_context->create();
		
		needsInitialize = true;
	}
	
	_context->makeCurrent(this);
	
	if(needsInitialize)
	{
		initializeOpenGLFunctions();
		Initialize();
	}
	
	Render();
	
	_context->swapBuffers(this);
}