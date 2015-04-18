//
//  AnimaModel.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaModel.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaModel::AnimaModel(const AnimaString& name, AnimaDataGeneratorsManager* dataGeneratorsManager, AnimaAllocator* allocator)
	: AnimaSceneObject(name, dataGeneratorsManager, allocator)
	, _materialName(allocator)
	, _originFileName(allocator)
	, _meshes(allocator)
{
	_material = nullptr;
}

AnimaModel::AnimaModel(const AnimaModel& src)
	: AnimaSceneObject(src)
	, _materialName(src._materialName)
	, _originFileName(src._originFileName)
	, _meshes(src._meshes)
{
	_material = src._material;
}

AnimaModel::AnimaModel(AnimaModel&& src)
	: AnimaSceneObject(src)
	, _materialName(src._materialName)
	, _originFileName(src._originFileName)
	, _meshes(src._meshes)
{
	_material = src._material;
}

AnimaModel::~AnimaModel()
{
}

AnimaModel& AnimaModel::operator=(const AnimaModel& src)
{
	if (this != &src)
	{
		AnimaSceneObject::operator=(src);

		_material = src._material;
		_materialName = src._materialName;
		_originFileName = src._originFileName;
		_meshes = src._meshes;
	}
	
	return *this;
}

AnimaModel& AnimaModel::operator=(AnimaModel&& src)
{
	if (this != &src)
	{
		AnimaSceneObject::operator=(src);

		_material = src._material;
		_materialName = src._materialName;
		_originFileName = src._originFileName;
		_meshes = src._meshes;
	}
	
	return *this;
}

void AnimaModel::SetMaterial(AnimaMaterial* material)
{
	_material = material;
	_materialName = material->GetAnimaName();
}

AnimaMaterial* AnimaModel::GetMaterial()
{
	return _material;
}

AInt AnimaModel::AddMesh(AnimaMesh* mesh)
{
	return _meshes.Add(mesh->GetAnimaName(), mesh);
}

AnimaMesh* AnimaModel::GetMesh(AInt index)
{
	return _meshes[index];
}

AnimaMesh* AnimaModel::GetMeshFromName(const AnimaString& name)
{
	return _meshes[name];
}

AnimaMesh* AnimaModel::GetMeshFromName(const char* name)
{
	AnimaString str(name, _allocator);
	return _meshes[name];
}

void AnimaModel::SetOriginFileName(const AnimaString& fileName)
{
	_originFileName = fileName;
}

void AnimaModel::SetOriginFileName(const char* fileName)
{
	_originFileName = fileName;
}

AnimaString AnimaModel::GetAnimaOriginFileName() const
{
	return _originFileName;
}

const char* AnimaModel::GetOriginFileName() const
{
	return _originFileName.GetConstBuffer();
}

END_ANIMA_ENGINE_NAMESPACE
