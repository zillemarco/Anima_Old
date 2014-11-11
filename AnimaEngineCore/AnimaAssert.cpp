//
//  AnimaAssert.cpp
//  Anima
//
//  Created by Marco Zille on 06/11/14.
//
//

#include <stdio.h>
//#include <QLabel>
//#include <QLineEdit>
//#include <QCheckBox>
//#include <QPushButton>
//#include <QHBoxLayout>
//#include <QVBoxLayout>
//#include <QStyleFactory>
//#include <QFile>
//#include <QApplication>

#include "AnimaAssert.h"

//AnimaAssertWindow::AnimaAssertWindow(const char* expr, const char* fileName, int line, QWidget* parent)
//	: QDialog(parent)
//{
//	QString errorString = tr("Error: ") + tr(expr);
//	QString fileString = tr("\n\nIn file: ") + tr(fileName);
//	QString lineString = QString("\n\nAt line: %1").arg(line);
//	
//	QString finalString = errorString + fileString + lineString;
//	
//	_label = new QLabel(finalString);
//	_reportButton = new QPushButton(tr("&Report error"));
//	_closeButton = new QPushButton(tr("&Close"));
//	
//	connect(_closeButton, SIGNAL(clicked()), this, SLOT(close()));
//	
//	QHBoxLayout* labelLayout = new QHBoxLayout;
//	QHBoxLayout* buttonsLayout = new QHBoxLayout;
//	
//	_label->setWordWrap(true);
//	_label->setMaximumWidth(350);
//	_closeButton->setDefault(true);
//	_closeButton->setFixedWidth(_closeButton->sizeHint().width());
//	_reportButton->setFixedWidth(_reportButton->sizeHint().width());
//	
//	buttonsLayout->setAlignment(Qt::AlignRight);
//	
//	labelLayout->addWidget(_label);
//	buttonsLayout->addWidget(_reportButton);
//	buttonsLayout->addWidget(_closeButton);
//	
//	QVBoxLayout* main = new QVBoxLayout;
//	main->addLayout(labelLayout);
//	main->addLayout(buttonsLayout);
//	
//	setLayout(main);
//	
//	setWindowTitle(tr("ANIMA Error"));
//	setFixedHeight(sizeHint().height());
//}

void reportAssertionFailure(const char* expr, const char* fileName, int line)
{
	printf("ANIMA_ASSERT: %s", expr);
//	int argc = 0;
//
//	QApplication app(argc, NULL);
//	
//	QApplication::setStyle(QStyleFactory::create("Fusion"));
//	QPalette palette;
//	palette.setColor(QPalette::Window, QColor(53,53,53));
//	palette.setColor(QPalette::WindowText, Qt::white);
//	palette.setColor(QPalette::Base, QColor(15,15,15));
//	palette.setColor(QPalette::AlternateBase, QColor(53,53,53));
//	palette.setColor(QPalette::ToolTipBase, Qt::white);
//	palette.setColor(QPalette::ToolTipText, Qt::white);
//	palette.setColor(QPalette::Text, Qt::white);
//	palette.setColor(QPalette::Button, QColor(53,53,53));
//	palette.setColor(QPalette::ButtonText, Qt::white);
//	palette.setColor(QPalette::BrightText, Qt::red);
//	palette.setColor(QPalette::Highlight, QColor(142,45,197).lighter());
//	palette.setColor(QPalette::HighlightedText, Qt::black);
//	app.setPalette(palette);
//	
//	AnimaAssertWindow* window = new AnimaAssertWindow(expr, fileName, line);
//	window->show();
//	
//	app.exec();
}
