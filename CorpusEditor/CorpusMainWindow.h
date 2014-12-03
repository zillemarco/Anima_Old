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

class CorpusMainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	CorpusMainWindow();
	
protected:
	void closeEvent(QCloseEvent *event);
	
 private slots:
	void newFile();
	void open();
	void save();
	void saveAs();
	void cut();
	void copy();
	void paste();
	void about();
	void updateMenus();
	void updateWindowMenu();
	EditWindow *createEditWindow();
	void switchLayoutDirection();
	void setActiveSubWindow(QWidget *window);
	
private:
	void createActions();
	void createMenus();
	void createToolBars();
	void createStatusBar();
	void readSettings();
	void writeSettings();
	EditWindow *activeEditWindow();
	QMdiSubWindow *findEditWindow(const QString &fileName);
	
	QMdiArea *mdiArea;
	QSignalMapper *windowMapper;
	
	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *windowMenu;
	QMenu *helpMenu;
	QToolBar *fileToolBar;
	QToolBar *editToolBar;
	QAction *newAct;
	QAction *openAct;
	QAction *saveAct;
	QAction *saveAsAct;
	QAction *exitAct;
	QAction *cutAct;
	QAction *copyAct;
	QAction *pasteAct;
	QAction *closeAct;
	QAction *closeAllAct;
	QAction *tileAct;
	QAction *cascadeAct;
	QAction *nextAct;
	QAction *previousAct;
	QAction *separatorAct;
	QAction *aboutAct;
	QAction *aboutQtAct;
};

#endif /* defined(__Anima__CorpusCorpusMainWindow__) */
