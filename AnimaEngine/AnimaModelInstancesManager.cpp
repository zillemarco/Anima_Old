//
//  AnimaModelInstancesManager.cpp
//  Anima
//
//  Created by Marco Zille on 20/11/14.
//
//

#include "AnimaModelInstancesManager.h"
#include "AnimaMaterialsManager.h"
#include "AnimaTexturesManager.h"
#include "AnimaTimer.h"
#include "AnimaScene.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaModelInstancesManager::AnimaModelInstancesManager(AnimaScene* scene, AnimaModelsManager* modelsManager, AnimaMeshInstancesManager* meshInstancesManager)
{
	ANIMA_ASSERT(scene != nullptr);
	ANIMA_ASSERT(modelsManager != nullptr);
	ANIMA_ASSERT(meshInstancesManager != nullptr);

	_scene = scene;
	_modelsManager = modelsManager;
	_meshInstancesManager = meshInstancesManager;
}

AnimaModelInstancesManager::~AnimaModelInstancesManager()
{
	ClearInstances();
}

AnimaModelInstance* AnimaModelInstancesManager::CreateInstance(const AnimaString& instanceName, AnimaModel* srcModel)
{
	AInt indexT = _topLevelModelInstances.Contains(instanceName);
	AInt indexM = _modelInstances.Contains(instanceName);
	if (indexT >= 0 || indexM >= 0)
		return nullptr;

	AnimaModelInstance* modelInstance = CreateInstanceFromModel(instanceName, srcModel);
	modelInstance->SetTopLevelModel(true);
	_topLevelModelInstances.Add(instanceName, modelInstance);

	return modelInstance;
}

AnimaModelInstance* AnimaModelInstancesManager::CreateInstance(const AnimaString& instanceName, const AnimaString& srcModelName, bool topLevelModel)
{
	AnimaModel* srcModel = _modelsManager->GetModelFromName(srcModelName, topLevelModel);
	if (srcModel == nullptr)
		return nullptr;
	return CreateInstance(instanceName, srcModel);
}

AnimaModelInstance* AnimaModelInstancesManager::CreateInstanceFromModel(const AnimaString& instanceName, AnimaModel* srcModel, bool useSrcModelName)
{
	if (srcModel == nullptr)
		return nullptr;

	AnimaString completeInstanceName;
	if (useSrcModelName)
		completeInstanceName = srcModel->GetName() + "." + instanceName;
	else
		completeInstanceName = instanceName;
	AnimaString modelName = srcModel->GetName();
	AnimaString modelInstanceName;
	AnimaModelInstance* newInstance = nullptr;
	int nameOffset = 0;
	while (newInstance == nullptr)
	{
		if (nameOffset > 0)
		{
			if (useSrcModelName)
				modelInstanceName = FormatString("%s.%s%d", modelName.c_str(), instanceName.c_str(), nameOffset);
			else
				modelInstanceName = FormatString("%s%d", instanceName.c_str(), nameOffset);
		}
		else
		{
			if (useSrcModelName)
				modelInstanceName = FormatString("%s.%s", modelName.c_str(), instanceName.c_str());
			else
				modelInstanceName = FormatString("%s", instanceName.c_str());
		}

		newInstance = CreateEmptyInstance(modelInstanceName);
		nameOffset++;
	}

	newInstance->CopyData(*srcModel);
	newInstance->SetMeshes(_meshInstancesManager->CreateInstances(srcModel));
	newInstance->SetModel(srcModel);

	AInt childrenCount = srcModel->GetChildrenCount();
	for (AInt i = 0; i < childrenCount; i++)
	{
		AnimaModelInstance* newChild = CreateInstanceFromModel(instanceName, (AnimaModel*)srcModel->GetChild(i), true);
		newInstance->AddChild(newChild);
	}

	newInstance->UpdateChildrenTransformation();

	return newInstance;
}

AnimaModelInstance* AnimaModelInstancesManager::CreateEmptyInstance(const AnimaString& instanceName, bool topLevelModel)
{
	AInt index = _modelInstances.Contains(instanceName);
	if (index >= 0)
		return nullptr;

	AnimaModelInstance* modelInstance = AnimaAllocatorNamespace::AllocateNew<AnimaModelInstance>(*(_scene->GetModelInstancesAllocator()), instanceName, _scene->GetDataGeneratorsManager(), _scene->GetModelInstancesAllocator());
	modelInstance->SetTopLevelModel(topLevelModel);
	
	if(topLevelModel)
		_topLevelModelInstances.Add(instanceName, modelInstance);
	_modelInstances.Add(instanceName, modelInstance);

	return modelInstance;
}

AInt AnimaModelInstancesManager::GetModelInstancesCount(bool topLevelModels)
{
	if(topLevelModels)
		return _topLevelModelInstances.GetSize();
	else
		return _modelInstances.GetSize();
}

AnimaModelInstance* AnimaModelInstancesManager::GetModelInstance(AInt index, bool topLevelModel)
{
	if(topLevelModel)
		return _topLevelModelInstances[index];
	else
		return _modelInstances[index];
}

AnimaModelInstance* AnimaModelInstancesManager::GetModelInstanceFromName(const AnimaString& name, bool topLevelModel)
{
	if(topLevelModel)
		return _topLevelModelInstances[name];
	else
		return _modelInstances[name];
}

void AnimaModelInstancesManager::ClearInstances()
{
	AInt count = _modelInstances.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaModelInstance* instance = _modelInstances[i];
		AnimaAllocatorNamespace::DeallocateObject(*(_scene->GetModelInstancesAllocator()), instance);
		instance = nullptr;
	}

	_modelInstances.RemoveAll();
	_topLevelModelInstances.RemoveAll();
}

AnimaModelInstance* AnimaModelInstancesManager::LoadModelInstanceFromFile(const AnimaString& filePath)
{
	std::ifstream fileStream(filePath);
	AnimaString xml((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
	fileStream.close();
	
	return LoadModelInstanceFromXml(xml);
}

AnimaModelInstance* AnimaModelInstancesManager::LoadModelInstanceFromXml(const AnimaString& modelXmlDefinition)
{
	AnimaModelInstance* modelInstance = nullptr;
	
	using boost::property_tree::ptree;
	ptree pt;
	
	std::stringstream ss(modelXmlDefinition);
	boost::property_tree::read_xml(ss, pt);
	
	AnimaString name = pt.get<AnimaString>("AnimaModelInstance.Name");
	bool topLevelModel = pt.get<bool>("AnimaModelInstance.TopLevelModel", false);
	
	// Controllo che il nome del modello non esista già e se esiste gli aggiungo un indice
	AnimaString originalName = name;
	AInt index = 0;
	while (_modelInstances.Contains(name) != -1)
		name = FormatString("%s_%d", originalName.c_str(), index);
	
	if(name != originalName)
		AnimaLogger::LogMessageFormat("WARNING - Error reading a model instance. A model instance named '%s' already existed so it's been renamed to '%s'", originalName.c_str(), name.c_str());
	
	modelInstance = CreateEmptyInstance(name, topLevelModel);
	
	if (modelInstance)
	{
		modelInstance->ReadObject(pt, _scene, false);
	}
	
	return modelInstance;
}

bool AnimaModelInstancesManager::LoadModelsInstances(const AnimaString& modelsInstancesPath)
{
	namespace fs = boost::filesystem;
	fs::path directory(modelsInstancesPath);
	
	if (fs::exists(directory) && fs::is_directory(directory))
	{
		fs::directory_iterator endIterator;
		for (fs::directory_iterator directoryIterator(directory); directoryIterator != endIterator; directoryIterator++)
		{
			if (directoryIterator->path().extension().string() == ".amodelinst")
			{
				if(LoadModelInstanceFromFile(directoryIterator->path().string()) == nullptr)
					return false;
			}
		}
	}
	return true;
}

void AnimaModelInstancesManager::SaveModelInstanceToFile(const AnimaString& modelInstanceName, const AnimaString& destinationPath, bool createFinalPath)
{
	AnimaModelInstance* modelInstance = _modelInstances[modelInstanceName];
	SaveModelInstanceToFile(modelInstance, destinationPath, createFinalPath);
}

void AnimaModelInstancesManager::SaveModelInstanceToFile(AnimaModelInstance* modelInstance, const AnimaString& destinationPath, bool createFinalPath)
{
	if (modelInstance == nullptr)
		return;
	
	namespace fs = boost::filesystem;
	
	AnimaString saveFileName = destinationPath;
	
	if (createFinalPath)
	{
		fs::path firstPart(destinationPath);
		fs::path secondPart(modelInstance->GetName() + ".amodelinst");
		fs::path completePath = firstPart / secondPart;
		
		saveFileName = completePath.string();
	}
	
	modelInstance->SaveObject(saveFileName);
}

void AnimaModelInstancesManager::SaveModelsInstances(const AnimaString& destinationPath)
{
	AInt count = _modelInstances.GetSize();
	for(AInt i = 0; i < count; i++)
	{
		SaveModelInstanceToFile(_modelInstances[i], destinationPath, true);
	}
}

bool AnimaModelInstancesManager::FinalizeObjectsAfterRead()
{
	AInt count = _modelInstances.GetSize();
	for(AInt i = 0; i < count; i++)
		_modelInstances[i]->FinalizeAfterRead(_scene);
	return true;
}

END_ANIMA_ENGINE_NAMESPACE