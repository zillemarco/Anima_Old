//
//  EditWindow.h
//  Anima
//
//  Created by Marco Zille on 03/12/14.
//
//

#ifndef __Anima__EditWindow__
#define __Anima__EditWindow__

#include <stdio.h>
#include <QTextEdit>

class EditWindow : public QTextEdit
{
	Q_OBJECT
	
public:
	EditWindow();
	
	void newFile();
	bool loadFile(const QString &fileName);
	bool save();
	bool saveAs();
	bool saveFile(const QString &fileName);
	QString userFriendlyCurrentFile();
	QString currentFile() { return curFile; }
	
protected:
	void closeEvent(QCloseEvent *event);
	
private slots:
	void documentWasModified();
	
private:
	bool maybeSave();
	void setCurrentFile(const QString &fileName);
	QString strippedName(const QString &fullFileName);
	
	QString curFile;
	bool isUntitled;
};

#endif /* defined(__Anima__EditWindow__) */
