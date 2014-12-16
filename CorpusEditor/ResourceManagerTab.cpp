//
//  ResourceManagerTab.cpp
//  Anima
//
//  Created by Marco Zille on 04/12/14.
//
//

#include "ResourceManagerTab.h"
#include "CorpusDocument.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <AnimaModelsManager.h>
#include <QMenu>
#include <AnimaModel.h>

Q_DECLARE_METATYPE(Anima::AnimaModel*)

ResourceTreeItemModel::ResourceTreeItemModel(CorpusDocument* doc, QObject *parent)
	: QStandardItemModel(parent)
{
	_document = doc;
}

ResourceTreeItemModel::ResourceTreeItemModel(CorpusDocument* doc, int rows, int columns, QObject *parent)
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
		Anima::AnimaString newString(value.toString().toLocal8Bit().constData(), _document->GetEngine());
		Anima::AnimaModel* model = itemFromIndex(index)->data().value<Anima::AnimaModel*>();
		
		if(index.column() == 0)
		{
			if(model->GetAnimaModelName() == newString)
				return false;
			
			bool bFound = false;
			for (int i = 0; i < _document->GetEngine()->GetModelsManager()->GetModelsNumber() && !bFound; i++)
			{
				if(_document->GetEngine()->GetModelsManager()->GetPModel(i)->GetAnimaModelName() == newString)
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
		
			model->SetModelName(newString);
			_document->SetMofications();
		}
		else if(index.column() == 1)
		{
			if(model->GetAnimaModelFileName() == newString)
				return false;
			
			bool bFound = false;
			for (int i = 0; i < _document->GetEngine()->GetModelsManager()->GetModelsNumber() && !bFound; i++)
			{
				if(_document->GetEngine()->GetModelsManager()->GetPModel(i)->GetAnimaModelFileName() == newString)
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
			
			QString oldFileName = QString("%0/%1").arg(_document->projectDataModelsPath()).arg(model->GetModelFileName());
			QString newFileName = QString("%0/%1").arg(_document->projectDataModelsPath()).arg(value.toString());
			
			QFile::rename(oldFileName, newFileName);
			
			model->SetModelFileName(newString);
			_document->SetMofications();
		}
	}

	return QStandardItemModel::setData(index, value, role);
}

ResourceManagerTab::ResourceManagerTab(CorpusDocument* doc)
{
	_document = doc;
	
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle(tr("Resources manager"));

	_resourcesTree = new QTreeView;
	_tmp = new QListView;
	
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
	
	_resourcesTree->setColumnWidth(0, 150);
	_resourcesTree->setColumnWidth(1, 100);
	
	_mainSplitter = new QSplitter(Qt::Horizontal);
	_mainSplitter->addWidget(_resourcesTree);
	_mainSplitter->addWidget(_tmp);
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
	
	Anima::AnimaModelsManager* mgr = _document->GetEngine()->GetModelsManager();
	for(int i = 0; i < mgr->GetModelsNumber(); i++)
	{
		QList<QStandardItem*> newItem;
		
		QStandardItem *resourceNameItem = new QStandardItem(QString("%0").arg(mgr->GetPModel(i)->GetModelName()));
		resourceNameItem->setData(QVariant::fromValue(mgr->GetPModel(i)), ResourceManagerTabRole);
		resourceNameItem->setEditable(true);
		QStandardItem *resourceFileNameItem = new QStandardItem(QString("%0").arg(mgr->GetPModel(i)->GetModelFileName()));
		resourceFileNameItem->setData(QVariant::fromValue(mgr->GetPModel(i)), ResourceManagerTabRole);
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