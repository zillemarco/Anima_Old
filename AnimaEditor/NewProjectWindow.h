//
//  NewProjectWindow.h
//  Anima
//
//  Created by Marco Zille on 04/12/14.
//
//

#ifndef __Anima__NewProjectWindow__
#define __Anima__NewProjectWindow__

#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;
class QDialogButtonBox;

class NewProjectWindow : public QDialog
{
	Q_OBJECT
public:
	explicit NewProjectWindow(QWidget* parent = nullptr);
	~NewProjectWindow();
	
private slots:
	void browseClicked();
	void createClicked();
	void cancelClicked();
	void projectNameChanged(const QString& text);
	void projectFolderPathChanged(const QString& text);
	
public:
	QString getProjectName() { return _projectName; }
	QString getProjectFolderPath() { return _projectFolderPath; }
	
private:
	QString _projectName;
	QString _projectFolderPath;
	
	QLabel*			_projectNameLabel;
	QLabel*			_projectFolderPathLabel;
	QLineEdit*		_projectNameEdit;
	QLineEdit*		_projectFolderPathEdit;
	QPushButton*	_browseButton;
	QPushButton*	_createButton;
	QPushButton*	_cancelButton;
	
	QDialogButtonBox* _buttonBox;
};

#endif /* defined(__Anima__NewProjectWindow__) */
