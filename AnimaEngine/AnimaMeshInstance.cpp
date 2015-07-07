//
//  AnimaMeshInstance.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaMeshInstance.h"
#include "AnimaShaderProgram.h"
#include "AnimaRenderer.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaMeshInstance::AnimaMeshInstance(const AnimaString& name, AnimaDataGeneratorsManager* dataGeneratorsManager, AnimaAllocator* allocator)
	: AnimaSceneObject(name, dataGeneratorsManager, allocator)
	, _shadersNames(allocator)
{
	_material = nullptr;
	_mesh = nullptr;
	_shaderProgramName = "";
}

AnimaMeshInstance::AnimaMeshInstance(const AnimaMeshInstance& src)
	: AnimaSceneObject(src)
	, _materialName(src._materialName)
	, _meshName(src._meshName)
	, _shadersNames(src._shadersNames)
	, _shaderProgramName(src._shaderProgramName)
{
	_material = src._material;
	_mesh = src._mesh;
}

AnimaMeshInstance::AnimaMeshInstance(AnimaMeshInstance&& src)
	: AnimaSceneObject(src)
	, _materialName(src._materialName)
	, _meshName(src._meshName)
	, _shadersNames(src._shadersNames)
	, _shaderProgramName(src._shaderProgramName)
{
	_material = src._material;
	_mesh = src._mesh;
}

AnimaMeshInstance::~AnimaMeshInstance()
{
}

AnimaMeshInstance& AnimaMeshInstance::operator=(const AnimaMeshInstance& src)
{
	if (this != &src)
	{
		AnimaSceneObject::operator=(src);

		_material = src._material;
		_materialName = src._materialName;
		
		_mesh = src._mesh;
		_meshName = src._meshName;

		_shadersNames.Copy(src._shadersNames);
		_shaderProgramName = src._shaderProgramName;
	}
	
	return *this;
}

AnimaMeshInstance& AnimaMeshInstance::operator=(AnimaMeshInstance&& src)
{
	if (this != &src)
	{
		AnimaSceneObject::operator=(src);

		_material = src._material;
		_materialName = src._materialName;

		_mesh = src._mesh;
		_meshName = src._meshName;

		_shadersNames.Copy(src._shadersNames);
		_shaderProgramName = src._shaderProgramName;
	}
	
	return *this;
}

void AnimaMeshInstance::SetMaterial(AnimaMaterial* material)
{
	_material = material;
	if (_material)
		_materialName = material->GetName();
	else
		_materialName = "";
}

AnimaMaterial* AnimaMeshInstance::GetMaterial()
{
	return _material;
}

AnimaMesh* AnimaMeshInstance::GetMesh() const
{
	return _mesh;
}

void AnimaMeshInstance::SetMesh(AnimaMesh* mesh)
{
	_mesh = mesh;
}

void AnimaMeshInstance::Draw(AnimaRenderer* renderer, AnimaShaderProgram* program, bool updateMaterial)
{
	ANIMA_FRAME_PUSH("MeshInstanceUpdateProp");
	program->UpdateSceneObjectProperties(this, renderer);
	ANIMA_FRAME_POP();

	if (updateMaterial)
	{
		ANIMA_FRAME_PUSH("MeshInstanceMaterialUpdateProp");
		program->UpdateMappedValuesObjectProperties(_material, renderer);
		ANIMA_FRAME_POP();
	}

#ifdef _WIN32
	ANIMA_FRAME_PUSH("BindVAO");
	glBindVertexArray(_mesh->GetVertexArrayObject());
	ANIMA_FRAME_POP();

	ANIMA_FRAME_PUSH("DrawElements");
	glDrawElements(GL_TRIANGLES, _mesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
	ANIMA_FRAME_POP();

	ANIMA_FRAME_PUSH("Bind0");
	glBindVertexArray(0);
	ANIMA_FRAME_POP();
#else
	program->EnableInputs(_mesh);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mesh->GetIndexesBufferObject());
	glDrawElements(GL_TRIANGLES, _mesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
	program->DisableInputs();
#endif
}

void AnimaMeshInstance::AddShader(AnimaShader* shader)
{
	_shadersNames.Add(shader->GetName());
}

void AnimaMeshInstance::AddShader(const AnimaString& shaderName)
{
	_shadersNames.Add(shaderName);
}

void AnimaMeshInstance::AddShader(const char* shaderName)
{
	_shadersNames.Add(AnimaString(shaderName));
}

AInt AnimaMeshInstance::GetShadersCount() const
{
	return _shadersNames.GetSize();
}

AnimaString AnimaMeshInstance::GetShaderName(AInt index) const
{
	return _shadersNames[index];
}

void AnimaMeshInstance::SetShaderProgram(const AnimaString& shaderProgramName)
{
	_shaderProgramName = shaderProgramName;
}

void AnimaMeshInstance::SetShaderProgram(const char* shaderProgramName)
{
	_shaderProgramName = shaderProgramName;
}

AnimaString AnimaMeshInstance::GetShaderProgramName() const
{
	return _shaderProgramName;
}

END_ANIMA_ENGINE_NAMESPACE
