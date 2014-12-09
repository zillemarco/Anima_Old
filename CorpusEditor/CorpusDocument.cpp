//
//  CorpusDocument.cpp
//  Anima
//
//  Created by Marco Zille on 04/12/14.
//
//

#include "CorpusDocument.h"
#include <QDir>
#include <QString>
#include <AnimaModelsManager.h>
#include <AnimaString.h>
#include <QMessageBox>

CorpusDocument::CorpusDocument()
{
	_projectName = QString("");
	_projectRootPath = QString("");
}

CorpusDocument::~CorpusDocument()
{
	
}

bool CorpusDocument::NewDocument(QString name, QString path)
{
	_projectName = name;
	_projectRootPath = path;
	
	if(!CreateProjectFolderStructure())
		return false;
	
	if(!CreateProjectFile())
		return false;
	
	if(!_engine.Initialize())
		return false;
	
	#if defined _MSC_VER
		Anima::AnimaString modelpath("D:/Modelli/Big_Dragon/Big_Dragon.fbx", &engine);
	#else
		Anima::AnimaString modelpath("/Users/marco/Documents/Modelli/ALDIUN/OBJ/alduin.obj", &_engine);
	#endif
	
	_engine.GetModelsManager()->LoadModel(modelpath);
	
	return true;
}

bool CorpusDocument::OpenDocument(QString path)
{
	bool bHasError = false;
	
	QFile file(path);
	
	if(!file.open(QIODevice::ReadOnly))
		return false;
	
	QXmlStreamReader* xmlReader = new QXmlStreamReader(&file);
	
	while(!bHasError && !xmlReader->atEnd() && !xmlReader->hasError())
	{
		QXmlStreamReader::TokenType token = xmlReader->readNext();
		
		if(token == QXmlStreamReader::StartDocument)
			continue;
		else if (token == QXmlStreamReader::StartElement)
		{
			if(xmlReader->name() == QString("CorpusProject"))
			{
				bHasError = !ReadProjectFile(xmlReader);
			}
		}
		else
			break;
	}
	
	delete xmlReader;
	xmlReader = nullptr;
	
	if(!bHasError)
	{
		if(!_engine.Initialize())
			return false;
		return true;
	}
	else
		return false;
}

void CorpusDocument::CloseDocument()
{
}

bool CorpusDocument::CreateProjectFolderStructure()
{
	QString root = _projectRootPath + QString("/") + _projectName;
		QString code = root + QString("/") + _projectName + QString("_Code");
			QString sorgenti = code + QString("/Source");
				QString src = sorgenti + QString("/src");
				QString include = sorgenti + QString("/include");
			QString depends = code + QString("/Dependencies");
				QString dependsWin = depends + QString("/Win");
					QString dependsWinDeb = dependsWin + QString("/Debug");
					QString dependsWinRel = dependsWin + QString("/Release");
					QString dependsWinRelDeb = dependsWin + QString("/RelWithDebInfo");
					QString dependsWinMinRel = dependsWin + QString("/MinSizeRel");
				QString dependsMac = depends + QString("/Mac");
					QString dependsMacDeb = dependsMac + QString("/Debug");
					QString dependsMacRel = dependsMac + QString("/Release");
					QString dependsMacRelDeb = dependsMac + QString("/RelWithDebInfo");
					QString dependsMacMinRel = dependsMac + QString("/MinSizeRel");
		QString build = root + QString("/") + _projectName + QString("_Build");
			QString buildWin = build + QString("/Win");
				QString buildWinDeb = buildWin + QString("/Debug");
				QString buildWinRel = buildWin + QString("/Release");
				QString buildWinRelDeb = buildWin + QString("/RelWithDebInfo");
				QString buildWinMinRel = buildWin + QString("/MinSizeRel");
			QString buildMac = build + QString("/Mac");
				QString buildMacDeb = buildMac + QString("/Debug");
				QString buildMacRel = buildMac + QString("/Release");
				QString buildMacRelDeb = buildMac + QString("/RelWithDebInfo");
				QString buildMacMinRel = buildMac + QString("/MinSizeRel");
			QString data = root + QString("/") + _projectName + QString("_Data");
				QString dataModels = data + QString("/Models");
				QString dataTextures = data + QString("/Textures");
	
	if(!QDir().mkdir(root))
		return false;
	if(!QDir().mkdir(code))
		return false;
	if(!QDir().mkdir(sorgenti))
		return false;
	if(!QDir().mkdir(src))
		return false;
	if(!QDir().mkdir(include))
		return false;
	if(!QDir().mkdir(depends))
		return false;
	if(!QDir().mkdir(dependsWin))
		return false;
	if(!QDir().mkdir(dependsWinDeb))
		return false;
	if(!QDir().mkdir(dependsWinRel))
		return false;
	if(!QDir().mkdir(dependsWinRelDeb))
		return false;
	if(!QDir().mkdir(dependsWinMinRel))
		return false;
	if(!QDir().mkdir(dependsMac))
		return false;
	if(!QDir().mkdir(dependsMacDeb))
		return false;
	if(!QDir().mkdir(dependsMacRel))
		return false;
	if(!QDir().mkdir(dependsMacRelDeb))
		return false;
	if(!QDir().mkdir(dependsMacMinRel))
		return false;
	if(!QDir().mkdir(build))
		return false;
	if(!QDir().mkdir(buildWin))
		return false;
	if(!QDir().mkdir(buildWinDeb))
		return false;
	if(!QDir().mkdir(buildWinRel))
		return false;
	if(!QDir().mkdir(buildWinRelDeb))
		return false;
	if(!QDir().mkdir(buildMac))
		return false;
	if(!QDir().mkdir(buildMacDeb))
		return false;
	if(!QDir().mkdir(buildMacRel))
		return false;
	if(!QDir().mkdir(buildMacRelDeb))
		return false;
	if(!QDir().mkdir(buildMacMinRel))
		return false;
	if(!QDir().mkdir(data))
		return false;
	if(!QDir().mkdir(dataModels))
		return false;
	if(!QDir().mkdir(dataTextures))
		return false;
	
	return true;
}

bool CorpusDocument::CreateProjectFile()
{
	QFile file(projectFilePath());
	
	if(!file.open(QIODevice::WriteOnly))
		return false;
	
	QXmlStreamWriter* xmlWriter = new QXmlStreamWriter();
	
	xmlWriter->setDevice(&file);
	
	xmlWriter->writeStartDocument();
	xmlWriter->writeStartElement("CorpusProject");
	
	xmlWriter->writeStartElement("Project");
	xmlWriter->writeTextElement("RootPath", _projectRootPath);
	xmlWriter->writeTextElement("Name", _projectName);
	xmlWriter->writeEndElement();
	
	xmlWriter->writeEndElement();
	xmlWriter->writeEndDocument();
	
	delete xmlWriter;
	
	return true;
}

bool CorpusDocument::ReadProjectFile(QXmlStreamReader* xmlReader)
{
	int nodeRead = 0;
	int maxNodesToRead = 2;
	
	if(xmlReader->atEnd() || xmlReader->hasError())
		return false;
	
	QXmlStreamReader::TokenType token = xmlReader->readNext();
	
	if (!xmlReader->atEnd() && !xmlReader->hasError() && token == QXmlStreamReader::StartElement && xmlReader->name() == QString("Project"))
	{
		while(nodeRead < maxNodesToRead && !xmlReader->atEnd() && !xmlReader->hasError())
		{
			token = xmlReader->readNext();
			
			if(token == QXmlStreamReader::EndElement)
				continue;
			
			if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("RootPath"))
			{
				token = xmlReader->readNext();
				
				if(token != QXmlStreamReader::Characters)
					return false;
				
				_projectRootPath = xmlReader->text().toString();
				
				nodeRead++;
			}
			else if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("Name"))
			{
				token = xmlReader->readNext();
				
				if(token != QXmlStreamReader::Characters)
					return false;
				
				_projectName = xmlReader->text().toString();
				
				nodeRead++;
			}
			else
				return false;
		}
	}
	else
		return false;
	
	if(_projectRootPath.isEmpty() || _projectName.isEmpty())
		return false;
	
	return true;
}








