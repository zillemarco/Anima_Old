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
#include <QFileDialog>
#include <QDebug>
#include <QFile>
#include <QFileInfo>

CorpusDocument::CorpusDocument()
{
	_projectName = QString("");
	_projectPath = QString("");
	_projectRootPath = QString("");
	_projectCodePath = QString("");
	_projectSourcesPath = QString("");
	_projectSrcPath = QString("");
	_projectIncludePath = QString("");
	_projectDependsPath = QString("");
	_projectDependsWinPath = QString("");
	_projectDependsWinDebPath = QString("");
	_projectDependsWinRelPath = QString("");
	_projectDependsWinRelDebPath = QString("");
	_projectDependsWinMinRelPath = QString("");
	_projectDependsMacPath = QString("");
	_projectDependsMacDebPath = QString("");
	_projectDependsMacRelPath = QString("");
	_projectDependsMacRelDebPath = QString("");
	_projectDependsMacMinRelPath = QString("");
	_projectBuildPath = QString("");
	_projectBuildWinPath = QString("");
	_projectBuildWinDebPath = QString("");
	_projectBuildWinRelPath = QString("");
	_projectBuildWinRelDebPath = QString("");
	_projectBuildWinMinRelPath = QString("");
	_projectBuildMacPath = QString("");
	_projectBuildMacDebPath = QString("");
	_projectBuildMacRelPath = QString("");
	_projectBuildMacRelDebPath = QString("");
	_projectBuildMacMinRelPath = QString("");
	_projectDataPath = QString("");
	_projectDataModelsPath = QString("");
	_projectDataTexturesPath = QString("");
	
	_engine = nullptr;
	
	_hasModifications = false;
}

CorpusDocument::~CorpusDocument()
{
	delete _engine;
}

bool CorpusDocument::NewDocument(QString name, QString path)
{
	_projectName = name;
	_projectPath = path;
	
	if(!CreateProjectFolderStructure())
		return false;
	
	if(_engine != nullptr)
	{
		delete _engine;
		_engine = nullptr;
	}
	
	_engine = new Anima::AnimaEngine();
	if(!_engine->Initialize())
		return false;
	
	_hasModifications = true;
	
	return true;
}

bool CorpusDocument::OpenDocument(QString path)
{
	if(_engine != nullptr)
	{
		delete _engine;
		_engine = nullptr;
	}
	
	_engine = new Anima::AnimaEngine();
	
	if(!_engine->Initialize())
		return false;
	
	_hasModifications = false;
	
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
	
	if(bHasError)
	{
		delete _engine;
		_engine = nullptr;
		
		return false;
	}
	return true;
}

bool CorpusDocument::SaveDocument()
{
	return SaveProjectFile();
}

void CorpusDocument::CloseDocument()
{
}

bool CorpusDocument::CreateProjectFolderStructure()
{
	_projectRootPath				= _projectPath + QString("/") + _projectName;
	_projectCodePath				= _projectRootPath + QString("/") + _projectName + QString("_Code");
	_projectSourcesPath				= _projectCodePath + QString("/Source");
	_projectSrcPath					= _projectSourcesPath + QString("/src");
	_projectIncludePath				= _projectSourcesPath + QString("/include");
	_projectDependsPath				= _projectCodePath + QString("/Dependencies");
	_projectDependsWinPath			= _projectDependsPath + QString("/Win");
	_projectDependsWinDebPath		= _projectDependsWinPath + QString("/Debug");
	_projectDependsWinRelPath		= _projectDependsWinPath + QString("/Release");
	_projectDependsWinRelDebPath	= _projectDependsWinPath + QString("/RelWithDebInfo");
	_projectDependsWinMinRelPath	= _projectDependsWinPath + QString("/MinSizeRel");
	_projectDependsMacPath			= _projectDependsPath + QString("/Mac");
	_projectDependsMacDebPath		= _projectDependsMacPath + QString("/Debug");
	_projectDependsMacRelPath		= _projectDependsMacPath + QString("/Release");
	_projectDependsMacRelDebPath	= _projectDependsMacPath + QString("/RelWithDebInfo");
	_projectDependsMacMinRelPath	= _projectDependsMacPath + QString("/MinSizeRel");
	_projectBuildPath				= _projectRootPath + QString("/") + _projectName + QString("_Build");
	_projectBuildWinPath			= _projectBuildPath + QString("/Win");
	_projectBuildWinDebPath			= _projectBuildWinPath + QString("/Debug");
	_projectBuildWinRelPath			= _projectBuildWinPath + QString("/Release");
	_projectBuildWinRelDebPath		= _projectBuildWinPath + QString("/RelWithDebInfo");
	_projectBuildWinMinRelPath		= _projectBuildWinPath + QString("/MinSizeRel");
	_projectBuildMacPath			= _projectBuildPath + QString("/Mac");
	_projectBuildMacDebPath			= _projectBuildMacPath + QString("/Debug");
	_projectBuildMacRelPath			= _projectBuildMacPath + QString("/Release");
	_projectBuildMacRelDebPath		= _projectBuildMacPath + QString("/RelWithDebInfo");
	_projectBuildMacMinRelPath		= _projectBuildMacPath + QString("/MinSizeRel");
	_projectDataPath				= _projectRootPath + QString("/") + _projectName + QString("_Data");
	_projectDataModelsPath			= _projectDataPath + QString("/Models");
	_projectDataTexturesPath		= _projectDataPath + QString("/Textures");
	
	if(!QDir().mkdir(_projectRootPath))
		return false;
	if(!QDir().mkdir(_projectCodePath))
		return false;
	if(!QDir().mkdir(_projectSourcesPath))
		return false;
	if(!QDir().mkdir(_projectSrcPath))
		return false;
	if(!QDir().mkdir(_projectIncludePath))
		return false;
	if(!QDir().mkdir(_projectDependsPath))
		return false;
	if(!QDir().mkdir(_projectDependsWinPath))
		return false;
	if(!QDir().mkdir(_projectDependsWinDebPath))
		return false;
	if(!QDir().mkdir(_projectDependsWinRelPath))
		return false;
	if(!QDir().mkdir(_projectDependsWinRelDebPath))
		return false;
	if(!QDir().mkdir(_projectDependsWinMinRelPath))
		return false;
	if(!QDir().mkdir(_projectDependsMacPath))
		return false;
	if(!QDir().mkdir(_projectDependsMacDebPath))
		return false;
	if(!QDir().mkdir(_projectDependsMacRelPath))
		return false;
	if(!QDir().mkdir(_projectDependsMacRelDebPath))
		return false;
	if(!QDir().mkdir(_projectDependsMacMinRelPath))
		return false;
	if(!QDir().mkdir(_projectBuildPath))
		return false;
	if(!QDir().mkdir(_projectBuildWinPath))
		return false;
	if(!QDir().mkdir(_projectBuildWinDebPath))
		return false;
	if(!QDir().mkdir(_projectBuildWinRelPath))
		return false;
	if(!QDir().mkdir(_projectBuildWinRelDebPath))
		return false;
	if(!QDir().mkdir(_projectBuildWinMinRelPath))
		return false;
	if(!QDir().mkdir(_projectBuildMacPath))
		return false;
	if(!QDir().mkdir(_projectBuildMacDebPath))
		return false;
	if(!QDir().mkdir(_projectBuildMacRelPath))
		return false;
	if(!QDir().mkdir(_projectBuildMacRelDebPath))
		return false;
	if(!QDir().mkdir(_projectBuildMacMinRelPath))
		return false;
	if(!QDir().mkdir(_projectDataPath))
		return false;
	if(!QDir().mkdir(_projectDataModelsPath))
		return false;
	if(!QDir().mkdir(_projectDataTexturesPath))
		return false;
	
	return true;
}

bool CorpusDocument::SaveProjectFile()
{
	QFile file(projectFilePath());
	
	if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return false;
	
	QXmlStreamWriter* xmlWriter = new QXmlStreamWriter();

	xmlWriter->setDevice(&file);
	
	xmlWriter->writeStartDocument();
	xmlWriter->writeStartElement("CorpusProject");
	
	xmlWriter->writeStartElement("Project");
	xmlWriter->writeTextElement("Path", _projectPath);
	xmlWriter->writeTextElement("Name", _projectName);
	xmlWriter->writeTextElement("RootPath", _projectRootPath);
	xmlWriter->writeTextElement("CodePath", _projectCodePath);
	xmlWriter->writeTextElement("CodeSourcesPath", _projectSourcesPath);
	xmlWriter->writeTextElement("CodeSourcesSrcPath", _projectSrcPath);
	xmlWriter->writeTextElement("CodeSourcesIncludePath", _projectIncludePath);
	xmlWriter->writeTextElement("DependsPath", _projectDependsPath);
	xmlWriter->writeTextElement("DependsWinPath", _projectDependsWinPath);
	xmlWriter->writeTextElement("DependsWinDebPath", _projectDependsWinDebPath);
	xmlWriter->writeTextElement("DependsWinRelPath", _projectDependsWinRelPath);
	xmlWriter->writeTextElement("DependsWinRelDebPath", _projectDependsWinRelDebPath);
	xmlWriter->writeTextElement("DependsWinMinRelPath", _projectDependsWinMinRelPath);
	xmlWriter->writeTextElement("DependsMacPath", _projectDependsMacPath);
	xmlWriter->writeTextElement("DependsMacDebPath", _projectDependsMacDebPath);
	xmlWriter->writeTextElement("DependsMacRelPath", _projectDependsMacRelPath);
	xmlWriter->writeTextElement("DependsMacRelDebPath", _projectDependsMacRelDebPath);
	xmlWriter->writeTextElement("DependsMacMinRelPath", _projectDependsMacMinRelPath);
	xmlWriter->writeTextElement("BuildPath", _projectBuildPath);
	xmlWriter->writeTextElement("BuildWinPath", _projectBuildWinPath);
	xmlWriter->writeTextElement("BuildWinDebPath", _projectBuildWinDebPath);
	xmlWriter->writeTextElement("BuildWinRelPath", _projectBuildWinRelPath);
	xmlWriter->writeTextElement("BuildWinRelDebPath", _projectBuildWinRelDebPath);
	xmlWriter->writeTextElement("BuildWinMinRelPath", _projectBuildWinMinRelPath);
	xmlWriter->writeTextElement("BuildMacPath", _projectBuildMacPath);
	xmlWriter->writeTextElement("BuildMacDebPath", _projectBuildMacDebPath);
	xmlWriter->writeTextElement("BuildMacRelPath", _projectBuildMacRelPath);
	xmlWriter->writeTextElement("BuildMacRelDebPath", _projectBuildMacRelDebPath);
	xmlWriter->writeTextElement("BuildMacMinRelPath", _projectBuildMacMinRelPath);
	xmlWriter->writeTextElement("DataPath", _projectDataPath);
	xmlWriter->writeTextElement("DataModelsPath", _projectDataModelsPath);
	xmlWriter->writeTextElement("DataTexturesPath", _projectDataTexturesPath);
	xmlWriter->writeEndElement();
	
	SaveModels(xmlWriter);
	
	xmlWriter->writeEndElement();
	xmlWriter->writeEndDocument();
	
	delete xmlWriter;
	
	return true;
}

bool CorpusDocument::ReadProjectFile(QXmlStreamReader* xmlReader)
{
	int nodeRead = 0;
	int maxNodesToRead = 32;
	
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
			
			if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("Path"))
			{
				token = xmlReader->readNext();
				if(token != QXmlStreamReader::Characters)
					return false;
				_projectPath = xmlReader->text().toString();
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
			else if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("RootPath"))
			{
				token = xmlReader->readNext();
				if(token != QXmlStreamReader::Characters)
					return false;
				_projectRootPath = xmlReader->text().toString();
				nodeRead++;
			}
			else if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("CodePath"))
			{
				token = xmlReader->readNext();
				if(token != QXmlStreamReader::Characters)
					return false;
				_projectCodePath = xmlReader->text().toString();
				nodeRead++;
			}
			else if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("CodeSourcesPath"))
			{
				token = xmlReader->readNext();
				if(token != QXmlStreamReader::Characters)
					return false;
				_projectSourcesPath = xmlReader->text().toString();
				nodeRead++;
			}
			else if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("CodeSourcesSrcPath"))
			{
				token = xmlReader->readNext();
				if(token != QXmlStreamReader::Characters)
					return false;
				_projectSrcPath = xmlReader->text().toString();
				nodeRead++;
			}
			else if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("CodeSourcesIncludePath"))
			{
				token = xmlReader->readNext();
				if(token != QXmlStreamReader::Characters)
					return false;
				_projectIncludePath = xmlReader->text().toString();
				nodeRead++;
			}
			else if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("DependsPath"))
			{
				token = xmlReader->readNext();
				if(token != QXmlStreamReader::Characters)
					return false;
				_projectDependsPath = xmlReader->text().toString();
				nodeRead++;
			}
			else if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("DependsWinPath"))
			{
				token = xmlReader->readNext();
				if(token != QXmlStreamReader::Characters)
					return false;
				_projectDependsWinPath = xmlReader->text().toString();
				nodeRead++;
			}
			else if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("DependsWinDebPath"))
			{
				token = xmlReader->readNext();
				if(token != QXmlStreamReader::Characters)
					return false;
				_projectDependsWinDebPath = xmlReader->text().toString();
				nodeRead++;
			}
			else if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("DependsWinRelPath"))
			{
				token = xmlReader->readNext();
				if(token != QXmlStreamReader::Characters)
					return false;
				_projectDependsWinRelPath = xmlReader->text().toString();
				nodeRead++;
			}
			else if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("DependsWinRelDebPath"))
			{
				token = xmlReader->readNext();
				if(token != QXmlStreamReader::Characters)
					return false;
				_projectDependsWinRelDebPath = xmlReader->text().toString();
				nodeRead++;
			}
			else if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("DependsWinMinRelPath"))
			{
				token = xmlReader->readNext();
				if(token != QXmlStreamReader::Characters)
					return false;
				_projectDependsWinMinRelPath = xmlReader->text().toString();
				nodeRead++;
			}
			else if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("DependsMacPath"))
			{
				token = xmlReader->readNext();
				if(token != QXmlStreamReader::Characters)
					return false;
				_projectDependsMacPath = xmlReader->text().toString();
				nodeRead++;
			}
			else if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("DependsMacDebPath"))
			{
				token = xmlReader->readNext();
				if(token != QXmlStreamReader::Characters)
					return false;
				_projectDependsMacDebPath = xmlReader->text().toString();
				nodeRead++;
			}
			else if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("DependsMacRelPath"))
			{
				token = xmlReader->readNext();
				if(token != QXmlStreamReader::Characters)
					return false;
				_projectDependsMacRelPath = xmlReader->text().toString();
				nodeRead++;
			}
			else if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("DependsMacRelDebPath"))
			{
				token = xmlReader->readNext();
				if(token != QXmlStreamReader::Characters)
					return false;
				_projectDependsMacRelDebPath = xmlReader->text().toString();
				nodeRead++;
			}
			else if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("DependsMacMinRelPath"))
			{
				token = xmlReader->readNext();
				if(token != QXmlStreamReader::Characters)
					return false;
				_projectDependsMacMinRelPath = xmlReader->text().toString();
				nodeRead++;
			}
			else if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("BuildPath"))
			{
				token = xmlReader->readNext();
				if(token != QXmlStreamReader::Characters)
					return false;
				_projectBuildPath = xmlReader->text().toString();
				nodeRead++;
			}
			else if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("BuildWinPath"))
			{
				token = xmlReader->readNext();
				if(token != QXmlStreamReader::Characters)
					return false;
				_projectBuildWinPath = xmlReader->text().toString();
				nodeRead++;
			}
			else if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("BuildWinDebPath"))
			{
				token = xmlReader->readNext();
				if(token != QXmlStreamReader::Characters)
					return false;
				_projectBuildWinDebPath = xmlReader->text().toString();
				nodeRead++;
			}
			else if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("BuildWinRelPath"))
			{
				token = xmlReader->readNext();
				if(token != QXmlStreamReader::Characters)
					return false;
				_projectBuildWinRelPath = xmlReader->text().toString();
				nodeRead++;
			}
			else if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("BuildWinRelDebPath"))
			{
				token = xmlReader->readNext();
				if(token != QXmlStreamReader::Characters)
					return false;
				_projectBuildWinRelDebPath = xmlReader->text().toString();
				nodeRead++;
			}
			else if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("BuildWinMinRelPath"))
			{
				token = xmlReader->readNext();
				if(token != QXmlStreamReader::Characters)
					return false;
				_projectBuildWinMinRelPath = xmlReader->text().toString();
				nodeRead++;
			}
			else if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("BuildMacPath"))
			{
				token = xmlReader->readNext();
				if(token != QXmlStreamReader::Characters)
					return false;
				_projectBuildMacPath = xmlReader->text().toString();
				nodeRead++;
			}
			else if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("BuildMacDebPath"))
			{
				token = xmlReader->readNext();
				if(token != QXmlStreamReader::Characters)
					return false;
				_projectBuildMacDebPath = xmlReader->text().toString();
				nodeRead++;
			}
			else if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("BuildMacRelPath"))
			{
				token = xmlReader->readNext();
				if(token != QXmlStreamReader::Characters)
					return false;
				_projectBuildMacRelPath = xmlReader->text().toString();
				nodeRead++;
			}
			else if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("BuildMacRelDebPath"))
			{
				token = xmlReader->readNext();
				if(token != QXmlStreamReader::Characters)
					return false;
				_projectBuildMacRelDebPath = xmlReader->text().toString();
				nodeRead++;
			}
			else if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("BuildMacMinRelPath"))
			{
				token = xmlReader->readNext();
				if(token != QXmlStreamReader::Characters)
					return false;
				_projectBuildMacMinRelPath = xmlReader->text().toString();
				nodeRead++;
			}
			else if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("DataPath"))
			{
				token = xmlReader->readNext();
				if(token != QXmlStreamReader::Characters)
					return false;
				_projectDataPath = xmlReader->text().toString();
				nodeRead++;
			}
			else if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("DataModelsPath"))
			{
				token = xmlReader->readNext();
				if(token != QXmlStreamReader::Characters)
					return false;
				_projectDataModelsPath = xmlReader->text().toString();
				nodeRead++;
			}
			else if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("DataTexturesPath"))
			{
				token = xmlReader->readNext();
				if(token != QXmlStreamReader::Characters)
					return false;
				_projectDataTexturesPath = xmlReader->text().toString();
				nodeRead++;
			}
			else
			{
				qDebug() << xmlReader->name();
				return false;
			}
		}
	}
	else
		return false;
	
	if(_projectPath.isEmpty()					||
	   _projectName.isEmpty()					||
	   _projectRootPath.isEmpty()				||
	   _projectCodePath.isEmpty()				||
	   _projectSourcesPath.isEmpty()			||
	   _projectSrcPath.isEmpty()				||
	   _projectIncludePath.isEmpty()			||
	   _projectDependsPath.isEmpty()			||
	   _projectDependsWinPath.isEmpty()			||
	   _projectDependsWinDebPath.isEmpty()		||
	   _projectDependsWinRelPath.isEmpty()		||
	   _projectDependsWinRelDebPath.isEmpty()	||
	   _projectDependsWinMinRelPath.isEmpty()	||
	   _projectDependsMacPath.isEmpty()			||
	   _projectDependsMacDebPath.isEmpty()		||
	   _projectDependsMacRelPath.isEmpty()		||
	   _projectDependsMacRelDebPath.isEmpty()	||
	   _projectDependsMacMinRelPath.isEmpty()	||
	   _projectBuildPath.isEmpty()				||
	   _projectBuildWinPath.isEmpty()			||
	   _projectBuildWinDebPath.isEmpty()		||
	   _projectBuildWinRelPath.isEmpty()		||
	   _projectBuildWinRelDebPath.isEmpty()		||
	   _projectBuildWinMinRelPath.isEmpty()		||
	   _projectBuildMacPath.isEmpty()			||
	   _projectBuildMacDebPath.isEmpty()		||
	   _projectBuildMacRelPath.isEmpty()		||
	   _projectBuildMacRelDebPath.isEmpty()		||
	   _projectBuildMacMinRelPath.isEmpty()		||
	   _projectDataPath.isEmpty()				||
	   _projectDataModelsPath.isEmpty()			||
	   _projectDataTexturesPath.isEmpty())
		return false;
	
	return ReadModels(xmlReader);
}

bool CorpusDocument::ImportModel()
{
	QString selfilter = QString("3ds Max 3DS (*.3ds)");
	QString filters = QString("3ds Max 3DS (*.3ds);;3ds Max ASE (*.ase);;Collada (*.dae);;Wavefront Object (*.obj);;Blender 3D (*.blend);;Industry Foundation Classes [IFC/Step] (*.ifc);;XGL (*.xgl);; XGL (*.zgl);;Stanford Polygon Library (*.ply);;All files (*.*)");
	
	QString originalFilePath = QFileDialog::getOpenFileName(NULL, QString("CorpusEditor - Import model"), QString(""), filters, &selfilter);
	
	if(originalFilePath.isEmpty())
		return false;
	
	QFileInfo fileInfo(originalFilePath);
	QString copiedFilePath = _projectDataModelsPath + "/" + fileInfo.fileName();
	
	if(!QFile::copy(originalFilePath, copiedFilePath))
	{
		QMessageBox msg;
		msg.setWindowTitle(QString("CorpusEditor"));
		msg.setText(QString("Unable to copy the model file into the project's models folder."));
		msg.exec();
		return false;
	}
	
	if(!_engine->GetModelsManager()->LoadModel(copiedFilePath.toLocal8Bit().constData()))
	{
		QMessageBox msg;
		msg.setWindowTitle(QString("CorpusEditor"));
		msg.setText(QString("Unable to the model file.\nMaybe the format isn't supported."));
		msg.exec();
		
		QFile::remove(copiedFilePath);
		return false;
	}
	
	_hasModifications = true;
	
	return true;
}

bool CorpusDocument::ImportTexture()
{
	return true;
}

bool CorpusDocument::AddMaterial()
{
	return true;
}

void CorpusDocument::SaveModels(QXmlStreamWriter* xmlWriter)
{
	Anima::AnimaModelsManager* mgr = _engine->GetModelsManager();
	Anima::ASizeT numeroModelli = mgr->GetModelsNumber();
	
	xmlWriter->writeStartElement("Models");
	xmlWriter->writeAttribute("N", QString("%0").arg(numeroModelli));
	
	for(Anima::ASizeT i = 0; i < numeroModelli; i++)
	{
		xmlWriter->writeStartElement("Model");
		
		xmlWriter->writeTextElement("Name", mgr->GetPModel(i)->GetModelName());
		xmlWriter->writeTextElement("FileName", mgr->GetPModel(i)->GetModelFileName());
		
		xmlWriter->writeEndElement();
	}
	
	xmlWriter->writeEndElement();
}

bool CorpusDocument::ReadModels(QXmlStreamReader* xmlReader)
{
	if(xmlReader->atEnd() || xmlReader->hasError())
		return false;
	
	bool foundModelsNode = false;
	
	QXmlStreamReader::TokenType token = xmlReader->readNext();
	
	while (!xmlReader->atEnd() && !xmlReader->hasError() && !foundModelsNode)
	{
		if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("Models"))
			foundModelsNode = true;
		else
			token = xmlReader->readNext();
	}
	
	if(!foundModelsNode)
		return false;
	
	QXmlStreamAttributes attributes = xmlReader->attributes();
	
	if(!attributes.hasAttribute(QString("N")))
	   return false;
	
	int savedModelsNumber = attributes.value(QString("N")).toInt();
	
	QString name;
	QString fileName;
	
	while(!xmlReader->atEnd() && !xmlReader->hasError())
	{
		token = xmlReader->readNext();
		
		if(token == QXmlStreamReader::EndElement)
			continue;
		
		if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("Name"))
		{
			if(!name.isEmpty())
				return false;
			
			token = xmlReader->readNext();
			if(token != QXmlStreamReader::Characters)
				return false;
			
			name = xmlReader->text().toString();
			
			if(!fileName.isEmpty())
			{
				if(!ImportModelInternal(name, fileName))
					return false;
				
				name = "";
				fileName = "";
			}
		}
		else if(token == QXmlStreamReader::StartElement && xmlReader->name() == QString("FileName"))
		{
			if(!fileName.isEmpty())
				return false;
			
			token = xmlReader->readNext();
			if(token != QXmlStreamReader::Characters)
				return false;
			
			fileName = xmlReader->text().toString();
			
			if(!name.isEmpty())
			{
				if(!ImportModelInternal(name, fileName))
					return false;
				
				name = "";
				fileName = "";
			}
		}
	}
	
	if(_engine->GetModelsManager()->GetModelsNumber() != savedModelsNumber)
		return false;
	
	return true;
}

bool CorpusDocument::ImportModelInternal(QString modelName, QString modelFileName)
{
	QString modelFilePath = _projectDataModelsPath + "/" + modelFileName;
	
	Anima::AnimaModel* model = _engine->GetModelsManager()->LoadModel(modelFilePath.toLocal8Bit().constData());
	
	if(!model)
		return false;
	
	model->SetModelName(modelName.toLocal8Bit().constData());
	model->SetModelFileName(modelFileName.toLocal8Bit().constData());
	
	return true;
}




