//
//  CorpusCorpusMainWindow.cpp
//  Anima
//
//  Created by Marco Zille on 03/12/14.
//
//

#include "CorpusMainWindow.h"
#include <QObject>
#include <QMdiArea>
#include <QSignalMapper>
#include <QCloseEvent>
#include <QFileDialog>
#include <QAction>
#include <QStatusBar>
#include <QMessageBox>
#include <QMenu>
#include <QMdiSubWindow>
#include <QApplication>
#include <QMenuBar>
#include <QToolBar>
#include <QSettings>
#include <QGridLayout>

#include "EditWindow.h"

CorpusMainWindow::CorpusMainWindow()
{
	_mdiArea = new QMdiArea;
	_mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	_mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	_mdiArea->setViewMode(QMdiArea::TabbedView);
	setCentralWidget(_mdiArea);
	connect(_mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(updateMenus()));

	_windowMapper = new QSignalMapper(this);
	connect(_windowMapper, SIGNAL(mapped(QWidget*)), this, SLOT(setActiveSubWindow(QWidget*)));
	
	createActions();
	createMenus();
	createStatusBar();
	updateMenus();
	
	readSettings();
	
	setWindowTitle(tr("CoprusEditor"));
	setUnifiedTitleAndToolBarOnMac(true);
}

void CorpusMainWindow::closeEvent(QCloseEvent *event)
{
	_mdiArea->closeAllSubWindows();
	if (_mdiArea->currentSubWindow()) {
		event->ignore();
	} else {
		writeSettings();
		event->accept();
	}
}

void CorpusMainWindow::newProject()
{
}

void CorpusMainWindow::updateMenus()
{
}

bool CorpusMainWindow::createNewProject()
{	
	return true;
}

void CorpusMainWindow::createActions()
{
	_newAct = new QAction(QIcon(":/Res/icona_new.png"), tr("&New"), this);
	_newAct->setShortcuts(QKeySequence::New);
	_newAct->setStatusTip(tr("Create a new file"));
	connect(_newAct, SIGNAL(triggered()), this, SLOT(newFile()));
	
	_exitAct = new QAction(tr("E&xit"), this);
	_exitAct->setShortcuts(QKeySequence::Quit);
	_exitAct->setStatusTip(tr("Exit the application"));
	connect(_exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
}

void CorpusMainWindow::createMenus()
{
	_fileMenu = menuBar()->addMenu(tr("&File"));
	_fileMenu->addAction(_newAct);
	_fileMenu->addSeparator();

	QAction *action = _fileMenu->addAction(tr("Switch layout direction"));
	connect(action, SIGNAL(triggered()), this, SLOT(switchLayoutDirection()));
	_fileMenu->addAction(_exitAct);
}

void CorpusMainWindow::createStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
}

void CorpusMainWindow::readSettings()
{
	QSettings settings("ZEB", "CorpusEditor");
	QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
	QSize size = settings.value("size", QSize(400, 400)).toSize();
	move(pos);
	resize(size);
}

void CorpusMainWindow::writeSettings()
{
	QSettings settings("ZEB", "CorpusEditor");
	settings.setValue("pos", pos());
	settings.setValue("size", size());
}

void CorpusMainWindow::switchLayoutDirection()
{
	if (layoutDirection() == Qt::LeftToRight)
		qApp->setLayoutDirection(Qt::RightToLeft);
	else
		qApp->setLayoutDirection(Qt::LeftToRight);
}

void CorpusMainWindow::setActiveSubWindow(QWidget *window)
{
	if (!window)
		return;
	_mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}