//
//  AnimaEngineModelsManager.cpp
//  Anima
//
//  Created by Marco Zille on 20/11/14.
//
//

#include "AnimaModelsManager.h"

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

AnimaModelsManager::AnimaModelsManager(AnimaEngine* engine)
{
	_engine = engine;
	
	_models = nullptr;
	_modelsNumber = 0;
}

AnimaModelsManager::~AnimaModelsManager()
{
	ClearModels();
}

bool AnimaModelsManager::LoadModel(const char* modelPath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelPath, aiProcessPreset_TargetRealtime_Quality);
	
	if(scene == nullptr)
		return false;
	
	AnimaModel* newModel = AnimaAllocatorNamespace::AllocateNew<AnimaModel>(*(_engine->GetModelsAllocator()), _engine);
	
	RecursiveLoadMesh(newModel, scene, scene->mRootNode);
	
	AddModel(*newModel);
	
	AnimaAllocatorNamespace::DeallocateObject(*(_engine->GetModelsAllocator()), newModel);
		
	importer.FreeScene();

	return true;
}

bool AnimaModelsManager::LoadModel(AnimaString& modelPath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelPath.GetBuffer(), aiProcessPreset_TargetRealtime_Quality);
	
	if(scene == nullptr)
		return false;
	
	AnimaModel* newModel = AnimaAllocatorNamespace::AllocateNew<AnimaModel>(*(_engine->GetModelsAllocator()), _engine);
	
	RecursiveLoadMesh(newModel, scene, scene->mRootNode);
	
	AddModel(*newModel);
	
	AnimaAllocatorNamespace::DeallocateObject(*(_engine->GetModelsAllocator()), newModel);
	
	importer.FreeScene();
	
	return true;
}

void AnimaModelsManager::RecursiveLoadMesh(AnimaModel* currentModel, const aiScene *scene, const aiNode* sceneNode)
{
	int numeroMesh = sceneNode->mNumMeshes;
	
	if(numeroMesh > 0)
	{
		AnimaMesh* meshes = AnimaAllocatorNamespace::AllocateArray<AnimaMesh>(*(_engine->GetModelsAllocator()), numeroMesh, _engine);
	
		for (int n = 0; n < numeroMesh; n++)
		{
			const aiMesh* mesh = scene->mMeshes[sceneNode->mMeshes[n]];
		
			AnimaMesh* currentMesh = &meshes[n];
		
			int numeroFacce = mesh->mNumFaces;
			int numeroVertici = mesh->mNumVertices;
		
			AnimaVertex4f* vertici = AnimaAllocatorNamespace::AllocateArray<AnimaVertex4f>(*(_engine->GetGenericAllocator()), numeroVertici, _engine);
			AnimaFace* facce = AnimaAllocatorNamespace::AllocateArray<AnimaFace>(*(_engine->GetGenericAllocator()), numeroFacce, _engine);
		
			int offsetFacce = 0;
			int offsetVertici = 0;
		
			for (int t = 0; t < numeroVertici; t++)
			{
				const aiVector3D* vert = &mesh->mVertices[t];
			
				vertici[offsetVertici][0] = vert->x;
				vertici[offsetVertici][1] = vert->y;
				vertici[offsetVertici][2] = vert->z;
				vertici[offsetVertici][3] = 1.0;
			
				offsetVertici++;
			}

			for (int t = 0; t < numeroFacce; t++)
			{
				const aiFace* face = &mesh->mFaces[t];
				
				int numeroIndiciFaccia = face->mNumIndices;
				ASizeT* indiciFaccia = AnimaAllocatorNamespace::AllocateArray<ASizeT>(*(_engine->GetGenericAllocator()), numeroIndiciFaccia);
				
				int offsetIndiciFaccia = 0;
				
				for(unsigned int i = 0; i < numeroIndiciFaccia; i++)
					indiciFaccia[offsetIndiciFaccia++] = face->mIndices[i];
				
				facce[offsetFacce++].SetIndexes(indiciFaccia, offsetIndiciFaccia);
				AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetGenericAllocator()), indiciFaccia);
			}

			if (mesh->HasNormals())
			{
				AnimaVertex4f* normali = AnimaAllocatorNamespace::AllocateArray<AnimaVertex4f>(*(_engine->GetGenericAllocator()), numeroVertici, _engine);
				int offsetNormali = 0;
				for (int t = 0; t < numeroVertici; t++)
				{
					const aiVector3D* norm = &mesh->mNormals[t];

					normali[offsetNormali][0] = norm->x;
					normali[offsetNormali][1] = norm->y;
					normali[offsetNormali][2] = norm->z;
					normali[offsetNormali][3] = 1.0;

					offsetNormali++;
				}

				currentMesh->SetNormals(normali, offsetNormali);
				AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetGenericAllocator()), normali);
			}

			//if (mesh->HasTextureCoords())
			//{
			//	AnimaVertex2f* textCoords = AnimaAllocatorNamespace::AllocateArray<AnimaVertex2f>(*(_engine->GetGenericAllocator()), numeroVertici, _engine);
			//	int offsetTextCoords = 0;
			//	for (int t = 0; t < numeroVertici; t++)
			//	{
			//		const aiVector3D* coord = &mesh->mNormals[t];

			//		textCoords[offsetTextCoords][0] = coord->x;
			//		textCoords[offsetTextCoords][1] = coord->y;

			//		offsetTextCoords++;
			//	}

			//	currentMesh->SetTextureCoords(textCoords, offsetTextCoords);
			//	AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetGenericAllocator()), textCoords);
			//}

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

void AnimaModelsManager::AddModel(const AnimaModel& model)
{
	ANIMA_ASSERT(_engine != nullptr);
	if(_modelsNumber > 0)
	{
		AnimaModel* tmpOldModels = AnimaAllocatorNamespace::AllocateArray<AnimaModel>(*(_engine->GetModelsAllocator()), _modelsNumber, _engine);
	
		//memcpy(tmpOldVertices, _vertices, sizeof(AnimaVertex4f) * _verticesNumber);
		for (int i = 0; i < _modelsNumber; i++)
			tmpOldModels[i] = _models[i];
	
		//_allocator->Deallocate(_vertices);
		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetModelsAllocator()), _models);
	
		_modelsNumber++;
		_models = AnimaAllocatorNamespace::AllocateArray<AnimaModel>(*(_engine->GetModelsAllocator()), _modelsNumber, _engine);
	
		//memcpy(_vertices, tmpOldVertices, sizeof(AnimaVertex4f) * (_verticesNumber - 1));
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

END_ANIMA_ENGINE_CORE_NAMESPACE