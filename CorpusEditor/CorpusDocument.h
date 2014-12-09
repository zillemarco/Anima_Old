//
//  CorpusDocument.h
//  Anima
//
//  Created by Marco Zille on 04/12/14.
//
//

#ifndef __Anima__CorpusDocument__
#define __Anima__CorpusDocument__

#include <QString>
#include <AnimaEngine.h>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

class CorpusDocument
{
public:
	CorpusDocument();
	~CorpusDocument();
	
	bool NewDocument(QString name, QString path);
	bool OpenDocument(QString path);
	void CloseDocument();
	
	QString projectFilePath() {
		return _projectRootPath + QString("/") + _projectName + QString("/") + _projectName + QString(".ceproj");
	}
	
	Anima::AnimaEngine* GetEngine() {
		return &_engine;
	}
	
	QString projectName() {
		return _projectName;
	}
	
	QString projectRootPath() {
		return _projectRootPath;
	}
	
private:
	bool CreateProjectFile();
	bool CreateProjectFolderStructure();
	bool ReadProjectFile(QXmlStreamReader* xmlReader);
	
private:
	QString _projectName;
	QString _projectRootPath;
	
	Anima::AnimaEngine _engine;
};

#endif /* defined(__Anima__CorpusDocument__) */
