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
class CRModelViewer;

class ResourceTreeItemModel : public QStandardItemModel
{
	Q_OBJECT
public:
	ResourceTreeItemModel(CorpusDocument* doc, QObject *parent = 0);
	ResourceTreeItemModel(CorpusDocument* doc, int rows, int columns, QObject *parent = 0);
	~ResourceTreeItemModel();

public:
	virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

private:
	CorpusDocument* _document;
};

class ResourceManagerTab : public QFrame
{
	Q_OBJECT
public:
	ResourceManagerTab(CorpusDocument* doc);
	
	void saveSettings(QSettings* settings);
	void readSettings(QSettings* settings);
	
protected:
	enum ResourceManagerTabRoles {
		ResourceManagerTabRole = Qt::UserRole + 1,
		ModelRole = Qt::UserRole + 2,
	};

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
	void resourceTreeItemDataChanged(QStandardItem* item);
	void resourceTreeItemSelectionChanged(const QItemSelection& current, const QItemSelection& previous);
	
private:
	CorpusDocument* _document;
	
	QSplitter* _mainSplitter;
	QTreeView* _resourcesTree;
	
	CRModelViewer* _modelViewer;
	
	QAction* _importModelAct;
	QAction* _importTextureAct;
	QAction* _addNewMaterialAct;
	
	QMenu*	_resourceTreeContextMenu;
	
	ResourceTreeItemModel* _resourcesTreeModel;
	QStandardItem* _modelResourcesTreeItem;
	QStandardItem* _materialResourcesTreeItem;
	QStandardItem* _textureResourcesTreeItem;
};

#endif /* defined(__Anima__ResourceManagerTab__) */
