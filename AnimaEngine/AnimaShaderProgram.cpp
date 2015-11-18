#include "AnimaShaderProgram.h"
#include "AnimaShadersManager.h"
#include "AnimaLightsManager.h"
#include "AnimaRenderer.h"
#include "AnimaPhysicsDebugDrawer.h"
#include "AnimaScene.h"
#include <thread>

BEGIN_ANIMA_ENGINE_NAMESPACE

#if !defined UPD_ERROR
#	define UPD_ERROR ANIMA_ASSERT(false)
#endif

AnimaShaderProgram::AnimaShaderProgram(const AnimaString& name, AnimaAllocator* allocator, AnimaShadersManager* shadersManager)
	: AnimaNamedObject(name, allocator)
{
	IMPLEMENT_ANIMA_CLASS(AnimaShaderProgram);

	_shaders = nullptr;
	_shadersCount = 0;
	_id = 0;
	_linked = false;

	_shadersManager = shadersManager;

	_supportsInstance = false;
	_maxInstances = 1;
}

AnimaShaderProgram::AnimaShaderProgram(const AnimaShaderProgram& src)
	: AnimaNamedObject(src)
{
	_id = src._id;
	_linked = src._linked;

	_shaders = nullptr;
	_shadersCount = 0;

	_shadersManager = src._shadersManager;
	_data = src._data;
	_staticGroupData = src._staticGroupData;
	_dynamicGroupData = src._dynamicGroupData;

	_supportsInstance = src._supportsInstance;
	_maxInstances = src._maxInstances;

	SetShaders(src._shaders, src._shadersCount);
}

AnimaShaderProgram::AnimaShaderProgram(AnimaShaderProgram&& src)
	: AnimaNamedObject(src)
	, _shaders(src._shaders)
	, _shadersCount(src._shadersCount)
	, _id(src._id)
	, _linked(src._linked)
{	
	_shadersManager = src._shadersManager;
	_data = src._data;
	_staticGroupData = src._staticGroupData;
	_dynamicGroupData = src._dynamicGroupData;

	_supportsInstance = src._supportsInstance;
	_maxInstances = src._maxInstances;

	src._shaders = nullptr;
	src._shadersCount = 0;
	src._id = 0;
}

AnimaShaderProgram::~AnimaShaderProgram()
{
	ANIMA_ASSERT(_allocator != nullptr);

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

		_shadersManager = src._shadersManager;
		_data = src._data;
		_staticGroupData = src._staticGroupData;
		_dynamicGroupData = src._dynamicGroupData;

		_supportsInstance = src._supportsInstance;
		_maxInstances = src._maxInstances;

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

		_shadersManager = src._shadersManager;
		_data = src._data;
		_staticGroupData = src._staticGroupData;
		_dynamicGroupData = src._dynamicGroupData;

		_supportsInstance = src._supportsInstance;
		_maxInstances = src._maxInstances;

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

bool AnimaShaderProgram::CompileShaders()
{
	for (int i = 0; i < _shadersCount; i++)
	{
		_shadersManager->AttachIncludes(_shaders[i]);
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

		printf("AnimaShaderProgram error linking:\n%s\n", infoLog);

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

bool AnimaShaderProgram::IsCreated() const
{
	return _id > 0;
}

bool AnimaShaderProgram::IsLinked() const
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

AInt AnimaShaderProgram::GetID() const
{
	return _id;
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
	//if (!GLEW_ARB_program_interface_query)
	//{
		GLint numActiveInputs = 0;
		glGetProgramiv(_id, GL_ACTIVE_ATTRIBUTES, &numActiveInputs);
		
		AnimaString name;
		AnimaString namePart1;
		AnimaString namePart2;
		AnimaString tmpName;
		AnimaString nameGetter;
		GLenum type;
		GLsizei bufLen;
		GLsizei elements;
		GLint location;
		
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
	//}
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

void AnimaShaderProgram::EnableInputs(AnimaGeometry* geometry)
{
	glBindVertexArray(geometry->GetVertexArrayObject());

	for (auto key : _inputs)
	{
		AnimaInputInfo info = key.second;

		if (info._name == "_position")
		{
			glBindBuffer(GL_ARRAY_BUFFER, geometry->GetVerticesBufferObject());
			glEnableVertexAttribArray(info._location);
			glVertexAttribPointer(info._location, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}
		else if (info._name == "_normal")
		{
			if (geometry->GetFloatVerticesNormalCount() > 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, geometry->GetNormalsBufferObject());
				glEnableVertexAttribArray(info._location);
				glVertexAttribPointer(info._location, 3, GL_FLOAT, GL_FALSE, 0, 0);
			}
		}
		else if (info._name == "_textureCoord")
		{
			if (geometry->GetFloatVerticesTextureCoordCount() > 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, geometry->GetTextureCoordsBufferObject());
				glEnableVertexAttribArray(info._location);
				glVertexAttribPointer(info._location, 2, GL_FLOAT, GL_FALSE, 0, 0);
			}
		}
		else if (info._name == "_tangent")
		{
			if (geometry->GetFloatVerticesTangentsCount() > 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, geometry->GetTangentsBufferObject());
				glEnableVertexAttribArray(info._location);
				glVertexAttribPointer(info._location, 3, GL_FLOAT, GL_FALSE, 0, 0);
			}
		}
		else if (info._name == "_bitangent")
		{
			if (geometry->GetFloatVerticesBitangentsCount() > 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, geometry->GetBitangentsBufferObject());
				glEnableVertexAttribArray(info._location);
				glVertexAttribPointer(info._location, 3, GL_FLOAT, GL_FALSE, 0, 0);
			}
		}
		else if (info._name == "_boneWeights")
		{
			if (geometry->GetBoneWeightsCount() > 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, geometry->GetBoneWeightsBufferObject());
				glEnableVertexAttribArray(info._location);
				glVertexAttribPointer(info._location, 4, GL_FLOAT, GL_FALSE, 0, 0);
			}
		}
		else if (info._name == "_boneIDs")
		{
			if (geometry->GetBoneIDsCount() > 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, geometry->GetBoneIDsBufferObject());
				glEnableVertexAttribArray(info._location);
				glVertexAttribPointer(info._location, 4, GL_FLOAT, GL_FALSE, 0, 0);
			}
		}
	}
}

void AnimaShaderProgram::EnableInputs(AnimaPhysicsDebugDrawer* drawer)
{
	AUint vao = drawer->GetVertexArrayObject();
	AUint verticesVBO = drawer->GetVerticesBufferObject();
	AUint colorsVBO = drawer->GetColorsBufferObject();
	
	if(vao <= 0 || verticesVBO <= 0 || colorsVBO <= 0)
		return;
	
	glBindVertexArray(vao);
	
	for (auto key : _inputs)
	{
		AnimaInputInfo info = key.second;
		
		if (info._name == "_position")
		{
			glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
			glEnableVertexAttribArray(info._location);
			glVertexAttribPointer(info._location, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}
		else if (info._name == "_color")
		{
			glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
			glEnableVertexAttribArray(info._location);
			glVertexAttribPointer(info._location, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}
	}
}

void AnimaShaderProgram::DisableInputs()
{
	for (auto key : _inputs)
	{
		AnimaInputInfo info = key.second;
		if (info._location > -1)
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

	AnimaShaderDataSourceObject source = object->GetShaderSource();

	AInt count = _data.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaShaderData* data = &_data[i];
		if(data->GetSourceObject() == source)
			data->UpdateValue(object, renderingManager);
	}
	
	count = _dynamicGroupData.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaShaderGroupData* groupData = &_dynamicGroupData[i];
		if (groupData->GetSourceObject() == source)
			groupData->UpdateValue(object, renderingManager, this);
	}
}

void AnimaShaderProgram::UpdateRenderingManagerProperies(AnimaRenderer* renderingManager)
{
	if (renderingManager == nullptr)
		return;

	AInt count = _data.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaShaderData* data = &_data[i];
		if (data->GetSourceObject() == ASDSO_RENDERER || data->GetSourceObject() == ASDSO_GBUFFER)
			data->UpdateValue(renderingManager);
	}
}

void AnimaShaderProgram::UpdateDataLookup()
{
	// Ripulisco i dati e li ricostruisco dagli shader
	_data.RemoveAll();
	_dynamicGroupData.RemoveAll();
	_staticGroupData.RemoveAll();
	for (AInt i = 0; i < _shadersCount; i++)
	{
		AnimaShader* shader = _shaders[i];

		AInt dataCount = shader->GetShaderDataCount();
		for (AInt j = 0; j < dataCount; j++)
		{
			AnimaShaderData* data = shader->GetShaderData(j);

			// Se non ho già trovato il dato lo vado ad aggiungere
			if (_data.Contains(data->GetName()) == -1)
				AddShaderData(*data);
		}

		AInt dynamicGroupDataCount = shader->GetShaderDynamicGroupDataCount();
		for (AInt j = 0; j < dynamicGroupDataCount; j++)
		{
			AnimaShaderGroupData* groupData = shader->GetShaderDynamicGroupData(j);

			// Se non ho già trovato il dato lo vado ad aggiungere
			if (_dynamicGroupData.Contains(groupData->GetName()) == -1)
				AddShaderDynamicGroupData(*groupData);
		}

		AInt staticGroupDataCount = shader->GetShaderStaticGroupDataCount();
		for (AInt j = 0; j < staticGroupDataCount; j++)
		{
			AnimaShaderGroupData* groupData = shader->GetShaderStaticGroupData(j);

			// Se non ho già trovato il dato lo vado ad aggiungere
			if (_staticGroupData.Contains(groupData->GetName()) == -1)
				AddShaderStaticGroupData(*groupData);
		}
	}

	// Una volta costruiti i dati li vado ad aggiornare
	AInt count = _data.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		_data[i].Analize(this);
	}

	AInt bindingPoint = 0;

	count = _dynamicGroupData.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		_dynamicGroupData[i].Analize(this);
		_dynamicGroupData[i].Create();
		_dynamicGroupData[i].SetBindingPoint(this, bindingPoint++);
	}

	count = _staticGroupData.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		_staticGroupData[i].Analize(this);
		_staticGroupData[i].Create();
		_staticGroupData[i].SetBindingPoint(this, bindingPoint++);
	}
}

void AnimaShaderProgram::AddShaderData(const AnimaShaderData& data)
{
	_data.Add(data.GetName(), data);
}

AInt AnimaShaderProgram::GetShaderDataCount() const
{
	return _data.GetSize();
}

AnimaShaderData* AnimaShaderProgram::GetShaderData(const AnimaString& name)
{
	return &_data[name];
}

AnimaShaderData* AnimaShaderProgram::GetShaderData(const AInt& index)
{
	return &_data[index];
}

void AnimaShaderProgram::AddShaderStaticGroupData(const AnimaShaderGroupData& groupData)
{
	_staticGroupData.Add(groupData.GetName(), groupData);
}

AInt AnimaShaderProgram::GetShaderStaticGroupDataCount() const
{
	return _staticGroupData.GetSize();
}

AnimaShaderGroupData* AnimaShaderProgram::GetShaderStaticGroupData(const AInt& index)
{
	return &_staticGroupData[index];
}

AnimaShaderGroupData* AnimaShaderProgram::GetShaderStaticGroupData(const AnimaString& name)
{
	return &_staticGroupData[name];
}

void AnimaShaderProgram::AddShaderDynamicGroupData(const AnimaShaderGroupData& groupData)
{
	_dynamicGroupData.Add(groupData.GetName(), groupData);
}

AInt AnimaShaderProgram::GetShaderDynamicGroupDataCount() const
{
	return _dynamicGroupData.GetSize();
}

AnimaShaderGroupData* AnimaShaderProgram::GetShaderDynamicGroupData(const AInt& index)
{
	return &_dynamicGroupData[index];
}

AnimaShaderGroupData* AnimaShaderProgram::GetShaderDynamicGroupData(const AnimaString& name)
{
	return &_dynamicGroupData[name];
}

void AnimaShaderProgram::SyncBuffers(AUint buffersIndex)
{
	AInt count = _dynamicGroupData.GetSize();
	for (AInt i = 0; i < count; i++)
		_dynamicGroupData[i].SyncBuffers(buffersIndex);

	count = _staticGroupData.GetSize();
	for (AInt i = 0; i < count; i++)
		_staticGroupData[i].SyncBuffers(buffersIndex);
}

END_ANIMA_ENGINE_NAMESPACE