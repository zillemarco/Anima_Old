//
//  AnimaEngineModelsManager.cpp
//  Anima
//
//  Created by Marco Zille on 20/11/14.
//
//

#include "AnimaModelsManager.h"

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

AnimaModelsManager::AnimaModelsManager(AnimaAllocator* allocator)
{
	_allocator = allocator;
	
	_models = nullptr;
	_modelsNumber = 0;
}

AnimaModelsManager::~AnimaModelsManager()
{
	if(_models != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _models);
		_models = nullptr;
		_modelsNumber = 0;
	}
}

bool AnimaModelsManager::LoadModel(const char* modelPath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelPath, aiProcessPreset_TargetRealtime_Quality);
	
	if(scene == nullptr)
		return false;
	
	AnimaModel* newModel = AnimaAllocatorNamespace::AllocateNew<AnimaModel>(*_allocator, _allocator);
	
	RecursiveLoadMesh(newModel, scene, scene->mRootNode);
	
	_modelsNumber++;
	_models = AnimaAllocatorNamespace::AllocateArray<AnimaModel>(*_allocator, _modelsNumber, _allocator);

	_models[0] = *newModel;
	
	AnimaAllocatorNamespace::DeallocateObject(*_allocator, newModel);
	
	//memcpy(_textureCoords, v, sizeof(AnimaVertex2f) * _textureCoordsNumber);
//	for (int i = 0; i < _indexesNumber; i++)
//		_indexes[i] = indexes[i];
	
	return true;
}

void AnimaModelsManager::RecursiveLoadMesh(AnimaModel* currentModel, const aiScene *scene, const aiNode* sceneNode)
{
	int numeroMesh = sceneNode->mNumMeshes;
	
	if(numeroMesh > 0)
	{
		AnimaMesh* meshes = AnimaAllocatorNamespace::AllocateArray<AnimaMesh>(*_allocator, numeroMesh, _allocator);
	
		for (int n = 0; n < numeroMesh; n++)
		{
			const aiMesh* mesh = scene->mMeshes[sceneNode->mMeshes[n]];
		
			AnimaMesh* currentMesh = &meshes[n];
		
			int numeroFacce = mesh->mNumFaces;
			int numeroVertici = mesh->mNumVertices;
		
			AnimaVertex4f* vertici = AnimaAllocatorNamespace::AllocateArray<AnimaVertex4f>(*_allocator, numeroVertici, _allocator);
			ASizeT* facce = AnimaAllocatorNamespace::AllocateArray<ASizeT>(*_allocator, numeroFacce * 3);
		
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
			
				for(int i = 0; i < face->mNumIndices && i < 3; i++)
					facce[offsetFacce++] = face->mIndices[i];
			}
		
			currentMesh->SetVertices(vertici, offsetVertici);
			currentMesh->SetIndexes(facce, offsetFacce);
			
			AnimaAllocatorNamespace::DeallocateArray(*_allocator, vertici);
			AnimaAllocatorNamespace::DeallocateArray(*_allocator, facce);
		}
	
		currentModel->SetMeshes(meshes, numeroMesh);
		
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, meshes);
	}
	
	int numeroFigli = sceneNode->mNumChildren;
	if(numeroFigli > 0)
	{
		AnimaModel* figli = AnimaAllocatorNamespace::AllocateArray<AnimaModel>(*_allocator, numeroFigli, _allocator);
	
		for (int n = 0; n < numeroFigli; n++)
			RecursiveLoadMesh(&figli[n], scene, sceneNode->mChildren[n]);
	
		currentModel->SetChildren(figli, numeroFigli);
		
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, figli);
	}
}

END_ANIMA_ENGINE_CORE_NAMESPACE