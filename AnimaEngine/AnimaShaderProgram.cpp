#include "AnimaShaderProgram.h"
#include "AnimaShadersManager.h"
#include "AnimaLightsManager.h"
#include "AnimaRenderer.h"
#include <thread>

BEGIN_ANIMA_ENGINE_NAMESPACE

#if !defined UPD_ERROR
#	define UPD_ERROR ANIMA_ASSERT(false)
#endif

#define NEW_DATA_MODE

AnimaShaderProgram::AnimaShaderProgram(const AnimaString& name, AnimaAllocator* allocator, AnimaShadersManager* shadersManager)
	: AnimaNamedObject(name, allocator)
{
	IMPLEMENT_ANIMA_CLASS(AnimaShaderProgram);

	_shaders = nullptr;
	_shadersCount = 0;
	_id = 0;
	_linked = false;

	_shadersManager = shadersManager;
}

AnimaShaderProgram::AnimaShaderProgram(const AnimaShaderProgram& src)
	: AnimaNamedObject(src)
{
	_id = src._id;
	_linked = src._linked;

	_shaders = nullptr;
	_shadersCount = 0;

	ClearUniforms();
	_uniforms = src._uniforms;

	_shadersManager = src._shadersManager;
	_data = src._data;

	SetShaders(src._shaders, src._shadersCount);
}

AnimaShaderProgram::AnimaShaderProgram(AnimaShaderProgram&& src)
	: AnimaNamedObject(src)
	, _shaders(src._shaders)
	, _shadersCount(src._shadersCount)
	, _id(src._id)
	, _linked(src._linked)
{
	ClearUniforms();
	_uniforms = src._uniforms;

	_shadersManager = src._shadersManager;
	_data = src._data;

	src._shaders = nullptr;
	src._shadersCount = 0;
	src._id = 0;
}

AnimaShaderProgram::~AnimaShaderProgram()
{
	ANIMA_ASSERT(_allocator != nullptr);

	ClearUniforms();

	if (_shadersCount > 0 && _shaders != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateArray<AnimaShader*>(*_allocator, _shaders);
		_shaders = nullptr;
		_shadersCount = 0;
	}

	Delete();
}

AnimaShaderProgram& AnimaShaderProgram::operator=(const AnimaShaderProgram& src)
{
	if (this != &src)
	{
		AnimaNamedObject::operator=(src);

		SetShaders(src._shaders, src._shadersCount);

		ClearUniforms();
		_uniforms = src._uniforms;

		_shadersManager = src._shadersManager;
		_data = src._data;

		_id = src._id;
		_linked = src._linked;
	}

	return *this;
}

AnimaShaderProgram& AnimaShaderProgram::operator=(AnimaShaderProgram&& src)
{
	if (this != &src)
	{
		AnimaNamedObject::operator=(src);

		SetShaders(src._shaders, src._shadersCount);

		ClearUniforms();
		_uniforms = src._uniforms;

		_shadersManager = src._shadersManager;
		_data = src._data;

		_id = src._id;
		_linked = src._linked;

		src._id = 0;
		src._shaders = nullptr;
		src._shadersCount = 0;
		src._linked = false;
	}

	return *this;
}

bool AnimaShaderProgram::operator==(const AnimaShaderProgram& left)
{
	if (_id != left._id) return false;
	if (_shadersManager != left._shadersManager) return false;
	if (_allocator != left._allocator) return false;
	if (_shadersCount != left._shadersCount) return false; 
	if (_linked != left._linked) return false;

	for (int i = 0; i < _shadersCount; i++)
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
	if (_shadersCount != left._shadersCount) return true;
	if (_linked != left._linked) return true;

	for (int i = 0; i < _shadersCount; i++)
	{
		if (_shaders[i] != left._shaders[i])
			return true;
	}

	return false;
}

void AnimaShaderProgram::AddShader(AnimaShader* shader)
{
	ANIMA_ASSERT(_allocator != nullptr);
	if (_shadersCount > 0)
	{
		AnimaShader** tmpOldShaders = AnimaAllocatorNamespace::AllocateArray<AnimaShader*>(*_allocator, _shadersCount);

		for (int i = 0; i < _shadersCount; i++)
			tmpOldShaders[i] = _shaders[i];

		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _shaders);

		_shadersCount++;
		_shaders = AnimaAllocatorNamespace::AllocateArray<AnimaShader*>(*_allocator, _shadersCount);

		for (int i = 0; i < _shadersCount - 1; i++)
			_shaders[i] = tmpOldShaders[i];

		_shaders[_shadersCount - 1] = shader;

		AnimaAllocatorNamespace::DeallocateArray(*_allocator, tmpOldShaders);
	}
	else
	{
		_shadersCount++;
		_shaders = AnimaAllocatorNamespace::AllocateArray<AnimaShader*>(*_allocator, _shadersCount);

		_shaders[_shadersCount - 1] = shader;
	}
}

void AnimaShaderProgram::SetShaders(AnimaShader** shaders, ASizeT count)
{
	ANIMA_ASSERT(_allocator != nullptr)
	ClearShaders();

	if (shaders != nullptr && count > 0)
	{
		_shadersCount = count;
		_shaders = AnimaAllocatorNamespace::AllocateArray<AnimaShader*>(*_allocator, count);

		for (int i = 0; i < _shadersCount; i++)
			_shaders[i] = shaders[i];
	}
}

void AnimaShaderProgram::ClearShaders()
{
	if (_shaders != nullptr && _shadersCount > 0)
	{
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _shaders);
		_shaders = nullptr;
		_shadersCount = 0;
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

		if (pair.second._locations != nullptr)
		{
			AnimaAllocatorNamespace::DeallocateArray(*_allocator, pair.second._locations);
			pair.second._locations = nullptr;
			pair.second._locationsCount = 0;
		}
	}
	_uniforms.clear();
}

bool AnimaShaderProgram::CompileShaders()
{
	for (int i = 0; i < _shadersCount; i++)
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

	for (int i = 0; i < _shadersCount; i++)
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
		UpdateDataLookup();
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

	if (_shadersManager != nullptr)
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
		glUniform1i(pair->second._locations[0], value);
}

void AnimaShaderProgram::SetUniformf(const AnimaString& uniformName, AFloat value)
{
	auto pair = _uniforms.find(uniformName);
	if (pair != _uniforms.end())
		glUniform1f(pair->second._locations[0], value);
}

void AnimaShaderProgram::SetUniform(const AnimaString& uniformName, const AnimaVertex2f& value)
{
	auto pair = _uniforms.find(uniformName);
	if (pair != _uniforms.end())
		glUniform2f(pair->second._locations[0], value.x, value.y);
}

void AnimaShaderProgram::SetUniform(const AnimaString& uniformName, const AnimaVertex3f& value)
{
	auto pair = _uniforms.find(uniformName);
	if (pair != _uniforms.end())
		glUniform3f(pair->second._locations[0], value.x, value.y, value.z);
}

void AnimaShaderProgram::SetUniform(const AnimaString& uniformName, const AnimaColor4f& value)
{
	auto pair = _uniforms.find(uniformName);
	if (pair != _uniforms.end())
		glUniform4f(pair->second._locations[0], value.x, value.y, value.z, value.w);
}

void AnimaShaderProgram::SetUniform(const AnimaString& uniformName, AFloat a, AFloat b, AFloat c)
{
	auto pair = _uniforms.find(uniformName);
	if (pair != _uniforms.end())
		glUniform3f(pair->second._locations[0], a, b, c);
}

void AnimaShaderProgram::SetUniform(const AnimaString& uniformName, AFloat a, AFloat b, AFloat c, AFloat d)
{
	auto pair = _uniforms.find(uniformName);
	if (pair != _uniforms.end())
		glUniform4f(pair->second._locations[0], a, b, c, d);
}

void AnimaShaderProgram::SetUniform(const AnimaString& uniformName, const AnimaMatrix& value, bool transpose)
{
	auto pair = _uniforms.find(uniformName);
	if (pair != _uniforms.end())
		glUniformMatrix4fv(pair->second._locations[0], 1, transpose ? GL_TRUE : GL_FALSE, value.m);
}

void AnimaShaderProgram::SetUniform(const AnimaString& uniformName, const AnimaArray<AnimaVectorGenerator*>* value, AUint type)
{
	if (value == nullptr)
		return;

	auto pair = _uniforms.find(uniformName);
	if (pair != _uniforms.end())
	{
		AInt countValue = value->size();
		AInt countUniformArray = pair->second._arraySize;
		AInt countUniformLocations = pair->second._arraySize;
		for (AInt i = 0; i < countValue && i < countUniformArray && i < countUniformLocations; i++)
		{
			if (type == GL_FLOAT_VEC2)
				SetUniform(pair->second._locations[i], value->at(i)->GetVector2f());
			else if (type == GL_FLOAT_VEC3)
				SetUniform(pair->second._locations[i], value->at(i)->GetVector3f());
			else if (type == GL_FLOAT_VEC4)
				SetUniform(pair->second._locations[i], value->at(i)->GetVector4f());
		}
	}
}

void AnimaShaderProgram::SetUniform(const AnimaString& uniformName, const AnimaArray<AnimaMatrix>* value)
{
	if (value == nullptr)
		return;

	auto pair = _uniforms.find(uniformName);
	if (pair != _uniforms.end())
	{
		AInt countValue = value->size();
		AInt countUniformArray = pair->second._arraySize;
		AInt countUniformLocations = pair->second._arraySize;

		for (AInt i = 0; i < countValue && i < countUniformArray && i < countUniformLocations; i++)
			SetUniform(pair->second._locations[i], value->at(i));
	}
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
#if defined NEW_DATA_MODE
	return;
#else
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

		const int propertiesSize = 4;

		AnimaString name;
		AnimaString namePart1;
		AnimaString namePart2;
		AnimaString tmpName;
		GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_ARRAY_SIZE };
		GLint values[propertiesSize];

		for (int i = 0; i < numActiveUniforms; i++)
		{
			glGetProgramResourceiv(_id, GL_UNIFORM, i, propertiesSize, &properties[0], propertiesSize, NULL, &values[0]);

			std::vector<GLchar> nameArray(values[0] + 1);
			glGetProgramResourceName(_id, GL_UNIFORM, i, nameArray.size(), NULL, &nameArray[0]);
			name = AnimaString(nameArray.begin(), nameArray.end() - 1);
			boost::algorithm::trim(name);
			AInt nameLen = name.length();
			AInt removeCount = 0;
			while (removeCount < nameLen && name[nameLen - 1 - removeCount] == '\0')
				removeCount++;

			if (removeCount > 0)
				name = name.substr(0, nameLen - removeCount);

			namePart1 = name;

			AInt arraySize = values[3];

			AInt* locations = nullptr;
			AInt locationsCount = 0;

			AInt pos = -1;

			if (arraySize > 0)
			{
				pos = namePart1.find('[');
				if (pos != -1)
				{
					name = namePart1.substr(0, pos);

					locations = AnimaAllocatorNamespace::AllocateArray<AInt>(*_allocator, arraySize);
					locationsCount = arraySize;

					for (AInt nu = 0; nu < arraySize; nu++)
					{
						tmpName = FormatString("%s[%d]", name.c_str(), nu);
						locations[nu] = glGetUniformLocation(_id, tmpName.c_str());
					}
				}
				else
				{
					arraySize = 0;
					locations = AnimaAllocatorNamespace::AllocateArray<AInt>(*_allocator, 1);
					locationsCount = 1;

					locations[0] = values[2];
				}
			}
			else
			{
				locations = AnimaAllocatorNamespace::AllocateArray<AInt>(*_allocator, 1);
				locationsCount = 1;

				locations[0] = values[2];
			}

			namePart1 = name;
			AUint offset = std::count(namePart1.begin(), namePart1.end(), ('_')) + 1;
			AnimaString* nameParts = AnimaAllocatorNamespace::AllocateArray<AnimaString>(*_allocator, offset);

			offset = 0;
			pos = namePart1.find('_');
			while (pos != -1)
			{
				namePart2 = namePart1.substr(0, pos);
				namePart1 = namePart1.substr(pos + 1, namePart1.length());

				if (!namePart2.empty())
					nameParts[offset++] = namePart2;
				pos = namePart1.find('_');
			}

			nameParts[offset++] = namePart1;

			AnimaUniformInfo info;
			info._locations = locations;
			info._locationsCount = locationsCount;
			info._type = values[1];
			info._arraySize = arraySize;
			info._name = name;
			info._nameParts = nameParts;
			info._namePartsCount = offset;

			_uniforms[name] = info;
		}

		for (int i = 0; i < numActiveInputs; i++)
		{
			glGetProgramResourceiv(_id, GL_PROGRAM_INPUT, i, propertiesSize, &properties[0], propertiesSize, NULL, &values[0]);

			std::vector<GLchar> nameArray(values[0] + 1);
			glGetProgramResourceName(_id, GL_PROGRAM_INPUT, i, nameArray.size(), NULL, &nameArray[0]);
			name = AnimaString(nameArray.begin(), nameArray.end() - 1);
			boost::algorithm::trim(name);
			AInt nameLen = name.length();
			AInt removeCount = 0;
			while (removeCount < nameLen && name[nameLen - 1 - removeCount] == '\0')
				removeCount++;

			if (removeCount > 0)
				name = name.substr(0, nameLen - removeCount);

			AnimaInputInfo info;
			info._location = values[2];
			info._type = values[1];
			info._name = name;

			_inputs[name] = info;
		}

		for (int i = 0; i < numActiveOutputs; i++)
		{
			glGetProgramResourceiv(_id, GL_PROGRAM_OUTPUT, i, propertiesSize, &properties[0], propertiesSize, NULL, &values[0]);

			std::vector<GLchar> nameArray(values[0] + 1);
			glGetProgramResourceName(_id, GL_PROGRAM_OUTPUT, i, nameArray.size(), NULL, &nameArray[0]);
			name = AnimaString(nameArray.begin(), nameArray.end() - 1);
			boost::algorithm::trim(name);
			AInt nameLen = name.length();
			AInt removeCount = 0;
			while (removeCount < nameLen && name[nameLen - 1 - removeCount] == '\0')
				removeCount++;

			if (removeCount > 0)
				name = name.substr(0, nameLen - removeCount);

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

		AnimaString name;
		AnimaString namePart1;
		AnimaString namePart2;
		AnimaString tmpName;
		AnimaString nameGetter;
		GLenum type;
		GLsizei bufLen;
		GLsizei elements;
		GLint location;
		
		for (int i = 0; i < numActiveUniforms; i++)
		{
			std::vector<GLchar> nameArray(200);
			glGetActiveUniform(_id, i, nameArray.size(), &bufLen, &elements, &type, &nameArray[0]);
			name = AnimaString(nameArray.begin(), nameArray.end() - 1);
			boost::algorithm::trim(name);
			AInt nameLen = name.length();
			AInt removeCount = 0;
			while (removeCount < nameLen && name[nameLen - 1 - removeCount] == '\0')
				removeCount++;

			if (removeCount > 0)
				name = name.substr(0, nameLen - removeCount);
			
			namePart1 = name;
			
			AInt arraySize = elements;
			
			AInt* locations = nullptr;
			AInt locationsCount = 0;
			
			AInt pos = -1;
			
			if (arraySize > 0)
			{
				pos = namePart1.find('[');
				if (pos != -1)
				{
					name = namePart1.substr(0, pos);
					
					locations = AnimaAllocatorNamespace::AllocateArray<AInt>(*_allocator, arraySize);
					locationsCount = arraySize;
					
					for (AInt nu = 0; nu < arraySize; nu++)
					{
						tmpName = FormatString("%s[%d]", name.c_str(), nu);
						
						locations[nu] = glGetUniformLocation(_id, tmpName.c_str());
					}
				}
				else
				{
					arraySize = 0;
					locations = AnimaAllocatorNamespace::AllocateArray<AInt>(*_allocator, 1);
					locationsCount = 1;
					
					locations[0] = glGetUniformLocation(_id, namePart1.c_str());
				}
			}
			else
			{
				locations = AnimaAllocatorNamespace::AllocateArray<AInt>(*_allocator, 1);
				locationsCount = 1;
				
				locations[0] = glGetUniformLocation(_id, namePart1.c_str());
			}
			
			namePart1 = name;
			AUint offset = std::count(namePart1.begin(), namePart1.end(), ('_')) + 1;
			AnimaString* nameParts = AnimaAllocatorNamespace::AllocateArray<AnimaString>(*_allocator, offset);
			
			offset = 0;
			pos = namePart1.find('_');
			while (pos != -1)
			{
				namePart2 = namePart1.substr(0, pos);
				namePart1 = namePart1.substr(pos + 1, namePart1.length());
				
				if (!namePart2.empty())
					nameParts[offset++] = namePart2;
				pos = namePart1.find('_');
			}
			
			nameParts[offset++] = namePart1;
			
			AnimaUniformInfo info;
			info._locations = locations;
			info._locationsCount = locationsCount;
			info._type = type;
			info._arraySize = arraySize;
			info._name = name;
			info._nameParts = nameParts;
			info._namePartsCount = offset;
			
			_uniforms[name] = info;
		}

		for (int i = 0; i < numActiveInputs; i++)
		{
			std::vector<GLchar> nameArray(200);
			glGetActiveAttrib(_id, i, nameArray.size(), &bufLen, &elements, &type, &nameArray[0]);
			name = AnimaString(nameArray.begin(), nameArray.end() - 1);
			boost::algorithm::trim(name);
			AInt nameLen = name.length();
			AInt removeCount = 0;
			while (removeCount < nameLen && name[nameLen - 1 - removeCount] == '\0')
				removeCount++;

			if (removeCount > 0)
				name = name.substr(0, nameLen - removeCount);

			location = glGetAttribLocation(_id, name.c_str());

			AnimaInputInfo info;
			info._location = location;
			info._type = type;
			info._name = name;

			_inputs[name] = info;
		}
	}
#endif
}

void AnimaShaderProgram::EnableInput(const AnimaString& inputName, AInt size, AUint type, AUint buffer)
{
	for (auto key : _inputs)
	{
		AnimaInputInfo info = key.second;

		if (info._name == inputName)
		{
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glEnableVertexAttribArray(info._location);
			glVertexAttribPointer(info._location, size, type, GL_FALSE, 0, 0);

			break;
		}
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
		else if (info._name == "_boneWeights")
		{
			if (mesh->GetBoneWeightsCount() > 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, mesh->GetBoneWeightsBufferObject());
				glEnableVertexAttribArray(info._location);
				glVertexAttribPointer(info._location, 4, GL_FLOAT, GL_FALSE, 0, 0);
			}
		}
		else if (info._name == "_boneIDs")
		{
			if (mesh->GetBoneIDsCount() > 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, mesh->GetBoneIDsBufferObject());
				glEnableVertexAttribArray(info._location);
				glVertexAttribPointer(info._location, 4, GL_FLOAT, GL_FALSE, 0, 0);
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

void AnimaShaderProgram::UpdateSceneObjectProperties(AnimaSceneObject* object, AnimaRenderer* renderingManager)
{
	UpdateMappedValuesObjectProperties(object, renderingManager);
}

void AnimaShaderProgram::UpdateMappedValuesObjectProperties(AnimaMappedValues* object, AnimaRenderer* renderingManager)
{
	if (object == nullptr)
		return;

	const char* prefix = object->GetShaderPrefix();

#if defined NEW_DATA_MODE

	//for(auto& data : _data)
	AInt count = _data.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaShaderData* data = &_data[i];
		if(data->GetPrefix() == prefix)
			data->UpdateValue(object, renderingManager);
	}

#else
	for (auto& pair : _uniforms)
	{
		AnimaUniformInfo info = pair.second;
		if (info._namePartsCount != 2 || info._nameParts[0] != prefix)
			continue;

		if (info._arraySize == 0)
		{
			if (info._type == GL_FLOAT_VEC2)
			{
				ANIMA_FRAME_PUSH("SetVec2");
				SetUniform(info._locations[0], object->GetVector2f(info._nameParts[1]));
				ANIMA_FRAME_POP();
			}
			else if (info._type == GL_FLOAT_VEC3)
			{
				ANIMA_FRAME_PUSH("CheckColo3");
				if (object->HasColor(info._nameParts[1]))
				{
					ANIMA_FRAME_POP();

					ANIMA_FRAME_PUSH("SetColor3");
					SetUniform(info._locations[0], object->GetColor3f(info._nameParts[1]));
					ANIMA_FRAME_POP();
				}
				else
				{
					ANIMA_FRAME_POP();

					ANIMA_FRAME_PUSH("SetVector3");
					SetUniform(info._locations[0], object->GetVector3f(info._nameParts[1]));
					ANIMA_FRAME_POP();
				}
			}
			else if (info._type == GL_FLOAT_VEC4)
			{
				ANIMA_FRAME_PUSH("CheckColor4");
				if (object->HasColor(info._nameParts[1]))
				{
					ANIMA_FRAME_POP();

					ANIMA_FRAME_PUSH("SetColor4");
					SetUniform(info._locations[0], object->GetColor4f(info._nameParts[1]));
					ANIMA_FRAME_POP();
				}
				else
				{
					ANIMA_FRAME_POP();

					ANIMA_FRAME_PUSH("SetVector4");
					SetUniform(info._locations[0], object->GetVector4f(info._nameParts[1]));
					ANIMA_FRAME_POP();
				}
			}
			else if (info._type == GL_FLOAT)
			{
				ANIMA_FRAME_PUSH("SetFloat");
				SetUniformf(info._locations[0], object->GetFloat(info._nameParts[1]));
				ANIMA_FRAME_POP();
			}
			else if (info._type == GL_BOOL)
			{
				ANIMA_FRAME_PUSH("SetBool");
				SetUniformi(info._locations[0], object->GetBoolean(info._nameParts[1]) ? 1 : 0);
				ANIMA_FRAME_POP();
			}
			else if (info._type == GL_INT)
			{
				ANIMA_FRAME_PUSH("Setint");
				SetUniformi(info._locations[0], object->GetInteger(info._nameParts[1]));
				ANIMA_FRAME_POP();
			}
			else if (info._type == GL_FLOAT_MAT4)
			{
				ANIMA_FRAME_PUSH("SetMatrix");
				SetUniform(info._locations[0], object->GetMatrix(info._nameParts[1]));
				ANIMA_FRAME_POP();
			}
			else if (info._type == GL_SAMPLER_2D)
			{
				ANIMA_FRAME_PUSH("GetTexture");
				AnimaTexture* texture = object->GetTexture(info._nameParts[1]);
				ANIMA_FRAME_POP();

				ANIMA_FRAME_PUSH("GetTextureSlot");
				AUint slot = renderingManager->GetTextureSlot(info._nameParts[1]);
				ANIMA_FRAME_POP();

				ANIMA_FRAME_PUSH("SetTextureSlot");
				SetUniformi(info._locations[0], slot);
				ANIMA_FRAME_POP();

				if (texture == nullptr)
				{
					ANIMA_FRAME_PUSH("DeactiveTextureSlot");
					glActiveTexture(GL_TEXTURE0 + slot);
					glBindTexture(GL_TEXTURE_2D, 0);
					ANIMA_FRAME_POP();
				}
				else
				{
					ANIMA_FRAME_PUSH("LoadTexture");
					texture->Load();
					ANIMA_FRAME_POP();

					ANIMA_FRAME_PUSH("BindTexture");
					texture->Bind(slot);
					ANIMA_FRAME_POP();
				}
			}
			else
			{
				ANIMA_ASSERT(false);
			}
		}
		else
		{
			if (info._type == GL_FLOAT_VEC2 || info._type == GL_FLOAT_VEC3 || info._type == GL_FLOAT_VEC4)
				SetUniform(info._name, object->GetVectorArray(info._nameParts[1]), info._type);
			else if (info._type == GL_FLOAT_MAT4)
				SetUniform(info._name, object->GetMatrixArray(info._nameParts[1]));
			else
			{
				ANIMA_ASSERT(false);
			}
		}
	}
#endif
}

void AnimaShaderProgram::UpdateRenderingManagerProperies(AnimaRenderer* renderingManager)
{
	if (renderingManager == nullptr)
		return;

	const char* prefix = RENDERER_PREFIX;

#if defined NEW_DATA_MODE

	AInt count = _data.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaShaderData* data = &_data[i];
		if(data->GetPrefix() == prefix)
			data->UpdateValue(renderingManager);
	}
#else
	for (auto& pair : _uniforms)
	{
		AnimaUniformInfo info = pair.second;

		if (info._namePartsCount < 2 || info._nameParts[0] != RENDERER_PREFIX)
			continue;

		if (info._type == GL_FLOAT_VEC2)
			SetUniform(info._locations[0], renderingManager->GetVector2f(info._nameParts[1]));
		else if (info._type == GL_FLOAT_VEC3)
			SetUniform(info._locations[0], renderingManager->GetVector3f(info._nameParts[1]));
		else if (info._type == GL_FLOAT_VEC4)
			SetUniform(info._locations[0], renderingManager->GetVector4f(info._nameParts[1]));
		else if (info._type == GL_FLOAT)
			SetUniformf(info._locations[0], renderingManager->GetFloat(info._nameParts[1]));
		else if (info._type == GL_BOOL)
			SetUniformi(info._locations[0], renderingManager->GetBoolean(info._nameParts[1]) ? 1 : 0);
		else if (info._type == GL_INT)
			SetUniformi(info._locations[0], renderingManager->GetInteger(info._nameParts[1]));
		else if (info._type == GL_SAMPLER_2D)
		{
			if (info._namePartsCount == 4 && info._nameParts[1] == GBUFFER_PREFIX)
			{
				AnimaGBuffer* buffer = renderingManager->GetGBuffer(info._nameParts[2]);
				if (!buffer)
					continue;

				AnimaTexture* texture = buffer->GetTexture(info._nameParts[3]);
				AUint slot = renderingManager->GetTextureSlot(info._nameParts[3]);
				SetUniformi(info._locations[0], slot);

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
				SetUniformi(info._locations[0], slot);

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
#endif
}

void AnimaShaderProgram::UpdateDataLookup()
{
	AInt count = _data.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		_data[i].FindLocation(this);
	}
}

void AnimaShaderProgram::AddShaderData(AnimaShaderData& data)
{
	_data.Add(data.GetName(), data);
}

AnimaShaderData* AnimaShaderProgram::GetShaderData(const AnimaString& name)
{
	return &_data[name];
}

END_ANIMA_ENGINE_NAMESPACE