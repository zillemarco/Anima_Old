#include "AnimaShaderProgram.h"
#include "AnimaShadersManager.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaShaderProgram::AnimaShaderProgram(AnimaEngine* engine, AnimaShadersManager* shadersManager)
{
	_engine = engine;
	_shaders = nullptr;
	_shadersNumber = 0;
	_id = 0;
	_linked = false;

	_shadersManager = shadersManager;
}

AnimaShaderProgram::AnimaShaderProgram(const AnimaShaderProgram& src)
{
	_engine = src._engine;
	_id = src._id;
	_linked = src._linked;

	_shaders = nullptr;
	_shadersNumber = 0;

	_uniforms.clear();
	_uniforms = src._uniforms;

	_shadersManager = src._shadersManager;

	SetShaders(src._shaders, src._shadersNumber);
}

AnimaShaderProgram::AnimaShaderProgram(AnimaShaderProgram&& src)
	: _engine(src._engine)
	, _shaders(src._shaders)
	, _shadersNumber(src._shadersNumber)
	, _id(src._id)
	, _linked(src._linked)
{
	_uniforms.clear();
	_uniforms = src._uniforms;

	_shadersManager = src._shadersManager;

	src._shaders = nullptr;
	src._shadersNumber = 0;
	src._id = 0;
}

AnimaShaderProgram::~AnimaShaderProgram()
{
	ANIMA_ASSERT(_engine != nullptr);

	if (_shadersNumber > 0 && _shaders != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateArray<AnimaShader*>(*(_engine->GetShadersAllocator()), _shaders);
		_shaders = nullptr;
		_shadersNumber = 0;
	}

	Delete();
}

AnimaShaderProgram& AnimaShaderProgram::operator=(const AnimaShaderProgram& src)
{
	if (this != &src)
	{
		SetShaders(src._shaders, src._shadersNumber);

		_uniforms.clear();
		_uniforms = src._uniforms;

		_shadersManager = src._shadersManager;

		_engine = src._engine;
		_id = src._id;
		_linked = src._linked;
	}

	return *this;
}

AnimaShaderProgram& AnimaShaderProgram::operator=(AnimaShaderProgram&& src)
{
	if (this != &src)
	{
		SetShaders(src._shaders, src._shadersNumber);

		_uniforms.clear();
		_uniforms = src._uniforms;

		_shadersManager = src._shadersManager;

		_engine = src._engine;
		_id = src._id;
		_linked = src._linked;

		src._id = 0;
		src._shaders = nullptr;
		src._shadersNumber = 0;
		src._linked = false;
	}

	return *this;
}

inline bool AnimaShaderProgram::operator==(const AnimaShaderProgram& left)
{
	if (_id != left._id) return false;
	if (_shadersManager != left._shadersManager) return false;
	if (_engine != left._engine) return false;
	if (_shadersNumber != left._shadersNumber) return false; 
	if (_linked != left._linked) return false;

	for (int i = 0; i < _shadersNumber; i++)
	{
		if (_shaders[i] != left._shaders[i])
			return false;
	}

	return true;
}

inline bool AnimaShaderProgram::operator!=(const AnimaShaderProgram& left)
{
	if (_id != left._id) return true;
	if (_shadersManager != left._shadersManager) return true;
	if (_engine != left._engine) return true;
	if (_shadersNumber != left._shadersNumber) return true;
	if (_linked != left._linked) return true;

	for (int i = 0; i < _shadersNumber; i++)
	{
		if (_shaders[i] != left._shaders[i])
			return true;
	}

	return false;
}

void AnimaShaderProgram::AddShader(AnimaShader* shader)
{
	ANIMA_ASSERT(_engine != nullptr);
	if (_shadersNumber > 0)
	{
		AnimaShader** tmpOldShaders = AnimaAllocatorNamespace::AllocateArray<AnimaShader*>(*(_engine->GetShadersAllocator()), _shadersNumber);

		for (int i = 0; i < _shadersNumber; i++)
			tmpOldShaders[i] = _shaders[i];

		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetShadersAllocator()), _shaders);

		_shadersNumber++;
		_shaders = AnimaAllocatorNamespace::AllocateArray<AnimaShader*>(*(_engine->GetShadersAllocator()), _shadersNumber);

		for (int i = 0; i < _shadersNumber - 1; i++)
			_shaders[i] = tmpOldShaders[i];

		_shaders[_shadersNumber - 1] = shader;

		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetShadersAllocator()), tmpOldShaders);
	}
	else
	{
		_shadersNumber++;
		_shaders = AnimaAllocatorNamespace::AllocateArray<AnimaShader*>(*(_engine->GetShadersAllocator()), _shadersNumber);

		_shaders[_shadersNumber - 1] = shader;
	}
}

void AnimaShaderProgram::SetShaders(AnimaShader** shaders, ASizeT count)
{
	ANIMA_ASSERT(_engine != nullptr)
	ClearShaders();

	if (shaders != nullptr && count > 0)
	{
		_shadersNumber = count;
		_shaders = AnimaAllocatorNamespace::AllocateArray<AnimaShader*>(*(_engine->GetShadersAllocator()), count);

		for (int i = 0; i < _shadersNumber; i++)
			_shaders[i] = shaders[i];
	}
}

void AnimaShaderProgram::ClearShaders()
{
	if (_shaders != nullptr && _shadersNumber > 0)
	{
		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetShadersAllocator()), _shaders);
		_shaders = nullptr;
		_shadersNumber = 0;
	}
}

bool AnimaShaderProgram::CompileShaders()
{
	for (int i = 0; i < _shadersNumber; i++)
	{
		if (!_shaders[i]->Compile())
			return false;
	}

	return true;
}

bool AnimaShaderProgram::Create()
{
	if (_id > 0)
		return true;

	_id = glCreateProgram();

	if (_id == 0)
		return false;
	return true;
}

bool AnimaShaderProgram::Link()
{
	if (!CompileShaders())
		return false;

	if (!IsCreated())
	{
		if (!Create())
			return false;
	}
	
	if (IsLinked())
		return true;

	if (glGetError() != GL_NO_ERROR)
		return false;

	for (int i = 0; i < _shadersNumber; i++)
		glAttachShader(_id, _shaders[i]->GetID());
	
	glLinkProgram(_id);

	GLint isLinked = 0;
	glGetProgramiv(_id, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &maxLength);

		char* infoLog = new char[maxLength];
		glGetProgramInfoLog(_id, maxLength, &maxLength, &infoLog[0]);

		ANIMA_ASSERT(false);// , infoLog);

		Delete();

		_linked = false;
	}
	else
	{
		_linked = true;

		ScanVariables();
	}

	return _linked;
}

bool AnimaShaderProgram::Use()
{
	if (!IsLinked())
	{
		if (!Link())
			return false;
	}

	_shadersManager->NotifyProgramActivation(this);
	glUseProgram(_id);

	return true;
}

bool AnimaShaderProgram::IsCreated()
{
	return _id > 0;
}

bool AnimaShaderProgram::IsLinked()
{
	return _linked;
}

bool AnimaShaderProgram::Delete()
{
	if (IsCreated())
	{
		glDeleteProgram(_id);
		if (glGetError() != GL_NO_ERROR)
			return false;
	}
	return true;
}

AInt AnimaShaderProgram::GetID()
{
	return _id;
}

void AnimaShaderProgram::SetUniformi(const AnimaString& uniformName, int value)
{
	if (_uniforms.find(uniformName) != _uniforms.end())
		glUniform1i(_uniforms.at(uniformName)._location, value);
}

void AnimaShaderProgram::SetUniformi(const char* uniformName, int value)
{
	AnimaString str(uniformName, _engine);
	SetUniformi(str, value);
}

void AnimaShaderProgram::SetUniformf(const AnimaString& uniformName, AFloat value)
{
	if (_uniforms.find(uniformName) != _uniforms.end())
		glUniform1f(_uniforms.at(uniformName)._location, value);
}

void AnimaShaderProgram::SetUniformf(const char* uniformName, AFloat value)
{
	AnimaString str(uniformName, _engine);
	SetUniformf(str, value);
}

void AnimaShaderProgram::SetUniform(const AnimaString& uniformName, const AnimaVertex2f& value)
{
	if (_uniforms.find(uniformName) != _uniforms.end())
		glUniform2f(_uniforms.at(uniformName)._location, value.x, value.y);
}

void AnimaShaderProgram::SetUniform(const char* uniformName, const AnimaVertex2f& value)
{
	AnimaString str(uniformName, _engine);
	SetUniform(str, value);
}

void AnimaShaderProgram::SetUniform(const AnimaString& uniformName, const AnimaVertex3f& value)
{
	if (_uniforms.find(uniformName) != _uniforms.end())
		glUniform3f(_uniforms.at(uniformName)._location, value.x, value.y, value.z);
}

void AnimaShaderProgram::SetUniform(const char* uniformName, const AnimaVertex3f& value)
{
	AnimaString str(uniformName, _engine);
	SetUniform(str, value);
}

void AnimaShaderProgram::SetUniform(const AnimaString& uniformName, const AnimaColor4f& value)
{
	if (_uniforms.find(uniformName) != _uniforms.end())
		glUniform4f(_uniforms.at(uniformName)._location, value.x, value.y, value.z, value.w);
}

void AnimaShaderProgram::SetUniform(const char* uniformName, const AnimaColor4f& value)
{
	AnimaString str(uniformName, _engine);
	SetUniform(str, value);
}

void AnimaShaderProgram::SetUniform(const AnimaString& uniformName, AFloat a, AFloat b, AFloat c)
{
	if (_uniforms.find(uniformName) != _uniforms.end())
		glUniform3f(_uniforms.at(uniformName)._location, a, b, c);
}

void AnimaShaderProgram::SetUniform(const char* uniformName, AFloat a, AFloat b, AFloat c)
{
	AnimaString str(uniformName, _engine);
	SetUniform(str, a, b, c);
}

void AnimaShaderProgram::SetUniform(const AnimaString& uniformName, AFloat a, AFloat b, AFloat c, AFloat d)
{
	if (_uniforms.find(uniformName) != _uniforms.end())
		glUniform4f(_uniforms.at(uniformName)._location, a, b, c, d);
}

void AnimaShaderProgram::SetUniform(const char* uniformName, AFloat a, AFloat b, AFloat c, AFloat d)
{
	AnimaString str(uniformName, _engine);
	SetUniform(str, a, b, c, d);
}

void AnimaShaderProgram::SetUniform(const AnimaString& uniformName, const AnimaMatrix& value, bool transpose)
{
	if (_uniforms.find(uniformName) != _uniforms.end())
		glUniformMatrix4fv(_uniforms.at(uniformName)._location, 1, transpose ? GL_TRUE : GL_FALSE, value.m);
}

void AnimaShaderProgram::SetUniform(const char* uniformName, const AnimaMatrix& value, bool transpose)
{
	AnimaString str(uniformName, _engine);
	SetUniform(str, value, transpose);
}

void AnimaShaderProgram::ScanVariables()
{
	_uniforms.clear();
	_inputs.clear(); 
	_outputs.clear();

	GLint numActiveUniforms = 0;
	glGetProgramInterfaceiv(_id, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numActiveUniforms);

	const int propertiesSize = 3;

	AnimaString name(_engine);
	GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION };
	GLint values[propertiesSize];

	for (int i = 0; i < numActiveUniforms; i++)
	{
		glGetProgramResourceiv(_id, GL_UNIFORM, i, propertiesSize, &properties[0], propertiesSize, NULL, &values[0]);

		name.Reserve(values[0] - 1);
		glGetProgramResourceName(_id, GL_UNIFORM, i, values[0], NULL, name.GetBuffer());

		AnimaUniformInfo info;
		info._location = values[2];
		info._type = values[1];
		info._name = name;

		_uniforms[name] = info;
	}

	GLint numActiveInputs = 0;
	glGetProgramInterfaceiv(_id, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numActiveInputs);

	for (int i = 0; i < numActiveInputs; i++)
	{
		glGetProgramResourceiv(_id, GL_PROGRAM_INPUT, i, propertiesSize, &properties[0], propertiesSize, NULL, &values[0]);

		name.Reserve(values[0] - 1);
		glGetProgramResourceName(_id, GL_PROGRAM_INPUT, i, values[0], NULL, name.GetBuffer());

		AnimaInputInfo info;
		info._location = values[2];
		info._type = values[1];
		info._name = name;

		_inputs[name] = info;
	}

	GLint numActiveOutputs = 0;
	glGetProgramInterfaceiv(_id, GL_PROGRAM_OUTPUT, GL_ACTIVE_RESOURCES, &numActiveOutputs);

	for (int i = 0; i < numActiveOutputs; i++)
	{
		glGetProgramResourceiv(_id, GL_PROGRAM_OUTPUT, i, propertiesSize, &properties[0], propertiesSize, NULL, &values[0]);

		name.Reserve(values[0] - 1);
		glGetProgramResourceName(_id, GL_PROGRAM_OUTPUT, i, values[0], NULL, name.GetBuffer());

		AnimaOutputInfo info;
		info._location = values[2];
		info._type = values[1];
		info._name = name;

		_outputs[name] = info;
	}
}

void AnimaShaderProgram::EnableInputs(AnimaMesh* mesh)
{
	glBindVertexArray(mesh->GetVertexArrayObject());

	for (auto key : _inputs)
	{
		AnimaInputInfo info = key.second;

		if (info._name == "_position")
		{
			glBindBuffer(GL_ARRAY_BUFFER, mesh->GetVerticesBufferObject());
			glEnableVertexAttribArray(info._location);
			glVertexAttribPointer(info._location, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}
		else if (info._name == "_normal")
		{
			if (mesh->GetFloatVerticesNormalCount() > 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, mesh->GetNormalsBufferObject());
				glEnableVertexAttribArray(info._location);
				glVertexAttribPointer(info._location, 3, GL_FLOAT, GL_FALSE, 0, 0);
			}
		}
		else if (info._name == "_textureCoord")
		{
			if (mesh->GetFloatVerticesTextureCoordCount() > 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, mesh->GetTextureCoordsBufferObject());
				glEnableVertexAttribArray(info._location);
				glVertexAttribPointer(info._location, 2, GL_FLOAT, GL_FALSE, 0, 0);
			}
		}
	}
}

void AnimaShaderProgram::DisableInputs()
{
	for (auto key : _inputs)
	{
		AnimaInputInfo info = key.second;
		glDisableVertexAttribArray(info._location);
	}
}

END_ANIMA_ENGINE_NAMESPACE