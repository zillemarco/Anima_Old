//
//  AnimaEngineModelsManager.cpp
//  Anima
//
//  Created by Marco Zille on 20/11/14.
//
//

#include "AnimaModelsManager.h"
#include "AnimaMaterialsManager.h"
#include "AnimaTexturesManager.h"
#include "AnimaBenchmarkTimer.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaModelsManager::AnimaModelsManager(AnimaScene* scene, AnimaMeshesManager* meshesManager, AnimaMaterialsManager* materialsManager)
: _models(scene->GetModelsAllocator())
, _topLevelModels(scene->GetModelsAllocator())
{
	ANIMA_ASSERT(scene != nullptr);
	ANIMA_ASSERT(meshesManager != nullptr);
	ANIMA_ASSERT(materialsManager != nullptr);

	_scene = scene;
	_meshesManager = meshesManager;
	_materialsManager = materialsManager;
}

AnimaModelsManager::~AnimaModelsManager()
{
	ClearModels();
}

AnimaModel* AnimaModelsManager::LoadModel(const char* modelPath, const char* name)
{
	AnimaString str(name, _scene->GetStringAllocator());
	return LoadModel(modelPath, str);
}

AnimaModel* AnimaModelsManager::LoadModel(const char* modelPath, const AnimaString& name)
{
	AnimaString str(modelPath, _scene->GetStringAllocator());
	return LoadModel(str, name);
}

AnimaModel* AnimaModelsManager::LoadModel(const AnimaString& modelPath, const char* name)
{
	AnimaString str(name, _scene->GetStringAllocator());
	return LoadModel(modelPath, str);
}

AnimaModel* AnimaModelsManager::LoadModel(const AnimaString& modelPath, const AnimaString& name)
{
	AInt index = _topLevelModels.Contains(name);
	if (index >= 0)
		return nullptr;
	
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelPath.GetConstBuffer(), aiProcessPreset_TargetRealtime_Quality);
	
	if (scene == nullptr)
		return nullptr;
	
	AnimaModel* newTopLevelModel = nullptr;

	if (_meshesManager->LoadMeshesFromModel(scene, name))
	{
		AnimaArray<AnimaString*>* nomi = _meshesManager->GetLastMeshesIndexMap();
		newTopLevelModel = LoadModelFromScene(scene, scene->mRootNode, nomi, name);
		newTopLevelModel->SetOriginFileName(modelPath);
		_topLevelModels.Add(name, newTopLevelModel);
	}

	newTopLevelModel->SetBoolean("ProvaProva", true);

	importer.FreeScene();
	return newTopLevelModel;
}

AnimaModel* AnimaModelsManager::LoadModelFromScene(const aiScene* scene, const aiNode* sceneNode, AnimaArray<AnimaString*>* meshesMap, const AnimaString& modelName)
{
	AnimaString newModelName(_scene->GetStringAllocator());
	if (!modelName.IsEmpty())
		newModelName = modelName;
	else
		newModelName = sceneNode->mName.C_Str();

	AnimaModel* currentModel = AnimaAllocatorNamespace::AllocateNew<AnimaModel>(*(_scene->GetModelsAllocator()), newModelName, _scene->GetDataGeneratorsManager(), _scene->GetModelsAllocator());
	_models.Add(currentModel);

	AnimaMatrix modelMatrix;
	modelMatrix.m[0] = sceneNode->mTransformation.a1;	modelMatrix.m[1] = sceneNode->mTransformation.a2;	modelMatrix.m[2] = sceneNode->mTransformation.a3;	modelMatrix.m[3] = sceneNode->mTransformation.a4;
	modelMatrix.m[4] = sceneNode->mTransformation.b1;	modelMatrix.m[5] = sceneNode->mTransformation.b2;	modelMatrix.m[6] = sceneNode->mTransformation.b3;	modelMatrix.m[7] = sceneNode->mTransformation.b4;
	modelMatrix.m[8] = sceneNode->mTransformation.c1;	modelMatrix.m[9] = sceneNode->mTransformation.c2;	modelMatrix.m[10] = sceneNode->mTransformation.c3;	modelMatrix.m[11] = sceneNode->mTransformation.c4;
	modelMatrix.m[12] = sceneNode->mTransformation.d1;	modelMatrix.m[13] = sceneNode->mTransformation.d2;	modelMatrix.m[14] = sceneNode->mTransformation.d3;	modelMatrix.m[15] = sceneNode->mTransformation.d4;

	currentModel->GetTransformation()->SetTransformationMatrix(modelMatrix.Transposed());

	for (AUint n = 0; n < sceneNode->mNumMeshes; n++)
	{
		AInt meshIndex = (AInt)sceneNode->mMeshes[n];
		AnimaString* meshName = meshesMap->GetAt(meshIndex);
		AnimaMesh* aMesh = _meshesManager->GetMeshFromName(*meshName);
		currentModel->AddMesh(aMesh);
	}

	for (AUint n = 0; n < sceneNode->mNumChildren; n++)
	{
		AnimaModel* child = LoadModelFromScene(scene, sceneNode->mChildren[n], meshesMap, AnimaString());
		currentModel->AddChild(child);
	}

	return currentModel;
}

AnimaModel* AnimaModelsManager::CreateModel(const AnimaString& name)
{
	AInt index = _topLevelModels.Contains(name);
	if (index >= 0)
		return nullptr;

	AnimaModel* newModel = AnimaAllocatorNamespace::AllocateNew<AnimaModel>(*(_scene->GetModelsAllocator()), name, _scene->GetDataGeneratorsManager(), _scene->GetModelsAllocator());
	_topLevelModels.Add(name, newModel);
	return newModel;
}

AnimaModel* AnimaModelsManager::CreateModel(const char* name)
{
	AnimaString str(name, _scene->GetStringAllocator());
	return CreateModel(str);
}

AInt AnimaModelsManager::GetModelsNumber()
{
	return _topLevelModels.GetSize();
}

AnimaModel* AnimaModelsManager::GetModel(AInt index)
{
	return _topLevelModels[index];
}

AnimaModel* AnimaModelsManager::GetModelFromName(const AnimaString& name)
{
	return _topLevelModels[name];
}

AnimaModel* AnimaModelsManager::GetModelFromName(const char* name)
{
	AnimaString str(name, _scene->GetStringAllocator());
	return GetModelFromName(str);
}

void AnimaModelsManager::ClearModels()
{
	AInt count = _models.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaModel* model = _models[i];
		AnimaAllocatorNamespace::DeallocateObject(*(_scene->GetModelsAllocator()), model);
		model = nullptr;
	}

	_models.RemoveAll();
	_topLevelModels.RemoveAll();
}

int texturesCaricate = 0;

void AnimaModelsManager::LoadMaterial(AnimaMesh* mesh, const aiMaterial* mtl)
{
	AnimaTexturesManager* texturesManager = _scene->GetTexturesManager();
	AnimaMaterialsManager* materialsManager = _scene->GetMaterialsManager();
	AnimaAllocator* stringAllocator = _scene->GetStringAllocator();

	AnimaMaterial* material = mesh->GetMaterial();
	AnimaString materialName(stringAllocator);

	if (material == nullptr)
	{
		int i = 1;

		AnimaString prefix = mesh->GetAnimaName();
		AnimaString suffix(stringAllocator);
		suffix.Format(".material.%d", i);

		while (material == nullptr)
		{
			materialName = prefix + suffix;
			material = materialsManager->CreateGenericMaterial(materialName);

			i++;
			suffix.Format(".material.%d", i);
		}

		mesh->SetMaterial(material);
	}
	else
		materialName = material->GetAnimaName();

	int ret1, ret2;
	aiColor4D diffuse;
	aiColor4D specular;
	aiColor4D ambient;
	aiColor4D emission;
	float shininess, strength;
	unsigned int max;
	int two_sided;
	
	if (aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse) == AI_SUCCESS)
		material->AddColor("DiffuseColor", diffuse.r, diffuse.g, diffuse.b, diffuse.a);
	else
		material->AddColor("DiffuseColor", 0.8f, 0.8f, 0.8f, 1.0f);

	if (aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular) == AI_SUCCESS)
		material->AddColor("SpecularColor", specular.r, specular.g, specular.b, specular.a);
	else
		material->AddColor("SpecularColor", 0.0f, 0.0f, 0.0f, 1.0f);

	if (aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient) == AI_SUCCESS)
		material->AddColor("AmbientColor", ambient.r, ambient.g, ambient.b, ambient.a);
	else
		material->AddColor("AmbientColor", 0.2f, 0.2f, 0.2f, 1.0f);

	if (aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission) == AI_SUCCESS)
		material->AddColor("EmissionColor", emission.r, emission.g, emission.b, emission.a);
	else
		material->AddColor("EmissionColor", 0.0f, 0.0f, 0.0f, 1.0f);
	
	max = 1;
	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
	if (ret1 == AI_SUCCESS) 
	{
		max = 1;
		ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);

		if (ret2 == AI_SUCCESS)
			material->AddFloat("Shininess", shininess * strength);
		else
			material->AddFloat("Shininess", shininess);
	}
	else 
	{
		material->AddFloat("Shininess", 0.0f);
		material->SetColor("SpecularColor", 0.0f, 0.0f, 0.0f, 1.0f);
	}

	max = 1;
	if ((aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max) == AI_SUCCESS) && two_sided)
		material->AddBoolean("TwoSided", true);
	else
		material->AddBoolean("TwoSided", false);
	
	aiString path;
	if (mtl->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
	{
		AnimaString txtCompletePath(ANIMA_ENGINE_MODELS_PATH, stringAllocator);
		AnimaString txtPath(path.C_Str(), stringAllocator);
		txtCompletePath += txtPath;
		txtCompletePath.Replace('\\', '/');

		material->SetTexture("DiffuseTexture", texturesManager->LoadTextureFromFile(txtCompletePath, txtPath));
		texturesCaricate++;
	}

	if (mtl->GetTexture(aiTextureType_SPECULAR, 0, &path) == AI_SUCCESS)
	{
		AnimaString txtCompletePath(ANIMA_ENGINE_MODELS_PATH, stringAllocator);
		AnimaString txtPath(path.C_Str(), stringAllocator);
		txtCompletePath += txtPath;
		txtCompletePath.Replace('\\', '/');

		AnimaTexture* texture = texturesManager->LoadTextureFromFile(txtCompletePath, txtPath);
		
		if (texture != nullptr)
		{
			material->SetTexture("SpecularTexture", texture);
			material->SetBoolean("HasSpecular", true);
		}
		else
		{
			ANIMA_ASSERT(false);
		}

		texturesCaricate++;
	}

	if (mtl->GetTexture(aiTextureType_AMBIENT, 0, &path) == AI_SUCCESS)
	{
		AnimaString txtCompletePath(ANIMA_ENGINE_MODELS_PATH, stringAllocator);
		AnimaString txtPath(path.C_Str(), stringAllocator);
		txtCompletePath += txtPath;
		txtCompletePath.Replace('\\', '/');

		AnimaTexture* texture = texturesManager->LoadTextureFromFile(txtCompletePath, txtPath);

		if (texture != nullptr)
		{
			material->SetTexture("AmbientTexture", texture);
		}
		else
		{
			ANIMA_ASSERT(false);
		}
		texturesCaricate++;
	}
	if (mtl->GetTexture(aiTextureType_EMISSIVE, 0, &path) == AI_SUCCESS)
	{
		AnimaString txtCompletePath(ANIMA_ENGINE_MODELS_PATH, stringAllocator);
		AnimaString txtPath(path.C_Str(), stringAllocator);
		txtCompletePath += txtPath;
		txtCompletePath.Replace('\\', '/');

		AnimaTexture* texture = texturesManager->LoadTextureFromFile(txtCompletePath, txtPath);

		if (texture != nullptr)
		{
			material->SetTexture("EmissiveTexture", texture);
		}
		else
		{
			ANIMA_ASSERT(false);
		}
		texturesCaricate++;
	}
	if (mtl->GetTexture(aiTextureType_HEIGHT, 0, &path) == AI_SUCCESS)
	{
		AnimaString txtCompletePath(ANIMA_ENGINE_MODELS_PATH, stringAllocator);
		AnimaString txtPath(path.C_Str(), stringAllocator);
		txtCompletePath += txtPath;
		txtCompletePath.Replace('\\', '/');

		AnimaTexture* texture = texturesManager->LoadTextureFromFile(txtCompletePath, txtPath);

		if (texture != nullptr)
		{
			material->SetTexture("BumpTexture", texture);
			material->SetBoolean("HasBump", true);
		}
		else
		{
			ANIMA_ASSERT(false);
		}
		texturesCaricate++;
	}
	if (mtl->GetTexture(aiTextureType_NORMALS, 0, &path) == AI_SUCCESS)
	{
		AnimaString txtCompletePath(ANIMA_ENGINE_MODELS_PATH, stringAllocator);
		AnimaString txtPath(path.C_Str(), stringAllocator);
		txtCompletePath += txtPath;
		txtCompletePath.Replace('\\', '/');

		AnimaTexture* texture = texturesManager->LoadTextureFromFile(txtCompletePath, txtPath);

		if (texture != nullptr)
		{
			material->SetTexture("BumpTexture", texture);
			material->SetBoolean("HasBump", true);
		}
		else
		{
			ANIMA_ASSERT(false);
		}
		texturesCaricate++;
	}
	if (mtl->GetTexture(aiTextureType_SHININESS, 0, &path) == AI_SUCCESS)
	{
		ANIMA_ASSERT(false);
		AnimaString txtCompletePath(ANIMA_ENGINE_MODELS_PATH, stringAllocator);
		AnimaString txtPath(path.C_Str(), stringAllocator);
		txtCompletePath += txtPath;
		txtCompletePath.Replace('\\', '/');

		AnimaTexture* texture = texturesManager->LoadTextureFromFile(txtCompletePath, txtPath);

		if (texture != nullptr)
		{
			material->SetTexture("ShininessTexture", texture);
		}
		else
		{
			ANIMA_ASSERT(false);
		}
		texturesCaricate++;
	}
	if (mtl->GetTexture(aiTextureType_OPACITY, 0, &path) == AI_SUCCESS)
	{
		AnimaString txtCompletePath(ANIMA_ENGINE_MODELS_PATH, stringAllocator);
		AnimaString txtPath(path.C_Str(), stringAllocator);
		txtCompletePath += txtPath;
		txtCompletePath.Replace('\\', '/');

		AnimaTexture* texture = texturesManager->LoadTextureFromFile(txtCompletePath, txtPath);

		if (texture != nullptr)
		{
			material->SetTexture("OpacityTexture", texture);
			material->SetBoolean("HasOpacity", true);
		}
		else
		{
			ANIMA_ASSERT(false);
		}
		texturesCaricate++;
	}
	if (mtl->GetTexture(aiTextureType_DISPLACEMENT, 0, &path) == AI_SUCCESS)
	{
		AnimaString txtCompletePath(ANIMA_ENGINE_MODELS_PATH, stringAllocator);
		AnimaString txtPath(path.C_Str(), stringAllocator);
		txtCompletePath += txtPath;
		txtCompletePath.Replace('\\', '/');

		AnimaTexture* texture = texturesManager->LoadTextureFromFile(txtCompletePath, txtPath);

		if (texture != nullptr)
		{
			material->SetTexture("DisplacementTexture", texture);
		}
		else
		{
			ANIMA_ASSERT(false);
		}
		texturesCaricate++;
	}
	if (mtl->GetTexture(aiTextureType_LIGHTMAP, 0, &path) == AI_SUCCESS)
	{
		AnimaString txtCompletePath(ANIMA_ENGINE_MODELS_PATH, stringAllocator);
		AnimaString txtPath(path.C_Str(), stringAllocator);
		txtCompletePath += txtPath;
		txtCompletePath.Replace('\\', '/');

		AnimaTexture* texture = texturesManager->LoadTextureFromFile(txtCompletePath, txtPath);

		if (texture != nullptr)
		{
			material->SetTexture("LightMapTexture", texture);
		}
		else
		{
			ANIMA_ASSERT(false);
		}
		texturesCaricate++;
	}
	if (mtl->GetTexture(aiTextureType_REFLECTION, 0, &path) == AI_SUCCESS)
	{
		ANIMA_ASSERT(false);
		AnimaString txtCompletePath(ANIMA_ENGINE_MODELS_PATH, stringAllocator);
		AnimaString txtPath(path.C_Str(), stringAllocator);
		txtCompletePath += txtPath;
		txtCompletePath.Replace('\\', '/');

		AnimaTexture* texture = texturesManager->LoadTextureFromFile(txtCompletePath, txtPath);
		if (texture != nullptr)
		{
			material->SetTexture("ReflectionTexture", texture);
		}
		else
		{
			ANIMA_ASSERT(false);
		}

		texturesCaricate++;
	}
}

END_ANIMA_ENGINE_NAMESPACE
