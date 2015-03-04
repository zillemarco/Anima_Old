//
//  AnimaEngineModelsManager.cpp
//  Anima
//
//  Created by Marco Zille on 20/11/14.
//
//

#include "AnimaModelsManager.h"
#include "AnimaMaterialsManager.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaModelsManager::AnimaModelsManager(AnimaStage* stage)
{
	_stage = stage;
	
	_models = nullptr;
	_modelsNumber = 0;
	
	_nextModelID = 0;
}

AnimaModelsManager::~AnimaModelsManager()
{
	ClearModels();
}

AnimaMesh* AnimaModelsManager::LoadModel(const char* modelPath, const char* name)
{
	AnimaString str(name, _stage->GetStringAllocator());
	return LoadModel(modelPath, str);
}

AnimaMesh* AnimaModelsManager::LoadModel(const char* modelPath, const AnimaString& name)
{
	AnimaString str(modelPath, _stage->GetStringAllocator());
	return LoadModel(str, name);
}

AnimaMesh* AnimaModelsManager::LoadModel(const AnimaString& modelPath, const char* name)
{
	AnimaString str(name, _stage->GetStringAllocator());
	return LoadModel(modelPath, str);
}

AnimaMesh* AnimaModelsManager::LoadModel(const AnimaString& modelPath, const AnimaString& name)
{
	if (_modelsMap.find(name) != _modelsMap.end())
		return nullptr;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelPath.GetConstBuffer(), aiProcessPreset_TargetRealtime_Quality);
	
	if(scene == nullptr)
		return nullptr;
	
	AnimaMesh* newModel = AnimaAllocatorNamespace::AllocateNew<AnimaMesh>(*(_stage->GetModelsAllocator()), _stage->GetModelDataAllocator());
	
	RecursiveLoadMesh(newModel, scene, scene->mRootNode);

	AnimaString tmpName(_stage->GetStringAllocator());
	tmpName.Format("AnimaModel_%lu", GetNextModelID());
	newModel->SetMeshName(tmpName);

	AInt pos = modelPath.ReverseFind('/');
	
	if(pos == -1)
		pos = modelPath.ReverseFind('\\');
	
	if(pos != -1)
		pos++;
	
	AnimaString modelFileName(_stage->GetStringAllocator());
	modelFileName = modelPath.Substring(pos, modelPath.GetBufferLength());
	
	newModel->SetMeshFileName(modelFileName);
	
	AddModel(*newModel, name);
	
	AnimaAllocatorNamespace::DeallocateObject(*(_stage->GetModelsAllocator()), newModel);
	
	importer.FreeScene();

	_modelsMap[name] = (AUint)_modelsNumber - 1;
	
	return &_models[_modelsNumber - 1];
}

void AnimaModelsManager::RecursiveLoadMesh(AnimaMesh* currentModel, const aiScene *scene, const aiNode* sceneNode)
{
	if (sceneNode->mName.length > 0)
		currentModel->SetMeshName(sceneNode->mName.C_Str());

	AnimaMatrix modelMatrix;
	modelMatrix.m[0] = sceneNode->mTransformation.a1;	modelMatrix.m[1] = sceneNode->mTransformation.a2;	modelMatrix.m[2] = sceneNode->mTransformation.a3;	modelMatrix.m[3] = sceneNode->mTransformation.a4;
	modelMatrix.m[4] = sceneNode->mTransformation.b1;	modelMatrix.m[5] = sceneNode->mTransformation.b2;	modelMatrix.m[6] = sceneNode->mTransformation.b3;	modelMatrix.m[7] = sceneNode->mTransformation.b4;
	modelMatrix.m[8] = sceneNode->mTransformation.c1;	modelMatrix.m[9] = sceneNode->mTransformation.c2;	modelMatrix.m[10] = sceneNode->mTransformation.c3;	modelMatrix.m[11] = sceneNode->mTransformation.c4;
	modelMatrix.m[12] = sceneNode->mTransformation.d1;	modelMatrix.m[13] = sceneNode->mTransformation.d2;	modelMatrix.m[14] = sceneNode->mTransformation.d3;	modelMatrix.m[15] = sceneNode->mTransformation.d4;

	currentModel->GetTransformation()->SetTransformationMatrix(modelMatrix.Transposed());

	int numeroMesh = sceneNode->mNumMeshes;
	
	if(numeroMesh > 0)
	{
		AnimaMesh* meshes = AnimaAllocatorNamespace::AllocateArray<AnimaMesh>(*(_stage->GetModelsAllocator()), numeroMesh, _stage->GetModelDataAllocator());
	
		for (int n = 0; n < numeroMesh; n++)
		{
			const aiMesh* mesh = scene->mMeshes[sceneNode->mMeshes[n]];
		
			AnimaMesh* currentMesh = &meshes[n];

			LoadMaterial(currentMesh, scene->mMaterials[mesh->mMaterialIndex]);
		
			int numeroFacce = mesh->mNumFaces;
			int numeroVertici = mesh->mNumVertices;

			AnimaVertex3f* vertici = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*(_stage->GetGenericAllocator()), numeroVertici);
			AnimaFace* facce = AnimaAllocatorNamespace::AllocateArray<AnimaFace>(*(_stage->GetGenericAllocator()), numeroFacce, _stage->GetModelDataAllocator());
		
			int offsetFacce = 0;
			int offsetVertici = 0;
		
			for (int t = 0; t < numeroVertici; t++)
			{
				const aiVector3D* vert = &mesh->mVertices[t];

				vertici[offsetVertici].x = vert->x;
				vertici[offsetVertici].y = vert->y;
				vertici[offsetVertici].z = vert->z;
			
				offsetVertici++;
			}

			for (int t = 0; t < numeroFacce; t++)
			{
				const aiFace* face = &mesh->mFaces[t];
				
				int numeroIndiciFaccia = face->mNumIndices;
				AUint* indiciFaccia = AnimaAllocatorNamespace::AllocateArray<AUint>(*(_stage->GetGenericAllocator()), numeroIndiciFaccia);
				
				int offsetIndiciFaccia = 0;
				
				for(int i = 0; i < numeroIndiciFaccia; i++)
					indiciFaccia[offsetIndiciFaccia++] = face->mIndices[i];
								
				facce[offsetFacce++].SetIndexes(indiciFaccia, offsetIndiciFaccia);
				AnimaAllocatorNamespace::DeallocateArray(*(_stage->GetGenericAllocator()), indiciFaccia);
			}

			if (mesh->HasNormals())
			{
				AnimaVertex3f* normali = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*(_stage->GetGenericAllocator()), numeroVertici);
				int offsetNormali = 0;
				for (int t = 0; t < numeroVertici; t++)
				{
					const aiVector3D* norm = &mesh->mNormals[t];

					normali[offsetNormali].x = norm->x;
					normali[offsetNormali].y = norm->y;
					normali[offsetNormali].z = norm->z;

					offsetNormali++;
				}

				currentMesh->SetNormals(normali, offsetNormali);
				AnimaAllocatorNamespace::DeallocateArray(*(_stage->GetGenericAllocator()), normali);
			}

			if (mesh->HasTangentsAndBitangents())
			{
				AnimaVertex3f* tangents = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*(_stage->GetGenericAllocator()), numeroVertici);
				AnimaVertex3f* bitangents = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*(_stage->GetGenericAllocator()), numeroVertici);
				
				int offsetTangents = 0;
				int offsetBitangents = 0;

				for (int t = 0; t < numeroVertici; t++)
				{
					const aiVector3D* tang = &mesh->mTangents[t];
					const aiVector3D* bita = &mesh->mBitangents[t];

					tangents[offsetTangents].x = tang->x;
					tangents[offsetTangents].y = tang->y;
					tangents[offsetTangents].z = tang->z;

					bitangents[offsetBitangents].x = bita->x;
					bitangents[offsetBitangents].y = bita->y;
					bitangents[offsetBitangents].z = bita->z;

					offsetTangents++;
					offsetBitangents++;
				}

				currentMesh->SetTangents(tangents, offsetTangents);
				currentMesh->SetBitangents(bitangents, offsetTangents);

				AnimaAllocatorNamespace::DeallocateArray(*(_stage->GetGenericAllocator()), tangents);
				AnimaAllocatorNamespace::DeallocateArray(*(_stage->GetGenericAllocator()), bitangents);
			}

			if (mesh->HasTextureCoords(0))
			{
				AnimaVertex2f* textCoords = AnimaAllocatorNamespace::AllocateArray<AnimaVertex2f>(*(_stage->GetGenericAllocator()), numeroVertici);
				int offsetTextCoords = 0;
				for (int t = 0; t < numeroVertici; t++)
				{
					textCoords[offsetTextCoords].u = mesh->mTextureCoords[0][t].x;
					textCoords[offsetTextCoords].v = mesh->mTextureCoords[0][t].y;

					offsetTextCoords++;
				}

				currentMesh->SetTextureCoords(textCoords, offsetTextCoords);
				AnimaAllocatorNamespace::DeallocateArray(*(_stage->GetGenericAllocator()), textCoords);
			}

			currentMesh->SetVertices(vertici, offsetVertici);
			currentMesh->SetFaces(facce, offsetFacce);
			
			AnimaAllocatorNamespace::DeallocateArray(*(_stage->GetGenericAllocator()), vertici);
			AnimaAllocatorNamespace::DeallocateArray(*(_stage->GetGenericAllocator()), facce);
		}
	
		currentModel->SetMeshes(meshes, numeroMesh);
		
		AnimaAllocatorNamespace::DeallocateArray(*(_stage->GetModelsAllocator()), meshes);
	}
	
	int numeroFigli = sceneNode->mNumChildren;
	if(numeroFigli > 0)
	{
		AnimaMesh* figli = AnimaAllocatorNamespace::AllocateArray<AnimaMesh>(*(_stage->GetGenericAllocator()), numeroFigli, _stage->GetModelDataAllocator());
	
		for (int n = 0; n < numeroFigli; n++)
			RecursiveLoadMesh(&figli[n], scene, sceneNode->mChildren[n]);
	
		currentModel->SetChildren(figli, numeroFigli);
		
		AnimaAllocatorNamespace::DeallocateArray(*(_stage->GetGenericAllocator()), figli);
	}
}

void AnimaModelsManager::AddModel(const AnimaMesh& model, const char* name)
{
	AnimaString str(name, _stage->GetStringAllocator());
	AddModel(model, str);
}

void AnimaModelsManager::AddModel(const AnimaMesh& model, const AnimaString& name)
{
	ANIMA_ASSERT(_stage != nullptr);
	if(_modelsNumber > 0)
	{
		AnimaMesh* tmpOldModels = AnimaAllocatorNamespace::AllocateArray<AnimaMesh>(*(_stage->GetModelsAllocator()), _modelsNumber, _stage->GetModelDataAllocator());
	
		for (int i = 0; i < _modelsNumber; i++)
			tmpOldModels[i] = _models[i];
	
		AnimaAllocatorNamespace::DeallocateArray(*(_stage->GetModelsAllocator()), _models);
	
		_modelsNumber++;
		_models = AnimaAllocatorNamespace::AllocateArray<AnimaMesh>(*(_stage->GetModelsAllocator()), _modelsNumber, _stage->GetModelDataAllocator());
	
		for (int i = 0; i < _modelsNumber - 1; i++)
			_models[i] = tmpOldModels[i];
	
		_models[_modelsNumber - 1] = model;
		
		AnimaAllocatorNamespace::DeallocateArray(*(_stage->GetModelsAllocator()), tmpOldModels);
	}
	else
	{
		_modelsNumber++;
		_models = AnimaAllocatorNamespace::AllocateArray<AnimaMesh>(*(_stage->GetModelsAllocator()), _modelsNumber, _stage->GetModelDataAllocator());
		_models[_modelsNumber - 1] = model;
	}
}

AnimaMesh* AnimaModelsManager::CreateModel(const AnimaString& name)
{
	if (_modelsMap.find(name) != _modelsMap.end())
		return nullptr;

	ANIMA_ASSERT(_stage != nullptr);
	if (_modelsNumber > 0)
	{
		AnimaMesh* tmpOldModels = AnimaAllocatorNamespace::AllocateArray<AnimaMesh>(*(_stage->GetModelsAllocator()), _modelsNumber, _stage->GetModelDataAllocator());

		for (int i = 0; i < _modelsNumber; i++)
			tmpOldModels[i] = _models[i];

		AnimaAllocatorNamespace::DeallocateArray(*(_stage->GetModelsAllocator()), _models);

		_modelsNumber++;
		_models = AnimaAllocatorNamespace::AllocateArray<AnimaMesh>(*(_stage->GetModelsAllocator()), _modelsNumber, _stage->GetModelDataAllocator());

		for (int i = 0; i < _modelsNumber - 1; i++)
			_models[i] = tmpOldModels[i];

		AnimaAllocatorNamespace::DeallocateArray(*(_stage->GetModelsAllocator()), tmpOldModels);
	}
	else
	{
		_modelsNumber++;
		_models = AnimaAllocatorNamespace::AllocateArray<AnimaMesh>(*(_stage->GetModelsAllocator()), _modelsNumber, _stage->GetModelDataAllocator());
	}

	_modelsMap[name] = (AUint)_modelsNumber - 1;
	return &_models[_modelsNumber - 1];
}

AnimaMesh* AnimaModelsManager::CreateModel(const char* name)
{
	AnimaString str(name, _stage->GetStringAllocator());
	return CreateModel(str);
}

AnimaMesh* AnimaModelsManager::CreatePlane(const AnimaString& name)
{
	if (_modelsMap.find(name) != _modelsMap.end())
		return nullptr;

	AnimaMesh* newModel = CreateModel(name);
	newModel->MakePlane();
	AnimaMaterial* material = newModel->GetMaterial();

	int i = 1;
	AnimaString prefix = name;
	AnimaString suffix(_stage->GetStringAllocator());
	suffix.Format(".material.%d", i);

	while (material == nullptr)
	{
		material = _stage->GetMaterialsManager()->CreateMaterial(prefix + suffix);

		i++;
		suffix.Format(".material.%d", i);
	}

	material->AddColor("diffuseColor", 0.8f, 0.8f, 0.8f, 1.0f);

	return newModel;
}

AnimaMesh* AnimaModelsManager::CreatePlane(const char* name)
{
	AnimaString str(name, _stage->GetStringAllocator());
	return CreatePlane(str);
}

ASizeT AnimaModelsManager::GetModelsNumber()
{
	return _modelsNumber;
}

AnimaMesh* AnimaModelsManager::GetModel(ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < _modelsNumber);
	return &_models[index];
}

AnimaMesh* AnimaModelsManager::GetModels()
{
	return _models;
}

void AnimaModelsManager::ClearModels()
{
	if(_models != nullptr && _modelsNumber > 0)
	{
		AnimaAllocatorNamespace::DeallocateArray(*(_stage->GetModelsAllocator()), _models);
		_models = nullptr;
		_modelsNumber = 0;
	}
}

ASizeT AnimaModelsManager::GetNextModelID()
{
	return _nextModelID++;
}

AnimaMesh* AnimaModelsManager::GetModelFromName(const AnimaString& name)
{
	if (_modelsMap.find(name) == _modelsMap.end())
		return nullptr;
	return GetModel((ASizeT)_modelsMap[name]);
}

AnimaMesh* AnimaModelsManager::GetModelFromName(const char* name)
{
	AnimaString str(name, _stage->GetStringAllocator());
	return GetModelFromName(str);
}

void AnimaModelsManager::LoadMaterial(AnimaMesh* mesh, const aiMaterial* mtl)
{
	AnimaMaterial* material = mesh->GetMaterial();

	if (material == nullptr)
	{
		int i = 1;

		AnimaString prefix = mesh->GetAnimaMeshName();
		AnimaString suffix(_stage->GetStringAllocator());
		suffix.Format(".material.%d", i);

		while (material == nullptr)
		{
			material = _stage->GetMaterialsManager()->CreateMaterial(prefix + suffix);

			i++;
			suffix.Format(".material.%d", i);
		}

		mesh->SetMaterial(material);
	}

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
}

END_ANIMA_ENGINE_NAMESPACE
