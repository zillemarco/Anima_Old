//
//  ImportModelWindow.cpp
//  Anima
//
//  Created by Marco Zille on 04/12/14.
//
//

#include "ImportModelWindow.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QGridLayout>

ImportModelWindow::ImportModelWindow(QWidget* parent, QString modelName, QString modelPath)
: QDialog(parent)
, _modelName(modelName)
, _modelPath(modelPath)
{
	
	_modelNameLabel = new QLabel(tr("Model name:"));
	_modelNameEdit = new QLineEdit;
	_modelNameLabel->setBuddy(_modelNameEdit);
	_modelNameEdit->setText(_modelName);
	
	_modelPathLabel = new QLabel(tr("Model path:"));
	_modelPathEdit = new QLineEdit;
	_modelPathLabel->setBuddy(_modelPathEdit);
	_modelPathEdit->setText(_modelPath);
	
	_browseButton = new QPushButton(tr("..."));
	_browseButton->setDefault(false);
	_browseButton->setFixedWidth(30);
	
	_importButton = new QPushButton(tr("Import"));
	_importButton->setDefault(true);
	
	_cancelButton = new QPushButton(tr("Cancel"));
	_cancelButton->setDefault(false);
	
	_buttonBox = new QDialogButtonBox(Qt::Horizontal);
	_buttonBox->addButton(_cancelButton, QDialogButtonBox::RejectRole);
	_buttonBox->addButton(_importButton, QDialogButtonBox::AcceptRole);
	
	connect(_modelNameEdit, SIGNAL(textChanged(const QString &)), this, SLOT(modelNameChanged(const QString &)));
	connect(_modelPathEdit, SIGNAL(textChanged(const QString &)), this, SLOT(modelPathChanged(const QString &)));
	connect(_browseButton, SIGNAL(clicked()), this, SLOT(browseClicked()));
	connect(_importButton, SIGNAL(clicked()), this, SLOT(createClicked()));
	connect(_cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));
	
	QGridLayout* mainLayout = new QGridLayout;
	mainLayout->addWidget(_modelNameLabel, 0, 0);
	mainLayout->addWidget(_modelNameEdit, 0, 1, 1, 2);
	mainLayout->addWidget(_modelPathLabel, 1, 0);
	mainLayout->addWidget(_modelPathEdit, 1, 1);
	mainLayout->addWidget(_browseButton, 1, 2);
	mainLayout->addWidget(_buttonBox, 2, 0, 1, 3);
	
	setLayout(mainLayout);
	
	setWindowTitle(tr("Import Model"));
	setFixedHeight(sizeHint().height());
	setFixedWidth(400);
}

ImportModelWindow::~ImportModelWindow()
{	
}

void ImportModelWindow::browseClicked()
{
	QString selfilter = QString("3ds Max 3DS (*.3ds)");
	QString filters = QString("3ds Max 3DS (*.3ds);;3ds Max ASE (*.ase);;Collada (*.dae);;Wavefront Object (*.obj);;Blender 3D (*.blend);;Industry Foundation Classes [IFC/Step] (*.ifc);;XGL (*.xgl);; XGL (*.zgl);;Stanford Polygon Library (*.ply);;All files (*.*)");

	QString filePath = QFileDialog::getOpenFileName(NULL, QString("AnimaEditor - Import model"), QString(""), filters, &selfilter);
	_modelPathEdit->setText(filePath);
}

void ImportModelWindow::createClicked()
{
	accept();
}

void ImportModelWindow::cancelClicked()
{
	reject();
}

void ImportModelWindow::modelNameChanged(const QString& text)
{
	_modelName = text;
}

void ImportModelWindow::modelPathChanged(const QString& text)
{
	_modelPath = text;
}