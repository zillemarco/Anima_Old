#include "AnimaShadersManager.h"
#include <fstream>

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaShadersManager::AnimaShadersManager(AnimaEngine* engine)
{
	_engine = engine;

	_shaders = nullptr;
	_shadersNumber = 0;

	_programs = nullptr;
	_programsNumber = 0;
}

AnimaShadersManager::~AnimaShadersManager()
{
	ClearShaders();
	ClearPrograms();
}

AnimaShader* AnimaShadersManager::CreateShader()
{
	ANIMA_ASSERT(_engine != nullptr);
	if (_shadersNumber > 0)
	{
		AnimaShader** tmpOldShaders = AnimaAllocatorNamespace::AllocateArray<AnimaShader*>(*(_engine->GetShadersAllocator()), _shadersNumber);

		for (int i = 0; i < _shadersNumber; i++)
			tmpOldShaders[i] = _shaders[i];

		ClearShaders(false, false);

		_shadersNumber++;
		_shaders = AnimaAllocatorNamespace::AllocateArray<AnimaShader*>(*(_engine->GetShadersAllocator()), _shadersNumber);

		for (int i = 0; i < _shadersNumber - 1; i++)
			_shaders[i] = tmpOldShaders[i];
		
		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetShadersAllocator()), tmpOldShaders);
		tmpOldShaders = nullptr;
	}
	else
	{
		_shadersNumber++;
		_shaders = AnimaAllocatorNamespace::AllocateArray<AnimaShader*>(*(_engine->GetShadersAllocator()), _shadersNumber);
	}

	_shaders[_shadersNumber - 1] = AnimaAllocatorNamespace::AllocateNew<AnimaShader>(*(_engine->GetShadersAllocator()), _engine);
	return _shaders[_shadersNumber - 1];
}

AnimaShaderProgram* AnimaShadersManager::CreateProgram()
{
	ANIMA_ASSERT(_engine != nullptr);
	if (_programsNumber > 0)
	{
		AnimaShaderProgram** tmpOldPrograms = AnimaAllocatorNamespace::AllocateArray<AnimaShaderProgram*>(*(_engine->GetShadersAllocator()), _programsNumber);

		for (int i = 0; i < _programsNumber; i++)
			tmpOldPrograms[i] = _programs[i];

		ClearPrograms(false, false);

		_programsNumber++;
		_programs = AnimaAllocatorNamespace::AllocateArray<AnimaShaderProgram*>(*(_engine->GetShadersAllocator()), _programsNumber);

		for (int i = 0; i < _programsNumber - 1; i++)
			_programs[i] = tmpOldPrograms[i];
		
		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetShadersAllocator()), tmpOldPrograms);
		tmpOldPrograms = nullptr;
	}
	else
	{
		_programsNumber++;
		_programs = AnimaAllocatorNamespace::AllocateArray<AnimaShaderProgram*>(*(_engine->GetShadersAllocator()), _programsNumber);
	}

	_programs[_programsNumber - 1] = AnimaAllocatorNamespace::AllocateNew<AnimaShaderProgram>(*(_engine->GetShadersAllocator()), _engine);
	return _programs[_programsNumber - 1];
}

AnimaShader* AnimaShadersManager::LoadShader(AnimaString text, AnimaShader::AnimaShaderType type)
{
	return LoadShader(text.GetConstBuffer(), type);
}

AnimaShader* AnimaShadersManager::LoadShader(const AChar* text, AnimaShader::AnimaShaderType type)
{
	AnimaShader* sh = CreateShader();
	sh->SetText(text);
	sh->SetType(type);
	return sh;
}

AnimaShader* AnimaShadersManager::LoadShaderFromFile(AnimaString filePath, AnimaShader::AnimaShaderType type)
{
	return LoadShaderFromFile(filePath.GetConstBuffer(), type);
}

AnimaShader* AnimaShadersManager::LoadShaderFromFile(const AChar* filePath, AnimaShader::AnimaShaderType type)
{
	AnimaShader* sh = nullptr;
	AnimaString str(_engine);
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
		{
			std::streamsize read = is.gcount();
			readCompletely = false;
		}

		is.close();
	}
	else
		return nullptr;

	if (!readCompletely)
		return nullptr;

	sh = LoadShader(str, type);
	return sh;
}

AnimaShader* AnimaShadersManager::LoadShader(AnimaShaderProgram::AnimaShaderInfo info)
{
	AnimaShader* sh = nullptr;

	if (info._infoType == AnimaShaderProgram::SHADER_TEXT)
	{
		sh = LoadShader(info._text, info._shaderType);
	}
	else
	{
		AnimaString str(_engine);
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
			{
				std::streamsize read = is.gcount();
				readCompletely = false;
			}

			is.close();
		}
		else
			return nullptr;

		if (!readCompletely)
			return nullptr;

		sh = LoadShader(str, info._shaderType);
	}

	return sh;
}

bool AnimaShadersManager::LoadShaders(const AnimaShaderProgram::AnimaShaderInfo* info, ASizeT count, AnimaShader** output)
{
	ANIMA_ASSERT(output != nullptr);

	for (int i = 0; i < (int)count; i++)
	{
		output[i] = LoadShader(info[i]);

		if (output[i] == nullptr)
			return false;
	}

	return true;
}

void AnimaShadersManager::ClearShaders(bool bDeleteObjects, bool bResetNumber)
{
	if (_shaders != nullptr)
	{
		if (bDeleteObjects)
		{
			for (int i = 0; i < (int)_shadersNumber; i++)
			{
				AnimaAllocatorNamespace::DeallocateObject(*(_engine->GetShadersAllocator()), _shaders[i]);
				_shaders[i] = nullptr;
			}
		}

		AnimaAllocatorNamespace::DeallocateArray<AnimaShader*>(*(_engine->GetShadersAllocator()), _shaders);
		_shaders = nullptr;
	}

	if (bResetNumber)
		_shadersNumber = 0;
}

void AnimaShadersManager::ClearPrograms(bool bDeleteObjects, bool bResetNumber)
{
	if (_programs != nullptr)
	{
		if (bDeleteObjects)
		{
			for (int i = 0; i < (int)_programsNumber; i++)
			{
				AnimaAllocatorNamespace::DeallocateObject(*(_engine->GetShadersAllocator()), _programs[i]);
				_programs[i] = nullptr;
			}
		}

		AnimaAllocatorNamespace::DeallocateArray<AnimaShaderProgram*>(*(_engine->GetShadersAllocator()), _programs);
		_programs = nullptr;
	}

	_programsNumber = 0;
}

END_ANIMA_ENGINE_NAMESPACE