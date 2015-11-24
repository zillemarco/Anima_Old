#include "AnimaShadersManager.h"
#include "AnimaGeometry.h"
#include "AnimaGeometryInstance.h"
#include "AnimaMaterial.h"
#include "AnimaMD5.h"
#include "AnimaShaderData.h"
#include "AnimaXmlTranslators.h"

#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaShadersManager::AnimaShadersManager(AnimaEngine* engine)
{
	_engine = engine;
	_activeProgram = nullptr;

	_defaultVertexShader = nullptr;
	_defaultFragmentShader = nullptr;
}

AnimaShadersManager::~AnimaShadersManager()
{
	ClearShaders();
	ClearPrograms();
	ClearIncludes();
}

AnimaShader* AnimaShadersManager::CreateShader(const AnimaString& name)
{
	AInt index = _shaders.Contains(name);
	if(index >= 0)
		return nullptr;
	
	AnimaShader* shader = AnimaAllocatorNamespace::AllocateNew<AnimaShader>(*(_engine->GetShadersAllocator()), name, _engine->GetShadersAllocator());
	_shaders.Add(name, shader);
	return shader;
}

AnimaShaderProgram* AnimaShadersManager::CreateProgram(const AnimaString& name)
{
	AInt index = _programs.Contains(name);
	if(index >= 0)
		return nullptr;
	
	AnimaShaderProgram* program = AnimaAllocatorNamespace::AllocateNew<AnimaShaderProgram>(*(_engine->GetShadersAllocator()), name, _engine->GetShadersAllocator(), this);
	_programs.Add(name, program);
	return program;
}

AnimaShaderInclude* AnimaShadersManager::CreateShaderInclude(const AnimaString& name)
{
	AInt index = _includes.Contains(name);
	if (index >= 0)
		return nullptr;

	AnimaShaderInclude* include = AnimaAllocatorNamespace::AllocateNew<AnimaShaderInclude>(*(_engine->GetShadersAllocator()), name, _engine->GetShadersAllocator());
	_includes.Add(name, include);
	return include;
}

AnimaShader* AnimaShadersManager::LoadShader(const AnimaString& name, const AnimaString& text, AnimaShaderType type)
{
	AnimaShader* sh = CreateShader(name);
	sh->SetText(text);
	sh->SetType(type);
	return sh;
}

AnimaShader* AnimaShadersManager::LoadShaderFromFile(const AnimaString& name, const AnimaString &filePath, AnimaShaderType type)
{
	AnimaShader* sh = nullptr;
	AnimaString str;
	std::ifstream is(filePath, std::ifstream::binary);
	if (is)
	{
		is.seekg(0, is.end);
		int length = (int)is.tellg();
		is.seekg(0, is.beg);

		str.reserve(length);
		str.assign((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());

		is.close();
	}
	else
		return nullptr;

	sh = LoadShader(name, str, type);
	return sh;
}

void AnimaShadersManager::ClearShaders()
{
	AInt count = _shaders.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaShader* shader = _shaders[i];
		AnimaAllocatorNamespace::DeallocateObject(*(_engine->GetShadersAllocator()), shader);
		shader = nullptr;
	}
	
	_shaders.RemoveAll();
}

void AnimaShadersManager::ClearPrograms()
{
	AInt count = _programs.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaShaderProgram* program = _programs[i];
		AnimaAllocatorNamespace::DeallocateObject(*(_engine->GetShadersAllocator()), program);
		program = nullptr;
	}
	
	_programs.RemoveAll();
}

void AnimaShadersManager::ClearIncludes()
{
	AInt count = _includes.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaShaderInclude* include = _includes[i];
		AnimaAllocatorNamespace::DeallocateObject(*(_engine->GetShadersAllocator()), include);
		include = nullptr;
	}

	_includes.RemoveAll();
}

AnimaShaderProgram* AnimaShadersManager::GetProgram(AInt index)
{
	return _programs[index];
}

AnimaShaderProgram* AnimaShadersManager::GetProgramFromName(const AnimaString& name)
{
	return _programs.GetWithName(name);
}

AnimaShader* AnimaShadersManager::GetShader(AInt index)
{
	return _shaders[index];
}

AnimaShader* AnimaShadersManager::GetShaderFromName(const AnimaString& name)
{
	return _shaders.GetWithName(name);
}

void AnimaShadersManager::NotifyProgramActivation(AnimaShaderProgram* program)
{
	_activeProgram = program;
}

void AnimaShadersManager::NotifyProgramDeactivation(AnimaShaderProgram* program)
{
	_activeProgram = nullptr;
}

void AnimaShadersManager::SetActiveProgram(AnimaShaderProgram* program)
{
	_activeProgram = program;
}

void AnimaShadersManager::SetActiveProgramFromName(const AnimaString& name)
{
	_activeProgram = GetProgramFromName(name);
}

AnimaShaderProgram* AnimaShadersManager::GetActiveProgram()
{
	return _activeProgram;
}

AnimaShaderProgram* AnimaShadersManager::CreateProgram(AnimaGeometry* geometry, const AnimaMaterial* material)
{
	AInt geometryShadersCount = geometry != nullptr ? geometry->GetShadersCount() : 0;
	AInt materialShadersCount = material != nullptr ? material->GetShadersCount() : 0;

	if (geometryShadersCount <= 0 || materialShadersCount <= 0)
		return nullptr;

	AnimaString str;

	for (AInt ms = 0; ms < geometryShadersCount; ms++)
		str += geometry->GetShaderName(ms);

	for (AInt ms = 0; ms < materialShadersCount; ms++)
		str += material->GetShaderName(ms);

	AnimaMD5 md5;
	AnimaString programName = md5(str.c_str());
	
	AnimaShaderProgram* program = CreateProgram(programName);
	if (program)
	{
		for (AInt ms = 0; ms < geometryShadersCount; ms++)
			program->AddShader(GetShaderFromName(geometry->GetShaderName(ms)));

		for (AInt ms = 0; ms < materialShadersCount; ms++)
			program->AddShader(GetShaderFromName(material->GetShaderName(ms)));
	}

	geometry->SetShaderProgram(programName);

	return program;
}

AnimaShaderProgram* AnimaShadersManager::CreateProgram(AnimaGeometryInstance* geometryInstance, const AnimaMaterial* material)
{
	AInt geometryShadersCount = geometryInstance != nullptr ? geometryInstance->GetShadersCount() : 0;
	AInt materialShadersCount = material != nullptr ? material->GetShadersCount() : 0;

	if (geometryShadersCount <= 0 && materialShadersCount <= 0)
		return nullptr;
	
	AnimaString str;

	for (AInt ms = 0; ms < geometryShadersCount; ms++)
		str += geometryInstance->GetShaderName(ms);

	for (AInt ms = 0; ms < materialShadersCount; ms++)
		str += material->GetShaderName(ms);
	
	AnimaMD5 md5;
	AnimaString programName = md5(str.c_str());

	AnimaShaderProgram* program = _programs[programName];

	if (program == nullptr)
	{
		program = CreateProgram(programName);
		if (program)
		{
			bool vertexFound = false;
			bool fragmentFound = false;

			for (AInt ms = 0; ms < geometryShadersCount; ms++)
			{
				AnimaShader* shader = GetShaderFromName(geometryInstance->GetShaderName(ms));

				if (shader->GetType() == FRAGMENT)
					fragmentFound = true;
				else if (shader->GetType() == VERTEX)
					vertexFound = true;

				program->AddShader(shader);
			}

			for (AInt ms = 0; ms < materialShadersCount; ms++)
			{
				AnimaShader* shader = GetShaderFromName(material->GetShaderName(ms));

				if (shader->GetType() == FRAGMENT)
					fragmentFound = true;
				else if (shader->GetType() == VERTEX)
					vertexFound = true;

				program->AddShader(shader);
			}

			if (!vertexFound)
			{
				if (_defaultVertexShader == nullptr)
					return nullptr;
				else
					program->AddShader(_defaultVertexShader);
			}

			if (!fragmentFound)
			{
				if (_defaultFragmentShader == nullptr)
					return nullptr;
				else
					program->AddShader(_defaultFragmentShader);
			}

			program->SetSupportsInstance(true);
		}
	}

	geometryInstance->SetShaderProgram(program);
	return program;
}

bool AnimaShadersManager::LoadShadersParts(const AnimaString& partsPath)
{
	namespace fs = boost::filesystem;
	fs::path directory(partsPath);

	if (fs::exists(directory) && fs::is_directory(directory))
	{
		fs::directory_iterator endIterator;
		for (fs::directory_iterator directoryIterator(directory); directoryIterator != endIterator; directoryIterator++)
		{
			if (directoryIterator->path().extension().string() == ".ash")
			{
				if (LoadShaderFromFile(directoryIterator->path().string().c_str()) == nullptr)
					return false;
			}
		}
	}

	return true;
}

AnimaShader* AnimaShadersManager::LoadShaderFromFile(const AnimaString& partFilePath)
{
	std::ifstream fileStream(partFilePath);
	AnimaString xml((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
	fileStream.close();

	return LoadShaderFromXml(xml);
}

AnimaShader* AnimaShadersManager::LoadShaderFromXml(const AnimaString& shaderXmlDefinition)
{
	using boost::property_tree::ptree;
	ptree pt;

	std::stringstream ss(shaderXmlDefinition);
	boost::property_tree::read_xml(ss, pt);

	return LoadShaderFromXml(pt);
}

AnimaShader* AnimaShadersManager::LoadShaderFromXml(const boost::property_tree::ptree& xmlTree)
{
	AnimaShader* shader = nullptr;

	AnimaString name = xmlTree.get<AnimaString>("AnimaShader.Name");
	AnimaString type = xmlTree.get<AnimaString>("AnimaShader.Type");

	shader = CreateShader(name.c_str());

	if (shader)
	{
		AnimaString code = xmlTree.get<AnimaString>("AnimaShader.Versions.Version.Code", "");
		
		try
		{
			for (auto& prop : xmlTree.get_child("AnimaShader.Versions.Version.Datas"))
			{
				if (prop.first == "Data")
				{
					AnimaString dataName = prop.second.get<AnimaString>("<xmlattr>.propertyName", "");
					AnimaString associatedWith = prop.second.get<AnimaString>("<xmlattr>.associatedWith", "");
					AnimaShaderDataType dataType = prop.second.get<AnimaShaderDataType>("<xmlattr>.type", ASDT_NONE);
					AnimaShaderDataSourceObject dataSource = prop.second.get<AnimaShaderDataSourceObject>("<xmlattr>.sourceObject", ASDSO_NONE);
					AInt dataArraySize = prop.second.get<AInt>("<xmlattr>.size", 0);
					
					AnimaShaderData data(dataName);
					data.SetType(dataType);
					data.SetSourceObject(dataSource);
					data.SetAssociatedWith(associatedWith);
					data.SetArraySize(dataArraySize);
					
					shader->AddShaderData(data);
				}
			}
		}
		catch (boost::property_tree::ptree_bad_path& exception)
		{
		}
		catch (boost::property_tree::ptree_bad_data& exception)
		{
			printf("AnimaShadersManager Error: error reading shader include\n\t- Shader include name: %s\n\t- Error: %s\n", name.c_str(), exception.what());
		}
		catch (boost::property_tree::ptree_error& exception)
		{
			printf("AnimaShadersManager Error: error reading shader include\n\t- Shader include name: %s\n\t- Error: %s\n", name.c_str(), exception.what());
		}
		
		try
		{
			for (auto& groupProp : xmlTree.get_child("AnimaShader.Versions.Version.GroupsData"))
			{
				if (groupProp.first == "Group")
				{
					AnimaString groupName = groupProp.second.get<AnimaString>("<xmlattr>.groupName");
					AnimaString associatedWith = groupProp.second.get<AnimaString>("<xmlattr>.associatedWith", "");
					bool dynamicGroup = groupProp.second.get<bool>("<xmlattr>.dynamic", true);
					bool supportsInstance = groupProp.second.get<bool>("<xmlattr>.supportsInstance", false);
					AnimaShaderDataSourceObject dataSource = groupProp.second.get<AnimaShaderDataSourceObject>("<xmlattr>.sourceObject", ASDSO_NONE);
					
					AnimaShaderGroupData groupData(groupName, _engine->GetShadersAllocator());
					groupData.SetSourceObject(dataSource);
					groupData.SetAssociatedWith(associatedWith);
					groupData.SetSupportsInstance(supportsInstance);
					
					for (auto& dataProp : groupProp.second.get_child("Datas"))
					{
						if (dataProp.first == "Data")
						{
							AnimaString dataName = dataProp.second.get<AnimaString>("<xmlattr>.propertyName");
							AnimaShaderDataType dataType = dataProp.second.get<AnimaShaderDataType>("<xmlattr>.type");
							AInt dataArraySize = dataProp.second.get<AInt>("<xmlattr>.size", 0);
							
							AnimaShaderData data(dataName);
							data.SetArraySize(dataArraySize);
							data.SetType(dataType);
							
							groupData.AddShaderData(data);
						}
					}
					
					if (dynamicGroup)
						shader->AddShaderDynamicGroupData(groupData);
					else
						shader->AddShaderStaticGroupData(groupData);
				}
			}
		}
		catch (boost::property_tree::ptree_bad_path& exception)
		{
		}
		catch (boost::property_tree::ptree_bad_data& exception)
		{
			printf("AnimaShadersManager Error: error reading shader include\n\t- Shader include name: %s\n\t- Error: %s\n", name.c_str(), exception.what());
		}
		catch (boost::property_tree::ptree_error& exception)
		{
			printf("AnimaShadersManager Error: error reading shader include\n\t- Shader include name: %s\n\t- Error: %s\n", name.c_str(), exception.what());
		}
		
		shader->SetText(code.c_str());

		if (type.compare("FS") == 0)
			shader->SetType(FRAGMENT);
		else if (type.compare("VS") == 0)
			shader->SetType(VERTEX);
		else if (type.compare("TCS") == 0)
			shader->SetType(TESSELLATION_CONTROL);
		else if (type.compare("TES") == 0)
			shader->SetType(TESSELLATION_EVALUATION);
		else if (type.compare("GS") == 0)
			shader->SetType(GEOMETRY);
		else
			shader->SetType(INVALID);
	}

	return shader;
}

AnimaShaderProgram* AnimaShadersManager::LoadShaderProgramFromFile(const AnimaString& filePath)
{
	std::ifstream fileStream(filePath);
	AnimaString xml((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
	fileStream.close();

	return LoadShaderProgramFromXml(xml);
}

AnimaShaderProgram* AnimaShadersManager::LoadShaderProgramFromXml(const AnimaString& shaderProgramXmlDefinition)
{
	using boost::property_tree::ptree;
	ptree pt;

	std::stringstream ss(shaderProgramXmlDefinition);
	boost::property_tree::read_xml(ss, pt);
	
	return LoadShaderProgramFromXml(pt);
}

AnimaShaderProgram* AnimaShadersManager::LoadShaderProgramFromXml(const boost::property_tree::ptree& xmlTree)
{
	AnimaShaderProgram* shaderProgram = nullptr;

	AnimaString name = xmlTree.get<AnimaString>("AnimaShaderProgram.Name");
	
	shaderProgram = CreateProgram(name.c_str());

	if (shaderProgram)
	{
		bool supportsInstance = xmlTree.get<bool>("AnimaShaderProgram.SupportsInstance", false);
		AUint maxInstances = xmlTree.get<AUint>("AnimaShaderProgram.MaxInstances", 1);

		shaderProgram->SetSupportsInstance(supportsInstance);
		shaderProgram->SetMaxInstances(maxInstances);

		for (auto& prop : xmlTree.get_child("AnimaShaderProgram.Shaders"))
		{
			if (prop.first == "Shader")
			{
				AnimaString shaderName = prop.second.get<AnimaString>("<xmlattr>.name");
				AnimaShader* shader = GetShaderFromName(shaderName);
				if (shader)
					shaderProgram->AddShader(shader);
			}
		}
	}

	return shaderProgram;
}

AnimaShaderInclude* AnimaShadersManager::LoadShaderIncludeFromFile(const AnimaString& name, const AnimaString& filePath)
{
	AnimaShaderInclude* include = CreateShaderInclude(name);
	if (include)
	{
		std::ifstream fileStream(filePath);
		AnimaString text((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
		fileStream.close();

		include->SetText(text);
	}

	return include;
}

AnimaShaderInclude* AnimaShadersManager::LoadShaderIncludeFromFile(const AnimaString& filePath)
{
	std::ifstream fileStream(filePath);
	AnimaString xml((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
	fileStream.close();

	return LoadShaderIncludeFromXml(xml);
}

AnimaShaderInclude* AnimaShadersManager::LoadShaderIncludeFromXml(const AnimaString& includeXmlDefinition)
{
	using boost::property_tree::ptree;
	ptree pt;

	std::stringstream ss(includeXmlDefinition);
	boost::property_tree::read_xml(ss, pt);

	return LoadShaderIncludeFromXml(pt);
}

AnimaShaderInclude* AnimaShadersManager::LoadShaderIncludeFromXml(const boost::property_tree::ptree& xmlTree)
{
	AnimaShaderInclude* shaderInclude = nullptr;

	AnimaString name = xmlTree.get<AnimaString>("AnimaShaderInclude.Name");

	shaderInclude = CreateShaderInclude(name.c_str());

	if (shaderInclude)
	{
		AnimaString code = xmlTree.get<AnimaString>("AnimaShaderInclude.Versions.Version.Code", "");

		try
		{
			for (auto& prop : xmlTree.get_child("AnimaShaderInclude.Versions.Version.Datas"))
			{
				if (prop.first == "Data")
				{
					AnimaString dataName = prop.second.get<AnimaString>("<xmlattr>.propertyName", "");
					AnimaString associatedWith = prop.second.get<AnimaString>("<xmlattr>.associatedWith", "");
					AnimaShaderDataType dataType = prop.second.get<AnimaShaderDataType>("<xmlattr>.type", ASDT_NONE);
					AnimaShaderDataSourceObject dataSource = prop.second.get<AnimaShaderDataSourceObject>("<xmlattr>.sourceObject", ASDSO_NONE);
					AInt dataArraySize = prop.second.get<AInt>("<xmlattr>.size", 0);

					AnimaShaderData data(dataName);
					data.SetType(dataType);
					data.SetSourceObject(dataSource);
					data.SetAssociatedWith(associatedWith);
					data.SetArraySize(dataArraySize);

					shaderInclude->AddShaderData(data);
				}
			}
		}
		catch (boost::property_tree::ptree_bad_path& exception)
		{
		}
		catch (boost::property_tree::ptree_bad_data& exception)
		{
			printf("AnimaShadersManager Error: error reading shader include\n\t- Shader include name: %s\n\t- Error: %s\n", name.c_str(), exception.what());
		}
		catch (boost::property_tree::ptree_error& exception)
		{
			printf("AnimaShadersManager Error: error reading shader include\n\t- Shader include name: %s\n\t- Error: %s\n", name.c_str(), exception.what());
		}

		try
		{
			for (auto& groupProp : xmlTree.get_child("AnimaShaderInclude.Versions.Version.GroupsData"))
			{
				if (groupProp.first == "Group")
				{
					AnimaString groupName = groupProp.second.get<AnimaString>("<xmlattr>.groupName");
					AnimaString associatedWith = groupProp.second.get<AnimaString>("<xmlattr>.associatedWith", "");
					bool dynamicGroup = groupProp.second.get<bool>("<xmlattr>.dynamic", true);
					bool supportsInstance = groupProp.second.get<bool>("<xmlattr>.supportsInstance", false);
					AnimaShaderDataSourceObject dataSource = groupProp.second.get<AnimaShaderDataSourceObject>("<xmlattr>.sourceObject", ASDSO_NONE);
					
					AnimaShaderGroupData groupData(groupName, _engine->GetShadersAllocator());
					groupData.SetSourceObject(dataSource);
					groupData.SetAssociatedWith(associatedWith);
					groupData.SetSupportsInstance(supportsInstance);
					
					for (auto& dataProp : groupProp.second.get_child("Datas"))
					{
						if (dataProp.first == "Data")
						{
							AnimaString dataName = dataProp.second.get<AnimaString>("<xmlattr>.propertyName");
							AnimaShaderDataType dataType = dataProp.second.get<AnimaShaderDataType>("<xmlattr>.type");
							AInt dataArraySize = dataProp.second.get<AInt>("<xmlattr>.size", 0);

							AnimaShaderData data(dataName);
							data.SetArraySize(dataArraySize);
							data.SetType(dataType);

							groupData.AddShaderData(data);
						}
					}

					if (dynamicGroup)
						shaderInclude->AddShaderDynamicGroupData(groupData);
					else
						shaderInclude->AddShaderStaticGroupData(groupData);
				}
			}
		}
		catch (boost::property_tree::ptree_bad_path& exception)
		{
		}
		catch (boost::property_tree::ptree_bad_data& exception)
		{
			printf("AnimaShadersManager Error: error reading shader include\n\t- Shader include name: %s\n\t- Error: %s\n", name.c_str(), exception.what());
		}
		catch (boost::property_tree::ptree_error& exception)
		{
			printf("AnimaShadersManager Error: error reading shader include\n\t- Shader include name: %s\n\t- Error: %s\n", name.c_str(), exception.what());
		}

		shaderInclude->SetText(code.c_str());
	}

	return shaderInclude;
}

bool AnimaShadersManager::LoadShadersIncludes(const AnimaString& includesPath)
{
	namespace fs = boost::filesystem;
	fs::path directory(includesPath);

	if (fs::exists(directory) && fs::is_directory(directory))
	{
		fs::directory_iterator endIterator;
		for (fs::directory_iterator directoryIterator(directory); directoryIterator != endIterator; directoryIterator++)
		{
			if (directoryIterator->path().extension().string() == ".asi")
			{
				if (LoadShaderIncludeFromFile(directoryIterator->path().string().c_str()) == nullptr)
					return false;
			}
		}
	}

	return true;
}

void AnimaShadersManager::AttachIncludes(AnimaShader* shader)
{
	if (shader == nullptr)
		return;

	AnimaString text = shader->GetText();

	AnimaString includeToken = "#include";
	AnimaString includeName = "";

	AInt tokenLen = includeToken.length();
	AInt textLen = text.length();
	AInt position = AnimaString::npos;
	AInt offset = position;

	// Ricerco il token degli include finchè non ne trovo piò
	while ((position = text.find(includeToken)) != AnimaString::npos)
	{
		textLen = text.length();

		offset = position;

		// sposto avanti la posizione trovata di tokenLen e cerco il primo doppio apice
		offset += tokenLen;

		// cerco il primo doppio apice
		while (offset < textLen && text[offset] != '\"')
			offset++;

		// Porto avanti la posizione per leggere il carattere successivo al primo doppio apice
		offset++;

		if (offset >= textLen - 1)
			return;

		// Leggo il nome finchè non raggiungo un altro doppio apice
		includeName = "";
		while (offset < textLen && text[offset] != '\"')
			includeName.push_back(text[offset++]);

		offset++;

		// Cerco l'include tra quelli in memoria
		AnimaShaderInclude* include = _includes[includeName];
		if (include == nullptr)
			return;

		// Se ho trovato l'include lo sostituisco e ripeto le operazioni
		text.replace(position, offset - position, include->GetText());

		// Ora aggiungo allo shader i dati dell'include
		AInt dataCount = include->GetShaderDataCount();
		for (AInt i = 0; i < dataCount; i++)
		{
			AnimaShaderData* data = include->GetShaderData(i);

			// Se lo shader non contiene già il dato lo vado ad aggiungere
			if (shader->GetShaderData(data->GetName()) == nullptr)
				shader->AddShaderData(*data);
		}

		AInt dynamicGroupDataCount = include->GetShaderDynamicGroupDataCount();
		for (AInt i = 0; i < dynamicGroupDataCount; i++)
		{
			AnimaShaderGroupData* groupData = include->GetShaderDynamicGroupData(i);

			// Se lo shader non contiene già il dato lo vado ad aggiungere
			if (shader->GetShaderDynamicGroupData(groupData->GetName()) == nullptr)
				shader->AddShaderDynamicGroupData(*groupData);
		}

		AInt staticGroupDataCount = include->GetShaderStaticGroupDataCount();
		for (AInt i = 0; i < staticGroupDataCount; i++)
		{
			AnimaShaderGroupData* groupData = include->GetShaderStaticGroupData(i);

			// Se lo shader non contiene già il dato lo vado ad aggiungere
			if (shader->GetShaderStaticGroupData(groupData->GetName()) == nullptr)
				shader->AddShaderStaticGroupData(*groupData);
		}
	}

	shader->SetText(text);
}

END_ANIMA_ENGINE_NAMESPACE