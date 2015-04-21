//
//  ImportModelWindow.h
//  Anima
//
//  Created by Marco Zille on 04/12/14.
//
//

#ifndef __Anima__ImportModelWindow__
#define __Anima__ImportModelWindow__

#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;
class QDialogButtonBox;

class ImportModelWindow : public QDialog
{
	Q_OBJECT
public:
	explicit ImportModelWindow(QWidget* parent = nullptr, QString modelName = tr(""), QString modelPath = tr(""));
	~ImportModelWindow();
	
private slots:
	void browseClicked();
	void createClicked();
	void cancelClicked();
	void modelNameChanged(const QString& text);
	void modelPathChanged(const QString& text);
	
public:
	QString getModelName() { return _modelName; }
	QString getModelPath() { return _modelPath; }
	
private:
	QString _modelName;
	QString _modelPath;
	
	QLabel*			_modelNameLabel;
	QLabel*			_modelPathLabel;
	QLineEdit*		_modelNameEdit;
	QLineEdit*		_modelPathEdit;
	QPushButton*	_browseButton;
	QPushButton*	_importButton;
	QPushButton*	_cancelButton;
	
	QDialogButtonBox* _buttonBox;
};

#endif /* defined(__Anima__ImportModelWindow__) */
