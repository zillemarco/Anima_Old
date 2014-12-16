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

	void DeleteProject();

	QString projectFilePath() {
		return _projectPath + QString("/") + _projectName + QString("/") + _projectName + QString(".ceproj");
	}

	Anima::AnimaEngine* GetEngine() {
		return _engine;
	}
	
	bool HasModifications() {
		return _hasModifications;
	}

	void SetMofications() {
		_hasModifications = true;
	}

	bool IsNewDocument() {
		return _newDocument;
	}
	
	QString projectName() {
		return _projectName;
	}
	
	QString projectPath() {
		return _projectPath;
	}
	
	QString projectRootPath() {
		return _projectRootPath;
	}
	
	QString projectCodePath() {
		return _projectCodePath;
	}
	
	QString projectSourcesPath() {
		return _projectSourcesPath;
	}
	
	QString projectSrcPath() {
		return _projectSrcPath;
	}
	
	QString projectIncludePath() {
		return _projectIncludePath;
	}
	
	QString projectDependsPath() {
		return _projectDependsPath;
	}
	
	QString projectDependsWinPath() {
		return _projectDependsWinPath;
	}
	
	QString projectDependsWinDebPath() {
		return _projectDependsWinDebPath;
	}
	
	QString projectDependsWinRelPath() {
		return _projectDependsWinRelPath;
	}
	
	QString projectDependsWinRelDebPath() {
		return _projectDependsWinRelDebPath;
	}
	
	QString projectDependsWinMinRelPath() {
		return _projectDependsWinMinRelPath;
	}
	
	QString projectDependsMacPath() {
		return _projectDependsMacPath;
	}
	
	QString projectDependsMacDebPath() {
		return _projectDependsMacDebPath;
	}
	
	QString projectDependsMacRelPath() {
		return _projectDependsMacRelPath;
	}
	
	QString projectDependsMacRelDebPath() {
		return _projectDependsMacRelDebPath;
	}
	
	QString projectDependsMacMinRelPath() {
		return _projectDependsMacMinRelPath;
	}
	
	QString projectBuildPath() {
		return _projectBuildPath;
	}
	
	QString projectBuildWinPath() {
		return _projectBuildWinPath;
	}
	
	QString projectBuildWinDebPath() {
		return _projectBuildWinDebPath;
	}
	
	QString projectBuildWinRelPath() {
		return _projectBuildWinRelPath;
	}
	
	QString projectBuildWinRelDebPath() {
		return _projectBuildWinRelDebPath;
	}
	
	QString projectBuildWinMinRelPath() {
		return _projectBuildWinMinRelPath;
	}
	
	QString projectBuildMacPath() {
		return _projectBuildMacPath;
	}
	
	QString projectBuildMacDebPath() {
		return _projectBuildMacDebPath;
	}
	
	QString projectBuildMacRelPath() {
		return _projectBuildMacRelPath;
	}
	
	QString projectBuildMacRelDebPath() {
		return _projectBuildMacRelDebPath;
	}
	
	QString projectBuildMacMinRelPath() {
		return _projectBuildMacMinRelPath;
	}
	
	QString projectDataPath() {
		return _projectDataPath;
	}
	
	QString projectDataModelsPath() {
		return _projectDataModelsPath;
	}
	
	QString projectDataTexturesPath() {
		return _projectDataTexturesPath;
	}
	
private:
	bool CreateProjectFolderStructure();
	
	void SaveProjectData(QXmlStreamWriter* xmlWriter);
	bool ReadProjectData(QXmlStreamReader* xmlReader);
	
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
	bool _newDocument;
	
	Anima::AnimaEngine* _engine;
};

#endif /* defined(__Anima__CorpusDocument__) */
