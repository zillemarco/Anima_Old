#include "AnimaShadersManager.h"
#include <fstream>

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaShadersManager::AnimaShadersManager(AnimaScene* scene)
	: _shaders(scene->GetShadersAllocator())
	, _programs(scene->GetShadersAllocator())
{
	_scene = scene;

//	_shaders = nullptr;
//	_shadersNumber = 0;
//
//	_programs = nullptr;
//	_programsNumber = 0;

	_activeProgram = nullptr;
}

AnimaShadersManager::~AnimaShadersManager()
{
	ClearShaders();
	ClearPrograms();
}

AnimaShader* AnimaShadersManager::CreateShader(const char* name)
{
	AnimaString str(name, _scene->GetStringAllocator());
	return CreateShader(str);
}

AnimaShader* AnimaShadersManager::CreateShader(const AnimaString& name)
{
	AInt index = _shaders.Contains(name);
	if(index >= 0)
		return nullptr;
	
	AnimaShader* shader = AnimaAllocatorNamespace::AllocateNew<AnimaShader>(*(_scene->GetShadersAllocator()), _scene->GetShadersAllocator());
	_shaders.Add(name, shader);
	return shader;
	
//	if (_shadersMap.find(name) != _shadersMap.end())
//		return nullptr;
//
//	ANIMA_ASSERT(_scene != nullptr);
//	if (_shadersNumber > 0)
//	{
//		AnimaShader** tmpOldShaders = AnimaAllocatorNamespace::AllocateArray<AnimaShader*>(*(_scene->GetShadersAllocator()), _shadersNumber);
//
//		for (int i = 0; i < _shadersNumber; i++)
//			tmpOldShaders[i] = _shaders[i];
//
//		ClearShaders(false, false);
//
//		_shadersNumber++;
//		_shaders = AnimaAllocatorNamespace::AllocateArray<AnimaShader*>(*(_scene->GetShadersAllocator()), _shadersNumber);
//
//		for (int i = 0; i < _shadersNumber - 1; i++)
//			_shaders[i] = tmpOldShaders[i];
//		
//		AnimaAllocatorNamespace::DeallocateArray(*(_scene->GetShadersAllocator()), tmpOldShaders);
//		tmpOldShaders = nullptr;
//	}
//	else
//	{
//		_shadersNumber++;
//		_shaders = AnimaAllocatorNamespace::AllocateArray<AnimaShader*>(*(_scene->GetShadersAllocator()), _shadersNumber);
//	}
//
//	_shaders[_shadersNumber - 1] = AnimaAllocatorNamespace::AllocateNew<AnimaShader>(*(_scene->GetShadersAllocator()), _scene->GetShadersAllocator());
//
//	_shadersMap[name] = (AUint)(_shadersNumber - 1);
//	
//	return _shaders[_shadersNumber - 1];
}

AnimaShaderProgram* AnimaShadersManager::CreateProgram(const char* name)
{
	AnimaString str(name, _scene->GetStringAllocator());
	return CreateProgram(str);
}

AnimaShaderProgram* AnimaShadersManager::CreateProgram(const AnimaString& name)
{
	AInt index = _programs.Contains(name);
	if(index >= 0)
		return nullptr;
	
	AnimaShaderProgram* program = AnimaAllocatorNamespace::AllocateNew<AnimaShaderProgram>(*(_scene->GetShadersAllocator()), _scene->GetShadersAllocator(), this);
	_programs.Add(name, program);
	return program;
	
//	if (_programsMap.find(name) != _programsMap.end())
//		return nullptr;
//
//	ANIMA_ASSERT(_scene != nullptr);
//	if (_programsNumber > 0)
//	{
//		AnimaShaderProgram** tmpOldPrograms = AnimaAllocatorNamespace::AllocateArray<AnimaShaderProgram*>(*(_scene->GetShadersAllocator()), _programsNumber);
//
//		for (int i = 0; i < _programsNumber; i++)
//			tmpOldPrograms[i] = _programs[i];
//
//		ClearPrograms(false, false);
//
//		_programsNumber++;
//		_programs = AnimaAllocatorNamespace::AllocateArray<AnimaShaderProgram*>(*(_scene->GetShadersAllocator()), _programsNumber);
//
//		for (int i = 0; i < _programsNumber - 1; i++)
//			_programs[i] = tmpOldPrograms[i];
//		
//		AnimaAllocatorNamespace::DeallocateArray(*(_scene->GetShadersAllocator()), tmpOldPrograms);
//		tmpOldPrograms = nullptr;
//	}
//	else
//	{
//		_programsNumber++;
//		_programs = AnimaAllocatorNamespace::AllocateArray<AnimaShaderProgram*>(*(_scene->GetShadersAllocator()), _programsNumber);
//	}
//
//	_programs[_programsNumber - 1] = AnimaAllocatorNamespace::AllocateNew<AnimaShaderProgram>(*(_scene->GetShadersAllocator()), _scene->GetShadersAllocator(), this);
//
//	_programsMap[name] = (AUint)(_programsNumber - 1);
//
//	return _programs[_programsNumber - 1];
}

AnimaShader* AnimaShadersManager::LoadShader(const char* name, const AnimaString& text, AnimaShader::AnimaShaderType type)
{
	AnimaString str(name, _scene->GetStringAllocator());
	return LoadShader(name, text.GetConstBuffer(), type);
}

AnimaShader* AnimaShadersManager::LoadShader(const AnimaString& name, const AnimaString& text, AnimaShader::AnimaShaderType type)
{
	return LoadShader(name, text.GetConstBuffer(), type);
}

AnimaShader* AnimaShadersManager::LoadShader(const char* name, const AChar* text, AnimaShader::AnimaShaderType type)
{
	AnimaString str(name, _scene->GetStringAllocator());
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
	AnimaString str(name, _scene->GetStringAllocator());
	return LoadShaderFromFile(str, filePath, type);
}

AnimaShader* AnimaShadersManager::LoadShaderFromFile(const AnimaString& name, const AnimaString &filePath, AnimaShader::AnimaShaderType type)
{
	return LoadShaderFromFile(name, filePath.GetConstBuffer(), type);
}

AnimaShader* AnimaShadersManager::LoadShaderFromFile(const char* name, const AChar* filePath, AnimaShader::AnimaShaderType type)
{
	AnimaString str(name, _scene->GetStringAllocator());
	return LoadShaderFromFile(str, filePath, type);
}

AnimaShader* AnimaShadersManager::LoadShaderFromFile(const AnimaString& name, const AChar* filePath, AnimaShader::AnimaShaderType type)
{
	AnimaShader* sh = nullptr;
	AnimaString str(_scene->GetStringAllocator());
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
	AnimaString str(name, _scene->GetStringAllocator());
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
		AnimaString str(_scene->GetStringAllocator());
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
		AnimaAllocatorNamespace::DeallocateObject(*(_scene->GetShadersAllocator()), shader);
		shader = nullptr;
	}
	
	_shaders.RemoveAll();
	
//	if (_shaders != nullptr)
//	{
//		if (bDeleteObjects)
//		{
//			for (int i = 0; i < (int)_shadersNumber; i++)
//			{
//				AnimaAllocatorNamespace::DeallocateObject(*(_scene->GetShadersAllocator()), _shaders[i]);
//				_shaders[i] = nullptr;
//			}
//
//			_shadersMap.clear();
//		}
//
//		AnimaAllocatorNamespace::DeallocateArray<AnimaShader*>(*(_scene->GetShadersAllocator()), _shaders);
//		_shaders = nullptr;
//	}
//
//	if (bResetNumber)
//		_shadersNumber = 0;
}

void AnimaShadersManager::ClearPrograms(bool bDeleteObjects, bool bResetNumber)
{
	AInt count = _programs.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaShaderProgram* program = _programs[i];
		AnimaAllocatorNamespace::DeallocateObject(*(_scene->GetShadersAllocator()), program);
		program = nullptr;
	}
	
	_programs.RemoveAll();
//	if (_programs != nullptr)
//	{
//		if (bDeleteObjects)
//		{
//			for (int i = 0; i < (int)_programsNumber; i++)
//			{
//				AnimaAllocatorNamespace::DeallocateObject(*(_scene->GetShadersAllocator()), _programs[i]);
//				_programs[i] = nullptr;
//			}
//
//			_programsMap.clear();
//		}
//
//		AnimaAllocatorNamespace::DeallocateArray<AnimaShaderProgram*>(*(_scene->GetShadersAllocator()), _programs);
//		_programs = nullptr;
//	}
//
//	if (bResetNumber)
//		_programsNumber = 0;
}

AnimaShaderProgram* AnimaShadersManager::GetProgram(ASizeT index)
{
//	ANIMA_ASSERT(index >= 0 && index < _programsNumber);
	return _programs[index];
}

AnimaShaderProgram* AnimaShadersManager::GetProgramFromName(const AnimaString& name)
{
	return _programs.Get(name);
//	auto pair = _programsMap.find(name);
//	if (pair == _programsMap.end())
//		return nullptr;
//	return GetProgram((ASizeT)pair->second);
}

AnimaShaderProgram* AnimaShadersManager::GetProgramFromName(const char* name)
{
	AnimaString str(name, _scene->GetStringAllocator());
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