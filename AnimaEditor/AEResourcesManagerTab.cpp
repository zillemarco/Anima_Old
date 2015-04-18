//
//  AEResourcesManagerTab.cpp
//  Anima
//
//  Created by Marco Zille on 04/12/14.
//
//

#include "AEResourcesManagerTab.h"
#include "AEDocument.h"
#include "AEResourcesManagerModelViewer.h"
#include "AELoadedResourcesTreeView.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <AnimaModelsManager.h>
#include <QMenu>
#include <QItemSelection>
#include <AnimaModel.h>
#include <AnimaScene.h>
#include <AnimaScenesManager.h>

AEResourcesManagerTab::AEResourcesManagerTab(AEDocument* doc)
{
	_document = doc;
	
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle(tr("Resources manager"));

	_resourcesTree = new AELoadedResourcesTreeView(doc);
	_modelViewer = new AEResourcesManagerModelViewer(_document->GetEngine());
	_modelViewer->InitRender();
	
	QList<int> dims;
	dims.push_back(300);
	dims.push_back(1);
		
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
}

void AEResourcesManagerTab::LoadResourcesTree()
{
	_resourcesTree->LoadResources();
}

void AEResourcesManagerTab::saveSettings(QSettings* settings)
{
	settings->beginGroup("AEResourcesManagerTab");
	settings->setValue("_mainSplitter", _mainSplitter->saveState());
	settings->endGroup();
}

void AEResourcesManagerTab::readSettings(QSettings* settings)
{
	settings->beginGroup("AEResourcesManagerTab");
	_mainSplitter->restoreState(settings->value("_mainSplitter").toByteArray());
	settings->endGroup();
}