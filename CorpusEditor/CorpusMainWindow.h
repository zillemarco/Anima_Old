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

class EditWindow;
class QAction;
class QMenu;
class QMdiArea;
class QMdiSubWindow;
class QSignalMapper;
class QGridLayout;

class CorpusMainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	CorpusMainWindow();
	
protected:
	void closeEvent(QCloseEvent *event);
	
 private slots:
	void newProject();
	void updateMenus();

	void switchLayoutDirection();
	void setActiveSubWindow(QWidget *window);

private:
	bool createNewProject();
	
private:
	void createActions();
	void createMenus();
	void createStatusBar();
	void readSettings();
	void writeSettings();
		
	QMdiArea*		_mdiArea;
	QSignalMapper*	_windowMapper;
		
	QMenu*		_fileMenu;
	QAction*	_newAct;
	QAction*	_exitAct;
};

#endif /* defined(__Anima__CorpusCorpusMainWindow__) */
