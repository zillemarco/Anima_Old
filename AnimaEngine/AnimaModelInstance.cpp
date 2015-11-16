//
//  AnimaModelInstance.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaModelInstance.h"
#include "AnimaModelsManager.h"
#include "AnimaMeshInstancesManager.h"
#include "AnimaScene.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaModelInstance::AnimaModelInstance(const AnimaString& name, AnimaDataGeneratorsManager* dataGeneratorsManager, AnimaAllocator* allocator)
	: AnimaSceneObject(name, dataGeneratorsManager, allocator)
{
	IMPLEMENT_ANIMA_CLASS(AnimaModelInstance);
	_model = nullptr;
	_topLevelModel = false;
	
	_shaderSource = ASDSO_NODE;
}

AnimaModelInstance::AnimaModelInstance(const AnimaModelInstance& src)
	: AnimaSceneObject(src)
	, _modelName(src._modelName)
	, _meshes(src._meshes)
{
	_model = src._model;
	_topLevelModel = src._topLevelModel;
}

AnimaModelInstance::AnimaModelInstance(AnimaModelInstance&& src)
	: AnimaSceneObject(src)
	, _modelName(src._modelName)
	, _meshes(src._meshes)
{
	_model = src._model;
	_topLevelModel = src._topLevelModel;
}

AnimaModelInstance::~AnimaModelInstance()
{
}

AnimaModelInstance& AnimaModelInstance::operator=(const AnimaModelInstance& src)
{
	if (this != &src)
	{
		AnimaSceneObject::operator=(src);

		_meshes = src._meshes;		
		_model = src._model;
		_modelName = src._modelName;
		_topLevelModel = src._topLevelModel;
	}
	
	return *this;
}

AnimaModelInstance& AnimaModelInstance::operator=(AnimaModelInstance&& src)
{
	if (this != &src)
	{
		AnimaSceneObject::operator=(src);

		_meshes = src._meshes;
		_model = src._model;
		_modelName = src._modelName;
		_topLevelModel = src._topLevelModel;
	}
	
	return *this;
}

ptree AnimaModelInstance::GetObjectTree(bool saveName) const
{
	ptree tree;
	
	if (saveName)
		tree.add("AnimaModelInstance.Name", GetName());
	
	tree.add("AnimaModelInstance.TopLevelModel", IsTopLevelModel());
	
	AnimaString modelName = _modelName;
	if(modelName.empty() && _model != nullptr)
		modelName = _model->GetName();
	tree.add("AnimaModelInstance.ModelName", modelName);
	
	ptree meshesTree;
	AInt count = _meshes.size();
	for (AInt i = 0; i < count; i++)
		meshesTree.add("MeshInstance", _meshes[i]->GetName());
	tree.add_child("AnimaModelInstance.MeshesInstances", meshesTree);
	
	tree.add_child("AnimaModelInstance.SceneObject", AnimaSceneObject::GetObjectTree(false));
	
	return tree;
}

bool AnimaModelInstance::ReadObject(const ptree& objectTree, AnimaScene* scene, bool readName)
{
	try
	{
		if (readName)
			SetName(objectTree.get<AnimaString>("AnimaModelInstance.Name"));
		
		_modelName = objectTree.get<AnimaString>("AnimaModelInstance.ModelName", "");
		SetTopLevelModel(objectTree.get<bool>("AnimaModelInstance.TopLevelModel", false));
		
		if (!_modelName.empty())
			_model = scene->GetModelsManager()->GetModelFromName(_modelName, false);
		else
		{
			AnimaLogger::LogMessageFormat("WARNING - Error reading a model instance named '%s'. The instance source model name isn't specified", GetName().c_str());
		}
		
		for (auto& meshData : objectTree.get_child("AnimaModelInstance.MeshesInstances"))
		{
			if (meshData.first == "MeshInstance")
			{
				AnimaString meshInstanceName = meshData.second.get_value<AnimaString>("");
				if (meshInstanceName.empty())
					AnimaLogger::LogMessageFormat("WARNING - Error reading a model instance called '%s': found a mesh instance without name", GetName().c_str());
				else
				{
					AnimaMeshInstance* meshInstance = scene->GetMeshInstancesManager()->GetMeshInstanceFromName(meshInstanceName);
					if (meshInstance == nullptr)
						AnimaLogger::LogMessageFormat("WARNING - Error reading a model instance called '%s': model instance has a mesh instance named '%s' bot the mesh instance doesn't exists", GetName().c_str(), meshInstanceName.c_str());
					else
						AddMesh(meshInstance);
				}
			}
		}
		
		ptree sceneObjectTree = objectTree.get_child("AnimaModelInstance.SceneObject");
		if (AnimaSceneObject::ReadObject(sceneObjectTree, scene, false))
		{
			SetPosition(GetPosition());
			return true;
		}
		
		return false;
	}
	catch (boost::property_tree::ptree_bad_path& exception)
	{
		AnimaLogger::LogMessageFormat("ERROR - Error parsing model: %s", exception.what());
		return false;
	}
	catch (boost::property_tree::ptree_bad_data& exception)
	{
		AnimaLogger::LogMessageFormat("ERROR - Error parsing mesh: %s", exception.what());
		return false;
	}
}

void AnimaModelInstance::SetMeshes(AnimaArray<AnimaMeshInstance*>* meshes)
{
	_meshes.clear();

	if (meshes != nullptr)
	{
		_meshes = *meshes;

		AInt meshesCount = _meshes.size();
		for (AInt i = 0; i < meshesCount; i++)
			_meshes[i]->SetParentObject(this);
	}
}

void AnimaModelInstance::AddMesh(AnimaMeshInstance* mesh)
{
	mesh->SetParentObject(this);
	_meshes.push_back(mesh);
}

AInt AnimaModelInstance::GetMeshesCount() const
{
	return _meshes.size();
}

AnimaMeshInstance* AnimaModelInstance::GetMesh(AInt index)
{
	return _meshes[index];
}

void AnimaModelInstance::SetModel(AnimaModel* model)
{
	_model = model;
	if(_model != nullptr)
		_modelName = _model->GetName();
}

AnimaArray<AnimaMeshInstance*>* AnimaModelInstance::GetMeshes() const
{
	return (AnimaArray<AnimaMeshInstance*>*)&_meshes;
}

AnimaModel* AnimaModelInstance::GetModel()
{
	return _model;
}

void AnimaModelInstance::UpdateChildrenTransformation()
{
	AnimaSceneObject::UpdateChildrenTransformation();
	
	AInt meshesCount = _meshes.size();
	for (AInt i = 0; i < meshesCount; i++)
		_meshes[i]->GetTransformation()->UpdateMatrix();
}

void AnimaModelInstance::GetAllMeshes(AnimaArray<AnimaMeshInstance*> *meshes)
{
	if (meshes == nullptr)
		return;

	for (AInt nm = 0; nm < _meshes.size(); nm++)
		meshes->push_back(_meshes[nm]);

	for (AInt nc = 0; nc < _children.GetSize(); nc++)
		((AnimaModelInstance*)_children[nc])->GetAllMeshes(meshes);
}

END_ANIMA_ENGINE_NAMESPACE
