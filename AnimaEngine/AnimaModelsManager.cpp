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

AnimaModelsManager::AnimaModelsManager(AnimaEngine* engine)
{
	_engine = engine;
	
	_models = nullptr;
	_modelsNumber = 0;
	
	_nextModelID = 0;
}

AnimaModelsManager::~AnimaModelsManager()
{
	ClearModels();
}

AnimaModel* AnimaModelsManager::LoadModel(const char* modelPath, const char* name)
{
	AnimaString str(name, _engine);
	return LoadModel(modelPath, str);
}

AnimaModel* AnimaModelsManager::LoadModel(const char* modelPath, const AnimaString& name)
{
	AnimaString str(modelPath, _engine);
	return LoadModel(str, name);
}

AnimaModel* AnimaModelsManager::LoadModel(const AnimaString& modelPath, const char* name)
{
	AnimaString str(name, _engine);
	return LoadModel(modelPath, str);
}

AnimaModel* AnimaModelsManager::LoadModel(const AnimaString& modelPath, const AnimaString& name)
{
	if (_modelsMap.find(name) != _modelsMap.end())
		return nullptr;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelPath.GetConstBuffer(), aiProcessPreset_TargetRealtime_Quality);
	
	if(scene == nullptr)
		return nullptr;
	
	AnimaModel* newModel = AnimaAllocatorNamespace::AllocateNew<AnimaModel>(*(_engine->GetModelsAllocator()), _engine);
	
	RecursiveLoadMesh(newModel, scene, scene->mRootNode);

	AnimaString tmpName(_engine);
	tmpName.Format("AnimaModel_%lu", GetNextModelID());
	newModel->SetModelName(tmpName);

	AInt pos = modelPath.ReverseFind('/');
	
	if(pos == -1)
		pos = modelPath.ReverseFind('\\');
	
	if(pos != -1)
		pos++;
	
	AnimaString modelFileName(_engine);
	modelFileName = modelPath.Substring(pos, modelPath.GetBufferLength());
	
	newModel->SetModelFileName(modelFileName);
	
	AddModel(*newModel, name);
	
	AnimaAllocatorNamespace::DeallocateObject(*(_engine->GetModelsAllocator()), newModel);
	
	importer.FreeScene();

	_modelsMap[name] = (AUint)_modelsNumber - 1;
	
	return &_models[_modelsNumber - 1];
}

void AnimaModelsManager::RecursiveLoadMesh(AnimaModel* currentModel, const aiScene *scene, const aiNode* sceneNode)
{
	if (sceneNode->mName.length > 0)
		currentModel->SetModelName(sceneNode->mName.C_Str());

	AnimaMatrix modelMatrix(_engine);
	modelMatrix.SetData(sceneNode->mTransformation.a1, 0);	modelMatrix.SetData(sceneNode->mTransformation.a2, 1);	modelMatrix.SetData(sceneNode->mTransformation.a3, 2);	modelMatrix.SetData(sceneNode->mTransformation.a4, 3);
	modelMatrix.SetData(sceneNode->mTransformation.b1, 4);	modelMatrix.SetData(sceneNode->mTransformation.b2, 5);	modelMatrix.SetData(sceneNode->mTransformation.b3, 6);	modelMatrix.SetData(sceneNode->mTransformation.b4, 7);
	modelMatrix.SetData(sceneNode->mTransformation.c1, 8);	modelMatrix.SetData(sceneNode->mTransformation.c2, 9);	modelMatrix.SetData(sceneNode->mTransformation.c3, 10); modelMatrix.SetData(sceneNode->mTransformation.c4, 11);
	modelMatrix.SetData(sceneNode->mTransformation.d1, 12); modelMatrix.SetData(sceneNode->mTransformation.d2, 13); modelMatrix.SetData(sceneNode->mTransformation.d3, 14); modelMatrix.SetData(sceneNode->mTransformation.d4, 15);

	currentModel->GetTransformation()->SetTransformationMatrix(modelMatrix.Transpose());

	int numeroMesh = sceneNode->mNumMeshes;
	
	if(numeroMesh > 0)
	{
		AnimaMesh* meshes = AnimaAllocatorNamespace::AllocateArray<AnimaMesh>(*(_engine->GetModelsAllocator()), numeroMesh, _engine);
	
		for (int n = 0; n < numeroMesh; n++)
		{
			const aiMesh* mesh = scene->mMeshes[sceneNode->mMeshes[n]];
		
			AnimaMesh* currentMesh = &meshes[n];

			LoadMaterial(currentMesh, scene->mMaterials[mesh->mMaterialIndex]);
		
			int numeroFacce = mesh->mNumFaces;
			int numeroVertici = mesh->mNumVertices;

			AnimaVertex3f* vertici = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*(_engine->GetGenericAllocator()), numeroVertici);
			//AnimaVertex3f* vertici = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*(_engine->GetGenericAllocator()), numeroVertici, _engine);
			AnimaFace* facce = AnimaAllocatorNamespace::AllocateArray<AnimaFace>(*(_engine->GetGenericAllocator()), numeroFacce, _engine);
		
			int offsetFacce = 0;
			int offsetVertici = 0;
		
			for (int t = 0; t < numeroVertici; t++)
			{
				const aiVector3D* vert = &mesh->mVertices[t];

				vertici[offsetVertici].x = vert->x;
				vertici[offsetVertici].y = vert->y;
				vertici[offsetVertici].z = vert->z;
				//vertici[offsetVertici][0] = vert->x;
				//vertici[offsetVertici][1] = vert->y;
				//vertici[offsetVertici][2] = vert->z;
			
				offsetVertici++;
			}

			for (int t = 0; t < numeroFacce; t++)
			{
				const aiFace* face = &mesh->mFaces[t];
				
				int numeroIndiciFaccia = face->mNumIndices;
				AUint* indiciFaccia = AnimaAllocatorNamespace::AllocateArray<AUint>(*(_engine->GetGenericAllocator()), numeroIndiciFaccia);
				
				int offsetIndiciFaccia = 0;
				
				for(int i = 0; i < numeroIndiciFaccia; i++)
					indiciFaccia[offsetIndiciFaccia++] = face->mIndices[i];
								
				facce[offsetFacce++].SetIndexes(indiciFaccia, offsetIndiciFaccia);
				AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetGenericAllocator()), indiciFaccia);
			}

			if (mesh->HasNormals())
			{
				AnimaVertex3f* normali = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*(_engine->GetGenericAllocator()), numeroVertici);
				//AnimaVertex3f* normali = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*(_engine->GetGenericAllocator()), numeroVertici, _engine);
				int offsetNormali = 0;
				for (int t = 0; t < numeroVertici; t++)
				{
					const aiVector3D* norm = &mesh->mNormals[t];

					normali[offsetNormali].x = norm->x;
					normali[offsetNormali].y = norm->y;
					normali[offsetNormali].z = norm->z;
					//normali[offsetNormali][0] = norm->x;
					//normali[offsetNormali][1] = norm->y;
					//normali[offsetNormali][2] = norm->z;

					offsetNormali++;
				}

				currentMesh->SetNormals(normali, offsetNormali);
				AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetGenericAllocator()), normali);
			}

			if (mesh->HasTextureCoords(0))
			{
				AnimaVertex2f* textCoords = AnimaAllocatorNamespace::AllocateArray<AnimaVertex2f>(*(_engine->GetGenericAllocator()), numeroVertici);
				//AnimaVertex2f* textCoords = AnimaAllocatorNamespace::AllocateArray<AnimaVertex2f>(*(_engine->GetGenericAllocator()), numeroVertici, _engine);
				int offsetTextCoords = 0;
				for (int t = 0; t < numeroVertici; t++)
				{
					textCoords[offsetTextCoords].u = mesh->mTextureCoords[0][t].x;
					textCoords[offsetTextCoords].v = mesh->mTextureCoords[0][t].y;
					//textCoords[offsetTextCoords][0] = mesh->mTextureCoords[0][t].x;
					//textCoords[offsetTextCoords][1] = mesh->mTextureCoords[0][t].y;

					offsetTextCoords++;
				}

				currentMesh->SetTextureCoords(textCoords, offsetTextCoords);
				AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetGenericAllocator()), textCoords);
			}

			currentMesh->SetVertices(vertici, offsetVertici);
			currentMesh->SetFaces(facce, offsetFacce);
			
			AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetGenericAllocator()), vertici);
			AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetGenericAllocator()), facce);
		}
	
		currentModel->SetMeshes(meshes, numeroMesh);
		
		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetModelsAllocator()), meshes);
	}
	
	int numeroFigli = sceneNode->mNumChildren;
	if(numeroFigli > 0)
	{
		AnimaModel* figli = AnimaAllocatorNamespace::AllocateArray<AnimaModel>(*(_engine->GetGenericAllocator()), numeroFigli, _engine);
	
		for (int n = 0; n < numeroFigli; n++)
			RecursiveLoadMesh(&figli[n], scene, sceneNode->mChildren[n]);
	
		currentModel->SetChildren(figli, numeroFigli);
		
		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetGenericAllocator()), figli);
	}
}

void AnimaModelsManager::AddModel(const AnimaModel& model, const char* name)
{
	AnimaString str(name, _engine);
	AddModel(model, str);
}

void AnimaModelsManager::AddModel(const AnimaModel& model, const AnimaString& name)
{
	ANIMA_ASSERT(_engine != nullptr);
	if(_modelsNumber > 0)
	{
		AnimaModel* tmpOldModels = AnimaAllocatorNamespace::AllocateArray<AnimaModel>(*(_engine->GetModelsAllocator()), _modelsNumber, _engine);
	
		for (int i = 0; i < _modelsNumber; i++)
			tmpOldModels[i] = _models[i];
	
		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetModelsAllocator()), _models);
	
		_modelsNumber++;
		_models = AnimaAllocatorNamespace::AllocateArray<AnimaModel>(*(_engine->GetModelsAllocator()), _modelsNumber, _engine);
	
		for (int i = 0; i < _modelsNumber - 1; i++)
			_models[i] = tmpOldModels[i];
	
		_models[_modelsNumber - 1] = model;
		
		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetModelsAllocator()), tmpOldModels);
	}
	else
	{
		_modelsNumber++;
		_models = AnimaAllocatorNamespace::AllocateArray<AnimaModel>(*(_engine->GetModelsAllocator()), _modelsNumber, _engine);
		_models[_modelsNumber - 1] = model;
	}
}

ASizeT AnimaModelsManager::GetModelsNumber()
{
	return _modelsNumber;
}

AnimaModel AnimaModelsManager::GetModel(ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < _modelsNumber);
	return _models[index];
}

AnimaModel* AnimaModelsManager::GetPModel(ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < _modelsNumber);
	return &_models[index];
}

AnimaModel* AnimaModelsManager::GetModels()
{
	return _models;
}

void AnimaModelsManager::ClearModels()
{
	if(_models != nullptr && _modelsNumber > 0)
	{
		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetModelsAllocator()), _models);
		_models = nullptr;
		_modelsNumber = 0;
	}
}

ASizeT AnimaModelsManager::GetNextModelID()
{
	return _nextModelID++;
}

AnimaModel AnimaModelsManager::GetModelFromName(const AnimaString& name)
{
	if (_modelsMap.find(name) == _modelsMap.end())
		return nullptr;
	return GetModel((ASizeT)_modelsMap[name]);
}

AnimaModel AnimaModelsManager::GetModelFromName(const char* name)
{
	AnimaString str(name, _engine);
	return GetModelFromName(str);
}

AnimaModel* AnimaModelsManager::GetPModelFromName(const AnimaString& name)
{
	if (_modelsMap.find(name) == _modelsMap.end())
		return nullptr;
	return GetPModel((ASizeT)_modelsMap[name]);
}

AnimaModel* AnimaModelsManager::GetPModelFromName(const char* name)
{
	AnimaString str(name, _engine);
	return GetPModelFromName(str);
}

void AnimaModelsManager::LoadMaterial(AnimaMesh* mesh, const aiMaterial* mtl)
{
	AnimaMaterial* material = mesh->GetMaterial();

	if (material == nullptr)
	{
		int i = 1;

		AnimaString prefix = mesh->GetAnimaMeshName();
		AnimaString suffix(_engine);
		suffix.Format(".material.%d", i);

		while (material == nullptr)
		{
			material = _engine->GetMaterialsManager()->CreateMaterial(prefix + suffix);

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
		material->AddColor("diffuseColor", diffuse.r, diffuse.g, diffuse.b, diffuse.a);
	else
		material->AddColor("diffuseColor", 0.8f, 0.8f, 0.8f, 1.0f);

	if (aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular) == AI_SUCCESS)
		material->AddColor("specularColor", specular.r, specular.g, specular.b, specular.a);
	else
		material->AddColor("specularColor", 0.0f, 0.0f, 0.0f, 1.0f);

	if (aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient) == AI_SUCCESS)
		material->AddColor("ambientColor", ambient.r, ambient.g, ambient.b, ambient.a);
	else
		material->AddColor("ambientColor", 0.2f, 0.2f, 0.2f, 1.0f);

	if (aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission) == AI_SUCCESS)
		material->AddColor("emissionColor", emission.r, emission.g, emission.b, emission.a);
	else
		material->AddColor("emissionColor", 0.0f, 0.0f, 0.0f, 1.0f);
	
	max = 1;
	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
	if (ret1 == AI_SUCCESS) 
	{
		max = 1;
		ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);

		if (ret2 == AI_SUCCESS)
			material->AddFloat("shiness", shininess * strength);
		else
			material->AddFloat("shiness", shininess);
	}
	else 
	{
		material->AddFloat("shiness", 0.0f);
		material->SetColor("specularColor", 0.0, 0.0, 0.0, 1.0f);
	}

	max = 1;
	if ((aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max) == AI_SUCCESS) && two_sided)
		material->AddBoolean("twoSided", true);
	else
		material->AddBoolean("twoSided", false);
}

END_ANIMA_ENGINE_NAMESPACE
