//
//  ResourceManagerTab.cpp
//  Anima
//
//  Created by Marco Zille on 04/12/14.
//
//

#include "ResourceManagerTab.h"
#include "AnimaEditorDocument.h"
#include "CRModelViewer.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <AnimaModelsManager.h>
#include <QMenu>
#include <QItemSelection>
#include <AnimaMesh.h>
#include <AnimaScene.h>
#include <AnimaScenesManager.h>

Q_DECLARE_METATYPE(Anima::AnimaMesh*)

ResourceTreeItemModel::ResourceTreeItemModel(AnimaEditorDocument* doc, QObject *parent)
	: QStandardItemModel(parent)
{
	_document = doc;
}

ResourceTreeItemModel::ResourceTreeItemModel(AnimaEditorDocument* doc, int rows, int columns, QObject *parent)
	: QStandardItemModel(rows, columns, parent)
{
	_document = doc;
}

ResourceTreeItemModel::~ResourceTreeItemModel()
{
}

bool ResourceTreeItemModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if (role == Qt::EditRole)
	{
		Anima::AnimaString newString(value.toString().toLocal8Bit().constData(), _document->GetEngine()->GetScenesManager()->GetScene("test-scene")->GetStringAllocator());
		Anima::AnimaMesh* model = itemFromIndex(index)->data().value<Anima::AnimaMesh*>();
		
		if(index.column() == 0)
		{
			if(model->GetAnimaName() == newString)
				return false;
			
			bool bFound = false;
			for (int i = 0; i < _document->GetEngine()->GetScenesManager()->GetScene("test-scene")->GetModelsManager()->GetModelsNumber() && !bFound; i++)
			{
				if (_document->GetEngine()->GetScenesManager()->GetScene("test-scene")->GetModelsManager()->GetModel(i)->GetAnimaName() == newString)
					bFound = true;
			}
		
			if(bFound)
			{
				QMessageBox msg;
				msg.setWindowTitle(QString("Error"));
				msg.setText(QString("A model with this name alredy exists."));
				msg.exec();
			
				return false;
			}
		
			model->SetName(newString);
			_document->SetMofications();
		}
		else if(index.column() == 1)
		{
			if(model->GetAnimaMeshFileName() == newString)
				return false;
			
			bool bFound = false;
			for (int i = 0; i < _document->GetEngine()->GetScenesManager()->GetScene("test-scene")->GetModelsManager()->GetModelsNumber() && !bFound; i++)
			{
				if (_document->GetEngine()->GetScenesManager()->GetScene("test-scene")->GetModelsManager()->GetModel(i)->GetAnimaMeshFileName() == newString)
					bFound = true;
			}
			
			if(bFound)
			{
				QMessageBox msg;
				msg.setWindowTitle(QString("Error"));
				msg.setText(QString("A model with this file name alredy exists."));
				msg.exec();
				
				return false;
			}
			
			QString oldFileName = QString("%0/%1").arg(_document->projectDataModelsPath()).arg(model->GetMeshFileName());
			QString newFileName = QString("%0/%1").arg(_document->projectDataModelsPath()).arg(value.toString());
			
			QFile::rename(oldFileName, newFileName);
			
			model->SetMeshFileName(newString);
			_document->SetMofications();
		}
	}

	return QStandardItemModel::setData(index, value, role);
}

ResourceManagerTab::ResourceManagerTab(AnimaEditorDocument* doc)
{
	_document = doc;
	
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle(tr("Resources manager"));

	_resourcesTree = new QTreeView;
	_modelViewer = new CRModelViewer(_document->GetEngine());
	
	QList<int> dims;
	dims.push_back(300);
	dims.push_back(1);
	
	_resourcesTreeModel = new ResourceTreeItemModel(_document, 0, 2);
	_resourcesTree->setModel(_resourcesTreeModel);
	
	_modelResourcesTreeItem = new QStandardItem(tr("Models"));
	_materialResourcesTreeItem = new QStandardItem(tr("Materials"));
	_textureResourcesTreeItem = new QStandardItem(tr("Textures"));
	
	_resourcesTreeModel->insertRow(0, _modelResourcesTreeItem);
	_resourcesTreeModel->insertRow(1, _materialResourcesTreeItem);
	_resourcesTreeModel->insertRow(2, _textureResourcesTreeItem);
	
	_resourcesTreeModel->setHorizontalHeaderItem(0, new QStandardItem(tr("Resource name")));
	_resourcesTreeModel->setHorizontalHeaderItem(1, new QStandardItem(tr("File name")));
	connect(_resourcesTreeModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(resourceTreeItemDataChanged(QStandardItem*)));

	QItemSelectionModel* selectionModel = _resourcesTree->selectionModel();
	connect(selectionModel, SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(resourceTreeItemSelectionChanged(const QItemSelection&, const QItemSelection&)));
	
	_resourcesTree->setColumnWidth(0, 150);
	_resourcesTree->setColumnWidth(1, 100);
	
	_mainSplitter = new QSplitter(Qt::Horizontal);
	_mainSplitter->addWidget(_resourcesTree);
	_mainSplitter->addWidget(QWidget::createWindowContainer(_modelViewer));
	_mainSplitter->setSizes(dims);
	_mainSplitter->setStretchFactor(1, 1);
	
	QHBoxLayout* mainLayout = new QHBoxLayout;
	mainLayout->addWidget(_mainSplitter);
	
	setLayout(mainLayout);
	
	LoadResourcesTree();
	
	createMenus();
}

void ResourceManagerTab::LoadResourcesTree()
{
	LoadModelsTree();
	LoadMaterialsTree();
	LoadTexturesTree();
}

void ResourceManagerTab::LoadModelsTree()
{
	_modelResourcesTreeItem->removeRows(0, _modelResourcesTreeItem->rowCount());
	
	Anima::AnimaModelsManager* mgr = _document->GetEngine()->GetScenesManager()->GetScene("test-scene")->GetModelsManager();
	for(int i = 0; i < mgr->GetModelsNumber(); i++)
	{
		QList<QStandardItem*> newItem;
		
		QStandardItem *resourceNameItem = new QStandardItem(QString("%0").arg(mgr->GetModel(i)->GetName()));
		resourceNameItem->setData(QVariant::fromValue(mgr->GetModel(i)), ModelRole);
		resourceNameItem->setEditable(true);
		QStandardItem *resourceFileNameItem = new QStandardItem(QString("%0").arg(mgr->GetModel(i)->GetMeshFileName()));
		resourceFileNameItem->setData(QVariant::fromValue(mgr->GetModel(i)), ModelRole);
		resourceFileNameItem->setEditable(true);
		
		newItem.append(resourceNameItem);
		newItem.append(resourceFileNameItem);
		
		_modelResourcesTreeItem->appendRow(newItem);
	}
}

void ResourceManagerTab::LoadMaterialsTree()
{
	
}

void ResourceManagerTab::LoadTexturesTree()
{
	
}

void ResourceManagerTab::saveSettings(QSettings* settings)
{
	settings->beginGroup("ResourceManagerTab");
	settings->setValue("_mainSplitter", _mainSplitter->saveState());
	settings->endGroup();
}

void ResourceManagerTab::readSettings(QSettings* settings)
{
	settings->beginGroup("ResourceManagerTab");
	_mainSplitter->restoreState(settings->value("_mainSplitter").toByteArray());
	settings->endGroup();
}

void ResourceManagerTab::createActions()
{
	_importModelAct = new QAction(tr("Import &model"), _resourceTreeContextMenu);
	_importModelAct->setStatusTip(tr("Import a new model into the resources"));
	connect(_importModelAct, SIGNAL(triggered()), this, SLOT(importModel()));
	
	_importTextureAct = new QAction(tr("Import &texture"), _resourceTreeContextMenu);
	_importTextureAct->setStatusTip(tr("Import a new texture into the resources"));
	connect(_importTextureAct, SIGNAL(triggered()), this, SLOT(importTexture()));
	
	_addNewMaterialAct = new QAction(tr("Add new m&aterial"), _resourceTreeContextMenu);
	_addNewMaterialAct->setStatusTip(tr("Add a new material to the resources"));
	connect(_addNewMaterialAct, SIGNAL(triggered()), this, SLOT(addNewMaterial()));
}

void ResourceManagerTab::importModel()
{
	if(_document->ImportModel())
		LoadModelsTree();
}

void ResourceManagerTab::importTexture()
{
	
}

void ResourceManagerTab::addNewMaterial()
{
	
}

void ResourceManagerTab::createMenus()
{
	_resourceTreeContextMenu = new QMenu(_resourcesTree);
	_resourcesTree->setContextMenuPolicy(Qt::ActionsContextMenu);
	
	createActions();
	
	_resourcesTree->addAction(_importModelAct);
	_resourcesTree->addAction(_importTextureAct);
	_resourcesTree->addAction(_addNewMaterialAct);
}

void ResourceManagerTab::resourceTreeItemDataChanged(QStandardItem* item)
{
}

void ResourceManagerTab::resourceTreeItemSelectionChanged(const QItemSelection& current, const QItemSelection& previous)
{
	const QModelIndex index = _resourcesTree->selectionModel()->currentIndex();
	QVariant var = index.data(ModelRole);

	if (var.isValid())
	{
		Anima::AnimaMesh* model = var.value<Anima::AnimaMesh*>();
		_modelViewer->setSelectedModel(model);
	}
}