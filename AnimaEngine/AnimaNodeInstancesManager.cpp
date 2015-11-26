//
//  AnimaNodeInstancesManager.cpp
//  Anima
//
//  Created by Marco Zille on 20/11/14.
//
//

#include "AnimaNodeInstancesManager.h"
#include "AnimaMaterialsManager.h"
#include "AnimaTexturesManager.h"
#include "AnimaTimer.h"
#include "AnimaScene.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaNodeInstancesManager::AnimaNodeInstancesManager(AnimaScene* scene, AnimaNodesManager* nodesManager, AnimaGeometryInstancesManager* geometryInstancesManager)
{
	ANIMA_ASSERT(scene != nullptr);
	ANIMA_ASSERT(nodesManager != nullptr);
	ANIMA_ASSERT(geometryInstancesManager != nullptr);

	_scene = scene;
	_nodesManager = nodesManager;
	_geometryInstancesManager = geometryInstancesManager;
}

AnimaNodeInstancesManager::~AnimaNodeInstancesManager()
{
	ClearInstances();
}

AnimaNodeInstance* AnimaNodeInstancesManager::CreateNodeInstance(const AnimaString& instanceName, AnimaNode* srcNode)
{
	// Non posso creare istanze di asset con questa funzione, si deve utilizzare CreateAssetInstance
	if(srcNode->IsAsset())
		return nullptr;
	
	AInt indexT = _nodeInstances.Contains(instanceName);
	AInt indexM = _assetInstances.Contains(instanceName);
	
	if (indexT >= 0 || indexM >= 0)
		return nullptr;

	AnimaNodeInstance* nodeInstance = CreateInstanceFromNode(instanceName, srcNode);
	_nodeInstances.Add(instanceName, nodeInstance);

	return nodeInstance;
}

AnimaNodeInstance* AnimaNodeInstancesManager::CreateNodeInstance(const AnimaString& instanceName, const AnimaString& srcNodeName)
{
	AnimaNode* srcNode = _nodesManager->GetNodeFromName(srcNodeName);
	
	if (srcNode == nullptr)
		return nullptr;
	
	return CreateNodeInstance(instanceName, srcNode);
}

AnimaNodeInstance* AnimaNodeInstancesManager::CreateAssetInstance(const AnimaString& instanceName, AnimaNode* srcAsset)
{
	// Non posso creare istanze di nodi con questa funzione, si deve utilizzare CreateNodeInstance
	if(!srcAsset->IsAsset())
		return nullptr;
	
	AInt indexT = _nodeInstances.Contains(instanceName);
	AInt indexM = _assetInstances.Contains(instanceName);
	
	if (indexT >= 0 || indexM >= 0)
		return nullptr;
	
	AnimaNodeInstance* assetInstance = CreateInstanceFromNode(instanceName, srcAsset);
	assetInstance->SetIsAsset(true);
	
	_nodeInstances.Add(instanceName, assetInstance);
	_assetInstances.Add(instanceName, assetInstance);
	
	return assetInstance;
}

AnimaNodeInstance* AnimaNodeInstancesManager::CreateAssetInstance(const AnimaString& instanceName, const AnimaString& srcAssetName)
{
	AnimaNode* srcAsset = _nodesManager->GetAssetFromName(srcAssetName);
	
	if (srcAsset == nullptr)
		return nullptr;
	
	return CreateAssetInstance(instanceName, srcAsset);
}

AnimaNodeInstance* AnimaNodeInstancesManager::CreateInstanceFromNode(const AnimaString& instanceName, AnimaNode* srcNode, bool useSrcNodeName)
{
	if (srcNode == nullptr)
		return nullptr;

	AnimaString completeInstanceName;
	if (useSrcNodeName)
		completeInstanceName = srcNode->GetName() + "." + instanceName;
	else
		completeInstanceName = instanceName;
	
	AnimaString nodeName = srcNode->GetName();
	AnimaString nodeInstanceName;
	AnimaNodeInstance* newInstance = nullptr;
	int nameOffset = 0;
	while (newInstance == nullptr)
	{
		if (nameOffset > 0)
		{
			if (useSrcNodeName)
				nodeInstanceName = FormatString("%s.%s%d", nodeName.c_str(), instanceName.c_str(), nameOffset);
			else
				nodeInstanceName = FormatString("%s%d", instanceName.c_str(), nameOffset);
		}
		else
		{
			if (useSrcNodeName)
				nodeInstanceName = FormatString("%s.%s", nodeName.c_str(), instanceName.c_str());
			else
				nodeInstanceName = FormatString("%s", instanceName.c_str());
		}

		newInstance = CreateEmptyInstance(nodeInstanceName);
		nameOffset++;
	}

	newInstance->CopyData(*srcNode);
	newInstance->SetGeometries(_geometryInstancesManager->CreateInstancesFromNode(srcNode));
	newInstance->SetNode(srcNode);

	AInt childrenCount = srcNode->GetChildrenCount();
	for (AInt i = 0; i < childrenCount; i++)
	{
		AnimaNodeInstance* newChild = CreateInstanceFromNode(instanceName, (AnimaNode*)srcNode->GetChild(i), true);
		newInstance->AddChild(newChild);
	}

	newInstance->UpdateChildrenTransformation();
	
	return newInstance;
}

AnimaNodeInstance* AnimaNodeInstancesManager::CreateEmptyInstance(const AnimaString& instanceName, bool asset)
{
	AInt index = _nodeInstances.Contains(instanceName);
	if (index >= 0)
		return nullptr;

	AnimaNodeInstance* nodeInstance = AnimaAllocatorNamespace::AllocateNew<AnimaNodeInstance>(*(_scene->GetNodeInstancesAllocator()), instanceName, _scene->GetDataGeneratorsManager(), _scene->GetNodeInstancesAllocator());
	nodeInstance->SetIsAsset(asset);
	
	if(asset)
		_assetInstances.Add(instanceName, nodeInstance);
	_nodeInstances.Add(instanceName, nodeInstance);

	return nodeInstance;
}

AInt AnimaNodeInstancesManager::GetNodeInstancesCount() const
{
	return _nodeInstances.GetSize();
}

AInt AnimaNodeInstancesManager::GetAssetInstancesCount() const
{
	return _assetInstances.GetSize();
}

AnimaNodeInstance* AnimaNodeInstancesManager::GetNodeInstance(AInt index)
{
	return _nodeInstances[index];
}

AnimaNodeInstance* AnimaNodeInstancesManager::GetAssetInstance(AInt index)
{
	return _assetInstances[index];
}

AnimaNodeInstance* AnimaNodeInstancesManager::GetNodeInstanceFromName(const AnimaString& name)
{
	return _nodeInstances[name];
}

AnimaNodeInstance* AnimaNodeInstancesManager::GetAssetInstanceFromName(const AnimaString& name)
{
	return _assetInstances[name];
}

void AnimaNodeInstancesManager::ClearInstances()
{
	AInt count = _nodeInstances.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaNodeInstance* instance = _nodeInstances[i];
		AnimaAllocatorNamespace::DeallocateObject(*(_scene->GetNodeInstancesAllocator()), instance);
		instance = nullptr;
	}

	_nodeInstances.RemoveAll();
	_assetInstances.RemoveAll();
}

AnimaNodeInstance* AnimaNodeInstancesManager::LoadNodeInstanceFromFile(const AnimaString& filePath)
{
	std::ifstream fileStream(filePath);
	AnimaString xml((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
	fileStream.close();
	
	return LoadNodeInstanceFromXml(xml);
}

AnimaNodeInstance* AnimaNodeInstancesManager::LoadNodeInstanceFromXml(const AnimaString& nodeXmlDefinition)
{
	AnimaNodeInstance* nodeInstance = nullptr;
	
	using boost::property_tree::ptree;
	ptree pt;
	
	std::stringstream ss(nodeXmlDefinition);
	boost::property_tree::read_xml(ss, pt);
	
	AnimaString name = pt.get<AnimaString>("AnimaNodeInstance.Name");
	bool isAsset = pt.get<bool>("AnimaNodeInstance.IsAsset", false);
	
	// Controllo che il nome del nodelo non esista già e se esiste gli aggiungo un indice
	AnimaString originalName = name;
	AInt index = 0;
	while (_nodeInstances.Contains(name) != -1)
		name = FormatString("%s_%d", originalName.c_str(), index);
	
	if(name != originalName)
		AnimaLogger::LogMessageFormat("WARNING - Error reading a node instance. A node instance named '%s' already existed so it's been renamed to '%s'", originalName.c_str(), name.c_str());
	
	nodeInstance = CreateEmptyInstance(name, isAsset);
	
	if (nodeInstance)
	{
		nodeInstance->ReadObject(pt, _scene, false);
	}
	
	return nodeInstance;
}

bool AnimaNodeInstancesManager::LoadNodesInstances(const AnimaString& nodesInstancesPath)
{
	namespace fs = boost::filesystem;
	fs::path directory(nodesInstancesPath);
	
	if (fs::exists(directory) && fs::is_directory(directory))
	{
		fs::directory_iterator endIterator;
		for (fs::directory_iterator directoryIterator(directory); directoryIterator != endIterator; directoryIterator++)
		{
			if (directoryIterator->path().extension().string() == ".anodeinst")
			{
				if(LoadNodeInstanceFromFile(directoryIterator->path().string()) == nullptr)
					return false;
			}
		}
	}
	return true;
}

void AnimaNodeInstancesManager::SaveNodeInstanceToFile(const AnimaString& nodeInstanceName, const AnimaString& destinationPath, bool createFinalPath)
{
	AnimaNodeInstance* nodeInstance = _nodeInstances[nodeInstanceName];
	SaveNodeInstanceToFile(nodeInstance, destinationPath, createFinalPath);
}

void AnimaNodeInstancesManager::SaveNodeInstanceToFile(AnimaNodeInstance* nodeInstance, const AnimaString& destinationPath, bool createFinalPath)
{
	if (nodeInstance == nullptr)
		return;
	
	namespace fs = boost::filesystem;
	
	AnimaString saveFileName = destinationPath;
	
	if (createFinalPath)
	{
		fs::path firstPart(destinationPath);
		fs::path secondPart(nodeInstance->GetName() + ".anodeinst");
		fs::path completePath = firstPart / secondPart;
		
		saveFileName = completePath.string();
	}
	
	nodeInstance->SaveObject(saveFileName);
}

void AnimaNodeInstancesManager::SaveNodesInstances(const AnimaString& destinationPath)
{
	AInt count = _nodeInstances.GetSize();
	for(AInt i = 0; i < count; i++)
	{
		SaveNodeInstanceToFile(_nodeInstances[i], destinationPath, true);
	}
}

bool AnimaNodeInstancesManager::FinalizeObjectsAfterRead()
{
	AInt count = _nodeInstances.GetSize();
	for(AInt i = 0; i < count; i++)
		_nodeInstances[i]->FinalizeAfterRead(_scene);
	return true;
}

END_ANIMA_ENGINE_NAMESPACE