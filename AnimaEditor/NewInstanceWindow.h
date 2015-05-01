//
//  NewInstanceWindow.h
//  Anima
//
//  Created by Marco Zille on 04/12/14.
//
//

#ifndef __Anima__NewInstanceWindow__
#define __Anima__NewInstanceWindow__

#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;
class QDialogButtonBox;

class NewInstanceWindow : public QDialog
{
	Q_OBJECT
public:
	explicit NewInstanceWindow(QWidget* parent = nullptr, QString instanceName = tr(""));
	~NewInstanceWindow();
	
private slots:
	void createClicked();
	void cancelClicked();
	void instanceNameChanged(const QString& text);
	
public:
	QString getInstanceName() { return _instanceName; }
	
private:
	QString _instanceName;
	
	QLabel*			_instanceNameLabel;
	QLineEdit*		_instanceNameEdit;
	QPushButton*	_createButton;
	QPushButton*	_cancelButton;
	
	QDialogButtonBox* _buttonBox;
};

#endif /* defined(__Anima__NewInstanceWindow__) */
