#include "AnimaShaderProgram.h"
#include "AnimaShadersManager.h"
#include "AnimaLightsManager.h"
#include "AnimaRenderingManager.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

#if !defined UPD_ERROR
#	define UPD_ERROR ANIMA_ASSERT(false)
#endif

const char* ModelPrefix				= "MOD";
const char* CameraPrefix			= "CAM";
const char* MaterialPrefix			= "MAT";
const char* RenderingEnginePrefix	= "REN";
const char* GBufferPrefix			= "GB";

AnimaShaderProgram::AnimaShaderProgram(AnimaAllocator* allocator, AnimaShadersManager* shadersManager)
{
	_allocator = allocator;
	_shaders = nullptr;
	_shadersNumber = 0;
	_id = 0;
	_linked = false;

	_maxPointLights = 0;
	_maxSpotLights = 0;

	_shadersManager = shadersManager;
}

AnimaShaderProgram::AnimaShaderProgram(const AnimaShaderProgram& src)
{
	_allocator = src._allocator;
	_id = src._id;
	_linked = src._linked;

	_shaders = nullptr;
	_shadersNumber = 0;

	ClearUniforms();
	_uniforms = src._uniforms;

	_shadersManager = src._shadersManager;

	_maxPointLights = src._maxPointLights;
	_maxSpotLights = src._maxSpotLights;

	SetShaders(src._shaders, src._shadersNumber);
}

AnimaShaderProgram::AnimaShaderProgram(AnimaShaderProgram&& src)
	: _allocator(src._allocator)
	, _shaders(src._shaders)
	, _shadersNumber(src._shadersNumber)
	, _id(src._id)
	, _linked(src._linked)
{
	ClearUniforms();
	_uniforms = src._uniforms;

	_shadersManager = src._shadersManager;
	_maxPointLights = src._maxPointLights;
	_maxSpotLights = src._maxSpotLights;

	src._shaders = nullptr;
	src._shadersNumber = 0;
	src._id = 0;
}

AnimaShaderProgram::~AnimaShaderProgram()
{
	ANIMA_ASSERT(_allocator != nullptr);

	ClearUniforms();

	if (_shadersNumber > 0 && _shaders != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateArray<AnimaShader*>(*_allocator, _shaders);
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

		ClearUniforms();
		_uniforms = src._uniforms;

		_shadersManager = src._shadersManager;
		_maxPointLights = src._maxPointLights;
		_maxSpotLights = src._maxSpotLights;

		_allocator = src._allocator;
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

		ClearUniforms();
		_uniforms = src._uniforms;

		_shadersManager = src._shadersManager;
		_maxPointLights = src._maxPointLights;
		_maxSpotLights = src._maxSpotLights;

		_allocator = src._allocator;
		_id = src._id;
		_linked = src._linked;

		src._id = 0;
		src._shaders = nullptr;
		src._shadersNumber = 0;
		src._linked = false;
	}

	return *this;
}

bool AnimaShaderProgram::operator==(const AnimaShaderProgram& left)
{
	if (_id != left._id) return false;
	if (_shadersManager != left._shadersManager) return false;
	if (_allocator != left._allocator) return false;
	if (_shadersNumber != left._shadersNumber) return false; 
	if (_linked != left._linked) return false;
	if(_maxPointLights != left._maxPointLights) return false;
	if(_maxSpotLights != left._maxSpotLights) return false;

	for (int i = 0; i < _shadersNumber; i++)
	{
		if (_shaders[i] != left._shaders[i])
			return false;
	}

	return true;
}

bool AnimaShaderProgram::operator!=(const AnimaShaderProgram& left)
{
	if (_id != left._id) return true;
	if (_shadersManager != left._shadersManager) return true;
	if (_allocator != left._allocator) return true;
	if (_shadersNumber != left._shadersNumber) return true;
	if (_linked != left._linked) return true;
	if (_maxPointLights != left._maxPointLights) return true;
	if (_maxSpotLights != left._maxSpotLights) return true;

	for (int i = 0; i < _shadersNumber; i++)
	{
		if (_shaders[i] != left._shaders[i])
			return true;
	}

	return false;
}

void AnimaShaderProgram::AddShader(AnimaShader* shader)
{
	ANIMA_ASSERT(_allocator != nullptr);
	if (_shadersNumber > 0)
	{
		AnimaShader** tmpOldShaders = AnimaAllocatorNamespace::AllocateArray<AnimaShader*>(*_allocator, _shadersNumber);

		for (int i = 0; i < _shadersNumber; i++)
			tmpOldShaders[i] = _shaders[i];

		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _shaders);

		_shadersNumber++;
		_shaders = AnimaAllocatorNamespace::AllocateArray<AnimaShader*>(*_allocator, _shadersNumber);

		for (int i = 0; i < _shadersNumber - 1; i++)
			_shaders[i] = tmpOldShaders[i];

		_shaders[_shadersNumber - 1] = shader;

		AnimaAllocatorNamespace::DeallocateArray(*_allocator, tmpOldShaders);
	}
	else
	{
		_shadersNumber++;
		_shaders = AnimaAllocatorNamespace::AllocateArray<AnimaShader*>(*_allocator, _shadersNumber);

		_shaders[_shadersNumber - 1] = shader;
	}
}

void AnimaShaderProgram::SetShaders(AnimaShader** shaders, ASizeT count)
{
	ANIMA_ASSERT(_allocator != nullptr)
	ClearShaders();

	if (shaders != nullptr && count > 0)
	{
		_shadersNumber = count;
		_shaders = AnimaAllocatorNamespace::AllocateArray<AnimaShader*>(*_allocator, count);

		for (int i = 0; i < _shadersNumber; i++)
			_shaders[i] = shaders[i];
	}
}

void AnimaShaderProgram::ClearShaders()
{
	if (_shaders != nullptr && _shadersNumber > 0)
	{
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _shaders);
		_shaders = nullptr;
		_shadersNumber = 0;
	}
}

void AnimaShaderProgram::ClearUniforms()
{
	for (auto& pair : _uniforms)
	{
		if (pair.second._nameParts != nullptr)
		{
			AnimaAllocatorNamespace::DeallocateArray(*_allocator, pair.second._nameParts);
			pair.second._nameParts = nullptr;
			pair.second._namePartsCount = 0;
		}
	}
	_uniforms.clear();
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
	auto pair = _uniforms.find(uniformName);
	if (pair != _uniforms.end())
		glUniform1i(pair->second._location, value);
}

void AnimaShaderProgram::SetUniformi(const char* uniformName, int value)
{
	AnimaString str(uniformName, _allocator);
	SetUniformi(str, value);
}

void AnimaShaderProgram::SetUniformf(const AnimaString& uniformName, AFloat value)
{
	auto pair = _uniforms.find(uniformName);
	if (pair != _uniforms.end())
		glUniform1f(pair->second._location, value);
}

void AnimaShaderProgram::SetUniformf(const char* uniformName, AFloat value)
{
	AnimaString str(uniformName, _allocator);
	SetUniformf(str, value);
}

void AnimaShaderProgram::SetUniform(const AnimaString& uniformName, const AnimaVertex2f& value)
{
	auto pair = _uniforms.find(uniformName);
	if (pair != _uniforms.end())
		glUniform2f(pair->second._location, value.x, value.y);
}

void AnimaShaderProgram::SetUniform(const char* uniformName, const AnimaVertex2f& value)
{
	AnimaString str(uniformName, _allocator);
	SetUniform(str, value);
}

void AnimaShaderProgram::SetUniform(const AnimaString& uniformName, const AnimaVertex3f& value)
{
	auto pair = _uniforms.find(uniformName);
	if (pair != _uniforms.end())
		glUniform3f(pair->second._location, value.x, value.y, value.z);
}

void AnimaShaderProgram::SetUniform(const char* uniformName, const AnimaVertex3f& value)
{
	AnimaString str(uniformName, _allocator);
	SetUniform(str, value);
}

void AnimaShaderProgram::SetUniform(const AnimaString& uniformName, const AnimaColor4f& value)
{
	auto pair = _uniforms.find(uniformName);
	if (pair != _uniforms.end())
		glUniform4f(pair->second._location, value.x, value.y, value.z, value.w);
}

void AnimaShaderProgram::SetUniform(const char* uniformName, const AnimaColor4f& value)
{
	AnimaString str(uniformName, _allocator);
	SetUniform(str, value);
}

void AnimaShaderProgram::SetUniform(const AnimaString& uniformName, AFloat a, AFloat b, AFloat c)
{
	auto pair = _uniforms.find(uniformName);
	if (pair != _uniforms.end())
		glUniform3f(pair->second._location, a, b, c);
}

void AnimaShaderProgram::SetUniform(const char* uniformName, AFloat a, AFloat b, AFloat c)
{
	AnimaString str(uniformName, _allocator);
	SetUniform(str, a, b, c);
}

void AnimaShaderProgram::SetUniform(const AnimaString& uniformName, AFloat a, AFloat b, AFloat c, AFloat d)
{
	auto pair = _uniforms.find(uniformName);
	if (pair != _uniforms.end())
		glUniform4f(pair->second._location, a, b, c, d);
}

void AnimaShaderProgram::SetUniform(const char* uniformName, AFloat a, AFloat b, AFloat c, AFloat d)
{
	AnimaString str(uniformName, _allocator);
	SetUniform(str, a, b, c, d);
}

void AnimaShaderProgram::SetUniform(const AnimaString& uniformName, const AnimaMatrix& value, bool transpose)
{
	auto pair = _uniforms.find(uniformName);
	if (pair != _uniforms.end())
		glUniformMatrix4fv(pair->second._location, 1, transpose ? GL_TRUE : GL_FALSE, value.m);
}

void AnimaShaderProgram::SetUniform(const char* uniformName, const AnimaMatrix& value, bool transpose)
{
	AnimaString str(uniformName, _allocator);
	SetUniform(str, value, transpose);
}

void AnimaShaderProgram::SetUniformi(AInt location, int value)
{
	glUniform1i(location, value);
}

void AnimaShaderProgram::SetUniformf(AInt location, AFloat value)
{
	glUniform1f(location, value);
}

void AnimaShaderProgram::SetUniform(AInt location, const AnimaVertex2f& value)
{
	glUniform2f(location, value.x, value.y);
}

void AnimaShaderProgram::SetUniform(AInt location, const AnimaVertex3f& value)
{
	glUniform3f(location, value.x, value.y, value.z);
}

void AnimaShaderProgram::SetUniform(AInt location, const AnimaColor4f& value)
{
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void AnimaShaderProgram::SetUniform(AInt location, AFloat a, AFloat b, AFloat c)
{
	glUniform3f(location, a, b, c);
}

void AnimaShaderProgram::SetUniform(AInt location, AFloat a, AFloat b, AFloat c, AFloat d)
{
	glUniform4f(location, a, b, c, d);
}

void AnimaShaderProgram::SetUniform(AInt location, const AnimaMatrix& value, bool transpose)
{
	glUniformMatrix4fv(location, 1, transpose ? GL_TRUE : GL_FALSE, value.m);
}

void AnimaShaderProgram::ScanVariables()
{
	ClearUniforms();
	_inputs.clear(); 
	_outputs.clear();

	GLint numActiveUniforms = 0;
	GLint numActiveOutputs = 0;
	GLint numActiveInputs = 0;

	if (GLEW_ARB_program_interface_query)
	{
		glGetProgramInterfaceiv(_id, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numActiveUniforms);
		glGetProgramInterfaceiv(_id, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numActiveInputs);
		glGetProgramInterfaceiv(_id, GL_PROGRAM_OUTPUT, GL_ACTIVE_RESOURCES, &numActiveOutputs);

		const int propertiesSize = 3;

		AnimaString name(_allocator);
		AnimaString namePart1(_allocator);
		AnimaString namePart2(_allocator);
		GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, };
		GLint values[propertiesSize];

		for (int i = 0; i < numActiveUniforms; i++)
		{
			glGetProgramResourceiv(_id, GL_UNIFORM, i, propertiesSize, &properties[0], propertiesSize, NULL, &values[0]);

			name.Reserve(values[0]);
			glGetProgramResourceName(_id, GL_UNIFORM, i, values[0] + 1, NULL, name.GetBuffer());
			name.Trim();

			namePart1 = name;

			AUint offset = namePart1.CountOf('_') + 1;
			AnimaString* nameParts = AnimaAllocatorNamespace::AllocateArray<AnimaString>(*_allocator, offset, _allocator);

			offset = 0;
			int pos = namePart1.Find('_');
			while (pos != -1)
			{
				namePart2 = namePart1.Substring(0, pos);
				namePart1 = namePart1.Substring(pos + 1, namePart1.GetBufferLength());

				if (!namePart2.IsEmpty())
					nameParts[offset++] = namePart2;
				pos = namePart1.Find('_');
			}

			nameParts[offset++] = namePart1;

			AnimaUniformInfo info;
			info._location = values[2];
			info._type = values[1];
			info._name = name;
			info._nameParts = nameParts;
			info._namePartsCount = offset;

			_uniforms[name] = info;
		}

		for (int i = 0; i < numActiveInputs; i++)
		{
			glGetProgramResourceiv(_id, GL_PROGRAM_INPUT, i, propertiesSize, &properties[0], propertiesSize, NULL, &values[0]);

			name.Reserve(values[0]);
			glGetProgramResourceName(_id, GL_PROGRAM_INPUT, i, values[0] + 1, NULL, name.GetBuffer());
			name.Trim();

			AnimaInputInfo info;
			info._location = values[2];
			info._type = values[1];
			info._name = name;

			_inputs[name] = info;
		}

		for (int i = 0; i < numActiveOutputs; i++)
		{
			glGetProgramResourceiv(_id, GL_PROGRAM_OUTPUT, i, propertiesSize, &properties[0], propertiesSize, NULL, &values[0]);

			name.Reserve(values[0]);
			glGetProgramResourceName(_id, GL_PROGRAM_OUTPUT, i, values[0] + 1, NULL, name.GetBuffer());
			name.Trim();

			AnimaOutputInfo info;
			info._location = values[2];
			info._type = values[1];
			info._name = name;

			_outputs[name] = info;
		}
	}
	else
	{
		glGetProgramiv(_id, GL_ACTIVE_UNIFORMS, &numActiveUniforms);
		glGetProgramiv(_id, GL_ACTIVE_ATTRIBUTES, &numActiveInputs);
		//glGetProgramiv(_id, GL_ACTIVE_RESOURCES, &numActiveOutputs);

		AnimaString name(_allocator);
		AnimaString namePart1(_allocator);
		AnimaString namePart2(_allocator);
		AnimaString tmp(_allocator);
		GLenum type;
		GLsizei bufLen;
		GLsizei elements;
		GLint location;

		tmp.Reserve(200);

		for (int i = 0; i < numActiveUniforms; i++)
		{
			glGetActiveUniform(_id, i, (int)tmp.GetBufferLength() - 1, &bufLen, &elements, &type, tmp.GetBuffer());
			name.Reserve(bufLen);
			name.SetString(tmp.GetBuffer());
			name.Trim();
			
			location = glGetUniformLocation(_id, name.GetConstBuffer());

			namePart1 = name;

			AUint offset = namePart1.CountOf('_') + 1;
			AnimaString* nameParts = AnimaAllocatorNamespace::AllocateArray<AnimaString>(*_allocator, offset, _allocator);

			offset = 0;
			int pos = namePart1.Find('_');
			while (pos != -1)
			{
				namePart2 = namePart1.Substring(0, pos);
				namePart1 = namePart1.Substring(pos + 1, namePart1.GetBufferLength());

				if (!namePart2.IsEmpty())
					nameParts[offset++] = namePart2;
				pos = namePart1.Find('_');
			}

			nameParts[offset++] = namePart1;

			AnimaUniformInfo info;
			info._location = location;
			info._type = type;
			info._name = name;
			info._nameParts = nameParts;
			info._namePartsCount = offset;

			_uniforms[name] = info;
		}

		for (int i = 0; i < numActiveInputs; i++)
		{
			glGetActiveAttrib(_id, i, (int)tmp.GetBufferLength() - 1, &bufLen, &elements, &type, tmp.GetBuffer());
			name.Reserve(bufLen);
			name.SetString(tmp.GetBuffer());
			
			location = glGetAttribLocation(_id, name.GetConstBuffer());

			AnimaInputInfo info;
			info._location = location;
			info._type = type;
			info._name = name;

			_inputs[name] = info;
		}
	}
		
	_maxPointLights = 0;
	_maxSpotLights = 0;

	AnimaString str(_allocator);
	bool stop = false;

	while (!stop)
	{
		str.Format("_pointLight[%d].position", _maxPointLights);
		if (_uniforms.find(str) != _uniforms.end())
			_maxPointLights++;
		else
			stop = true;
	}

	stop = false;
	while (!stop)
	{
		str.Format("_spotLight[%d].direction", _maxSpotLights);
		if (_uniforms.find(str) != _uniforms.end())
			_maxSpotLights++;
		else
			stop = true;
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
		else if (info._name == "_tangent")
		{
			if (mesh->GetFloatVerticesTangentsCount() > 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, mesh->GetTangentsBufferObject());
				glEnableVertexAttribArray(info._location);
				glVertexAttribPointer(info._location, 3, GL_FLOAT, GL_FALSE, 0, 0);
			}
		}
		else if (info._name == "_bitangent")
		{
			if (mesh->GetFloatVerticesBitangentsCount() > 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, mesh->GetBitangentsBufferObject());
				glEnableVertexAttribArray(info._location);
				glVertexAttribPointer(info._location, 3, GL_FLOAT, GL_FALSE, 0, 0);
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

void AnimaShaderProgram::UpdateMeshProperies(AnimaMesh* mesh, const AnimaMatrix& transformation)
{
	AnimaString str(_allocator);
	AnimaUniformInfo info;
	auto end = _uniforms.end();

	str = "MOD_modelMatrix";
	if (_uniforms.find(str) != end)
	{
		info = _uniforms[str];

		if (info._type == GL_FLOAT_MAT4)
			SetUniform(info._location, transformation);
		else
		{
			UPD_ERROR;
		}
	}
}

void AnimaShaderProgram::UpdateCameraProperies(AnimaCamera* camera)
{
	AnimaString str(_allocator);
	AnimaUniformInfo info;
	auto end = _uniforms.end();

	str = "CAM_Position";
	if (_uniforms.find(str) != end)
	{
		info = _uniforms[str];

		if (info._type == GL_FLOAT_VEC3)
			SetUniform(info._location, camera->GetPosition());
		else
		{
			UPD_ERROR;
		}
	}

	str = "CAM_windowSize";
	if (_uniforms.find(str) != end)
	{
		info = _uniforms[str];

		if (info._type == GL_FLOAT_VEC2)
			SetUniform(info._location, camera->GetWindowSize());
		else
		{
			UPD_ERROR;
		}
	}

	str = "CAM_inverseWindowSize";
	if (_uniforms.find(str) != end)
	{
		info = _uniforms[str];

		if (info._type == GL_FLOAT_VEC2)
			SetUniform(info._location, AnimaVertex2f(1.0f / camera->GetWindowSize().x, 1.0f / camera->GetWindowSize().y));
		else
		{
			UPD_ERROR;
		}
	}

	str = "CAM_viewMatrix";
	if (_uniforms.find(str) != end)
	{
		info = _uniforms[str];

		if (info._type == GL_FLOAT_MAT4)
			SetUniform(info._location, camera->GetViewMatrix());
		else
		{
			UPD_ERROR;
		}
	}

	str = "CAM_projectionMatrix";
	if (_uniforms.find(str) != end)
	{
		info = _uniforms[str];

		if (info._type == GL_FLOAT_MAT4)
			SetUniform(info._location, camera->GetProjectionMatrix());
		else
		{
			UPD_ERROR;
		}
	}

	str = "CAM_projectionViewMatrix";
	if (_uniforms.find(str) != end)
	{
		info = _uniforms[str];
		
		if (info._type == GL_FLOAT_MAT4)
			SetUniform(info._location, camera->GetProjectionViewMatrix());
		else
		{
			UPD_ERROR;
		}
	}
}

void AnimaShaderProgram::UpdateMaterialProperies(AnimaMaterial* material, AnimaRenderingManager* renderingManager)
{
	if (material == nullptr)
		return;

	for (auto& pair : _uniforms)
	{
		AnimaUniformInfo info = pair.second;

		if (info._namePartsCount != 2 || info._nameParts[0] != MaterialPrefix)
			continue;
		
		if (info._type == GL_FLOAT_VEC3)
			SetUniform(info._location, material->GetColor3f(info._nameParts[1]));
		else if (info._type == GL_FLOAT_VEC4)
			SetUniform(info._location, material->GetColor4f(info._nameParts[1]));
		else if (info._type == GL_FLOAT)
			SetUniformf(info._location, material->GetFloat(info._nameParts[1]));
		else if (info._type == GL_BOOL)
			SetUniformi(info._location, material->GetBoolean(info._nameParts[1]) ? 1 : 0);
		else if (info._type == GL_INT)
			SetUniformi(info._location, material->GetInteger(info._nameParts[1]));
		else if (info._type == GL_SAMPLER_2D)
		{
			AnimaTexture* texture = material->GetTexture(info._nameParts[1]);

			AUint slot = renderingManager->GetTextureSlot(info._nameParts[1]);
			SetUniformi(info._location, slot);

			if (texture == nullptr)
			{
				glActiveTexture(GL_TEXTURE0 + slot);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			else
			{
				texture->Load();
				texture->Bind(slot);
			}
		}
		else
		{
			ANIMA_ASSERT(false);
		}
	}

	if (material->GetBoolean("TwoSided"))
		glDisable(GL_CULL_FACE);
	else
		glEnable(GL_CULL_FACE);

	glFrontFace(material->GetInteger("FrontFace"));
	glCullFace(material->GetInteger("CullFace"));
}

void AnimaShaderProgram::UpdateLightProperies(AnimaLight* light)
{
	SetUniform("LIG_ProjectionViewMatrix", light->GetProjectionViewMatrix());

	if (light->IsAmbientLight())
	{
		SetUniform("AML_Color", light->GetColor());
		//SetUniform("_ambientLight", light->GetColor());
	}
	else if (light->IsDirectionalLight())
	{
		SetUniform("DIL_Direction", light->GetDirection());
		SetUniform("DIL_Color", light->GetColor());
		SetUniformf("DIL_Intensity", light->GetIntensity());
//		SetUniform("_directionalLight.direction", light->GetDirection());
//		SetUniform("_directionalLight.base.color", light->GetColor());
//		SetUniformf("_directionalLight.base.intensity", light->GetIntensity());
	}
	else if (light->IsPointLight())
	{
		SetUniform("PTL_Position", light->GetPosition());
		SetUniformf("PTL_Range", light->GetRange());
		SetUniform("PTL_Color", light->GetColor());
		SetUniformf("PTL_Intensity", light->GetIntensity());
		SetUniformf("PTL_ConstantAttenuation", light->GetConstantAttenuation());
		SetUniformf("PTL_LinearAttenuation", light->GetLinearAttenuation());
		SetUniformf("PTL_ExponentAttenuation", light->GetExponentAttenuation());
		//SetUniform("_pointLight.position", light->GetPosition());
		//SetUniformf("_pointLight.range", light->GetRange());
		//SetUniform("_pointLight.base.color", light->GetColor());
		//SetUniformf("_pointLight.base.intensity", light->GetIntensity());
		//SetUniformf("_pointLight.attenuation.constant", light->GetConstantAttenuation());
		//SetUniformf("_pointLight.attenuation.linear", light->GetLinearAttenuation());
		//SetUniformf("_pointLight.attenuation.exponent", light->GetExponentAttenuation());
	}
	else if (light->IsSpotLight())
	{
		SetUniform("_spotLight.direction", light->GetDirection());
		SetUniformf("_spotLight.cutoff", light->GetCutoff());
		SetUniform("_spotLight.pointLight.position", light->GetPosition());
		SetUniformf("_spotLight.pointLight.range", light->GetRange());
		SetUniform("_spotLight.pointLight.base.color", light->GetColor());
		SetUniformf("_spotLight.pointLight.base.intensity", light->GetIntensity());
		SetUniformf("_spotLight.pointLight.attenuation.constant", light->GetConstantAttenuation());
		SetUniformf("_spotLight.pointLight.attenuation.linear", light->GetLinearAttenuation());
		SetUniformf("_spotLight.pointLight.attenuation.exponent", light->GetExponentAttenuation());
	}
}

void AnimaShaderProgram::UpdateLightsProperies(AnimaStage* stage)
{
	AnimaLightsManager* lightsManager = stage->GetLightsManager();

	AInt lightsCount = lightsManager->GetTotalLightsCount();

	if (lightsCount <= 0)
		return;

	int nextPointLight = 0;
	int nextSpotLight = 0;

	AnimaString str(_allocator);
	AnimaLight* light = nullptr;
	
	for (int i = 0; i < lightsCount; i++)
	{
		light = lightsManager->GetLight(i);

		if (light->IsAmbientLight())
		{
			SetUniform("_ambientLight", light->GetColor());
		}
		else if (light->IsDirectionalLight())
		{
			SetUniform("_directionalLight.direction", light->GetDirection());
			SetUniform("_directionalLight.base.color", light->GetColor());
			SetUniformf("_directionalLight.base.intensity", light->GetIntensity());
		}
		else if (light->IsPointLight())
		{
			if (_maxPointLights == 0)
			{
				SetUniform("_pointLight.position", light->GetPosition());
				SetUniformf("_pointLight.range", light->GetRange());
				SetUniform("_pointLight.base.color", light->GetColor());
				SetUniformf("_pointLight.base.intensity", light->GetIntensity());
				SetUniformf("_pointLight.attenuation.constant", light->GetConstantAttenuation());
				SetUniformf("_pointLight.attenuation.linear", light->GetLinearAttenuation());
				SetUniformf("_pointLight.attenuation.exponent", light->GetExponentAttenuation());
			}
			else
			{
				str.Format("_pointLight[%d].position", nextPointLight);
				SetUniform(str, light->GetPosition());

				str.Format("_pointLight[%d].range", nextPointLight);
				SetUniformf(str, light->GetRange());

				str.Format("_pointLight[%d].base.color", nextPointLight);
				SetUniform(str, light->GetColor());

				str.Format("_pointLight[%d].base.intensity", nextPointLight);
				SetUniformf(str, light->GetIntensity());

				str.Format("_pointLight[%d].attenuation.constant", nextPointLight);
				SetUniformf(str, light->GetConstantAttenuation());

				str.Format("_pointLight[%d].attenuation.linear", nextPointLight);
				SetUniformf(str, light->GetLinearAttenuation());

				str.Format("_pointLight[%d].attenuation.exponent", nextPointLight);
				SetUniformf(str, light->GetExponentAttenuation());

				nextPointLight++;
			}
		}
		else if (light->IsSpotLight())
		{
			if (_maxPointLights == 0)
			{
				SetUniform("_spotLight.direction", light->GetDirection());
				SetUniformf("_spotLight.cutoff", light->GetCutoff());
				SetUniform("_spotLight.pointLight.position", light->GetPosition());
				SetUniformf("_spotLight.pointLight.range", light->GetRange());
				SetUniform("_spotLight.pointLight.base.color", light->GetColor());
				SetUniformf("_spotLight.pointLight.base.intensity", light->GetIntensity());
				SetUniformf("_spotLight.pointLight.attenuation.constant", light->GetConstantAttenuation());
				SetUniformf("_spotLight.pointLight.attenuation.linear", light->GetLinearAttenuation());
				SetUniformf("_spotLight.pointLight.attenuation.exponent", light->GetExponentAttenuation());
			}
			else
			{
				str.Format("_spotLight[%d].direction", nextSpotLight);
				SetUniform(str, light->GetDirection());

				str.Format("_spotLight[%d].cutoff", nextSpotLight);
				SetUniformf(str, light->GetCutoff());

				str.Format("_spotLight[%d].pointLight.position", nextSpotLight);
				SetUniform(str, light->GetPosition());

				str.Format("_spotLight[%d].pointLight.range", nextSpotLight);
				SetUniformf(str, light->GetRange());

				str.Format("_spotLight[%d].pointLight.base.color", nextSpotLight);
				SetUniform(str, light->GetColor());

				str.Format("_spotLight[%d].pointLight.base.intensity", nextSpotLight);
				SetUniformf(str, light->GetIntensity());

				str.Format("_spotLight[%d].pointLight.attenuation.constant", nextSpotLight);
				SetUniformf(str, light->GetConstantAttenuation());

				str.Format("_spotLight[%d].pointLight.attenuation.linear", nextSpotLight);
				SetUniformf(str, light->GetLinearAttenuation());

				str.Format("_spotLight[%d].pointLight.attenuation.exponent", nextSpotLight);
				SetUniformf(str, light->GetExponentAttenuation());

				nextSpotLight++;
			}
		}
	}
}

void AnimaShaderProgram::UpdateRenderingManagerProperies(AnimaRenderingManager* renderingManager)
{
	for (auto& pair : _uniforms)
	{
		AnimaUniformInfo info = pair.second;

		if (info._namePartsCount < 2 || info._nameParts[0] != RenderingEnginePrefix)
			continue;

		if (info._type == GL_FLOAT_VEC2)
			SetUniform(info._location, renderingManager->GetVector2f(info._nameParts[1]));
		else if (info._type == GL_FLOAT_VEC3)
			SetUniform(info._location, renderingManager->GetColor3f(info._nameParts[1]));
		else if (info._type == GL_FLOAT_VEC4)
			SetUniform(info._location, renderingManager->GetColor4f(info._nameParts[1]));
		else if (info._type == GL_FLOAT)
			SetUniformf(info._location, renderingManager->GetFloat(info._nameParts[1]));
		else if (info._type == GL_BOOL)
			SetUniformi(info._location, renderingManager->GetBoolean(info._nameParts[1]) ? 1 : 0);
		else if (info._type == GL_INT)
			SetUniformi(info._location, renderingManager->GetInteger(info._nameParts[1]));
		else if (info._type == GL_SAMPLER_2D)
		{
			if (info._namePartsCount == 4 && info._nameParts[1] == GBufferPrefix)
			{
				AnimaGBuffer* buffer = renderingManager->GetGBuffer(info._nameParts[2]);
				if (!buffer)
					continue;

				AnimaTexture* texture = buffer->GetTexture(info._nameParts[3]);
				AUint slot = renderingManager->GetTextureSlot(info._nameParts[3]);
				SetUniformi(info._location, slot);

				if (texture == nullptr)
				{
					glActiveTexture(GL_TEXTURE0 + slot);
					glBindTexture(GL_TEXTURE_2D, 0);
				}
				else
				{
					texture->Load();
					texture->Bind(slot);
				}
			}
			else
			{
				AnimaTexture* texture = renderingManager->GetTexture(info._nameParts[1]);
				AUint slot = renderingManager->GetTextureSlot(info._nameParts[1]);
				SetUniformi(info._location, slot);

				if (texture == nullptr)
				{
					glActiveTexture(GL_TEXTURE0 + slot);
					glBindTexture(GL_TEXTURE_2D, 0);
				}
				else
				{
					texture->Load();
					texture->Bind(slot);
				}
			}
		}
		else
		{
			ANIMA_ASSERT(false);
		}
	}
}

END_ANIMA_ENGINE_NAMESPACE