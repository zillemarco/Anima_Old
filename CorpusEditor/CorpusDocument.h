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
	bool SaveDocument();
	void CloseDocument();
	
	QString projectFilePath() {
		return _projectPath + QString("/") + _projectName + QString("/") + _projectName + QString(".ceproj");
	}
	
	Anima::AnimaEngine* GetEngine() {
		return _engine;
	}
	
	QString projectName() {
		return _projectName;
	}
	
	QString projectRootPath() {
		return _projectPath;
	}
	
	bool HasModifications() {
		return _hasModifications;
	}
	
private:
	bool SaveProjectFile();
	bool CreateProjectFolderStructure();
	bool ReadProjectFile(QXmlStreamReader* xmlReader);
	
	void SaveModels(QXmlStreamWriter* xmlWriter);
	bool ReadModels(QXmlStreamReader* xmlReader);
	
	bool ImportModelInternal(QString modelName, QString modelFileName);
	
public:
	bool ImportModel();
	bool ImportTexture();
	bool AddMaterial();
	
private:
	QString _projectName;
	QString _projectPath;
	QString _projectRootPath;
	QString _projectCodePath;
	QString _projectSourcesPath;
	QString _projectSrcPath;
	QString _projectIncludePath;
	QString _projectDependsPath;
	QString _projectDependsWinPath;
	QString _projectDependsWinDebPath;
	QString _projectDependsWinRelPath;
	QString _projectDependsWinRelDebPath;
	QString _projectDependsWinMinRelPath;
	QString _projectDependsMacPath;
	QString _projectDependsMacDebPath;
	QString _projectDependsMacRelPath;
	QString _projectDependsMacRelDebPath;
	QString _projectDependsMacMinRelPath;
	QString _projectBuildPath;
	QString _projectBuildWinPath;
	QString _projectBuildWinDebPath;
	QString _projectBuildWinRelPath;
	QString _projectBuildWinRelDebPath;
	QString _projectBuildWinMinRelPath;
	QString _projectBuildMacPath;
	QString _projectBuildMacDebPath;
	QString _projectBuildMacRelPath;
	QString _projectBuildMacRelDebPath;
	QString _projectBuildMacMinRelPath;
	QString _projectDataPath;
	QString _projectDataModelsPath;
	QString _projectDataTexturesPath;
	
	bool _hasModifications;
	
	Anima::AnimaEngine* _engine;
};

#endif /* defined(__Anima__CorpusDocument__) */
