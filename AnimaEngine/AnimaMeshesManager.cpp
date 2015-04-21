//
//  AnimaMeshesManager.cpp
//  Anima
//
//  Created by Marco Zille on 20/11/14.
//
//

#include "AnimaMeshesManager.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaMeshesManager::AnimaMeshesManager(AnimaScene* scene)
	: _meshes(scene->GetMeshesAllocator())
	, _lastMeshesIndexMap(scene->GetGenericAllocator())
{
	_scene = scene;
}

AnimaMeshesManager::~AnimaMeshesManager()
{
	ClearMeshes();
	ClearLastMeshesIndexMap();
}

bool AnimaMeshesManager::LoadMeshesFromModel(const aiScene* scene, const AnimaString& modelName)
{
	ClearLastMeshesIndexMap();

	for (AUint i = 0; i < scene->mNumMeshes; i++)
	{
		AnimaMesh* newMesh = AnimaAllocatorNamespace::AllocateNew<AnimaMesh>(*(_scene->GetMeshesAllocator()), _scene->GetMeshesAllocator());
		const aiMesh* mesh = scene->mMeshes[i];

		AnimaString* meshName = AnimaAllocatorNamespace::AllocateNew<AnimaString>(*(_scene->GetStringAllocator()), _scene->GetStringAllocator());
		meshName->Format("%s.%d", modelName.GetConstBuffer(), i);
		newMesh->SetName(*meshName);

		int numeroFacce = mesh->mNumFaces;
		int numeroVertici = mesh->mNumVertices;

		AnimaVertex3f* vertici = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*(_scene->GetGenericAllocator()), numeroVertici);
		AnimaFace* facce = AnimaAllocatorNamespace::AllocateArray<AnimaFace>(*(_scene->GetGenericAllocator()), numeroFacce);

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
		newMesh->SetVertices(vertici, offsetVertici);
		AnimaAllocatorNamespace::DeallocateArray(*(_scene->GetGenericAllocator()), vertici);

		for (int t = 0; t < numeroFacce; t++)
		{
			const aiFace* face = &mesh->mFaces[t];

			int numeroIndiciFaccia = face->mNumIndices;
			ANIMA_ASSERT(numeroIndiciFaccia == 3);

			AUint* indiciFaccia = AnimaAllocatorNamespace::AllocateArray<AUint>(*(_scene->GetGenericAllocator()), numeroIndiciFaccia);

			int offsetIndiciFaccia = 0;

			for (int i = 0; i < numeroIndiciFaccia; i++)
				indiciFaccia[offsetIndiciFaccia++] = face->mIndices[i];

			facce[offsetFacce++].SetIndexes(indiciFaccia);
			AnimaAllocatorNamespace::DeallocateArray(*(_scene->GetGenericAllocator()), indiciFaccia);
		}
		newMesh->SetFaces(facce, offsetFacce);
		AnimaAllocatorNamespace::DeallocateArray(*(_scene->GetGenericAllocator()), facce);

		if (mesh->HasNormals())
		{
			AnimaVertex3f* normali = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*(_scene->GetGenericAllocator()), numeroVertici);
			int offsetNormali = 0;
			for (int t = 0; t < numeroVertici; t++)
			{
				const aiVector3D* norm = &mesh->mNormals[t];

				normali[offsetNormali].x = norm->x;
				normali[offsetNormali].y = norm->y;
				normali[offsetNormali].z = norm->z;

				offsetNormali++;
			}

			newMesh->SetNormals(normali, offsetNormali);
			AnimaAllocatorNamespace::DeallocateArray(*(_scene->GetGenericAllocator()), normali);
		}

		if (mesh->HasTangentsAndBitangents())
		{
			AnimaVertex3f* tangents = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*(_scene->GetGenericAllocator()), numeroVertici);
			AnimaVertex3f* bitangents = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*(_scene->GetGenericAllocator()), numeroVertici);

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

			newMesh->SetTangents(tangents, offsetTangents);
			AnimaAllocatorNamespace::DeallocateArray(*(_scene->GetGenericAllocator()), tangents);

			newMesh->SetBitangents(bitangents, offsetTangents);
			AnimaAllocatorNamespace::DeallocateArray(*(_scene->GetGenericAllocator()), bitangents);
		}

		if (mesh->HasTextureCoords(0))
		{
			AnimaVertex2f* textCoords = AnimaAllocatorNamespace::AllocateArray<AnimaVertex2f>(*(_scene->GetGenericAllocator()), numeroVertici);
			int offsetTextCoords = 0;
			for (int t = 0; t < numeroVertici; t++)
			{
				textCoords[offsetTextCoords].u = mesh->mTextureCoords[0][t].x;
				textCoords[offsetTextCoords].v = mesh->mTextureCoords[0][t].y;

				offsetTextCoords++;
			}

			newMesh->SetTextureCoords(textCoords, offsetTextCoords);
			AnimaAllocatorNamespace::DeallocateArray(*(_scene->GetGenericAllocator()), textCoords);
		}

		_meshes.Add(*meshName, newMesh);
		_lastMeshesIndexMap.Add(meshName);
	}

	return true;
}

void AnimaMeshesManager::ClearMeshes()
{
	AInt meshesCount = _meshes.GetSize();
	for (AInt i = 0; i < meshesCount; i++)
	{
		AnimaMesh* mesh = _meshes[i];
		AnimaAllocatorNamespace::DeallocateObject(*(_scene->GetMeshesAllocator()), mesh);
		mesh = nullptr;
	}
	_meshes.RemoveAll();
}

void AnimaMeshesManager::ClearLastMeshesIndexMap()
{
	AInt meshesIndexCount = _lastMeshesIndexMap.GetSize();
	for (AInt i = 0; i < meshesIndexCount; i++)
	{
		AnimaString* meshIndex = _lastMeshesIndexMap[i];
		AnimaAllocatorNamespace::DeallocateObject(*(_scene->GetStringAllocator()), meshIndex);
		meshIndex = nullptr;
	}
	_lastMeshesIndexMap.RemoveAll();
}

AnimaMesh* AnimaMeshesManager::GetMesh(const AnimaString& name)
{
	return _meshes[name];
}

AnimaArray<AnimaString*>* AnimaMeshesManager::GetLastMeshesIndexMap()
{
	return &_lastMeshesIndexMap;
}

END_ANIMA_ENGINE_NAMESPACE