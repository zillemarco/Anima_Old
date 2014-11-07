//
//  AnimaAssert.cpp
//  Anima
//
//  Created by Marco Zille on 06/11/14.
//
//

#include <stdio.h>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "AnimaAssert.h"

AnimaAssertWindow::AnimaAssertWindow(const char* expr, const char* fileName, int line, QWidget* parent)
	: QDialog(parent)
{
	QString errorString = tr("Error: ") + tr(expr);
	QString fileString = tr("\nIn file: ") + tr(fileName);
	QString lineString = QString("\nAt line: %1").arg(line);
	
	QString finalString = errorString + fileString + lineString;
	
	_label = new QLabel(finalString);
	_reportButton = new QPushButton(tr("&Report error"));
	_closeButton = new QPushButton(tr("&Close"));
	
	connect(_closeButton, SIGNAL(clicked()), this, SLOT(close()));
	
	QHBoxLayout* labelLayout = new QHBoxLayout;
	QHBoxLayout* buttonsLayout = new QHBoxLayout;
	
	_closeButton->setDefault(true);
	
	_closeButton->setFixedWidth(_closeButton->sizeHint().width());
	_reportButton->setFixedWidth(_reportButton->sizeHint().width());
	buttonsLayout->setAlignment(Qt::AlignRight);
	
	labelLayout->addWidget(_label);
	buttonsLayout->addWidget(_reportButton);
	buttonsLayout->addWidget(_closeButton);
	
	QVBoxLayout* main = new QVBoxLayout;
	main->addLayout(labelLayout);
	main->addLayout(buttonsLayout);
	
	setLayout(main);
	
	setWindowTitle(tr("ANIMA Error"));
	setFixedHeight(sizeHint().height());
}

void reportAssertionFailure(const char* expr, const char* fileName, int line)
{
	int argc = 0;
	QApplication app(argc, NULL);
	
	AnimaAssertWindow* window = new AnimaAssertWindow(expr, fileName, line);
	window->show();
	
	app.exec();
}
