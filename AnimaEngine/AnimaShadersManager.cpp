#include "AnimaShadersManager.h"
#include "AnimaMesh.h"
#include "AnimaMeshInstance.h"
#include "AnimaMaterial.h"
#include "AnimaMD5.h"

#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaShadersManager::AnimaShadersManager(AnimaEngine* engine)
	: _shaders(engine->GetShadersAllocator())
	, _programs(engine->GetShadersAllocator())
{
	_engine = engine;
	_activeProgram = nullptr;
}

AnimaShadersManager::~AnimaShadersManager()
{
	ClearShaders();
	ClearPrograms();
}

AnimaShader* AnimaShadersManager::CreateShader(const char* name)
{
	AnimaString str(name, _engine->GetStringAllocator());
	return CreateShader(str);
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

AnimaShaderProgram* AnimaShadersManager::CreateProgram(const char* name)
{
	AnimaString str(name, _engine->GetStringAllocator());
	return CreateProgram(str);
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

AnimaShader* AnimaShadersManager::LoadShader(const char* name, const AnimaString& text, AnimaShaderType type)
{
	AnimaString str(name, _engine->GetStringAllocator());
	return LoadShader(name, text.GetConstBuffer(), type);
}

AnimaShader* AnimaShadersManager::LoadShader(const AnimaString& name, const AnimaString& text, AnimaShaderType type)
{
	return LoadShader(name, text.GetConstBuffer(), type);
}

AnimaShader* AnimaShadersManager::LoadShader(const char* name, const AChar* text, AnimaShaderType type)
{
	AnimaString str(name, _engine->GetStringAllocator());
	return LoadShader(str, text, type);
}

AnimaShader* AnimaShadersManager::LoadShader(const AnimaString& name, const AChar* text, AnimaShaderType type)
{
	AnimaShader* sh = CreateShader(name);
	sh->SetText(text);
	sh->SetType(type);
	return sh;
}

AnimaShader* AnimaShadersManager::LoadShaderFromFile(const char* name, const AnimaString &filePath, AnimaShaderType type)
{
	AnimaString str(name, _engine->GetStringAllocator());
	return LoadShaderFromFile(str, filePath, type);
}

AnimaShader* AnimaShadersManager::LoadShaderFromFile(const AnimaString& name, const AnimaString &filePath, AnimaShaderType type)
{
	return LoadShaderFromFile(name, filePath.GetConstBuffer(), type);
}

AnimaShader* AnimaShadersManager::LoadShaderFromFile(const char* name, const AChar* filePath, AnimaShaderType type)
{
	AnimaString str(name, _engine->GetStringAllocator());
	return LoadShaderFromFile(str, filePath, type);
}

AnimaShader* AnimaShadersManager::LoadShaderFromFile(const AnimaString& name, const AChar* filePath, AnimaShaderType type)
{
	AnimaShader* sh = nullptr;
	AnimaString str(_engine->GetStringAllocator());
	bool readCompletely = true;

	std::ifstream is(filePath, std::ifstream::binary);
	if (is)
	{
		is.seekg(0, is.end);
		int length = (int)is.tellg();
		is.seekg(0, is.beg);

		str.Reserve(length);

		// read data as a block:
		is.read(str.GetBuffer(), length);

		if (!is)
			readCompletely = false;

		is.close();
	}
	else
		return nullptr;

	if (!readCompletely)
		return nullptr;

	sh = LoadShader(name, str, type);
	return sh;
}

AnimaShader* AnimaShadersManager::LoadShader(const char* name, AnimaShaderProgram::AnimaShaderInfo info)
{
	AnimaString str(name, _engine->GetStringAllocator());
	return LoadShader(str, info);
}

AnimaShader* AnimaShadersManager::LoadShader(const AnimaString& name, AnimaShaderProgram::AnimaShaderInfo info)
{
	AnimaShader* sh = nullptr;

	if (info._infoType == SHADER_TEXT)
	{
		sh = LoadShader(name, info._text, info._shaderType);
	}
	else
	{
		AnimaString str(_engine->GetStringAllocator());
		bool readCompletely = true;

		std::ifstream is(info._text, std::ifstream::binary);
		if (is) 
		{
			is.seekg(0, is.end);
			int length = (int)is.tellg();
			is.seekg(0, is.beg);
			
			str.Reserve(length);

			// read data as a block:
			is.read(str.GetBuffer(), length);

			if (!is)
				readCompletely = false;

			is.close();
		}
		else
			return nullptr;

		if (!readCompletely)
			return nullptr;

		sh = LoadShader(name, str, info._shaderType);
	}

	return sh;
}

void AnimaShadersManager::ClearShaders(bool bDeleteObjects, bool bResetNumber)
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

void AnimaShadersManager::ClearPrograms(bool bDeleteObjects, bool bResetNumber)
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

AnimaShaderProgram* AnimaShadersManager::GetProgramFromName(const char* name)
{
	AnimaString str(name, _engine->GetStringAllocator());
	return GetProgramFromName(str);
}

AnimaShader* AnimaShadersManager::GetShader(AInt index)
{
	return _shaders[index];
}

AnimaShader* AnimaShadersManager::GetShaderFromName(const AnimaString& name)
{
	return _shaders.GetWithName(name);
}

AnimaShader* AnimaShadersManager::GetShaderFromName(const char* name)
{
	AnimaString str(name, _engine->GetStringAllocator());
	return GetShaderFromName(str);
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

void AnimaShadersManager::SetActiveProgramFromName(const char* name)
{
	_activeProgram = GetProgramFromName(name);
}

AnimaShaderProgram* AnimaShadersManager::GetActiveProgram()
{
	return _activeProgram;
}

AnimaShaderProgram* AnimaShadersManager::CreateProgram(AnimaMesh* mesh, const AnimaMaterial* material)
{
	AnimaString str(_engine->GetStringAllocator());

	AInt meshShadersCount = mesh->GetShadersCount();
	for (AInt ms = 0; ms < meshShadersCount; ms++)
		str += mesh->GetShaderName(ms);

	AInt materialShadersCount = material->GetShadersCount();
	for (AInt ms = 0; ms < materialShadersCount; ms++)
		str += material->GetShaderName(ms);

	AnimaString programName(AnimaMD5::MD5(str.GetConstBuffer()).c_str(), _engine->GetStringAllocator());
	
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
	AnimaString str(_engine->GetStringAllocator());

	AInt meshShadersCount = meshInstance->GetShadersCount();
	for (AInt ms = 0; ms < meshShadersCount; ms++)
		str += meshInstance->GetShaderName(ms);

	AInt materialShadersCount = material->GetShadersCount();
	for (AInt ms = 0; ms < materialShadersCount; ms++)
		str += material->GetShaderName(ms);

	AnimaString programName(AnimaMD5::MD5(str.GetConstBuffer()).c_str(), _engine->GetStringAllocator());

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
	return LoadShadersParts(partsPath.GetConstBuffer());
}

bool AnimaShadersManager::LoadShadersParts(const char* partsPath)
{
	namespace fs = boost::filesystem;
	fs::path directory(partsPath);

	if (fs::exists(directory) && fs::is_directory(directory))
	{
		fs::directory_iterator endIterator;
		for (fs::directory_iterator directoryIterator(directory); directoryIterator != endIterator; directoryIterator++)
		{
			if (LoadShaderFromPartFile(directoryIterator->path().string().c_str()) == nullptr)
				return false;
		}
	}

	return true;
}

AnimaShader* AnimaShadersManager::LoadShaderFromPartFile(const AnimaString& partFilePath)
{
	return LoadShaderFromPartFile(partFilePath.GetConstBuffer());
}

AnimaShader* AnimaShadersManager::LoadShaderFromPartFile(const char* partFilePath)
{
	AnimaShader* shader = nullptr;

	std::ifstream fileStream(partFilePath);

	using boost::property_tree::ptree;
	ptree pt;

	boost::property_tree::read_xml(fileStream, pt);

	std::string name = pt.get<std::string>("AnimaShaderPart.<xmlattr>.name");
	std::string type = pt.get<std::string>("AnimaShaderPart.<xmlattr>.type");
	std::string code = pt.get<std::string>("AnimaShaderPart.Part.ShaderCode");

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

END_ANIMA_ENGINE_NAMESPACE