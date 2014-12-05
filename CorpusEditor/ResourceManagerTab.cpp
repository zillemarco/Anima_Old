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
#include <AnimaModelsManager.h>

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
	
	_resourcesTreeModel = new QStandardItemModel;
	_resourcesTree->setModel(_resourcesTreeModel);
	
	_modelResourcesTreeItem = new QStandardItem(tr("Models"));
	_materialResourcesTreeItem = new QStandardItem(tr("Materials"));
	_textureResourcesTreeItem = new QStandardItem(tr("Textures"));
	
	_resourcesTreeModel->insertRow(0, _modelResourcesTreeItem);
	_resourcesTreeModel->insertRow(1, _materialResourcesTreeItem);
	_resourcesTreeModel->insertRow(2, _textureResourcesTreeItem);
	
	_resourcesTreeModel->setHorizontalHeaderItem(0, new QStandardItem(tr("Elements")));
	
	_mainSplitter = new QSplitter(Qt::Horizontal);
	_mainSplitter->addWidget(_resourcesTree);
	_mainSplitter->addWidget(_tmp);
	_mainSplitter->setSizes(dims);
	_mainSplitter->setStretchFactor(1, 1);
	
	QHBoxLayout* mainLayout = new QHBoxLayout;
	mainLayout->addWidget(_mainSplitter);
	
	setLayout(mainLayout);
	
	LoadResourcesTree();
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
		QStandardItem *child = new QStandardItem(QString("Model %0").arg(i));
		child->setEditable(true);
		_modelResourcesTreeItem->appendRow(child);
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