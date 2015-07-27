#include "AnimaShadersManager.h"
#include "AnimaMesh.h"
#include "AnimaMeshInstance.h"
#include "AnimaMaterial.h"
#include "AnimaMD5.h"
#include "AnimaShaderData.h"
#include "AnimaXmlTranslators.h"

#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaShadersManager::AnimaShadersManager(AnimaEngine* engine)
{
	_engine = engine;
	_activeProgram = nullptr;
}

AnimaShadersManager::~AnimaShadersManager()
{
	ClearShaders();
	ClearPrograms();
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

AnimaShaderProgram* AnimaShadersManager::CreateProgram(AnimaMesh* mesh, const AnimaMaterial* material)
{
	AInt meshShadersCount = mesh != nullptr ? mesh->GetShadersCount() : 0;
	AInt materialShadersCount = material != nullptr ? material->GetShadersCount() : 0;

	if (meshShadersCount <= 0 || materialShadersCount <= 0)
		return nullptr;

	AnimaString str;

	for (AInt ms = 0; ms < meshShadersCount; ms++)
		str += mesh->GetShaderName(ms);

	for (AInt ms = 0; ms < materialShadersCount; ms++)
		str += material->GetShaderName(ms);

	AnimaMD5 md5;
	AnimaString programName = md5(str.c_str());
	
	AnimaShaderProgram* program = CreateProgram(programName);
	if (program)
	{
		for (AInt ms = 0; ms < meshShadersCount; ms++)
			program->AddShader(GetShaderFromName(mesh->GetShaderName(ms)));

		for (AInt ms = 0; ms < materialShadersCount; ms++)
			program->AddShader(GetShaderFromName(material->GetShaderName(ms)));
	}

	mesh->SetShaderProgram(programName);

	return program;
}

AnimaShaderProgram* AnimaShadersManager::CreateProgram(AnimaMeshInstance* meshInstance, const AnimaMaterial* material)
{
	AInt meshShadersCount = meshInstance != nullptr ? meshInstance->GetShadersCount() : 0;
	AInt materialShadersCount = material != nullptr ? material->GetShadersCount() : 0;

	if (meshShadersCount <= 0 || materialShadersCount <= 0)
		return nullptr;
	
	AnimaString str;

	for (AInt ms = 0; ms < meshShadersCount; ms++)
		str += meshInstance->GetShaderName(ms);

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
			for (AInt ms = 0; ms < meshShadersCount; ms++)
				program->AddShader(GetShaderFromName(meshInstance->GetShaderName(ms)));

			for (AInt ms = 0; ms < materialShadersCount; ms++)
				program->AddShader(GetShaderFromName(material->GetShaderName(ms)));
		}
	}

	meshInstance->SetShaderProgram(programName);
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
			if (LoadShaderFromFile(directoryIterator->path().string().c_str()) == nullptr)
				return false;
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

	AnimaShader* shader = nullptr;
	ptree pt;

	std::stringstream ss(shaderXmlDefinition);
	boost::property_tree::read_xml(ss, pt);

	AnimaString name = pt.get<AnimaString>("AnimaShader.<xmlattr>.name");
	AnimaString type = pt.get<AnimaString>("AnimaShader.<xmlattr>.type");
	AnimaString code = pt.get<AnimaString>("AnimaShader.Part.ShaderCode");

	shader = CreateShader(name.c_str());

	if (shader)
	{
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

	AnimaShaderProgram* shaderProgram = nullptr;
	ptree pt;

	std::stringstream ss(shaderProgramXmlDefinition);
	boost::property_tree::read_xml(ss, pt);

	AnimaString name = pt.get<AnimaString>("AnimaShaderProgram.<xmlattr>.name");

	shaderProgram = CreateProgram(name.c_str());

	if (shaderProgram)
	{
		for (auto& prop : pt.get_child("AnimaShaderProgram.Shaders"))
		{
			if (prop.first == "Shader")
			{
				AnimaString shaderName = prop.second.get<AnimaString>("<xmlattr>.name");
				AnimaShader* shader = GetShaderFromName(shaderName);
				if (shader)
					shaderProgram->AddShader(shader);
			}
		}

		try
		{
			for (auto& prop : pt.get_child("AnimaShaderProgram.Datas"))
			{
				if (prop.first == "Data")
				{
					AnimaString dataName = prop.second.get<AnimaString>("<xmlattr>.name");
					AnimaShaderDataType dataType = prop.second.get<AnimaShaderDataType>("<xmlattr>.type");
					AInt dataArraySize = prop.second.get<AInt>("<xmlattr>.size", 0);

					AnimaShaderData data(dataName);
					data.SetArraySize(dataArraySize);
					data.SetType(dataType);

					shaderProgram->AddShaderData(data);
				}
			}
		}
		catch (boost::property_tree::ptree_bad_path& exception)
		{
			printf("[AnimaShadersManager] Error reading AnimaShaderProgram data:\n\t- Shader program name '%s'\n\n\t- Error: %s\n", name.c_str(), exception.what());
		}
		catch (boost::property_tree::ptree_bad_data& exception)
		{
			printf("[AnimaShadersManager] Error reading AnimaShaderProgram data:\n\t- Shader program name '%s'\n\n\t- Error: %s\n", name.c_str(), exception.what());
		}
		catch (boost::property_tree::ptree_error& exception)
		{
			printf("[AnimaShadersManager] Error reading AnimaShaderProgram data:\n\t- Shader program name '%s'\n\n\t- Error: %s\n", name.c_str(), exception.what());
		}
	}

	return shaderProgram;
}

END_ANIMA_ENGINE_NAMESPACE