//
//  NewInstanceWindow.cpp
//  Anima
//
//  Created by Marco Zille on 04/12/14.
//
//

#include "NewInstanceWindow.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QGridLayout>

NewInstanceWindow::NewInstanceWindow(QWidget* parent, QString instanceName)
: QDialog(parent)
, _instanceName(instanceName)
{
	
	_instanceNameLabel = new QLabel(tr("Model name:"));
	_instanceNameEdit = new QLineEdit;
	_instanceNameLabel->setBuddy(_instanceNameEdit);
	_instanceNameEdit->setText(_instanceName);
	
	_createButton = new QPushButton(tr("Create"));
	_createButton->setDefault(true);
	
	_cancelButton = new QPushButton(tr("Cancel"));
	_cancelButton->setDefault(false);
	
	_buttonBox = new QDialogButtonBox(Qt::Horizontal);
	_buttonBox->addButton(_cancelButton, QDialogButtonBox::RejectRole);
	_buttonBox->addButton(_createButton, QDialogButtonBox::AcceptRole);
	
	connect(_instanceNameEdit, SIGNAL(textChanged(const QString &)), this, SLOT(instanceNameChanged(const QString &)));
	connect(_createButton, SIGNAL(clicked()), this, SLOT(createClicked()));
	connect(_cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));
	
	QGridLayout* mainLayout = new QGridLayout;
	mainLayout->addWidget(_instanceNameLabel, 0, 0);
	mainLayout->addWidget(_instanceNameEdit, 0, 1, 1, 2);
	mainLayout->addWidget(_buttonBox, 1, 0, 1, 3);
	
	setLayout(mainLayout);
	
	setWindowTitle(tr("Create New Instace"));
	setFixedHeight(sizeHint().height());
	setFixedWidth(400);
}

NewInstanceWindow::~NewInstanceWindow()
{	
}

void NewInstanceWindow::createClicked()
{
	accept();
}

void NewInstanceWindow::cancelClicked()
{
	reject();
}

void NewInstanceWindow::instanceNameChanged(const QString& text)
{
	_instanceName = text;
}