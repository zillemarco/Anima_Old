//
//  CorpusCorpusMainWindow.cpp
//  Anima
//
//  Created by Marco Zille on 03/12/14.
//
//

#include "CorpusMainWindow.h"
#include "CorpusDocument.h"
#include "ResourceManagerTab.h"
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

#include "EditWindow.h"

#include "NewProjectWindow.h"

CorpusMainWindow::CorpusMainWindow()
{
	_activeDocument = nullptr;
	_resourceManagerTab = nullptr;
	_worldEditorTab = nullptr;
	
	_recentFiles.clear();
	_recentFilesAction.clear();
	_recentFilesMapper = nullptr;
	
	_mdiArea = new QMdiArea;
	_mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	_mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	_mdiArea->setViewMode(QMdiArea::TabbedView);
	setCentralWidget(_mdiArea);
	connect(_mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(updateMenus()));

	_windowMapper = new QSignalMapper(this);
	connect(_windowMapper, SIGNAL(mapped(QWidget*)), this, SLOT(setActiveSubWindow(QWidget*)));
	
	readSettings();
	readRecentFileList();
	
	createActions();
	createMenus();
	createStatusBar();
	updateMenus();
	
	setWindowTitle(tr("CoprusEditor"));
	setUnifiedTitleAndToolBarOnMac(true);
}

void CorpusMainWindow::closeEvent(QCloseEvent *event)
{
	if(!closeProject())
		event->ignore();
}

void CorpusMainWindow::newProject()
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
		
		_activeDocument = new CorpusDocument;
		if(!_activeDocument->NewDocument(dlg.getProjectName(), dlg.getProjectFolderPath()))
		{
			QMessageBox msg;
			msg.setText(tr("Error during project creation."));
			msg.setStandardButtons(QMessageBox::Ok);
			msg.exec();
			
			setWindowTitle(tr("CorpusEditor"));
			
			return;
		}
		
		_resourceManagerTab = new ResourceManagerTab(_activeDocument);
		_worldEditorTab = new WorldEditorTab(_activeDocument);
		
		_mdiArea->addSubWindow(_resourceManagerTab);
		_mdiArea->addSubWindow(_worldEditorTab);
		
		_worldEditorTab->show();
		_resourceManagerTab->show();
		
		_mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(_resourceManagerTab));
		
		QSettings settings("ZEB", "CorpusEditor");
		if(_resourceManagerTab != nullptr)
			_resourceManagerTab->readSettings(&settings);
		
		if(_worldEditorTab != nullptr)
			_worldEditorTab->readSettings(&settings);
		
		setWindowTitle(tr("CorpusEditor - ") + _activeDocument->projectName());
	}
}

void CorpusMainWindow::openProject()
{
	QString filePath = tr("");
	
	QFileDialog *fd = new QFileDialog;
	fd->setFileMode(QFileDialog::ExistingFile);
	fd->setViewMode(QFileDialog::Detail);
	
	if (fd->exec())
	{
		filePath = fd->selectedFiles()[0];
		
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
		
		_activeDocument = new CorpusDocument;
		if(!_activeDocument->OpenDocument(filePath))
		{
			QMessageBox msg;
			msg.setText(tr("Error during project opening."));
			msg.setStandardButtons(QMessageBox::Ok);
			msg.exec();
			
			setWindowTitle(tr("CorpusEditor"));
			
			return;
		}
		
		saveRecentFileList();
		
		_resourceManagerTab = new ResourceManagerTab(_activeDocument);
		_worldEditorTab = new WorldEditorTab(_activeDocument);
		
		_mdiArea->addSubWindow(_resourceManagerTab);
		_mdiArea->addSubWindow(_worldEditorTab);
		
		_worldEditorTab->show();
		_resourceManagerTab->show();
		
		_mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(_resourceManagerTab));
		
		QSettings settings("ZEB", "CorpusEditor");
		if(_resourceManagerTab != nullptr)
			_resourceManagerTab->readSettings(&settings);
		
		if(_worldEditorTab != nullptr)
			_worldEditorTab->readSettings(&settings);
		
		setWindowTitle(tr("CorpusEditor - ") + _activeDocument->projectName());
	}
}

void CorpusMainWindow::updateMenus()
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
	
	for(int i = 0; i < _recentFilesAction.count(); i++)
	{
		if(_recentFilesMapper)
			_recentFilesMapper->removeMappings(_recentFilesAction[i]);
		else
			qDebug() << tr("Null recent files mapper");
		
		delete _recentFilesAction[i];
		_recentFilesAction[i] = nullptr;
	}
	
	_recentFilesAction.clear();
	
	if(_recentFilesMapper)
	{
		delete _recentFilesMapper;
		_recentFilesMapper = nullptr;
	}
	
	_recentFilesMapper = new QSignalMapper(this);
	
	_recentFilesMenu->clear();
	for(int i = 0; i < _recentFiles.count(); i++)
	{
		QAction* action = new QAction(_recentFiles[i]._fileName, this);
		action->setStatusTip(_recentFiles[i]._filePath);
		connect(action, SIGNAL(triggered()), _recentFilesMapper, SLOT(map()));
		
		_recentFilesMenu->addAction(action);
		
		_recentFilesMapper->setMapping(action, i);
		_recentFilesAction.append(action);
	}
	
	connect(_recentFilesMapper, SIGNAL(mapped(int)), this, SLOT(loadProjectFromRecentFiles(int)));
}

void CorpusMainWindow::createActions()
{
	_newAct = new QAction(tr("&New project"), this);
	_newAct->setShortcuts(QKeySequence::New);
	_newAct->setStatusTip(tr("Create a new project"));
	connect(_newAct, SIGNAL(triggered()), this, SLOT(newProject()));
	
	_openAct = new QAction(tr("&Open project"), this);
	_openAct->setShortcuts(QKeySequence::Open);
	_openAct->setStatusTip(tr("Open a project"));
	connect(_openAct, SIGNAL(triggered()), this, SLOT(openProject()));
	
	_closeProjectAct = new QAction(tr("&Close project"), this);
	_closeProjectAct->setShortcut(Qt::CTRL + Qt::ALT + Qt::Key_W);
	_closeProjectAct->setStatusTip(tr("Close the current project"));
	connect(_closeProjectAct, SIGNAL(triggered()), this, SLOT(closeProject()));
	
	_exitAct = new QAction(tr("E&xit"), this);
	_exitAct->setShortcuts(QKeySequence::Quit);
	_exitAct->setStatusTip(tr("Exit from CorpusEditor"));
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

void CorpusMainWindow::createMenus()
{
	_fileMenu = menuBar()->addMenu(tr("&File"));
	_fileMenu->addAction(_newAct);
	_fileMenu->addAction(_openAct);
	_fileMenu->addSeparator();
	_fileMenu->addAction(_closeProjectAct);
	_fileMenu->addSeparator();
	_fileMenu->addAction(_exitAct);

	_recentFilesMenu = _fileMenu->addMenu(tr("Recent files"));
	
	_resourcesMenu = menuBar()->addMenu(tr("&Resources"));
	_resourcesMenu->addAction(_importModelAct);
	_resourcesMenu->addAction(_importTextureAct);
	_resourcesMenu->addSeparator();
	_resourcesMenu->addAction(_addNewMaterialAct);
}

void CorpusMainWindow::createStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
}

void CorpusMainWindow::readSettings()
{
	QSettings settings("ZEB", "CorpusEditor");
	
	settings.beginGroup("CorpusMainWindow");
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

void CorpusMainWindow::writeSettings()
{
	QSettings settings("ZEB", "CorpusEditor");
	
#if defined WIN32
	bool bMax = isMaximized();
#else
	bool bMax = isFullScreen();
#endif

	settings.beginGroup("CorpusMainWindow");
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

void CorpusMainWindow::setActiveSubWindow(QWidget *window)
{
	if (!window)
		return;
	
	_mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

bool CorpusMainWindow::closeProject()
{
	if (_activeDocument && _activeDocument->HasModifications())
	{
		QMessageBox::StandardButton btn = QMessageBox::question(this, tr("Warning"), tr("The opened project has modification.\nSave it?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

		if (btn == QMessageBox::Cancel)
			return false;
		else if (btn == QMessageBox::Yes)
		{
			_activeDocument->SaveDocument();
			
			saveRecentFileList();
		}
		else if (btn == QMessageBox::No && _activeDocument->IsNewDocument())
		{
			if (QMessageBox::question(this, tr("Warning"), tr("The opened project was a new one.\nCompletely remove the create directories?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
				_activeDocument->DeleteProject();
		}
	}
	
	writeSettings();

	if (_activeDocument)
	{
		delete _activeDocument;
		_activeDocument = nullptr;
	}

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

	setWindowTitle(tr("CorpusEditor"));
	
	return true;
}

void CorpusMainWindow::importModel()
{
	if(_activeDocument->ImportModel())
	{
		_resourceManagerTab->LoadModelsTree();
	}
}

void CorpusMainWindow::importTexture()
{
	
}

void CorpusMainWindow::addNewMaterial()
{
	
}

void CorpusMainWindow::loadProjectFromRecentFiles(int index)
{
	QString filePath = _recentFiles[index]._filePath;
	
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
	
	_activeDocument = new CorpusDocument;
	if(!_activeDocument->OpenDocument(filePath))
	{
		QMessageBox msg;
		msg.setText(tr("Error during project opening."));
		msg.setStandardButtons(QMessageBox::Ok);
		msg.exec();
		
		setWindowTitle(tr("CorpusEditor"));
		
		return;
	}
	
	saveRecentFileList();
	
	_resourceManagerTab = new ResourceManagerTab(_activeDocument);
	_worldEditorTab = new WorldEditorTab(_activeDocument);
	
	_mdiArea->addSubWindow(_resourceManagerTab);
	_mdiArea->addSubWindow(_worldEditorTab);
	
	_worldEditorTab->show();
	_resourceManagerTab->show();
	
	_mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(_resourceManagerTab));
	
	QSettings settings("ZEB", "CorpusEditor");
	if(_resourceManagerTab != nullptr)
		_resourceManagerTab->readSettings(&settings);
	
	if(_worldEditorTab != nullptr)
		_worldEditorTab->readSettings(&settings);
	
	setWindowTitle(tr("CorpusEditor - ") + _activeDocument->projectName());
}

void CorpusMainWindow::saveRecentFileList()
{
	int recentFileIndex = -1;
	for(int i = 0; i < _recentFiles.count() && recentFileIndex == -1; i++)
	{
		if(_recentFiles[i]._filePath == _activeDocument->projectFilePath())
			recentFileIndex = i;
	}
	
	if(recentFileIndex == -1)
	{
		QFileInfo info(_activeDocument->projectFilePath());
		
		RecentFileElement rfe;
		rfe._fileName = info.fileName();
		rfe._filePath = _activeDocument->projectFilePath();
		
		_recentFiles.insert(0, rfe);
	}
	else
	{
		RecentFileElement rfe = _recentFiles[recentFileIndex];
		_recentFiles.removeAt(recentFileIndex);
		_recentFiles.insert(0, rfe);
	}
	
	QSettings settings("ZEB", "CorpusEditor");
	settings.beginGroup("RecentFiles");
	settings.setValue("n", _recentFiles.count());
	
	for (int i = 0; i < _recentFiles.count(); i++)
	{
		QString fileName = QString("Name_%0").arg(i);
		QString filePath = QString("Path_%0").arg(i);
		settings.setValue(fileName, _recentFiles[i]._fileName);
		settings.setValue(filePath, _recentFiles[i]._filePath);
	}

	settings.endGroup();
}

void CorpusMainWindow::readRecentFileList()
{
	QSettings settings("ZEB", "CorpusEditor");
	settings.beginGroup("RecentFiles");
	
	int n =	settings.value("n", 0).toInt();
	
	for (int i = 0; i < n; i++)
	{
		QString fileName = QString("Name_%0").arg(i);
		QString filePath = QString("Path_%0").arg(i);
		
		RecentFileElement rfe;
		
		rfe._fileName = settings.value(fileName, QString("")).toString();
		rfe._filePath = settings.value(filePath, QString("")).toString();
		
		_recentFiles.append(rfe);
	}
	
	settings.endGroup();
}