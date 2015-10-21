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
#include "AnimaScene.h"
#include "AnimaMaterialsManager.h"
#include "AnimaMeshesManager.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaMeshInstance::AnimaMeshInstance(const AnimaString& name, AnimaDataGeneratorsManager* dataGeneratorsManager, AnimaAllocator* allocator)
	: AnimaSceneObject(name, dataGeneratorsManager, allocator)
{
	IMPLEMENT_ANIMA_CLASS(AnimaMeshInstance);
	_material = nullptr;
	_mesh = nullptr;
	_shaderProgramName = "";
	_shaderProgram = nullptr;
}

AnimaMeshInstance::AnimaMeshInstance(const AnimaMeshInstance& src)
	: AnimaSceneObject(src)
	, _materialName(src._materialName)
	, _meshName(src._meshName)
	, _shadersNames(src._shadersNames)
	, _shaderProgram(src._shaderProgram)
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
	, _shaderProgram(src._shaderProgram)
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

		_shadersNames = src._shadersNames;
		_shaderProgramName = src._shaderProgramName;
		_shaderProgram = src._shaderProgram;
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

		_shadersNames = src._shadersNames;
		_shaderProgramName = src._shaderProgramName;
		_shaderProgram = src._shaderProgram;
	}
	
	return *this;
}

ptree AnimaMeshInstance::GetObjectTree(bool saveName) const
{
	ptree tree;
	
	if (saveName)
		tree.add("AnimaMeshInstance.Name", GetName());
	
	AnimaString materialName = _materialName;
	if (materialName.empty() && _material != nullptr)
		materialName = _material->GetName();
	tree.put("AnimaMeshInstance.Material", materialName);
	
	AnimaString meshName = _meshName;
	if(meshName.empty() && _mesh != nullptr)
		meshName = _mesh->GetName();
	tree.put("AnimaMeshInstance.MeshName", meshName);
	
	ptree shadersNamesTree;
	for(auto& shaderName : _shadersNames)
		shadersNamesTree.add("ShaderName", shaderName);
	tree.add_child("AnimaMeshInstance.ShaderNames", shadersNamesTree);
	
	tree.add_child("AnimaMeshInstance.SceneObject", AnimaSceneObject::GetObjectTree(false));
	
	return tree;
}

bool AnimaMeshInstance::ReadObject(const ptree& objectTree, AnimaScene* scene, bool readName)
{
	try
	{
		if (readName)
			SetName(objectTree.get<AnimaString>("AnimaMeshInstance.Name"));
		
		_materialName = objectTree.get<AnimaString>("AnimaMeshInstance.Material", "");
		_meshName = objectTree.get<AnimaString>("AnimaMeshInstance.MeshName", "");
		
		if (!_materialName.empty())
		{
			_material = scene->GetMaterialsManager()->GetMaterialFromName(_materialName);
			if(_material == nullptr)
			{
				AnimaLogger::LogMessageFormat("WARNING - Error reading mesh instance named '%s': this mesh instance has material named '%s' but this material can't be found", GetName().c_str(), _materialName.c_str());
			}
		}
		else
		{
			AnimaLogger::LogMessageFormat("WARNING - Error reading mesh instance named '%s': this mesh instance has a blank material name", GetName().c_str());
		}
		
		if (!_meshName.empty())
		{
			_mesh = scene->GetMeshesManager()->GetMeshFromName(_meshName);
			if(_mesh != nullptr)
				_mesh->AddInstance(this);
			else
			{
				AnimaLogger::LogMessageFormat("WARNING - Error reading mesh instance named '%s': this mesh instance is derived from a mesh named '%s' but this mesh can't be found, so this instance won't be rendered", GetName().c_str(), _meshName.c_str());
			}
		}
		else
		{
			AnimaLogger::LogMessageFormat("WARNING - Error reading mesh instance named '%s': this mesh instance has a blank source mesh name so this instance won't be rendered", GetName().c_str());
		}
		
		for(auto& shadersName : objectTree.get_child("AnimaMeshInstance.ShaderNames"))
		{
			if(shadersName.first == "ShaderName")
			{
				AnimaString shaderName = shadersName.second.get_value<AnimaString>("");
				_shadersNames.push_back(shaderName);
			}
		}
		
		ptree sceneObjectTree = objectTree.get_child("AnimaMeshInstance.SceneObject");
		if (AnimaSceneObject::ReadObject(sceneObjectTree, scene, false))
		{
			SetPosition(GetPosition());
			return true;
		}
		
		return false;
	}
	catch (boost::property_tree::ptree_bad_path& exception)
	{
		AnimaLogger::LogMessageFormat("ERROR - Error parsing mesh instance: %s", exception.what());
		return false;
	}
	catch (boost::property_tree::ptree_bad_data& exception)
	{
		AnimaLogger::LogMessageFormat("ERROR - Error parsing mesh instance: %s", exception.what());
		return false;
	}
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

void AnimaMeshInstance::Draw(AnimaRenderer* renderer, AnimaShaderProgram* program, bool start, bool end, bool updateMaterial)
{
	ANIMA_FRAME_PUSH("MeshInstanceUpdateProp");
	program->UpdateSceneObjectProperties(this, renderer);
	ANIMA_FRAME_POP();

	if (updateMaterial)
	{
		AnimaMaterial* material = _material;
		if (material == nullptr)
			material = AnimaMaterialsManager::GetDefaultMaterial();

		ANIMA_FRAME_PUSH("MeshInstanceMaterialUpdateProp");
		program->UpdateMappedValuesObjectProperties(material, renderer);
		ANIMA_FRAME_POP();
	}

#ifdef _WIN32
	if (start)
	{
		ANIMA_FRAME_PUSH("BindVAO");
		glBindVertexArray(_mesh->GetVertexArrayObject());
		ANIMA_FRAME_POP();
	}

	ANIMA_FRAME_PUSH("DrawElements");
	glDrawElements(GL_TRIANGLES, _mesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
	ANIMA_FRAME_POP();

	if (end)
	{
		ANIMA_FRAME_PUSH("Bind0");
		glBindVertexArray(0);
		ANIMA_FRAME_POP();
	}

#else
	program->EnableInputs(_mesh);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mesh->GetIndexesBufferObject());
	glDrawElements(GL_TRIANGLES, _mesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
	program->DisableInputs();
#endif
}

void AnimaMeshInstance::AddShader(AnimaShader* shader)
{
	_shadersNames.push_back(shader->GetName());
}

void AnimaMeshInstance::AddShader(const AnimaString& shaderName)
{
	_shadersNames.push_back(shaderName);
}

AInt AnimaMeshInstance::GetShadersCount() const
{
	return _shadersNames.size();
}

AnimaString AnimaMeshInstance::GetShaderName(AInt index) const
{
	return _shadersNames[index];
}

void AnimaMeshInstance::SetShaderProgram(AnimaShaderProgram* program)
{
	_shaderProgram = program;
	if (_shaderProgram)
		_shaderProgramName = _shaderProgram->GetName();
	else
		_shaderProgramName = "";
}

AnimaShaderProgram* AnimaMeshInstance::GetShaderProgram()
{
	return _shaderProgram;
}

void AnimaMeshInstance::SetShaderProgramName(const AnimaString& shaderProgramName)
{
	_shaderProgramName = shaderProgramName;
}

AnimaString AnimaMeshInstance::GetShaderProgramName() const
{
	return _shaderProgramName;
}

END_ANIMA_ENGINE_NAMESPACE
