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
	if (_shaders != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateArray<AnimaShader*>(*(_engine->GetShadersAllocator()), _shaders);
		_shaders = nullptr;
	}

	_shadersNumber = 0;

	if (_programs != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateArray<AnimaShaderProgram*>(*(_engine->GetShadersAllocator()), _programs);
		_programs = nullptr;
	}

	_programsNumber = 0;
}

AnimaShader* AnimaShadersManager::CreateShader(bool allocate)
{
	ANIMA_ASSERT(_engine != nullptr);
	if (_shadersNumber > 0)
	{
		AnimaShader** tmpOldShaders = AnimaAllocatorNamespace::AllocateArray<AnimaShader*>(*(_engine->GetShadersAllocator()), _shadersNumber);

		for (int i = 0; i < _shadersNumber; i++)
			tmpOldShaders[i] = _shaders[i];

		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetModelDataAllocator()), _shaders);

		_shadersNumber++;
		_shaders = AnimaAllocatorNamespace::AllocateArray<AnimaShader*>(*(_engine->GetModelDataAllocator()), _shadersNumber);

		for (int i = 0; i < _shadersNumber - 1; i++)
			_shaders[i] = tmpOldShaders[i];
		
		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetModelDataAllocator()), tmpOldShaders);
	}
	else
	{
		_shadersNumber++;
		_shaders = AnimaAllocatorNamespace::AllocateArray<AnimaShader*>(*(_engine->GetModelDataAllocator()), _shadersNumber);
	}

	if (allocate)
		_shaders[_shadersNumber - 1] = AnimaAllocatorNamespace::AllocateNew<AnimaShader>(*(_engine->GetShadersAllocator()), _engine);

	return _shaders[_shadersNumber - 1];
}

AnimaShaderProgram* AnimaShadersManager::CreateProgram(bool allocate)
{
	ANIMA_ASSERT(_engine != nullptr);
	if (_programsNumber > 0)
	{
		AnimaShaderProgram** tmpOldPrograms = AnimaAllocatorNamespace::AllocateArray<AnimaShaderProgram*>(*(_engine->GetShadersAllocator()), _programsNumber);

		for (int i = 0; i < _programsNumber; i++)
			tmpOldPrograms[i] = _programs[i];

		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetModelDataAllocator()), _programs);

		_programsNumber++;
		_programs = AnimaAllocatorNamespace::AllocateArray<AnimaShaderProgram*>(*(_engine->GetModelDataAllocator()), _programsNumber);

		for (int i = 0; i < _programsNumber - 1; i++)
			_programs[i] = tmpOldPrograms[i];
		
		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetModelDataAllocator()), tmpOldPrograms);
	}
	else
	{
		_programsNumber++;
		_programs = AnimaAllocatorNamespace::AllocateArray<AnimaShaderProgram*>(*(_engine->GetModelDataAllocator()), _programsNumber);
	}

	if (allocate)
		_programs[_programsNumber - 1] = AnimaAllocatorNamespace::AllocateNew<AnimaShaderProgram>(*(_engine->GetShadersAllocator()), _engine);

	return _programs[_programsNumber - 1];
}

AnimaShader* AnimaShadersManager::LoadShader(AnimaString text, AnimaShader::AnimaShaderType type)
{
	return LoadShader(text.GetConstBuffer(), type);
}

AnimaShader* AnimaShadersManager::LoadShader(const AChar* text, AnimaShader::AnimaShaderType type)
{
	AnimaShader* sh = CreateShader(false);
	sh = AnimaAllocatorNamespace::AllocateNew<AnimaShader>(*(_engine->GetShadersAllocator()), _engine, text, type);
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

END_ANIMA_ENGINE_NAMESPACE