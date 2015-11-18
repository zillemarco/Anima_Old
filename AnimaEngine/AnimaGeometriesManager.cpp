//
//  AnimaGeometriesManager.cpp
//  Anima
//
//  Created by Marco Zille on 20/11/14.
//
//

#include "AnimaGeometriesManager.h"
#include "AnimaXmlTranslators.h"
#include "AnimaScene.h"

#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/timer.hpp>

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaGeometriesManager::AnimaGeometriesManager(AnimaScene* scene, AnimaMaterialsManager* materialsManager)
{
	ANIMA_ASSERT(scene != nullptr);
	ANIMA_ASSERT(materialsManager != nullptr);

	_scene = scene;
	_materialsManager = materialsManager;
}

AnimaGeometriesManager::~AnimaGeometriesManager()
{
	ClearGeometries();
	ClearLastGeometriesIndexMap();
	ClearLastGeometriesBonesData();
}

bool AnimaGeometriesManager::LoadGeometriesFromNode(const aiScene* scene, const AnimaString& nodeName/*, AnimaArray<AnimaString>* materialNamesMap*/)
{
	ClearLastGeometriesIndexMap();
	ClearLastGeometriesBonesData();

	for (AUint i = 0; i < scene->mNumMeshes; i++)
	{
		const aiMesh* geometry = scene->mMeshes[i];
		
		AnimaString geometryName;
		AnimaGeometry* newGeometry = nullptr;
		int nameOffset = 0;
		while (newGeometry == nullptr)
		{
			if (nameOffset > 0)
				geometryName = FormatString("%s.geometry%d", nodeName.c_str(), i + nameOffset);
			else
				geometryName = FormatString("%s.geometry%d", nodeName.c_str(), i);

			newGeometry = CreateGeometry(geometryName);
			nameOffset++;
		}

		int numeroFacce = geometry->mNumFaces;
		int numeroVertici = geometry->mNumVertices;

		AnimaArray<AnimaVertex3f> vertici;
		AnimaArray<AnimaFace> facce;

		for (int t = 0; t < numeroVertici; t++)
		{
			const aiVector3D* vert = &geometry->mVertices[t];
			vertici.push_back(AnimaVertex3f(vert->x, vert->y, vert->z));
		}
		newGeometry->SetVertices(vertici);

		for (int t = 0; t < numeroFacce; t++)
		{
			const aiFace* face = &geometry->mFaces[t];

			int numeroIndiciFaccia = face->mNumIndices;
			if (numeroIndiciFaccia == 3)
				facce.push_back(AnimaFace(face->mIndices[0], face->mIndices[1], face->mIndices[2]));
			else
				AnimaLogger::LogMessageFormat("\tFace with %d vertices", numeroIndiciFaccia);
		}
		newGeometry->SetFaces(facce);

		if (geometry->HasNormals())
		{
			AnimaArray<AnimaVertex3f> normali;
			for (int t = 0; t < numeroVertici; t++)
			{
				const aiVector3D* norm = &geometry->mNormals[t];

				normali.push_back(AnimaVertex3f(norm->x, norm->y, norm->z));
			}
			newGeometry->SetNormals(normali);
		}

		if (geometry->HasTangentsAndBitangents())
		{
			AnimaArray<AnimaVertex3f> tangents;
			AnimaArray<AnimaVertex3f> bitangents;

			for (int t = 0; t < numeroVertici; t++)
			{
				const aiVector3D* tang = &geometry->mTangents[t];
				const aiVector3D* bita = &geometry->mBitangents[t];

				tangents.push_back(AnimaVertex3f(tang->x, tang->y, tang->z));
				bitangents.push_back(AnimaVertex3f(bita->x, bita->y, bita->z));
			}

			newGeometry->SetTangents(tangents);
			newGeometry->SetBitangents(bitangents);
		}

		if (geometry->HasTextureCoords(0))
		{
			AnimaArray<AnimaVertex2f> textCoords;
			for (int t = 0; t < numeroVertici; t++)
				textCoords.push_back(AnimaVertex2f(geometry->mTextureCoords[0][t].x, geometry->mTextureCoords[0][t].y));

			newGeometry->SetTextureCoords(textCoords);
		}

		if (geometry->mNumBones > 0)
		{
			AnimaArray<AnimaVertex4f> geometryBoneWeights;
			AnimaArray<AnimaVertex4f> geometryBoneIDs;

			geometryBoneWeights.reserve(newGeometry->GetVerticesCount());
			geometryBoneIDs.reserve(newGeometry->GetVerticesCount());

			for (AUint nb = 0; nb < geometry->mNumBones; nb++)
			{
				aiBone* bone = geometry->mBones[nb];
				AnimaString boneName(bone->mName.data);
				AInt boneIndex = _lastGeometriesBonesInfo.Contains(boneName);

				if (boneIndex < 0)
				{
					AnimaMatrix boneOffset;
					boneOffset.m[0] = bone->mOffsetMatrix.a1;	boneOffset.m[1] = bone->mOffsetMatrix.a2;	boneOffset.m[2] = bone->mOffsetMatrix.a3;	boneOffset.m[3] = bone->mOffsetMatrix.a4;
					boneOffset.m[4] = bone->mOffsetMatrix.b1;	boneOffset.m[5] = bone->mOffsetMatrix.b2;	boneOffset.m[6] = bone->mOffsetMatrix.b3;	boneOffset.m[7] = bone->mOffsetMatrix.b4;
					boneOffset.m[8] = bone->mOffsetMatrix.c1;	boneOffset.m[9] = bone->mOffsetMatrix.c2;	boneOffset.m[10] = bone->mOffsetMatrix.c3;	boneOffset.m[11] = bone->mOffsetMatrix.c4;
					boneOffset.m[12] = bone->mOffsetMatrix.d1;	boneOffset.m[13] = bone->mOffsetMatrix.d2;	boneOffset.m[14] = bone->mOffsetMatrix.d3;	boneOffset.m[15] = bone->mOffsetMatrix.d4;

					AnimaGeometryBoneInfo* info = AnimaAllocatorNamespace::AllocateNew<AnimaGeometryBoneInfo>(*(_scene->GetGeometriesAllocator()), boneName, _scene->GetGeometriesAllocator());
					info->SetBoneOffset(boneOffset.Transposed());

					boneIndex = _lastGeometriesBonesInfo.Add(boneName, info);
				}

				AInt numWeights = bone->mNumWeights;
				for (AInt nw = 0; nw < numWeights; nw++)
				{
					aiVertexWeight* weight = &bone->mWeights[nw];
					AInt vertexID = weight->mVertexId;

					ANIMA_ASSERT(geometryBoneIDs[vertexID].SetNextValue((AFloat)boneIndex));
					ANIMA_ASSERT(geometryBoneWeights[vertexID].SetNextValue(weight->mWeight));
				}
			}

			newGeometry->SetBoneWeights(geometryBoneWeights);
			newGeometry->SetBoneIDs(geometryBoneIDs);
		}

		_geometries.Add(geometryName, newGeometry);
		_lastGeometriesIndexMap.push_back(geometryName);
	}

	return true;
}

void AnimaGeometriesManager::ClearGeometries()
{
	AInt geometriesCount = _geometries.GetSize();
	for (AInt i = 0; i < geometriesCount; i++)
	{
		AnimaGeometry* geometry = _geometries[i];
		AnimaAllocatorNamespace::DeallocateObject(*(_scene->GetGeometriesAllocator()), geometry);
		geometry = nullptr;
	}
	_geometries.RemoveAll();
}

void AnimaGeometriesManager::ClearLastGeometriesIndexMap()
{
	_lastGeometriesIndexMap.clear();
}

void AnimaGeometriesManager::ClearLastGeometriesBonesData()
{
	AInt count = _lastGeometriesBonesInfo.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaGeometryBoneInfo* geometryBoneInfo = _lastGeometriesBonesInfo[i];
		AnimaAllocatorNamespace::DeallocateObject(*(_scene->GetGeometriesAllocator()), geometryBoneInfo);
		geometryBoneInfo = nullptr;
	}

	_lastGeometriesBonesInfo.RemoveAll();
}

AnimaGeometry* AnimaGeometriesManager::GetGeometry(AInt index)
{
	return _geometries[index];
}

AnimaGeometry* AnimaGeometriesManager::GetGeometryFromName(const AnimaString& name)
{
	return _geometries[name];
}

AInt AnimaGeometriesManager::GetGeometriesCount() const
{
	return _geometries.GetSize();
}

AnimaArray<AnimaString>* AnimaGeometriesManager::GetLastGeometriesIndexMap()
{
	return &_lastGeometriesIndexMap;
}

AnimaMappedArray<AnimaGeometryBoneInfo*>* AnimaGeometriesManager::GetLastGeometriesBonesInfo()
{
	return &_lastGeometriesBonesInfo;
}

AnimaGeometry* AnimaGeometriesManager::CreateGeometry(const AnimaString& name)
{
	AInt index = _geometries.Contains(name);
	if (index >= 0)
		return nullptr;

	AnimaGeometry* newGeometry = AnimaAllocatorNamespace::AllocateNew<AnimaGeometry>(*(_scene->GetGeometriesAllocator()), name, _scene->GetDataGeneratorsManager(), _scene->GetGeometriesAllocator());
	_geometries.Add(name, newGeometry);
	return newGeometry;
}

void AnimaGeometriesManager::SaveGeometryToFile(const AnimaString& geometryName, const AnimaString& destinationPath, bool createFinalPath)
{
	AnimaGeometry* geometry = _geometries[geometryName];
	SaveGeometryToFile(geometry, destinationPath, createFinalPath);
}

void AnimaGeometriesManager::SaveGeometryToFile(AnimaGeometry* geometry, const AnimaString& destinationPath, bool createFinalPath)
{
	if (geometry == nullptr)
		return;

	namespace fs = boost::filesystem;

	AnimaString saveFileName = destinationPath;

	if (createFinalPath)
	{
		fs::path firstPart(destinationPath);
		fs::path secondPart(geometry->GetName() + ".ageometry");
		fs::path completePath = firstPart / secondPart;

		saveFileName = completePath.string();
	}

	geometry->SaveObject(saveFileName);
}

AnimaGeometry* AnimaGeometriesManager::LoadGeometryFromFile(const AnimaString& geometryFilePath)
{
	std::ifstream fileStream(geometryFilePath);
	AnimaString xml((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
	fileStream.close();

	return LoadGeometryFromXml(xml);
}

AnimaGeometry* AnimaGeometriesManager::LoadGeometryFromXml(const AnimaString& geometryXmlDefinition)
{
	AnimaGeometry* geometry = nullptr;

	using boost::property_tree::ptree;
	ptree pt;

	std::stringstream ss(geometryXmlDefinition);
	boost::property_tree::read_xml(ss, pt);

#if !defined SAVE_SCENE
	AnimaString name = pt.get<AnimaString>("AnimaGeometry.Name");
#else
	AnimaString name = pt.get<AnimaString>("AnimaGeometry.<xmlattr>.name");
#endif

	geometry = CreateGeometry(name);

	if (geometry)
	{
#if !defined SAVE_SCENE
		geometry->ReadObject(pt, _scene, false);
#else		
		geometry->SetVertices(pt.get<AnimaArray<AnimaVertex3f> >("AnimaGeometry.Vertices"));
		geometry->SetNormals(pt.get<AnimaArray<AnimaVertex3f> >("AnimaGeometry.Normals"));
		geometry->SetTextureCoords(pt.get<AnimaArray<AnimaVertex2f> >("AnimaGeometry.TextureCoords"));
		geometry->SetTangents(pt.get<AnimaArray<AnimaVertex3f> >("AnimaGeometry.Tangents"));
		geometry->SetBitangents(pt.get<AnimaArray<AnimaVertex3f> >("AnimaGeometry.Bitangents"));
		geometry->SetBoneWeights(pt.get<AnimaArray<AnimaVertex4f> >("AnimaGeometry.BoneWeights"));
		geometry->SetBoneIDs(pt.get<AnimaArray<AnimaVertex4f> >("AnimaGeometry.BoneIDs"));

		AnimaArray<AUint> indexes = pt.get<AnimaArray<AUint> >("AnimaGeometry.Indexes");
		AnimaArray<AnimaVertex3f> facesNormals = pt.get<AnimaArray<AnimaVertex3f> >("AnimaGeometry.FacesNormals");

		AInt indexesCount = indexes.size();
		AInt facesNormalsCount = facesNormals.size();

		bool hasFacesNormal = (facesNormalsCount == indexesCount % 3);

		for (AInt i = 0; i < indexesCount; i += 3)
		{
			AnimaFace face;
			face.SetIndexes(indexes[i], indexes[i + 1], indexes[i + 2]);

			if (hasFacesNormal)
				face.SetNormal(facesNormals[i % 3]);

			geometry->AddFace(face);
		}

		geometry->SetPosition(pt.get<AnimaVertex3f>("AnimaGeometry.SpaceData.Position"));
		geometry->GetTransformation()->SetTranslation(pt.get<AnimaVertex3f>("AnimaGeometry.SpaceData.Translation"));
		geometry->GetTransformation()->SetRotation(pt.get<AnimaVertex3f>("AnimaGeometry.SpaceData.Rotation"));
		geometry->GetTransformation()->SetScale(pt.get<AnimaVertex3f>("AnimaGeometry.SpaceData.Scale"));

		AnimaString materialName = pt.get<AnimaString>("AnimaGeometry.<xmlattr>.material");
		if (!materialName.empty())
			geometry->SetMaterial(_materialsManager->GetMaterialFromName(materialName));
#endif
	}

	return geometry;
}

bool AnimaGeometriesManager::LoadGeometries(const AnimaString& geometriesPath)
{
	namespace fs = boost::filesystem;
	fs::path directory(geometriesPath);

	if (fs::exists(directory) && fs::is_directory(directory))
	{
		fs::directory_iterator endIterator;
		for (fs::directory_iterator directoryIterator(directory); directoryIterator != endIterator; directoryIterator++)
		{
			if (directoryIterator->path().extension().string() == ".ageometry")
			{
				if(LoadGeometryFromFile(directoryIterator->path().string()) == nullptr)
					return false;
			}
		}
	}
	return true;
}

void AnimaGeometriesManager::SaveGeometries(const AnimaString& destinationPath)
{
	AInt count = _geometries.GetSize();
	for(AInt i = 0; i < count; i++)
	{
		SaveGeometryToFile(_geometries[i], destinationPath, true);
	}
}

bool AnimaGeometriesManager::FinalizeObjectsAfterRead()
{
	AInt count = _geometries.GetSize();
	for (AInt i = 0; i < count; i++)
		_geometries[i]->FinalizeAfterRead(_scene);

	return true;
}

END_ANIMA_ENGINE_NAMESPACE
