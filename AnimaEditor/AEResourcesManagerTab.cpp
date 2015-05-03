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
#include "AELoadedResourcesPanel.h"
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
	setProperty("AEMDIFrame", true);
	style()->unpolish(this);
	style()->polish(this);
	_document = doc;
	
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle(tr("Resources manager"));

	_resourcesPanel = nullptr;
	_modelViewer = new AEResourcesManagerModelViewer(_document->GetEngine());
					
	QHBoxLayout* mainLayout = new QHBoxLayout;
	mainLayout->addWidget(QWidget::createWindowContainer(_modelViewer));
	
	setLayout(mainLayout);
}

void AEResourcesManagerTab::SetResourcesPanel(AELoadedResourcesPanel* resourcesPanel)
{
	_resourcesPanel = resourcesPanel;
}

void AEResourcesManagerTab::saveSettings(QSettings* settings)
{
	settings->beginGroup("AEResourcesManagerTab");
	//settings->setValue("_mainSplitter", _mainSplitter->saveState());
	settings->endGroup();
}

void AEResourcesManagerTab::readSettings(QSettings* settings)
{
	settings->beginGroup("AEResourcesManagerTab");
	//_mainSplitter->restoreState(settings->value("_mainSplitter").toByteArray());
	settings->endGroup();
}

void AEResourcesManagerTab::LoadResourcesTree()
{
	_resourcesPanel->LoadAllResources();
}