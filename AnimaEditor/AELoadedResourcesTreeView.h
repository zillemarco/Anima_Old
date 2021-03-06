#pragma once

#include <qtreeview.h>
#include <QStandardItemModel>
#include <QStandardItem>
#include <qdockwidget.h>
#include "AnimaModel.h"

class AEDocument;
class QMenu;
class QAction;
class QPoint;
class AEResourcesManagerModelViewer;

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
	AELoadedResourcesTreeView(AEDocument* doc, AEResourcesManagerModelViewer* resourcesViewer);
	virtual ~AELoadedResourcesTreeView();

protected:
	enum AELoadedResourcesTreeViewRoles {
		ModelRole = Qt::UserRole + 1,
		ModelInstanceRole,
		MeshRole,
		MeshInstanceRole,
		MaterialRole,
		TextureRole
	};

public:
	void LoadResources();

private:
	void LoadModels();
	void LoadMaterials();
	void LoadTextures();
	// void LoadMeshes();
	// void LoadModelInstances();
	// void LoadMeshInstances();

	void LoadModelMeshes(Anima::AnimaModel* model, QStandardItem* parentItem);
	void LoadModelAnimations(Anima::AnimaModel* model, QStandardItem* parentItem);

protected:
	void contextMenu(const QPoint& point);

private:
	void createActions();
	void createMenus();

private slots:
	void resourceSelectionChanged(const QItemSelection& current, const QItemSelection& previous);

	void importModel();
	void createModelInstance();
	void importTexture();
	void addNewMaterial();

protected:
	AEDocument* _document;
	AEResourcesManagerModelViewer* _resourcesViewer;
	AELoadedResourcesTreeViewItemModel* _resourcesModel;

	QStandardItem* _modelResourcesItem;
	QStandardItem* _materialResourcesItem;
	QStandardItem* _textureResourcesItem;
	// QStandardItem* _meshResourcesItem;
	// QStandardItem* _modelInstancesResourcesItem;
	// QStandardItem* _meshInstancesResourcesItem;

	QMenu*	_contextMenu;
	QAction* _importModelAct;
	QAction* _createModelInstanceAct;
	QAction* _importTextureAct;
	QAction* _addNewMaterialAct;
};