//
//  AnimaMeshesManager.cpp
//  Anima
//
//  Created by Marco Zille on 20/11/14.
//
//

#include "AnimaMeshesManager.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaMeshesManager::AnimaMeshesManager(AnimaScene* scene, AnimaMaterialsManager* materialsManager)
	: _meshes(scene->GetMeshesAllocator())
	, _lastMeshesIndexMap(scene->GetGenericAllocator())
	, _lastMeshesBonesInfo(scene->GetGenericAllocator())
{
	ANIMA_ASSERT(scene != nullptr);
	ANIMA_ASSERT(materialsManager != nullptr);

	_scene = scene;
	_materialsManager = materialsManager;
}

AnimaMeshesManager::~AnimaMeshesManager()
{
	ClearMeshes();
	ClearLastMeshesIndexMap();
	ClearLastMeshesBonesData();
}

bool AnimaMeshesManager::LoadMeshesFromModel(const aiScene* scene, const AnimaString& modelName, AnimaArray<AnimaString*>* materialNamesMap)
{
	ClearLastMeshesIndexMap();
	ClearLastMeshesBonesData();

	for (AUint i = 0; i < scene->mNumMeshes; i++)
	{
		const aiMesh* mesh = scene->mMeshes[i];
		
		AnimaString* meshName = AnimaAllocatorNamespace::AllocateNew<AnimaString>(*(_scene->GetStringAllocator()), _scene->GetStringAllocator());
		AnimaMesh* newMesh = nullptr;
		int nameOffset = 0;
		while (newMesh == nullptr)
		{
			if (nameOffset > 0)
				meshName->Format("%s.mesh%d", modelName.GetConstBuffer(), i + nameOffset);
			else
				meshName->Format("%s.mesh%d", modelName.GetConstBuffer(), i);

			newMesh = CreateEmptyMesh(*meshName);
			nameOffset++;
		}

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

		if (mesh->mNumBones > 0)
		{
			AnimaArray<AnimaVertex4f> meshBoneWeights(_scene->GetGenericAllocator());
			AnimaArray<AnimaVertex4f> meshBoneIDs(_scene->GetGenericAllocator());

			meshBoneWeights.SetSize(newMesh->GetVerticesNumber());
			meshBoneIDs.SetSize(newMesh->GetVerticesNumber());

			for (AUint nb = 0; nb < mesh->mNumBones; nb++)
			{
				aiBone* bone = mesh->mBones[nb];
				AnimaString boneName(bone->mName.data, _scene->GetStringAllocator());
				AUint boneIndex = _lastMeshesBonesInfo.Contains(boneName);

				if (boneIndex < 0)
				{
					AnimaMatrix boneOffset;
					boneOffset.m[0] = bone->mOffsetMatrix.a1;	boneOffset.m[1] = bone->mOffsetMatrix.a2;	boneOffset.m[2] = bone->mOffsetMatrix.a3;	boneOffset.m[3] = bone->mOffsetMatrix.a4;
					boneOffset.m[4] = bone->mOffsetMatrix.b1;	boneOffset.m[5] = bone->mOffsetMatrix.b2;	boneOffset.m[6] = bone->mOffsetMatrix.b3;	boneOffset.m[7] = bone->mOffsetMatrix.b4;
					boneOffset.m[8] = bone->mOffsetMatrix.c1;	boneOffset.m[9] = bone->mOffsetMatrix.c2;	boneOffset.m[10] = bone->mOffsetMatrix.c3;	boneOffset.m[11] = bone->mOffsetMatrix.c4;
					boneOffset.m[12] = bone->mOffsetMatrix.d1;	boneOffset.m[13] = bone->mOffsetMatrix.d2;	boneOffset.m[14] = bone->mOffsetMatrix.d3;	boneOffset.m[15] = bone->mOffsetMatrix.d4;

					AnimaMeshBoneInfo* info = AnimaAllocatorNamespace::AllocateNew<AnimaMeshBoneInfo>(*(_scene->GetMeshesAllocator()), boneName, _scene->GetMeshesAllocator());
					info->SetBoneOffset(boneOffset);

					boneIndex = (AUint)_lastMeshesBonesInfo.Add(boneName, info);
				}

				AInt numWeights = bone->mNumWeights;
				for (AInt nw = 0; nw < numWeights; nw++)
				{
					aiVertexWeight* weight = &bone->mWeights[nw];
					AInt vertexID = weight->mVertexId;

					ANIMA_ASSERT(meshBoneIDs[vertexID].SetNextValue((AFloat)boneIndex));
					ANIMA_ASSERT(meshBoneWeights[vertexID].SetNextValue(weight->mWeight));
				}
			}

			newMesh->SetBoneWeights(&meshBoneWeights);
			newMesh->SetBoneIDs(&meshBoneIDs);
		}

		AInt materialIndex = (AInt)mesh->mMaterialIndex;
		AnimaString* materialName = materialNamesMap->GetAt(materialIndex);
		AnimaMaterial* material = _materialsManager->GetMaterialFromName(*materialName);

		newMesh->SetMaterial(material);

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

void AnimaMeshesManager::ClearLastMeshesBonesData()
{
	AInt count = _lastMeshesBonesInfo.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaMeshBoneInfo* meshBoneInfo = _lastMeshesBonesInfo[i];
		AnimaAllocatorNamespace::DeallocateObject(*(_scene->GetMeshesAllocator()), meshBoneInfo);
		meshBoneInfo = nullptr;
	}

	_lastMeshesBonesInfo.RemoveAll();
}

AnimaMesh* AnimaMeshesManager::GetMesh(AInt index)
{
	return _meshes[index];
}

AnimaMesh* AnimaMeshesManager::GetMeshFromName(const AnimaString& name)
{
	return _meshes[name];
}

AnimaMesh* AnimaMeshesManager::GetMeshFromName(const char* name)
{
	AnimaString str(name, _scene->GetStringAllocator());
	return GetMeshFromName(str);
}

AInt AnimaMeshesManager::GetMeshesCount() const
{
	return _meshes.GetSize();
}

AnimaArray<AnimaString*>* AnimaMeshesManager::GetLastMeshesIndexMap()
{
	return &_lastMeshesIndexMap;
}

AnimaMappedArray<AnimaMeshBoneInfo*>* AnimaMeshesManager::GetLastMeshesBonesInfo()
{
	return &_lastMeshesBonesInfo;
}

AnimaMesh* AnimaMeshesManager::CreateEmptyMesh(const AnimaString& name)
{
	AInt index = _meshes.Contains(name);
	if (index >= 0)
		return nullptr;

	AnimaMesh* newMesh = AnimaAllocatorNamespace::AllocateNew<AnimaMesh>(*(_scene->GetMeshesAllocator()), name, _scene->GetDataGeneratorsManager(), _scene->GetMeshesAllocator());
	_meshes.Add(name, newMesh);
	return newMesh;
}

AnimaMesh* AnimaMeshesManager::CreateEmptyMesh(const char* name)
{
	AnimaString str(name, _scene->GetStringAllocator());
	return CreateEmptyMesh(str);
}

END_ANIMA_ENGINE_NAMESPACE
