#include "AELoadedResourcesTreeView.h"
#include "AEDocument.h"

#include <qmenu.h>
#include <qaction.h>
#include <qpoint.h>

#include <AnimaModel.h>
#include <AnimaScene.h>
#include <AnimaModelsManager.h>
#include <AnimaScenesManager.h>

Q_DECLARE_METATYPE(Anima::AnimaModel*)

AELoadedResourcesTreeViewItemModel::AELoadedResourcesTreeViewItemModel(AEDocument* doc, QObject *parent)
	: QStandardItemModel(parent)
{
	_document = doc;
}

AELoadedResourcesTreeViewItemModel::AELoadedResourcesTreeViewItemModel(AEDocument* doc, int rows, int columns, QObject *parent)
	: QStandardItemModel(rows, columns, parent)
{
	_document = doc;
}

AELoadedResourcesTreeViewItemModel::~AELoadedResourcesTreeViewItemModel()
{
}

bool AELoadedResourcesTreeViewItemModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if (role == Qt::EditRole)
	{
		Anima::AnimaString newString(value.toString().toLocal8Bit().constData(), _document->GetEngine()->GetStringAllocator());
		Anima::AnimaModel* model = itemFromIndex(index)->data().value<Anima::AnimaModel*>();

		if (index.column() == 0)
		{
			//if(model->GetAnimaName() == newString)
			//	return false;
			//
			//bool bFound = false;
			//for (int i = 0; i < _document->GetEngine()->GetScenesManager()->GetScene("AnimaEditor")->GetModelsManager()->GetModelsNumber() && !bFound; i++)
			//{
			//	if (_document->GetEngine()->GetScenesManager()->GetScene("AnimaEditor")->GetModelsManager()->GetModel(i)->GetAnimaName() == newString)
			//		bFound = true;
			//}
			//
			//if(bFound)
			//{
			//	QMessageBox msg;
			//	msg.setWindowTitle(QString("Error"));
			//	msg.setText(QString("A model with this name alredy exists."));
			//	msg.exec();
			//
			//	return false;
			//}
			//
			//model->SetName(newString);
			//_document->SetMofications();
		}
		else if (index.column() == 1)
		{
			//if(model->GetAnimaOriginFileName() == newString)
			//	return false;
			//
			//bool bFound = false;
			//for (int i = 0; i < _document->GetEngine()->GetScenesManager()->GetScene("AnimaEditor")->GetModelsManager()->GetModelsNumber() && !bFound; i++)
			//{
			//	if (_document->GetEngine()->GetScenesManager()->GetScene("AnimaEditor")->GetModelsManager()->GetModel(i)->GetAnimaOriginFileName() == newString)
			//		bFound = true;
			//}
			//
			//if(bFound)
			//{
			//	QMessageBox msg;
			//	msg.setWindowTitle(QString("Error"));
			//	msg.setText(QString("A model with this file name alredy exists."));
			//	msg.exec();
			//	
			//	return false;
			//}
			//
			//QString oldFileName = QString("%0/%1").arg(_document->projectDataModelsPath()).arg(model->GetOriginFileName());
			//QString newFileName = QString("%0/%1").arg(_document->projectDataModelsPath()).arg(value.toString());
			//
			//QFile::rename(oldFileName, newFileName);
			//
			//model->SetOriginFileName(newString);
			//_document->SetMofications();
		}
	}

	return QStandardItemModel::setData(index, value, role);
}

AELoadedResourcesTreeView::AELoadedResourcesTreeView(AEDocument* doc)
{
	_document = doc;

	_resourcesModel = new AELoadedResourcesTreeViewItemModel(_document, 0, 2);
	setModel(_resourcesModel);

	_modelResourcesItem = new QStandardItem(tr("Models"));
	_materialResourcesItem = new QStandardItem(tr("Materials"));
	_textureResourcesItem = new QStandardItem(tr("Textures"));

	_resourcesModel->insertRow(0, _modelResourcesItem);
	_resourcesModel->insertRow(1, _materialResourcesItem);
	_resourcesModel->insertRow(2, _textureResourcesItem);

	_resourcesModel->setHorizontalHeaderItem(0, new QStandardItem(tr("Resource name")));
	_resourcesModel->setHorizontalHeaderItem(1, new QStandardItem(tr("File name")));

	QItemSelectionModel* selectModel = selectionModel();
	connect(selectModel, SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(resourceModelSelectionChanged(const QItemSelection&, const QItemSelection&)));
	
	createMenus();
}

AELoadedResourcesTreeView::~AELoadedResourcesTreeView()
{
}

void AELoadedResourcesTreeView::LoadResources()
{
	LoadModels();
	LoadMaterials();
	LoadTextures();
}

void AELoadedResourcesTreeView::LoadModels()
{
	_modelResourcesItem->removeRows(0, _modelResourcesItem->rowCount());

	Anima::AnimaModelsManager* mgr = _document->GetEngine()->GetScenesManager()->GetScene("AnimaEditor")->GetModelsManager();
	for (int i = 0; i < mgr->GetModelsNumber(); i++)
	{
		QList<QStandardItem*> newItem;

		QStandardItem *resourceNameItem = new QStandardItem(QString("%0").arg(mgr->GetModel(i)->GetName()));
		resourceNameItem->setData(QVariant::fromValue(mgr->GetModel(i)), ModelRole);
		resourceNameItem->setEditable(false);
		QStandardItem *resourceFileNameItem = new QStandardItem(QString("%0").arg(mgr->GetModel(i)->GetOriginFileName()));
		resourceFileNameItem->setData(QVariant::fromValue(mgr->GetModel(i)), ModelRole);
		resourceFileNameItem->setEditable(false);

		newItem.append(resourceNameItem);
		newItem.append(resourceFileNameItem);

		_modelResourcesItem->appendRow(newItem);
	}
}

void AELoadedResourcesTreeView::LoadMaterials()
{
}

void AELoadedResourcesTreeView::LoadTextures()
{
}

void AELoadedResourcesTreeView::resourceModelSelectionChanged(const QItemSelection& current, const QItemSelection& previous)
{
	const QModelIndex index = selectionModel()->currentIndex();
	QVariant var = index.data(ModelRole);

	if (var.isValid())
	{
		Anima::AnimaModel* model = var.value<Anima::AnimaModel*>();
		//_modelViewer->setSelectedModel(model);
	}
}

void AELoadedResourcesTreeView::createActions()
{
	_importModelAct = new QAction(tr("Import &model"), _contextMenu);
	_importModelAct->setStatusTip(tr("Import a new model into the resources"));
	connect(_importModelAct, SIGNAL(triggered()), this, SLOT(importModel()));

	_importTextureAct = new QAction(tr("Import &texture"), _contextMenu);
	_importTextureAct->setStatusTip(tr("Import a new texture into the resources"));
	connect(_importTextureAct, SIGNAL(triggered()), this, SLOT(importTexture()));

	_addNewMaterialAct = new QAction(tr("Add new m&aterial"), _contextMenu);
	_addNewMaterialAct->setStatusTip(tr("Add a new material to the resources"));
	connect(_addNewMaterialAct, SIGNAL(triggered()), this, SLOT(addNewMaterial()));
}

void AELoadedResourcesTreeView::importModel()
{
	if (_document->ImportModel())
		LoadResources();
}

void AELoadedResourcesTreeView::importTexture()
{

}

void AELoadedResourcesTreeView::addNewMaterial()
{

}

void AELoadedResourcesTreeView::createMenus()
{
	_contextMenu = new QMenu(this);
	setContextMenuPolicy(Qt::ActionsContextMenu);

	createActions();

	addAction(_importModelAct);
	addAction(_importTextureAct);
	addAction(_addNewMaterialAct);
}