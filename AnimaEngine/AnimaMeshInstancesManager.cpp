//
//  AnimaMeshInstancesManager.cpp
//  Anima
//
//  Created by Marco Zille on 20/11/14.
//
//

#include "AnimaMeshInstancesManager.h"
#include "AnimaMaterialsManager.h"
#include "AnimaTexturesManager.h"
#include "AnimaTimer.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaMeshInstancesManager::AnimaMeshInstancesManager(AnimaScene* scene, AnimaMeshesManager* meshesManager)
{
	ANIMA_ASSERT(scene != nullptr);
	ANIMA_ASSERT(meshesManager != nullptr);

	_scene = scene;
	_meshesManager = meshesManager;
}

AnimaMeshInstancesManager::~AnimaMeshInstancesManager()
{
	ClearInstances();
	ClearLastInstancesFromModel();
}

AnimaMeshInstance* AnimaMeshInstancesManager::CreateInstance(const AnimaString& instanceName, AnimaMesh* srcMesh)
{
	AInt index = _meshInstances.Contains(instanceName);
	if (index >= 0)
		return nullptr;

	//AnimaMaterial* oldMaterial = srcMesh->GetMaterial();
	//AnimaMaterial* newMaterial = _scene->GetMaterialsManager()->CreateMaterial(instanceName + ".material");

	//if (oldMaterial)
	//{
	//	newMaterial->CopyData(*oldMaterial);

	//	for (AInt ns = 0; ns < oldMaterial->GetShadersCount(); ns++)
	//		newMaterial->AddShader(oldMaterial->GetShaderName(ns));
	//}

	AnimaMeshInstance* meshInstance = AnimaAllocatorNamespace::AllocateNew<AnimaMeshInstance>(*(_scene->GetMeshInstancesAllocator()), instanceName, _scene->GetDataGeneratorsManager(), _scene->GetMeshInstancesAllocator());
	meshInstance->CopyData(*srcMesh);
	//meshInstance->SetMaterial(newMaterial);
	meshInstance->SetMesh(srcMesh);

	for (AInt ns = 0; ns < srcMesh->GetShadersCount(); ns++)
		meshInstance->AddShader(srcMesh->GetShaderName(ns));

	srcMesh->AddInstance(meshInstance);

	_meshInstances.Add(instanceName, meshInstance);

	return meshInstance;
}

AnimaMeshInstance* AnimaMeshInstancesManager::CreateInstance(const AnimaString& instanceName, const AnimaString& srcMeshName)
{
	AnimaMesh* srcMesh = _meshesManager->GetMeshFromName(srcMeshName);
	if (srcMesh == nullptr)
		return nullptr;
	return CreateInstance(instanceName, srcMesh);
}

AnimaMeshInstance* AnimaMeshInstancesManager::CreateEmptyInstance(const AnimaString& instanceName)
{
	AInt index = _meshInstances.Contains(instanceName);
	if (index >= 0)
		return nullptr;

	AnimaMeshInstance* meshInstance = AnimaAllocatorNamespace::AllocateNew<AnimaMeshInstance>(*(_scene->GetMeshInstancesAllocator()), instanceName, _scene->GetDataGeneratorsManager(), _scene->GetMeshInstancesAllocator());
	_meshInstances.Add(instanceName, meshInstance);

	return meshInstance;
}

AnimaArray<AnimaMeshInstance*>* AnimaMeshInstancesManager::CreateInstances(AnimaModel* srcModel)
{
	ClearLastInstancesFromModel();

	if (srcModel == nullptr)
		return nullptr;
	
	AInt modelMeshesCount = srcModel->GetMeshesCount();
	for (AInt i = 0; i < modelMeshesCount; i++)
	{
		AnimaMesh* mesh = srcModel->GetMesh(i);

		AnimaString meshName = mesh->GetName();
		AnimaString meshInstanceName;
		AnimaMeshInstance* newInstance = nullptr;
		int nameOffset = 0;
		while (newInstance == nullptr)
		{
			if (nameOffset > 0)
				meshInstanceName = FormatString("%s.instance%d", meshName.c_str(), i + nameOffset);
			else
				meshInstanceName = FormatString("%s.instance%d", meshName.c_str(), i);

			newInstance = CreateEmptyInstance(meshInstanceName);
			nameOffset++;
		}
		
		//AnimaMaterial* oldMaterial = mesh->GetMaterial();
		//AnimaMaterial* newMaterial = _scene->GetMaterialsManager()->CreateMaterial(meshInstanceName + ".material");

		//if (oldMaterial)
		//{
		//	newMaterial->CopyData(*oldMaterial);

		//	for (AInt ns = 0; ns < oldMaterial->GetShadersCount(); ns++)
		//		newMaterial->AddShader(oldMaterial->GetShaderName(ns));
		//}

		newInstance->CopyData(*mesh);
		//newInstance->SetMaterial(newMaterial);
		newInstance->SetMesh(mesh);

		for (AInt ns = 0; ns < mesh->GetShadersCount(); ns++)
			newInstance->AddShader(mesh->GetShaderName(ns));

		mesh->AddInstance(newInstance);

		_lastInstancesFromModel.push_back(newInstance);
	}

	return &_lastInstancesFromModel;
}

AInt AnimaMeshInstancesManager::GetMeshInstancesCount()
{
	return _meshInstances.GetSize();
}

AnimaMeshInstance* AnimaMeshInstancesManager::GetMeshInstance(AInt index)
{
	return _meshInstances[index];
}

AnimaMeshInstance* AnimaMeshInstancesManager::GetMeshInstanceFromName(const AnimaString& name)
{
	return _meshInstances[name];
}

void AnimaMeshInstancesManager::ClearInstances()
{
	AInt count = _meshInstances.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaMeshInstance* instance = _meshInstances[i];
		AnimaAllocatorNamespace::DeallocateObject(*(_scene->GetMeshInstancesAllocator()), instance);
		instance = nullptr;
	}

	_meshInstances.RemoveAll();
}

void AnimaMeshInstancesManager::ClearLastInstancesFromModel()
{
	_lastInstancesFromModel.clear();
}

AnimaMeshInstance* AnimaMeshInstancesManager::LoadMeshInstanceFromFile(const AnimaString& filePath)
{
	std::ifstream fileStream(filePath);
	AnimaString xml((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
	fileStream.close();
	
	return LoadMeshInstanceFromXml(xml);
}

AnimaMeshInstance* AnimaMeshInstancesManager::LoadMeshInstanceFromXml(const AnimaString& meshXmlDefinition)
{
	AnimaMeshInstance* meshInstance = nullptr;
	
	using boost::property_tree::ptree;
	ptree pt;
	
	std::stringstream ss(meshXmlDefinition);
	boost::property_tree::read_xml(ss, pt);
	
	AnimaString name = pt.get<AnimaString>("AnimaMeshInstance.Name");
	
	// Controllo che il nome della mesh non esista già e se esiste gli aggiungo un indice
	AnimaString originalName = name;
	AInt index = 0;
	while (_meshInstances.Contains(name) != -1)
		name = FormatString("%s_%d", originalName.c_str(), index);
	
	if(name != originalName)
		AnimaLogger::LogMessageFormat("WARNING - Error reading a mesh instance. A mesh instance named '%s' already existed so it's been renamed to '%s'", originalName.c_str(), name.c_str());
	
	meshInstance = CreateEmptyInstance(name);
	
	if (meshInstance)
	{
		meshInstance->ReadObject(pt, _scene, false);
	}
	
	return meshInstance;
}

bool AnimaMeshInstancesManager::LoadMeshesInstances(const AnimaString& meshesInstancesPath)
{
	namespace fs = boost::filesystem;
	fs::path directory(meshesInstancesPath);
	
	if (fs::exists(directory) && fs::is_directory(directory))
	{
		fs::directory_iterator endIterator;
		for (fs::directory_iterator directoryIterator(directory); directoryIterator != endIterator; directoryIterator++)
		{
			if (directoryIterator->path().extension().string() == ".ameshinst")
			{
				if(LoadMeshInstanceFromFile(directoryIterator->path().string()) == nullptr)
					return false;
			}
		}
	}
	return true;
}

void AnimaMeshInstancesManager::SaveMeshInstanceToFile(const AnimaString& meshInstanceName, const AnimaString& destinationPath, bool createFinalPath)
{
	AnimaMeshInstance* meshInstance = _meshInstances[meshInstanceName];
	SaveMeshInstanceToFile(meshInstance, destinationPath, createFinalPath);
}

void AnimaMeshInstancesManager::SaveMeshInstanceToFile(AnimaMeshInstance* meshInstance, const AnimaString& destinationPath, bool createFinalPath)
{
	if (meshInstance == nullptr)
		return;
	
	namespace fs = boost::filesystem;
	
	AnimaString saveFileName = destinationPath;
	
	if (createFinalPath)
	{
		fs::path firstPart(destinationPath);
		fs::path secondPart(meshInstance->GetName() + ".ameshinst");
		fs::path completePath = firstPart / secondPart;
		
		saveFileName = completePath.string();
	}
	
	meshInstance->SaveObject(saveFileName);
}

void AnimaMeshInstancesManager::SaveMeshesInstances(const AnimaString& destinationPath)
{
	AInt count = _meshInstances.GetSize();
	for(AInt i = 0; i < count; i++)
	{
		SaveMeshInstanceToFile(_meshInstances[i], destinationPath, true);
	}
}

bool AnimaMeshInstancesManager::FinalizeObjectsAfterRead()
{
	AInt count = _meshInstances.GetSize();
	for(AInt i = 0; i < count; i++)
		_meshInstances[i]->FinalizeAfterRead(_scene);
	return true;
}

END_ANIMA_ENGINE_NAMESPACE