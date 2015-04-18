//
//  AEMainWindow.h
//  Anima
//
//  Created by Marco Zille on 03/12/14.
//
//

#ifndef __Anima__AnimaEditorMainWindow__
#define __Anima__AnimaEditorMainWindow__

#include <QMainWindow>

class AEDocument;
class AEResourcesManagerTab;
class WorldEditorTab;
class EditWindow;
class QAction;
class QMenu;
class QMdiArea;
class QMdiSubWindow;
class QSignalMapper;
class QGridLayout;
class QShortcut;
#include <QList>

class AEMainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	AEMainWindow();
	
protected:
	void closeEvent(QCloseEvent *event);
	
private slots:
	void newProject();
	void openProjectSlot();
	void openRecentProject();

	bool closeProject();
	void updateMenus();
	
	void importModel();
	void importTexture();
	void addNewMaterial();

	void setActiveSubWindow(QWidget *window);
	
private:
	void openProject(QString filePath, bool askIfEmpty = true);

	void createActions();
	void createMenus();
	void createStatusBar();
	void readSettings();
	void writeSettings();

	void updateRecentProjectActions();
	QString strippedName(const QString &fullFileName);
	void setCurrentProject(const QString &fileName, const QString& projectName);

private:		
	QMdiArea*		_mdiArea;
	QSignalMapper*	_windowMapper;
		
	QMenu*		_fileMenu;
	QMenu*		_resourcesMenu;
	
	enum { MaxRecentFiles = 5 };
	QAction* recentProjectActs[MaxRecentFiles];
	QAction* recentProjectSeparatorAct;

	QAction*	_newAct;
	QAction*	_openAct;
	QAction*	_closeProjectAct;
	QAction*	_exitAct;
		
	QAction*	_importModelAct;
	QAction*	_importTextureAct;
	QAction*	_addNewMaterialAct;
	
	AEDocument* _activeDocument;
	
	AEResourcesManagerTab* _resourceManagerTab;
	WorldEditorTab*		_worldEditorTab;
};

#endif /* defined(__Anima__AnimaEditorMainWindow__) */
