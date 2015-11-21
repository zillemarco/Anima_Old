//
//  AnimaGeometryInstancesManager.cpp
//  Anima
//
//  Created by Marco Zille on 20/11/14.
//
//

#include "AnimaGeometryInstancesManager.h"
#include "AnimaMaterialsManager.h"
#include "AnimaTexturesManager.h"
#include "AnimaTimer.h"
#include "AnimaScene.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaGeometryInstancesManager::AnimaGeometryInstancesManager(AnimaScene* scene, AnimaGeometriesManager* geometriesManager)
{
	ANIMA_ASSERT(scene != nullptr);
	ANIMA_ASSERT(geometriesManager != nullptr);

	_scene = scene;
	_geometriesManager = geometriesManager;
}

AnimaGeometryInstancesManager::~AnimaGeometryInstancesManager()
{
	ClearInstances();
	ClearLastInstancesFromNode();
}

AnimaGeometryInstance* AnimaGeometryInstancesManager::CreateGeometryInstance(const AnimaString& instanceName, AnimaGeometry* srcGeometry)
{
	AInt index = _geometryInstances.Contains(instanceName);
	if (index >= 0)
		return nullptr;

	//AnimaMaterial* oldMaterial = srcGeometry->GetMaterial();
	//AnimaMaterial* newMaterial = _scene->GetMaterialsManager()->CreateMaterial(instanceName + ".material");

	//if (oldMaterial)
	//{
	//	newMaterial->CopyData(*oldMaterial);

	//	for (AInt ns = 0; ns < oldMaterial->GetShadersCount(); ns++)
	//		newMaterial->AddShader(oldMaterial->GetShaderName(ns));
	//}

	AnimaGeometryInstance* geometryInstance = AnimaAllocatorNamespace::AllocateNew<AnimaGeometryInstance>(*(_scene->GetGeometryInstancesAllocator()), instanceName, _scene->GetDataGeneratorsManager(), _scene->GetGeometryInstancesAllocator());
	geometryInstance->CopyData(*srcGeometry);
	//geometryInstance->SetMaterial(newMaterial);
	geometryInstance->SetGeometry(srcGeometry);

	for (AInt ns = 0; ns < srcGeometry->GetShadersCount(); ns++)
		geometryInstance->AddShader(srcGeometry->GetShaderName(ns));

	srcGeometry->AddInstance(geometryInstance);

	_geometryInstances.Add(instanceName, geometryInstance);

	return geometryInstance;
}

AnimaGeometryInstance* AnimaGeometryInstancesManager::CreateGeometryInstance(const AnimaString& instanceName, const AnimaString& srcGeometryName)
{
	AnimaGeometry* srcGeometry = _geometriesManager->GetGeometryFromName(srcGeometryName);
	if (srcGeometry == nullptr)
		return nullptr;
	return CreateGeometryInstance(instanceName, srcGeometry);
}

AnimaGeometryInstance* AnimaGeometryInstancesManager::CreateEmptyInstance(const AnimaString& instanceName)
{
	AInt index = _geometryInstances.Contains(instanceName);
	if (index >= 0)
		return nullptr;

	AnimaGeometryInstance* geometryInstance = AnimaAllocatorNamespace::AllocateNew<AnimaGeometryInstance>(*(_scene->GetGeometryInstancesAllocator()), instanceName, _scene->GetDataGeneratorsManager(), _scene->GetGeometryInstancesAllocator());
	_geometryInstances.Add(instanceName, geometryInstance);

	return geometryInstance;
}

AnimaArray<AnimaGeometryInstance*>* AnimaGeometryInstancesManager::CreateInstancesFromNode(AnimaNode* srcNode)
{
	ClearLastInstancesFromNode();

	if (srcNode == nullptr)
		return nullptr;
	
	AInt nodeGeometriesCount = srcNode->GetGeometriesCount();
	for (AInt i = 0; i < nodeGeometriesCount; i++)
	{
		AnimaGeometry* geometry = srcNode->GetGeometry(i);

		AnimaString geometryName = geometry->GetName();
		AnimaString geometryInstanceName;
		AnimaGeometryInstance* newInstance = nullptr;
		int nameOffset = 0;
		while (newInstance == nullptr)
		{
			if (nameOffset > 0)
				geometryInstanceName = FormatString("%s.instance%d", geometryName.c_str(), i + nameOffset);
			else
				geometryInstanceName = FormatString("%s.instance%d", geometryName.c_str(), i);

			newInstance = CreateEmptyInstance(geometryInstanceName);
			nameOffset++;
		}
		
		//AnimaMaterial* oldMaterial = geometry->GetMaterial();
		//AnimaMaterial* newMaterial = _scene->GetMaterialsManager()->CreateMaterial(geometryInstanceName + ".material");

		//if (oldMaterial)
		//{
		//	newMaterial->CopyData(*oldMaterial);

		//	for (AInt ns = 0; ns < oldMaterial->GetShadersCount(); ns++)
		//		newMaterial->AddShader(oldMaterial->GetShaderName(ns));
		//}

		newInstance->CopyData(*geometry);
		//newInstance->SetMaterial(newMaterial);
		newInstance->SetGeometry(geometry);

		for (AInt ns = 0; ns < geometry->GetShadersCount(); ns++)
			newInstance->AddShader(geometry->GetShaderName(ns));

		geometry->AddInstance(newInstance);

		_lastInstancesFromNode.push_back(newInstance);
	}

	return &_lastInstancesFromNode;
}

AInt AnimaGeometryInstancesManager::GetGeometryInstancesCount()
{
	return _geometryInstances.GetSize();
}

AnimaGeometryInstance* AnimaGeometryInstancesManager::GetGeometryInstance(AInt index)
{
	return _geometryInstances[index];
}

AnimaGeometryInstance* AnimaGeometryInstancesManager::GetGeometryInstanceFromName(const AnimaString& name)
{
	return _geometryInstances[name];
}

void AnimaGeometryInstancesManager::ClearInstances()
{
	AInt count = _geometryInstances.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaGeometryInstance* instance = _geometryInstances[i];
		AnimaAllocatorNamespace::DeallocateObject(*(_scene->GetGeometryInstancesAllocator()), instance);
		instance = nullptr;
	}

	_geometryInstances.RemoveAll();
}

void AnimaGeometryInstancesManager::ClearLastInstancesFromNode()
{
	_lastInstancesFromNode.clear();
}

AnimaGeometryInstance* AnimaGeometryInstancesManager::LoadGeometryInstanceFromFile(const AnimaString& filePath)
{
	std::ifstream fileStream(filePath);
	AnimaString xml((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
	fileStream.close();
	
	return LoadGeometryInstanceFromXml(xml);
}

AnimaGeometryInstance* AnimaGeometryInstancesManager::LoadGeometryInstanceFromXml(const AnimaString& geometryXmlDefinition)
{
	AnimaGeometryInstance* geometryInstance = nullptr;
	
	using boost::property_tree::ptree;
	ptree pt;
	
	std::stringstream ss(geometryXmlDefinition);
	boost::property_tree::read_xml(ss, pt);
	
	AnimaString name = pt.get<AnimaString>("AnimaGeometryInstance.Name");
	
	// Controllo che il nome della geometry non esista già e se esiste gli aggiungo un indice
	AnimaString originalName = name;
	AInt index = 0;
	while (_geometryInstances.Contains(name) != -1)
		name = FormatString("%s_%d", originalName.c_str(), index);
	
	if(name != originalName)
		AnimaLogger::LogMessageFormat("WARNING - Error reading a geometry instance. A geometry instance named '%s' already existed so it's been renamed to '%s'", originalName.c_str(), name.c_str());
	
	geometryInstance = CreateEmptyInstance(name);
	
	if (geometryInstance)
	{
		geometryInstance->ReadObject(pt, _scene, false);
	}
	
	return geometryInstance;
}

bool AnimaGeometryInstancesManager::LoadGeometriesInstances(const AnimaString& geometriesInstancesPath)
{
	namespace fs = boost::filesystem;
	fs::path directory(geometriesInstancesPath);
	
	if (fs::exists(directory) && fs::is_directory(directory))
	{
		fs::directory_iterator endIterator;
		for (fs::directory_iterator directoryIterator(directory); directoryIterator != endIterator; directoryIterator++)
		{
			if (directoryIterator->path().extension().string() == ".ageometryinst")
			{
				if(LoadGeometryInstanceFromFile(directoryIterator->path().string()) == nullptr)
					return false;
			}
		}
	}
	return true;
}

void AnimaGeometryInstancesManager::SaveGeometryInstanceToFile(const AnimaString& geometryInstanceName, const AnimaString& destinationPath, bool createFinalPath)
{
	AnimaGeometryInstance* geometryInstance = _geometryInstances[geometryInstanceName];
	SaveGeometryInstanceToFile(geometryInstance, destinationPath, createFinalPath);
}

void AnimaGeometryInstancesManager::SaveGeometryInstanceToFile(AnimaGeometryInstance* geometryInstance, const AnimaString& destinationPath, bool createFinalPath)
{
	if (geometryInstance == nullptr)
		return;
	
	namespace fs = boost::filesystem;
	
	AnimaString saveFileName = destinationPath;
	
	if (createFinalPath)
	{
		fs::path firstPart(destinationPath);
		fs::path secondPart(geometryInstance->GetName() + ".ageometryinst");
		fs::path completePath = firstPart / secondPart;
		
		saveFileName = completePath.string();
	}
	
	geometryInstance->SaveObject(saveFileName);
}

void AnimaGeometryInstancesManager::SaveGeometriesInstances(const AnimaString& destinationPath)
{
	AInt count = _geometryInstances.GetSize();
	for(AInt i = 0; i < count; i++)
	{
		SaveGeometryInstanceToFile(_geometryInstances[i], destinationPath, true);
	}
}

bool AnimaGeometryInstancesManager::FinalizeObjectsAfterRead()
{
	AInt count = _geometryInstances.GetSize();
	for(AInt i = 0; i < count; i++)
		_geometryInstances[i]->FinalizeAfterRead(_scene);
	return true;
}

END_ANIMA_ENGINE_NAMESPACE