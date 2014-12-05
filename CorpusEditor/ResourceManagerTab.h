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
	
private:
	void LoadModelsTree();
	void LoadMaterialsTree();
	void LoadTexturesTree();
	
private:
	CorpusDocument* _document;
	
	QSplitter* _mainSplitter;
	QTreeView* _resourcesTree;
	
	QListView* _tmp;
	
	QStandardItemModel* _resourcesTreeModel;
	QStandardItem* _modelResourcesTreeItem;
	QStandardItem* _materialResourcesTreeItem;
	QStandardItem* _textureResourcesTreeItem;
};

#endif /* defined(__Anima__ResourceManagerTab__) */
