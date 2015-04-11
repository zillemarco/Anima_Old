#include "AnimaShadersManager.h"
#include <fstream>

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
	
	AnimaShader* shader = AnimaAllocatorNamespace::AllocateNew<AnimaShader>(*(_engine->GetShadersAllocator()), _engine->GetShadersAllocator());
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
	
	AnimaShaderProgram* program = AnimaAllocatorNamespace::AllocateNew<AnimaShaderProgram>(*(_engine->GetShadersAllocator()), _engine->GetShadersAllocator(), this);
	_programs.Add(name, program);
	return program;
}

AnimaShader* AnimaShadersManager::LoadShader(const char* name, const AnimaString& text, AnimaShader::AnimaShaderType type)
{
	AnimaString str(name, _engine->GetStringAllocator());
	return LoadShader(name, text.GetConstBuffer(), type);
}

AnimaShader* AnimaShadersManager::LoadShader(const AnimaString& name, const AnimaString& text, AnimaShader::AnimaShaderType type)
{
	return LoadShader(name, text.GetConstBuffer(), type);
}

AnimaShader* AnimaShadersManager::LoadShader(const char* name, const AChar* text, AnimaShader::AnimaShaderType type)
{
	AnimaString str(name, _engine->GetStringAllocator());
	return LoadShader(str, text, type);
}

AnimaShader* AnimaShadersManager::LoadShader(const AnimaString& name, const AChar* text, AnimaShader::AnimaShaderType type)
{
	AnimaShader* sh = CreateShader(name);
	sh->SetText(text);
	sh->SetType(type);
	return sh;
}

AnimaShader* AnimaShadersManager::LoadShaderFromFile(const char* name, const AnimaString &filePath, AnimaShader::AnimaShaderType type)
{
	AnimaString str(name, _engine->GetStringAllocator());
	return LoadShaderFromFile(str, filePath, type);
}

AnimaShader* AnimaShadersManager::LoadShaderFromFile(const AnimaString& name, const AnimaString &filePath, AnimaShader::AnimaShaderType type)
{
	return LoadShaderFromFile(name, filePath.GetConstBuffer(), type);
}

AnimaShader* AnimaShadersManager::LoadShaderFromFile(const char* name, const AChar* filePath, AnimaShader::AnimaShaderType type)
{
	AnimaString str(name, _engine->GetStringAllocator());
	return LoadShaderFromFile(str, filePath, type);
}

AnimaShader* AnimaShadersManager::LoadShaderFromFile(const AnimaString& name, const AChar* filePath, AnimaShader::AnimaShaderType type)
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

	if (info._infoType == AnimaShaderProgram::SHADER_TEXT)
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
	return _programs.Get(name);
}

AnimaShaderProgram* AnimaShadersManager::GetProgramFromName(const char* name)
{
	AnimaString str(name, _engine->GetStringAllocator());
	return GetProgramFromName(str);
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

END_ANIMA_ENGINE_NAMESPACE