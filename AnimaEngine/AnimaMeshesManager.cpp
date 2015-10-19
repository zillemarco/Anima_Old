//
//  AnimaMeshesManager.cpp
//  Anima
//
//  Created by Marco Zille on 20/11/14.
//
//

#include "AnimaMeshesManager.h"
#include "AnimaXmlTranslators.h"

#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/timer.hpp>

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaMeshesManager::AnimaMeshesManager(AnimaScene* scene, AnimaMaterialsManager* materialsManager)
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

bool AnimaMeshesManager::LoadMeshesFromModel(const aiScene* scene, const AnimaString& modelName/*, AnimaArray<AnimaString>* materialNamesMap*/)
{
	ClearLastMeshesIndexMap();
	ClearLastMeshesBonesData();

	for (AUint i = 0; i < scene->mNumMeshes; i++)
	{
		const aiMesh* mesh = scene->mMeshes[i];
		
		AnimaString meshName;
		AnimaMesh* newMesh = nullptr;
		int nameOffset = 0;
		while (newMesh == nullptr)
		{
			if (nameOffset > 0)
				meshName = FormatString("%s.mesh%d", modelName.c_str(), i + nameOffset);
			else
				meshName = FormatString("%s.mesh%d", modelName.c_str(), i);

			newMesh = CreateMesh(meshName);
			nameOffset++;
		}

		int numeroFacce = mesh->mNumFaces;
		int numeroVertici = mesh->mNumVertices;

		AnimaArray<AnimaVertex3f> vertici;
		AnimaArray<AnimaFace> facce;

		for (int t = 0; t < numeroVertici; t++)
		{
			const aiVector3D* vert = &mesh->mVertices[t];
			vertici.push_back(AnimaVertex3f(vert->x, vert->y, vert->z));
		}
		newMesh->SetVertices(vertici);

		for (int t = 0; t < numeroFacce; t++)
		{
			const aiFace* face = &mesh->mFaces[t];

			int numeroIndiciFaccia = face->mNumIndices;
			if (numeroIndiciFaccia == 3)
				facce.push_back(AnimaFace(face->mIndices[0], face->mIndices[1], face->mIndices[2]));
		}
		newMesh->SetFaces(facce);

		if (mesh->HasNormals())
		{
			AnimaArray<AnimaVertex3f> normali;
			for (int t = 0; t < numeroVertici; t++)
			{
				const aiVector3D* norm = &mesh->mNormals[t];

				normali.push_back(AnimaVertex3f(norm->x, norm->y, norm->z));
			}
			newMesh->SetNormals(normali);
		}

		if (mesh->HasTangentsAndBitangents())
		{
			AnimaArray<AnimaVertex3f> tangents;
			AnimaArray<AnimaVertex3f> bitangents;

			for (int t = 0; t < numeroVertici; t++)
			{
				const aiVector3D* tang = &mesh->mTangents[t];
				const aiVector3D* bita = &mesh->mBitangents[t];

				tangents.push_back(AnimaVertex3f(tang->x, tang->y, tang->z));
				bitangents.push_back(AnimaVertex3f(bita->x, bita->y, bita->z));
			}

			newMesh->SetTangents(tangents);
			newMesh->SetBitangents(bitangents);
		}

		if (mesh->HasTextureCoords(0))
		{
			AnimaArray<AnimaVertex2f> textCoords;
			for (int t = 0; t < numeroVertici; t++)
				textCoords.push_back(AnimaVertex2f(mesh->mTextureCoords[0][t].x, mesh->mTextureCoords[0][t].y));

			newMesh->SetTextureCoords(textCoords);
		}

		if (mesh->mNumBones > 0)
		{
			AnimaArray<AnimaVertex4f> meshBoneWeights;
			AnimaArray<AnimaVertex4f> meshBoneIDs;

			meshBoneWeights.reserve(newMesh->GetVerticesCount());
			meshBoneIDs.reserve(newMesh->GetVerticesCount());

			for (AUint nb = 0; nb < mesh->mNumBones; nb++)
			{
				aiBone* bone = mesh->mBones[nb];
				AnimaString boneName(bone->mName.data);
				AInt boneIndex = _lastMeshesBonesInfo.Contains(boneName);

				if (boneIndex < 0)
				{
					AnimaMatrix boneOffset;
					boneOffset.m[0] = bone->mOffsetMatrix.a1;	boneOffset.m[1] = bone->mOffsetMatrix.a2;	boneOffset.m[2] = bone->mOffsetMatrix.a3;	boneOffset.m[3] = bone->mOffsetMatrix.a4;
					boneOffset.m[4] = bone->mOffsetMatrix.b1;	boneOffset.m[5] = bone->mOffsetMatrix.b2;	boneOffset.m[6] = bone->mOffsetMatrix.b3;	boneOffset.m[7] = bone->mOffsetMatrix.b4;
					boneOffset.m[8] = bone->mOffsetMatrix.c1;	boneOffset.m[9] = bone->mOffsetMatrix.c2;	boneOffset.m[10] = bone->mOffsetMatrix.c3;	boneOffset.m[11] = bone->mOffsetMatrix.c4;
					boneOffset.m[12] = bone->mOffsetMatrix.d1;	boneOffset.m[13] = bone->mOffsetMatrix.d2;	boneOffset.m[14] = bone->mOffsetMatrix.d3;	boneOffset.m[15] = bone->mOffsetMatrix.d4;

					AnimaMeshBoneInfo* info = AnimaAllocatorNamespace::AllocateNew<AnimaMeshBoneInfo>(*(_scene->GetMeshesAllocator()), boneName, _scene->GetMeshesAllocator());
					info->SetBoneOffset(boneOffset.Transposed());

					boneIndex = _lastMeshesBonesInfo.Add(boneName, info);
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

			newMesh->SetBoneWeights(meshBoneWeights);
			newMesh->SetBoneIDs(meshBoneIDs);
		}

		_meshes.Add(meshName, newMesh);
		_lastMeshesIndexMap.push_back(meshName);
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
	_lastMeshesIndexMap.clear();
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

AInt AnimaMeshesManager::GetMeshesCount() const
{
	return _meshes.GetSize();
}

AnimaArray<AnimaString>* AnimaMeshesManager::GetLastMeshesIndexMap()
{
	return &_lastMeshesIndexMap;
}

AnimaMappedArray<AnimaMeshBoneInfo*>* AnimaMeshesManager::GetLastMeshesBonesInfo()
{
	return &_lastMeshesBonesInfo;
}

AnimaMesh* AnimaMeshesManager::CreateMesh(const AnimaString& name)
{
	AInt index = _meshes.Contains(name);
	if (index >= 0)
		return nullptr;

	AnimaMesh* newMesh = AnimaAllocatorNamespace::AllocateNew<AnimaMesh>(*(_scene->GetMeshesAllocator()), name, _scene->GetDataGeneratorsManager(), _scene->GetMeshesAllocator());
	_meshes.Add(name, newMesh);
	return newMesh;
}

void AnimaMeshesManager::SaveMeshToFile(const AnimaString& meshName, const AnimaString& destinationPath, bool createFinalPath)
{
	AnimaMesh* mesh = _meshes[meshName];
	SaveMeshToFile(mesh, destinationPath, createFinalPath);
}

void AnimaMeshesManager::SaveMeshToFile(AnimaMesh* mesh, const AnimaString& destinationPath, bool createFinalPath)
{
	if (mesh == nullptr)
		return;

	namespace fs = boost::filesystem;

	AnimaString saveFileName = destinationPath;

	if (createFinalPath)
	{
		fs::path firstPart(destinationPath);
		fs::path secondPart(mesh->GetName() + ".amesh");
		fs::path completePath = firstPart / secondPart;

		saveFileName = completePath.string();
	}

	mesh->SaveObject(saveFileName);
}

AnimaMesh* AnimaMeshesManager::LoadMeshFromFile(const AnimaString& meshFilePath)
{
	std::ifstream fileStream(meshFilePath);
	AnimaString xml((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
	fileStream.close();

	return LoadMeshFromXml(xml);
}

AnimaMesh* AnimaMeshesManager::LoadMeshFromXml(const AnimaString& meshXmlDefinition)
{
	AnimaMesh* mesh = nullptr;

	using boost::property_tree::ptree;
	ptree pt;

	std::stringstream ss(meshXmlDefinition);
	boost::property_tree::read_xml(ss, pt);

#if !defined SAVE_SCENE
	AnimaString name = pt.get<AnimaString>("AnimaMesh.Name");
#else
	AnimaString name = pt.get<AnimaString>("AnimaMesh.<xmlattr>.name");
#endif

	mesh = CreateMesh(name);

	if (mesh)
	{
#if !defined SAVE_SCENE
		mesh->ReadObject(pt, _scene, false);
#else		
		mesh->SetVertices(pt.get<AnimaArray<AnimaVertex3f> >("AnimaMesh.Vertices"));
		mesh->SetNormals(pt.get<AnimaArray<AnimaVertex3f> >("AnimaMesh.Normals"));
		mesh->SetTextureCoords(pt.get<AnimaArray<AnimaVertex2f> >("AnimaMesh.TextureCoords"));
		mesh->SetTangents(pt.get<AnimaArray<AnimaVertex3f> >("AnimaMesh.Tangents"));
		mesh->SetBitangents(pt.get<AnimaArray<AnimaVertex3f> >("AnimaMesh.Bitangents"));
		mesh->SetBoneWeights(pt.get<AnimaArray<AnimaVertex4f> >("AnimaMesh.BoneWeights"));
		mesh->SetBoneIDs(pt.get<AnimaArray<AnimaVertex4f> >("AnimaMesh.BoneIDs"));

		AnimaArray<AUint> indexes = pt.get<AnimaArray<AUint> >("AnimaMesh.Indexes");
		AnimaArray<AnimaVertex3f> facesNormals = pt.get<AnimaArray<AnimaVertex3f> >("AnimaMesh.FacesNormals");

		AInt indexesCount = indexes.size();
		AInt facesNormalsCount = facesNormals.size();

		bool hasFacesNormal = (facesNormalsCount == indexesCount % 3);

		for (AInt i = 0; i < indexesCount; i += 3)
		{
			AnimaFace face;
			face.SetIndexes(indexes[i], indexes[i + 1], indexes[i + 2]);

			if (hasFacesNormal)
				face.SetNormal(facesNormals[i % 3]);

			mesh->AddFace(face);
		}

		mesh->SetPosition(pt.get<AnimaVertex3f>("AnimaMesh.SpaceData.Position"));
		mesh->GetTransformation()->SetTranslation(pt.get<AnimaVertex3f>("AnimaMesh.SpaceData.Translation"));
		mesh->GetTransformation()->SetRotation(pt.get<AnimaVertex3f>("AnimaMesh.SpaceData.Rotation"));
		mesh->GetTransformation()->SetScale(pt.get<AnimaVertex3f>("AnimaMesh.SpaceData.Scale"));

		AnimaString materialName = pt.get<AnimaString>("AnimaMesh.<xmlattr>.material");
		if (!materialName.empty())
			mesh->SetMaterial(_materialsManager->GetMaterialFromName(materialName));
#endif
	}

	return mesh;
}

bool AnimaMeshesManager::LoadMeshes(const AnimaString& meshesPath)
{
	namespace fs = boost::filesystem;
	fs::path directory(meshesPath);

	if (fs::exists(directory) && fs::is_directory(directory))
	{
		fs::directory_iterator endIterator;
		for (fs::directory_iterator directoryIterator(directory); directoryIterator != endIterator; directoryIterator++)
		{
			if (directoryIterator->path().extension().string() == ".amesh")
			{
				if(LoadMeshFromFile(directoryIterator->path().string()) == nullptr)
					return false;
			}
		}
	}
	return true;
}

void AnimaMeshesManager::SaveMeshes(const AnimaString& destinationPath)
{
	AInt count = _meshes.GetSize();
	for(AInt i = 0; i < count; i++)
	{
		SaveMeshToFile(_meshes[i], destinationPath, true);
	}
}

bool AnimaMeshesManager::FinalizeObjectsAfterRead()
{
	AInt count = _meshes.GetSize();
	for (AInt i = 0; i < count; i++)
		_meshes[i]->FinalizeAfterRead(_scene);

	return true;
}

END_ANIMA_ENGINE_NAMESPACE
