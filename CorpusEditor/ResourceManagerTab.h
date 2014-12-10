//
//  ResourceManagerTab.h
//  Anima
//
//  Created by Marco Zille on 04/12/14.
//
//

#ifndef __Anima__ResourceManagerTab__
#define __Anima__ResourceManagerTab__

#include <stdio.h>
#include <QFrame>
#include <QCloseEvent>
#include <QSplitter>
#include <QTreeView>
#include <QListView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QPushButton>
#include <QSettings>
#include <QPoint>
#include <QAction>
#include <QMenu>

class CorpusDocument;

class ResourceManagerTab : public QFrame
{
	Q_OBJECT
public:
	ResourceManagerTab(CorpusDocument* doc);
	
	void saveSettings(QSettings* settings);
	void readSettings(QSettings* settings);
	
public:
	void LoadResourcesTree();
	void LoadModelsTree();
	void LoadMaterialsTree();
	void LoadTexturesTree();
	
private:
	void createActions();
	void createMenus();
	
protected:
	void resourceTreeContextMenu(const QPoint& point);
	
private slots:
	void importModel();
	void importTexture();
	void addNewMaterial();
	
private:
	CorpusDocument* _document;
	
	QSplitter* _mainSplitter;
	QTreeView* _resourcesTree;
	
	QListView* _tmp;
	
	QAction* _importModelAct;
	QAction* _importTextureAct;
	QAction* _addNewMaterialAct;
	
	QMenu*	_resourceTreeContextMenu;
	
	QStandardItemModel* _resourcesTreeModel;
	QStandardItem* _modelResourcesTreeItem;
	QStandardItem* _materialResourcesTreeItem;
	QStandardItem* _textureResourcesTreeItem;
};

#endif /* defined(__Anima__ResourceManagerTab__) */
