#pragma once

#include <qtreeview.h>
#include <QStandardItemModel>
#include <QStandardItem>
#include <qdockwidget.h>

class AEDocument;
class QMenu;
class QAction;
class QPoint;

class AELoadedResourcesTreeViewItemModel : public QStandardItemModel
{
	Q_OBJECT
public:
	AELoadedResourcesTreeViewItemModel(AEDocument* doc, QObject *parent = 0);
	AELoadedResourcesTreeViewItemModel(AEDocument* doc, int rows, int columns, QObject *parent = 0);
	~AELoadedResourcesTreeViewItemModel();

public:
	virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

private:
	AEDocument* _document;
};

class AELoadedResourcesTreeView : public QTreeView
{
	Q_OBJECT
public:
	AELoadedResourcesTreeView(AEDocument* doc);
	virtual ~AELoadedResourcesTreeView();

protected:
	enum AELoadedResourcesTreeViewRoles {
		ModelRole = Qt::UserRole + 1,
	};

public:
	void LoadResources();

private:
	void LoadModels();
	void LoadMaterials();
	void LoadTextures();
	void LoadMeshes();
	void LoadModelInstances();
	void LoadMeshInstances();

protected:
	void contextMenu(const QPoint& point);

private:
	void createActions();
	void createMenus();

private slots:
	void resourceModelSelectionChanged(const QItemSelection& current, const QItemSelection& previous);

	void importModel();
	void createModelInstance();
	void importTexture();
	void addNewMaterial();

protected:
	AEDocument* _document;
	AELoadedResourcesTreeViewItemModel* _resourcesModel;

	QStandardItem* _modelResourcesItem;
	QStandardItem* _materialResourcesItem;
	QStandardItem* _textureResourcesItem;
	QStandardItem* _meshResourcesItem;
	QStandardItem* _modelInstancesResourcesItem;
	QStandardItem* _meshInstancesResourcesItem;

	QMenu*	_contextMenu;
	QAction* _importModelAct;
	QAction* _createModelInstanceAct;
	QAction* _importTextureAct;
	QAction* _addNewMaterialAct;
};

class AELoadedResourcesPanel : public QDockWidget
{
	Q_OBJECT
public:
	AELoadedResourcesPanel(AEDocument* doc, QWidget* parent);
	virtual ~AELoadedResourcesPanel();

public:
	void LoadAllResources();

protected:
	AEDocument* _document;
	AELoadedResourcesTreeView* _resourcesTree;
};