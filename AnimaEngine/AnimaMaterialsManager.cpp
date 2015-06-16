//
//  AnimaMaterialsManager.cpp
//  Anima
//
//  Created by Marco Zille on 14/01/15.
//
//

#include "AnimaMaterialsManager.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaMaterialsManager::AnimaMaterialsManager(AnimaScene* scene, AnimaTexturesManager* texturesManager)
	: _materials(scene->GetEngine()->GetMaterialsAllocator())
	, _lastMaterialsIndexMap(scene->GetGenericAllocator())
{
	ANIMA_ASSERT(texturesManager != nullptr);
	_scene = scene;
	_texturesManager = texturesManager;
}

AnimaMaterialsManager::~AnimaMaterialsManager()
{
	ClearMaterials();
	ClearLastMaterialsIndexMap();
}

AnimaMaterial* AnimaMaterialsManager::CreateGenericMaterial(const AnimaString& name)
{
	return CreateMaterial<AnimaMaterial>(name);
}

AnimaMaterial* AnimaMaterialsManager::CreateGenericMaterial(const char* name)
{
	AnimaString str(name, _scene->GetStringAllocator());
	return CreateGenericMaterial(str);
}

void AnimaMaterialsManager::ClearMaterials()
{
	boost::unordered_map<AnimaString, AnimaMappedArray<AnimaMaterial*>*, AnimaString::Hasher>* materialsMap = _materials.GetArraysMap();
	for (auto materialsPair : (*materialsMap))
	{
		AnimaMappedArray<AnimaMaterial*>* materialsArray = materialsPair.second;
		AInt count = materialsArray->GetSize();
		for (AInt i = 0; i < count; i++)
		{
			AnimaMaterial* material = (*materialsArray)[i];
			AnimaAllocatorNamespace::DeallocateObject(*(_scene->GetMaterialsAllocator()), material);
			material = nullptr;
		}
	}
	
	_materials.RemoveAll();
}

AnimaMaterial* AnimaMaterialsManager::GetMaterial(AInt index)
{
	return _materials.GetWithIndex(index);
}

AnimaMaterial* AnimaMaterialsManager::GetMaterialFromName(const AnimaString& name)
{
	return _materials.GetWithName(name);
}

AnimaMaterial* AnimaMaterialsManager::GetMaterialFromName(const char* name)
{
	AnimaString str(name, _scene->GetStringAllocator());
	return GetMaterialFromName(str);
}

AInt AnimaMaterialsManager::GetTotalMaterialsCount()
{
	return _materials.GetTotalSize();
}

AnimaTypeMappedArray<AnimaMaterial*>* AnimaMaterialsManager::GetMaterials()
{
	return &_materials;
}

bool AnimaMaterialsManager::LoadMaterialsFromModel(const aiScene* scene, const AnimaString& modelName)
{
	ClearLastMaterialsIndexMap();

	AnimaAllocator* stringsAllocator = _scene->GetStringAllocator();

	for (AUint i = 0; i < scene->mNumMaterials; i++)
	{
		const aiMaterial* material = scene->mMaterials[i];
		AnimaMaterial* newMaterial = nullptr;

		AnimaString* materialName = AnimaAllocatorNamespace::AllocateNew<AnimaString>(*(_scene->GetStringAllocator()), stringsAllocator);
		int nameOffset = 0;
		while (newMaterial == nullptr)
		{
			materialName->Format("%s.material%d", modelName.GetConstBuffer(), i + nameOffset);
			newMaterial = CreateGenericMaterial(*materialName);
			nameOffset++;
		}
		
		int ret1, ret2;
		aiColor4D diffuse;
		aiColor4D specular;
		aiColor4D ambient;
		aiColor4D emission;
		float shininess, strength;
		unsigned int max;
		int two_sided;

		if (aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse) == AI_SUCCESS)
			newMaterial->AddColor("DiffuseColor", diffuse.r, diffuse.g, diffuse.b, diffuse.a);
		else
			newMaterial->AddColor("DiffuseColor", 0.8f, 0.8f, 0.8f, 1.0f);

		if (aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular) == AI_SUCCESS)
			newMaterial->AddColor("SpecularColor", specular.r, specular.g, specular.b, specular.a);
		else
			newMaterial->AddColor("SpecularColor", 0.0f, 0.0f, 0.0f, 1.0f);

		if (aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambient) == AI_SUCCESS)
			newMaterial->AddColor("AmbientColor", ambient.r, ambient.g, ambient.b, ambient.a);
		else
			newMaterial->AddColor("AmbientColor", 0.2f, 0.2f, 0.2f, 1.0f);

		if (aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &emission) == AI_SUCCESS)
			newMaterial->AddColor("EmissionColor", emission.r, emission.g, emission.b, emission.a);
		else
			newMaterial->AddColor("EmissionColor", 0.0f, 0.0f, 0.0f, 1.0f);

		max = 1;
		ret1 = aiGetMaterialFloatArray(material, AI_MATKEY_SHININESS, &shininess, &max);
		if (ret1 == AI_SUCCESS)
		{
			max = 1;
			ret2 = aiGetMaterialFloatArray(material, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);

			if (ret2 == AI_SUCCESS)
				newMaterial->AddFloat("Shininess", shininess * strength);
			else
				newMaterial->AddFloat("Shininess", shininess);
		}
		else
		{
			newMaterial->AddFloat("Shininess", 0.0f);
			newMaterial->SetColor("SpecularColor", 0.0f, 0.0f, 0.0f, 1.0f);
		}

		max = 1;
		if ((aiGetMaterialIntegerArray(material, AI_MATKEY_TWOSIDED, &two_sided, &max) == AI_SUCCESS) && two_sided)
			newMaterial->AddBoolean("TwoSided", true);
		else
			newMaterial->AddBoolean("TwoSided", false);

		aiString path;
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
		{
			AnimaString txtCompletePath(ANIMA_ENGINE_MODELS_PATH, stringsAllocator);
			AnimaString txtPath(path.C_Str(), stringsAllocator);
			txtCompletePath += txtPath;
			txtCompletePath.Replace('\\', '/');

			newMaterial->SetTexture("DiffuseTexture", _texturesManager->LoadTextureFromFile(txtCompletePath, txtPath));
		}

		if (material->GetTexture(aiTextureType_SPECULAR, 0, &path) == AI_SUCCESS)
		{
			AnimaString txtCompletePath(ANIMA_ENGINE_MODELS_PATH, stringsAllocator);
			AnimaString txtPath(path.C_Str(), stringsAllocator);
			txtCompletePath += txtPath;
			txtCompletePath.Replace('\\', '/');

			AnimaTexture* texture = _texturesManager->LoadTextureFromFile(txtCompletePath, txtPath);

			if (texture != nullptr)
			{
				newMaterial->SetTexture("SpecularTexture", texture);
				newMaterial->SetBoolean("HasSpecular", true);
			}
			else
			{
//				ANIMA_ASSERT(false);
			}
		}

		if (material->GetTexture(aiTextureType_AMBIENT, 0, &path) == AI_SUCCESS)
		{
			AnimaString txtCompletePath(ANIMA_ENGINE_MODELS_PATH, stringsAllocator);
			AnimaString txtPath(path.C_Str(), stringsAllocator);
			txtCompletePath += txtPath;
			txtCompletePath.Replace('\\', '/');

			AnimaTexture* texture = _texturesManager->LoadTextureFromFile(txtCompletePath, txtPath);

			if (texture != nullptr)
			{
				newMaterial->SetTexture("AmbientTexture", texture);
			}
			else
			{
//				ANIMA_ASSERT(false);
			}
		}
		if (material->GetTexture(aiTextureType_EMISSIVE, 0, &path) == AI_SUCCESS)
		{
			AnimaString txtCompletePath(ANIMA_ENGINE_MODELS_PATH, stringsAllocator);
			AnimaString txtPath(path.C_Str(), stringsAllocator);
			txtCompletePath += txtPath;
			txtCompletePath.Replace('\\', '/');

			AnimaTexture* texture = _texturesManager->LoadTextureFromFile(txtCompletePath, txtPath);

			if (texture != nullptr)
			{
				newMaterial->SetTexture("EmissiveTexture", texture);
			}
			else
			{
//				ANIMA_ASSERT(false);
			}
		}
		if (material->GetTexture(aiTextureType_HEIGHT, 0, &path) == AI_SUCCESS)
		{
			AnimaString txtCompletePath(ANIMA_ENGINE_MODELS_PATH, stringsAllocator);
			AnimaString txtPath(path.C_Str(), stringsAllocator);
			txtCompletePath += txtPath;
			txtCompletePath.Replace('\\', '/');

			AnimaTexture* texture = _texturesManager->LoadTextureFromFile(txtCompletePath, txtPath);

			if (texture != nullptr)
			{
				newMaterial->SetTexture("BumpTexture", texture);
				newMaterial->SetBoolean("HasBump", true);
			}
			else
			{
//				ANIMA_ASSERT(false);
			}
		}
		if (material->GetTexture(aiTextureType_NORMALS, 0, &path) == AI_SUCCESS)
		{
			AnimaString txtCompletePath(ANIMA_ENGINE_MODELS_PATH, stringsAllocator);
			AnimaString txtPath(path.C_Str(), stringsAllocator);
			txtCompletePath += txtPath;
			txtCompletePath.Replace('\\', '/');

			AnimaTexture* texture = _texturesManager->LoadTextureFromFile(txtCompletePath, txtPath);

			if (texture != nullptr)
			{
				newMaterial->SetTexture("BumpTexture", texture);
				newMaterial->SetBoolean("HasBump", true);
			}
			else
			{
//				ANIMA_ASSERT(false);
			}
		}
		if (material->GetTexture(aiTextureType_SHININESS, 0, &path) == AI_SUCCESS)
		{
			ANIMA_ASSERT(false);
			AnimaString txtCompletePath(ANIMA_ENGINE_MODELS_PATH, stringsAllocator);
			AnimaString txtPath(path.C_Str(), stringsAllocator);
			txtCompletePath += txtPath;
			txtCompletePath.Replace('\\', '/');

			AnimaTexture* texture = _texturesManager->LoadTextureFromFile(txtCompletePath, txtPath);

			if (texture != nullptr)
			{
				newMaterial->SetTexture("ShininessTexture", texture);
			}
			else
			{
//				ANIMA_ASSERT(false);
			}
		}
		if (material->GetTexture(aiTextureType_OPACITY, 0, &path) == AI_SUCCESS)
		{
			AnimaString txtCompletePath(ANIMA_ENGINE_MODELS_PATH, stringsAllocator);
			AnimaString txtPath(path.C_Str(), stringsAllocator);
			txtCompletePath += txtPath;
			txtCompletePath.Replace('\\', '/');

			AnimaTexture* texture = _texturesManager->LoadTextureFromFile(txtCompletePath, txtPath);

			if (texture != nullptr)
			{
				newMaterial->SetTexture("OpacityTexture", texture);
				newMaterial->SetBoolean("HasOpacity", true);
			}
			else
			{
//				ANIMA_ASSERT(false);
			}
		}
		if (material->GetTexture(aiTextureType_DISPLACEMENT, 0, &path) == AI_SUCCESS)
		{
			AnimaString txtCompletePath(ANIMA_ENGINE_MODELS_PATH, stringsAllocator);
			AnimaString txtPath(path.C_Str(), stringsAllocator);
			txtCompletePath += txtPath;
			txtCompletePath.Replace('\\', '/');

			AnimaTexture* texture = _texturesManager->LoadTextureFromFile(txtCompletePath, txtPath);

			if (texture != nullptr)
			{
				newMaterial->SetTexture("DisplacementTexture", texture);
			}
			else
			{
//				ANIMA_ASSERT(false);
			}
		}
		if (material->GetTexture(aiTextureType_LIGHTMAP, 0, &path) == AI_SUCCESS)
		{
			AnimaString txtCompletePath(ANIMA_ENGINE_MODELS_PATH, stringsAllocator);
			AnimaString txtPath(path.C_Str(), stringsAllocator);
			txtCompletePath += txtPath;
			txtCompletePath.Replace('\\', '/');

			AnimaTexture* texture = _texturesManager->LoadTextureFromFile(txtCompletePath, txtPath);

			if (texture != nullptr)
			{
				newMaterial->SetTexture("LightMapTexture", texture);
			}
			else
			{
//				ANIMA_ASSERT(false);
			}
		}
		if (material->GetTexture(aiTextureType_REFLECTION, 0, &path) == AI_SUCCESS)
		{
			ANIMA_ASSERT(false);
			AnimaString txtCompletePath(ANIMA_ENGINE_MODELS_PATH, stringsAllocator);
			AnimaString txtPath(path.C_Str(), stringsAllocator);
			txtCompletePath += txtPath;
			txtCompletePath.Replace('\\', '/');

			AnimaTexture* texture = _texturesManager->LoadTextureFromFile(txtCompletePath, txtPath);
			if (texture != nullptr)
			{
				newMaterial->SetTexture("ReflectionTexture", texture);
			}
			else
			{
//				ANIMA_ASSERT(false);
			}
		}

		_lastMaterialsIndexMap.Add(materialName);
	}

	return true;
}

AnimaArray<AnimaString*>* AnimaMaterialsManager::GetLastMaterialsIndexMap()
{
	return &_lastMaterialsIndexMap;
}

void AnimaMaterialsManager::ClearLastMaterialsIndexMap()
{
	AInt materialsIndexCount = _lastMaterialsIndexMap.GetSize();
	for (AInt i = 0; i < materialsIndexCount; i++)
	{
		AnimaString* materialIndex = _lastMaterialsIndexMap[i];
		AnimaAllocatorNamespace::DeallocateObject(*(_scene->GetStringAllocator()), materialIndex);
		materialIndex = nullptr;
	}
	_lastMaterialsIndexMap.RemoveAll();
}

END_ANIMA_ENGINE_NAMESPACE