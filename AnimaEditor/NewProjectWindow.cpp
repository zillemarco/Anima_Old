//
//  NewProjectWindow.cpp
//  Anima
//
//  Created by Marco Zille on 04/12/14.
//
//

#include "NewProjectWindow.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QGridLayout>

NewProjectWindow::NewProjectWindow(QWidget* parent)
: QDialog(parent)
, _projectName(tr(""))
, _projectFolderPath(tr(""))
{
	
	_projectNameLabel = new QLabel(tr("Project name:"));
	_projectNameEdit = new QLineEdit;
	_projectNameLabel->setBuddy(_projectNameEdit);
	
	_projectFolderPathLabel = new QLabel(tr("Project folder:"));
	_projectFolderPathEdit = new QLineEdit;
	_projectFolderPathLabel->setBuddy(_projectFolderPathEdit);
	
	_browseButton = new QPushButton(tr("..."));
	_browseButton->setDefault(false);
	_browseButton->setFixedWidth(30);
	
	_createButton = new QPushButton(tr("Create"));
	_createButton->setDefault(true);
	
	_cancelButton = new QPushButton(tr("Cancel"));
	_cancelButton->setDefault(false);
	
	_buttonBox = new QDialogButtonBox(Qt::Horizontal);
	_buttonBox->addButton(_cancelButton, QDialogButtonBox::RejectRole);
	_buttonBox->addButton(_createButton, QDialogButtonBox::AcceptRole);
	
	connect(_projectNameEdit, SIGNAL(textChanged(const QString &)), this, SLOT(projectNameChanged(const QString &)));
	connect(_projectFolderPathEdit, SIGNAL(textChanged(const QString &)), this, SLOT(projectFolderPathChanged(const QString &)));
	connect(_browseButton, SIGNAL(clicked()), this, SLOT(browseClicked()));
	connect(_createButton, SIGNAL(clicked()), this, SLOT(createClicked()));
	connect(_cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));
	
	QGridLayout* mainLayout = new QGridLayout;
	mainLayout->addWidget(_projectNameLabel, 0, 0);
	mainLayout->addWidget(_projectNameEdit, 0, 1, 1, 2);
	mainLayout->addWidget(_projectFolderPathLabel, 1, 0);
	mainLayout->addWidget(_projectFolderPathEdit, 1, 1);
	mainLayout->addWidget(_browseButton, 1, 2);
	mainLayout->addWidget(_buttonBox, 2, 0, 1, 3);
	
	setLayout(mainLayout);
	
	setWindowTitle(tr("New project"));
	setFixedHeight(sizeHint().height());
	setFixedWidth(400);
}

NewProjectWindow::~NewProjectWindow()
{
	
}

void NewProjectWindow::browseClicked()
{
 	QString directory = tr("");
	
	QFileDialog *fd = new QFileDialog;
	fd->setFileMode(QFileDialog::Directory);
	fd->setOption(QFileDialog::ShowDirsOnly);
	fd->setViewMode(QFileDialog::Detail);
	
	if (fd->exec())
	{
		directory = fd->selectedFiles()[0];
		_projectFolderPathEdit->setText(directory);
	}
}

void NewProjectWindow::createClicked()
{
	accept();
}

void NewProjectWindow::cancelClicked()
{
	reject();
}

void NewProjectWindow::projectNameChanged(const QString& text)
{
	_projectName = text;
}

void NewProjectWindow::projectFolderPathChanged(const QString& text)
{
	_projectFolderPath = text;
}