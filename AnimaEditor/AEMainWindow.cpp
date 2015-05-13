//
//  AEMainWindow.cpp
//  Anima
//
//  Created by Marco Zille on 03/12/14.
//
//

#include "AEMainWindow.h"
#include "AEDocument.h"
#include "AEResourcesManagerTab.h"
#include "WorldEditorTab.h"
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
#include <QTreeView>
#include <QDebug>
#include <QShortcut>
#include <QKeySequence>
#include <QT>

#include "EditWindow.h"

#include "NewProjectWindow.h"

AEMainWindow::AEMainWindow()
{
	_activeDocument = nullptr;
	_resourceManagerTab = nullptr;
	_worldEditorTab = nullptr;
		
	_mdiArea = new QMdiArea;
	_mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	_mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	_mdiArea->setViewMode(QMdiArea::TabbedView);
	setCentralWidget(_mdiArea);
	connect(_mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(updateMenus()));

	_windowMapper = new QSignalMapper(this);
	connect(_windowMapper, SIGNAL(mapped(QWidget*)), this, SLOT(setActiveSubWindow(QWidget*)));
	
	readSettings();
	
	createActions();
	createMenus();
	createStatusBar();
	updateMenus();
	
	setWindowTitle(tr("AnimaEditor"));
	setUnifiedTitleAndToolBarOnMac(true);
}

void AEMainWindow::closeEvent(QCloseEvent *event)
{
	if(!closeProject())
		event->ignore();
}

void AEMainWindow::openProjectSlot()
{
	openProject("");
}

void AEMainWindow::newProject()
{
	NewProjectWindow dlg;
	if(dlg.exec())
	{
		if(_activeDocument != nullptr)
		{
			if(QMessageBox::question(this, tr("Warning"), tr("A project is already open.\nClose it to open the new project?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
			{
				return;
			}
			
			closeProject();
		}
		
		_activeDocument = new AEDocument;
		if(!_activeDocument->NewDocument(dlg.getProjectName(), dlg.getProjectFolderPath()))
		{
			QMessageBox msg;
			msg.setText(tr("Error during project creation."));
			msg.setStandardButtons(QMessageBox::Ok);
			msg.exec();
			
			setWindowTitle(tr("AnimaEditor"));
			
			return;
		}

		setCurrentProject(_activeDocument->projectFilePath(), _activeDocument->projectName());
		
		_resourceManagerTab = new AEResourcesManagerTab(_activeDocument);
		_worldEditorTab = new WorldEditorTab(_activeDocument);
		
		_mdiArea->addSubWindow(_resourceManagerTab);
		_mdiArea->addSubWindow(_worldEditorTab);
		
		_worldEditorTab->show();
		_resourceManagerTab->show();
		
		_mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(_resourceManagerTab));
		
		QSettings settings("ZEB", "AnimaEditor");
		if(_resourceManagerTab != nullptr)
			_resourceManagerTab->readSettings(&settings);
		
		if(_worldEditorTab != nullptr)
			_worldEditorTab->readSettings(&settings);
		
		setWindowTitle(tr("AnimaEditor - ") + _activeDocument->projectName());
	}
}

void AEMainWindow::openProject(QString filePath, bool askIfEmpty)
{
	if (filePath.isEmpty() && askIfEmpty)
	{
		QFileDialog *fd = new QFileDialog;
		fd->setFileMode(QFileDialog::ExistingFile);
		fd->setViewMode(QFileDialog::Detail);

		if (fd->exec())
			filePath = fd->selectedFiles()[0];
	}

	if(filePath.isEmpty())
	{
		QMessageBox msg;
		msg.setText(tr("Not a valid path to the project file was specified."));
		msg.setStandardButtons(QMessageBox::Ok);
		msg.exec();
			
		return;
	}
		
	if(_activeDocument != nullptr)
	{
		if(QMessageBox::question(this, tr("Warning"), tr("A project is already open.\nClose it to open the project?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
		{
			return;
		}
			
		closeProject();
	}
		
	_activeDocument = new AEDocument;
	if(!_activeDocument->OpenDocument(filePath))
	{
		QMessageBox msg;
		msg.setText(tr("Error during project opening."));
		msg.setStandardButtons(QMessageBox::Ok);
		msg.exec();
			
		setWindowTitle(tr("AnimaEditor"));
			
		return;
	}
		
	setCurrentProject(_activeDocument->projectFilePath(), _activeDocument->projectName());

	_resourceManagerTab = new AEResourcesManagerTab(_activeDocument);
	_worldEditorTab = new WorldEditorTab(_activeDocument);

	_resourceManagerTab->LoadResourcesTree();

	_mdiArea->addSubWindow(_resourceManagerTab);
	_mdiArea->addSubWindow(_worldEditorTab);
		
	_worldEditorTab->show();
	_resourceManagerTab->show();
		
	_mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(_resourceManagerTab));
		
	QSettings settings("ZEB", "AnimaEditor");
	if(_resourceManagerTab != nullptr)
		_resourceManagerTab->readSettings(&settings);
		
	if(_worldEditorTab != nullptr)
		_worldEditorTab->readSettings(&settings);
		
	setWindowTitle(tr("AnimaEditor - ") + _activeDocument->projectName());
}

void AEMainWindow::updateMenus()
{
	if(_activeDocument)
	{
		_resourcesMenu->setEnabled(true);
		_closeProjectAct->setEnabled(true);
	}
	else
	{
		_resourcesMenu->setEnabled(false);
		_closeProjectAct->setEnabled(false);
	}
}

void AEMainWindow::createActions()
{
	_newAct = new QAction(tr("&New project"), this);
	_newAct->setShortcuts(QKeySequence::New);
	_newAct->setStatusTip(tr("Create a new project"));
	connect(_newAct, SIGNAL(triggered()), this, SLOT(newProject()));
	
	_openAct = new QAction(tr("&Open project"), this);
	_openAct->setShortcuts(QKeySequence::Open);
	_openAct->setStatusTip(tr("Open a project"));
	connect(_openAct, SIGNAL(triggered()), this, SLOT(openProjectSlot()));
	
	_closeProjectAct = new QAction(tr("&Close project"), this);
	_closeProjectAct->setShortcut(Qt::CTRL + Qt::ALT + Qt::Key_W);
	_closeProjectAct->setStatusTip(tr("Close the current project"));
	connect(_closeProjectAct, SIGNAL(triggered()), this, SLOT(closeProject()));

	for (int i = 0; i < MaxRecentFiles; i++) 
	{
		recentProjectActs[i] = new QAction(this);
		recentProjectActs[i]->setVisible(false);
		connect(recentProjectActs[i], SIGNAL(triggered()), this, SLOT(openRecentProject()));
	}
	
	_exitAct = new QAction(tr("E&xit"), this);
	_exitAct->setShortcuts(QKeySequence::Quit);
	_exitAct->setStatusTip(tr("Exit from AnimaEditor"));
	connect(_exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
	
	_importModelAct = new QAction(tr("Import &model"), this);
	_importModelAct->setStatusTip(tr("Import a new model into the resources"));
	connect(_importModelAct, SIGNAL(triggered()), this, SLOT(importModel()));
	
	_importTextureAct = new QAction(tr("Import &texture"), this);
	_importTextureAct->setStatusTip(tr("Import a new texture into the resources"));
	connect(_importTextureAct, SIGNAL(triggered()), this, SLOT(importTexture()));
	
	_addNewMaterialAct = new QAction(tr("Add m&aterial"), this);
	_addNewMaterialAct->setStatusTip(tr("Add a new material into the resources"));
	connect(_addNewMaterialAct, SIGNAL(triggered()), this, SLOT(addNewMaterial()));
}

void AEMainWindow::createMenus()
{
	_fileMenu = menuBar()->addMenu(tr("&File"));
	_fileMenu->addAction(_newAct);
	_fileMenu->addAction(_openAct);
	
	_fileMenu->addSeparator();
	_fileMenu->addAction(_closeProjectAct);

	recentProjectSeparatorAct = _fileMenu->addSeparator();
	for (int i = 0; i < MaxRecentFiles; i++)
		_fileMenu->addAction(recentProjectActs[i]);
	
	_fileMenu->addSeparator();
	_fileMenu->addAction(_exitAct);

	updateRecentProjectActions();
		
	_resourcesMenu = menuBar()->addMenu(tr("&Resources"));
	_resourcesMenu->addAction(_importModelAct);
	_resourcesMenu->addAction(_importTextureAct);
	
	_resourcesMenu->addSeparator();
	_resourcesMenu->addAction(_addNewMaterialAct);
}

void AEMainWindow::updateRecentProjectActions()
{
	QSettings settings("ZEB", "AnimaEditor");
	QStringList files = settings.value("recentFileList").toStringList();

	int numData = qMin(files.size(), (int)MaxRecentFiles);
	int numRecentFiles = numData / 2;
	
	int offset = 0;
	for (int i = 0; i < numData; i += 2)
	{
		QString projectName = files[i];
		QString filePath = files[i + 1];
		recentProjectActs[offset]->setText(projectName);
		recentProjectActs[offset]->setData(filePath);
		recentProjectActs[offset]->setVisible(true);
		offset++;
	}

	for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
		recentProjectActs[j]->setVisible(false);

	recentProjectSeparatorAct->setVisible(numRecentFiles > 0);
}

void AEMainWindow::createStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
}

void AEMainWindow::readSettings()
{
	QSettings settings("ZEB", "AnimaEditor");
	
	settings.beginGroup("AEMainWindow");
	bool fullScreen = settings.value("full", false).toBool();
	QPoint pos = settings.value("pos", QPoint(0, 0)).toPoint();
	QSize size = settings.value("size", QSize(600, 500)).toSize();
	settings.endGroup();
		
	if (fullScreen)
	{
#if defined WIN32
		showMaximized();
#else
		showFullScreen();
#endif
	}
	else
	{
		move(pos);
		resize(size);
	}
	
	if(_resourceManagerTab != nullptr)
		_resourceManagerTab->readSettings(&settings);
	
	if(_worldEditorTab != nullptr)
		_worldEditorTab->readSettings(&settings);
}

void AEMainWindow::writeSettings()
{
	QSettings settings("ZEB", "AnimaEditor");
	
#if defined WIN32
	bool bMax = isMaximized();
#else
	bool bMax = isFullScreen();
#endif

	settings.beginGroup("AEMainWindow");
	settings.setValue("full", bMax);

	if (!bMax)
	{
		settings.setValue("pos", pos());
		settings.setValue("size", size());
	}

	settings.endGroup();
	
	if(_resourceManagerTab != nullptr)
		_resourceManagerTab->saveSettings(&settings);
	
	if(_worldEditorTab != nullptr)
		_worldEditorTab->saveSettings(&settings);
}

void AEMainWindow::setActiveSubWindow(QWidget *window)
{
	if (!window)
		return;
	
	_mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

bool AEMainWindow::closeProject()
{
	if (_activeDocument && _activeDocument->HasModifications())
	{
		QMessageBox::StandardButton btn = QMessageBox::question(this, tr("Warning"), tr("The opened project has modification.\nSave it?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

		if (btn == QMessageBox::Cancel)
			return false;
		else if (btn == QMessageBox::Yes)
			_activeDocument->SaveDocument();
		else if (btn == QMessageBox::No && _activeDocument->IsNewDocument())
		{
			if (QMessageBox::question(this, tr("Warning"), tr("The opened project was a new one.\nCompletely remove the create directories?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
				_activeDocument->DeleteProject();
		}
	}
		
	writeSettings();

	_mdiArea->closeAllSubWindows();

	if (_resourceManagerTab)
	{
		delete _resourceManagerTab;
		_resourceManagerTab = nullptr;
	}

	if (_worldEditorTab)
	{
		delete _worldEditorTab;
		_worldEditorTab = nullptr;
	}

	if (_activeDocument)
	{
		delete _activeDocument;
		_activeDocument = nullptr;
	}

	setWindowTitle(tr("AnimaEditor"));
	
	return true;
}

void AEMainWindow::importModel()
{
	if(_activeDocument->ImportModel())
	{
		_resourceManagerTab->LoadResourcesTree();
	}
}

void AEMainWindow::importTexture()
{
	
}

void AEMainWindow::addNewMaterial()
{
	
}

void AEMainWindow::openRecentProject()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
		openProject(action->data().toString(), false);
}

void AEMainWindow::setCurrentProject(const QString &fileName, const QString& projectName)
{
	QSettings settings("ZEB", "AnimaEditor");
	QStringList files = settings.value("recentFileList").toStringList();
	files.removeAll(fileName);
	files.removeAll(projectName);
	files.prepend(fileName);
	files.prepend(projectName);
	while (files.size() > MaxRecentFiles * 2)
		files.removeLast();

	settings.setValue("recentFileList", files);

	updateRecentProjectActions();
}