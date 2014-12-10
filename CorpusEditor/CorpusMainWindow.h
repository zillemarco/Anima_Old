//
//  CorpusCorpusMainWindow.h
//  Anima
//
//  Created by Marco Zille on 03/12/14.
//
//

#ifndef __Anima__CorpusCorpusMainWindow__
#define __Anima__CorpusCorpusMainWindow__

#include <QMainWindow>

class CorpusDocument;
class ResourceManagerTab;
class WorldEditorTab;
class EditWindow;
class QAction;
class QMenu;
class QMdiArea;
class QMdiSubWindow;
class QSignalMapper;
class QGridLayout;
class QShortcut;

class CorpusMainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	CorpusMainWindow();
	
protected:
	void closeEvent(QCloseEvent *event);
	
private slots:
	void newProject();
	void openProject();
	void closeProject();
	void updateMenus();
	
	void importModel();
	void importTexture();
	void addNewMaterial();

	void setActiveSubWindow(QWidget *window);

private:
	void createActions();
	void createMenus();
	void createStatusBar();
	void readSettings();
	void writeSettings();
		
	QMdiArea*		_mdiArea;
	QSignalMapper*	_windowMapper;
		
	QMenu*		_fileMenu;
	QMenu*		_resourcesMenu;
	
	QAction*	_newAct;
	QAction*	_openAct;
	QAction*	_closeProjectAct;
	QAction*	_exitAct;
	
	QAction*	_importModelAct;
	QAction*	_importTextureAct;
	QAction*	_addNewMaterialAct;
	
	CorpusDocument* _activeDocument;
	
	ResourceManagerTab* _resourceManagerTab;
	WorldEditorTab*		_worldEditorTab;
};

#endif /* defined(__Anima__CorpusCorpusMainWindow__) */
