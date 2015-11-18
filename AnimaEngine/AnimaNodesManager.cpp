//
//  AnimaEngineNodesManager.cpp
//  Anima
//
//  Created by Marco Zille on 20/11/14.
//
//

#include "AnimaNodesManager.h"
#include "AnimaMaterialsManager.h"
#include "AnimaTexturesManager.h"
#include "AnimaTimer.h"
#include "AnimaXmlTranslators.h"
#include "AnimaScene.h"

#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaNodesManager::AnimaNodesManager(AnimaScene* scene, AnimaGeometriesManager* geometriesManager, AnimaMaterialsManager* materialsManager, AnimaAnimationsManager* animationsManager)
{
	ANIMA_ASSERT(scene != nullptr);
	ANIMA_ASSERT(geometriesManager != nullptr);
	ANIMA_ASSERT(materialsManager != nullptr);
	ANIMA_ASSERT(animationsManager != nullptr);

	_scene = scene;
	_geometriesManager = geometriesManager;
	_materialsManager = materialsManager;
	_animationsManager = animationsManager;
}

AnimaNodesManager::~AnimaNodesManager()
{
	ClearNodes();
}

AnimaNode* AnimaNodesManager::LoadNodeFromExternalFile(const AnimaString& nodePath, const AnimaString& name)
{
	AInt index = _topLevelNodes.Contains(name);
	if (index >= 0)
		return nullptr;
	
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(nodePath.c_str(), /*aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs*/aiProcessPreset_TargetRealtime_Quality);
	
	if (scene == nullptr)
		return nullptr;
	
	AnimaNode* newTopLevelNode = nullptr;
	
	AnimaLogger::LogMessageFormat("Loading %s", name.c_str());

	//_materialsManager->LoadMaterialsFromNode(scene, name);
	//AnimaArray<AnimaString>* materialNames = _materialsManager->GetLastMaterialsIndexMap();

	_animationsManager->LoadAnimations(scene);
	AnimaMappedArray<AnimaAnimation*>* loadedAnimations = _animationsManager->GetLastLoadedAnimations();

	if (_geometriesManager->LoadGeometriesFromNode(scene, name/*, materialNames*/))
	{
		AnimaMappedArray<AnimaGeometryBoneInfo*>* geometriesBonesInfo = _geometriesManager->GetLastGeometriesBonesInfo();

		AnimaArray<AnimaString>* geometriesName = _geometriesManager->GetLastGeometriesIndexMap();
		newTopLevelNode = LoadNodeFromScene(scene, scene->mRootNode, geometriesName, name);
						
		newTopLevelNode->SetOriginFileName(nodePath);
		newTopLevelNode->SetGeometriesBonesInfo(geometriesBonesInfo);
		newTopLevelNode->SetAnimations(loadedAnimations->GetArray());
		newTopLevelNode->GetTransformation()->SetAnimationGlobalInverseMatrix(AnimaMatrix());

		_topLevelNodes.Add(name, newTopLevelNode);
	}

	importer.FreeScene();
	return newTopLevelNode;
}

AnimaNode* AnimaNodesManager::LoadNodeFromScene(const aiScene* scene, const aiNode* sceneNode, AnimaArray<AnimaString>* geometriesMap, const AnimaString& nodeName)
{
	AnimaString newNodeName;
	AnimaString animationNodeName;
	if (!nodeName.empty())
		newNodeName = nodeName;
	else
		newNodeName = sceneNode->mName.C_Str();
	
	// Controllo che il nome del nodelo non esista già e se esiste gli aggiungo un indice
	AnimaString originalName = newNodeName;
	AInt index = 0;
	while (_nodes.Contains(newNodeName) != -1)
		newNodeName = FormatString("%s_%d", originalName.c_str(), index++);
	
	animationNodeName = sceneNode->mName.C_Str();

	AnimaNode* currentNode = AnimaAllocatorNamespace::AllocateNew<AnimaNode>(*(_scene->GetNodesAllocator()), newNodeName, _scene->GetDataGeneratorsManager(), _scene->GetNodesAllocator());
	_nodes.Add(newNodeName, currentNode);

	currentNode->SetAnimationNodeName(animationNodeName);

	AnimaMatrix nodeTransformationMatrix;
	nodeTransformationMatrix.m[0] = sceneNode->mTransformation.a1;	nodeTransformationMatrix.m[1] = sceneNode->mTransformation.a2;	nodeTransformationMatrix.m[2] = sceneNode->mTransformation.a3;	nodeTransformationMatrix.m[3] = sceneNode->mTransformation.a4;
	nodeTransformationMatrix.m[4] = sceneNode->mTransformation.b1;	nodeTransformationMatrix.m[5] = sceneNode->mTransformation.b2;	nodeTransformationMatrix.m[6] = sceneNode->mTransformation.b3;	nodeTransformationMatrix.m[7] = sceneNode->mTransformation.b4;
	nodeTransformationMatrix.m[8] = sceneNode->mTransformation.c1;	nodeTransformationMatrix.m[9] = sceneNode->mTransformation.c2;	nodeTransformationMatrix.m[10] = sceneNode->mTransformation.c3;	nodeTransformationMatrix.m[11] = sceneNode->mTransformation.c4;
	nodeTransformationMatrix.m[12] = sceneNode->mTransformation.d1;	nodeTransformationMatrix.m[13] = sceneNode->mTransformation.d2;	nodeTransformationMatrix.m[14] = sceneNode->mTransformation.d3;	nodeTransformationMatrix.m[15] = sceneNode->mTransformation.d4;

	currentNode->GetTransformation()->SetAnimationGlobalInverseMatrix(nodeTransformationMatrix.Transposed());
	
	for (AUint n = 0; n < sceneNode->mNumMeshes; n++)
	{
		AInt geometryIndex = (AInt)sceneNode->mMeshes[n];
		AnimaString geometryName = geometriesMap->at(geometryIndex);
		AnimaGeometry* geometry = _geometriesManager->GetGeometryFromName(geometryName);
		
		ANIMA_ASSERT(geometry->GetParentObject() == nullptr);
		geometry->SetParentObject(currentNode);

		currentNode->AddGeometry(geometry);
	}

	for (AUint n = 0; n < sceneNode->mNumChildren; n++)
	{
		AnimaNode* child = LoadNodeFromScene(scene, sceneNode->mChildren[n], geometriesMap, AnimaString());
		currentNode->AddChild(child);
	}

	return currentNode;
}

AnimaNode* AnimaNodesManager::CreateNode(const AnimaString& name, bool topLevelNode)
{
	AInt index = _nodes.Contains(name);
	if (index >= 0)
		return nullptr;

	AnimaNode* newNode = AnimaAllocatorNamespace::AllocateNew<AnimaNode>(*(_scene->GetNodesAllocator()), name, _scene->GetDataGeneratorsManager(), _scene->GetNodesAllocator());
	newNode->SetTopLevelNode(topLevelNode);
	
	_nodes.Add(name, newNode);
	
	if(topLevelNode)
		_topLevelNodes.Add(name, newNode);

	return newNode;
}

AInt AnimaNodesManager::GetNodesCount(bool topLevelNodes)
{
	if(topLevelNodes)
		return _topLevelNodes.GetSize();
	else
		return _nodes.GetSize();
}

AnimaNode* AnimaNodesManager::GetNode(AInt index, bool topLevelNode)
{
	if(topLevelNode)
		return _topLevelNodes[index];
	else
		return _nodes[index];
}

AnimaNode* AnimaNodesManager::GetNodeFromName(const AnimaString& name, bool topLevelNode)
{
	if(topLevelNode)
		return _topLevelNodes[name];
	else
		return _nodes[name];
}

void AnimaNodesManager::ClearNodes()
{
	AInt count = _nodes.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaNode* node = _nodes[i];
		AnimaAllocatorNamespace::DeallocateObject(*(_scene->GetNodesAllocator()), node);
		node = nullptr;
	}

	_nodes.RemoveAll();
	_topLevelNodes.RemoveAll();
}

AnimaNode* AnimaNodesManager::LoadNodeFromFile(const AnimaString& filePath)
{
	std::ifstream fileStream(filePath);
	AnimaString xml((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
	fileStream.close();

	return LoadNodeFromXml(xml);
}

AnimaNode* AnimaNodesManager::LoadNodeFromXml(const AnimaString& nodeXmlDefinition)
{
	AnimaNode* node = nullptr;
	
	using boost::property_tree::ptree;
	ptree pt;

	std::stringstream ss(nodeXmlDefinition);
	boost::property_tree::read_xml(ss, pt);

	AnimaString name = pt.get<AnimaString>("AnimaNode.Name");
	bool topLevelNode = pt.get<bool>("AnimaNode.TopLevelNode", false);
	
	// Controllo che il nome del nodelo non esista già e se esiste gli aggiungo un indice
	AnimaString originalName = name;
	AInt index = 0;
	while (_nodes.Contains(name) != -1)
		name = FormatString("%s_%d", originalName.c_str(), index);
	
	if(name != originalName)
		AnimaLogger::LogMessageFormat("WARNING - Error reading a node. A node named '%s' already existed so it's been renamed to '%s'", originalName.c_str(), name.c_str());
	
	node = CreateNode(name, topLevelNode);

	if (node)
	{
#if !defined SAVE_SCENE
		node->ReadObject(pt, _scene, false);
#else		
		try
		{
			for (auto& geometryData : pt.get_child("AnimaNode.Geometries"))
			{
				if (geometryData.first == "Geometry")
				{
					AnimaString geometryName = geometryData.second.get_value<AnimaString>("");
					AnimaGeometry* geometry = _geometriesManager->GetGeometryFromName(geometryName);

					if (geometry != nullptr)
					{
						node->AddGeometry(geometry);
						geometry->SetParentObject(node);
					}
				}
			}
		}
		catch (boost::property_tree::ptree_bad_path& exception)
		{
		}
		catch (boost::property_tree::ptree_bad_data& exception)
		{
		}
		catch (boost::property_tree::ptree_error& exception)
		{
		}

		try
		{
			for (auto& childNodeData : pt.get_child("AnimaNode.Children"))
			{
				if (childNodeData.first == "Child")
				{
					AnimaNode* child = LoadNodeFromTree(&childNodeData.second);

					if (child != nullptr)
						node->AddChild(child);
				}
			}
		}
		catch (boost::property_tree::ptree_bad_path& exception)
		{
		}
		catch (boost::property_tree::ptree_bad_data& exception)
		{
		}
		catch (boost::property_tree::ptree_error& exception)
		{
		}

		node->SetPosition(pt.get<AnimaVertex3f>("AnimaNode.SpaceData.Position"));
		node->GetTransformation()->SetTranslation(pt.get<AnimaVertex3f>("AnimaNode.SpaceData.Translation"));
		node->GetTransformation()->SetRotation(pt.get<AnimaVertex3f>("AnimaNode.SpaceData.Rotation"));
		node->GetTransformation()->SetScale(pt.get<AnimaVertex3f>("AnimaNode.SpaceData.Scale"));
#endif
	}

	return node;
}

AnimaNode* AnimaNodesManager::LoadNodeFromTree(boost::property_tree::ptree* tree)
{
	if (tree == nullptr)
		return nullptr;

	AnimaNode* node = nullptr;

	AnimaString name = tree->get<AnimaString>("AnimaNode.Name");
	
	// Controllo che il nome del nodelo non esista già e se esiste gli aggiungo un indice
	AnimaString originalName = name;
	AInt index = 0;
	while (_nodes.Contains(name) != -1)
		name = FormatString("%s_%d", originalName.c_str(), index);
	
	node = AnimaAllocatorNamespace::AllocateNew<AnimaNode>(*(_scene->GetNodesAllocator()), name, _scene->GetDataGeneratorsManager(), _scene->GetNodesAllocator());
	_nodes.Add(name, node);

	try
	{
		for (auto& geometryData : tree->get_child("AnimaNode.Geometries"))
		{
			if (geometryData.first == "Geometry")
			{
				AnimaString geometryName = geometryData.second.get_value<AnimaString>("");
				AnimaGeometry* geometry = _geometriesManager->GetGeometryFromName(geometryName);

				if (geometry != nullptr)
				{
					node->AddGeometry(geometry);
					geometry->SetParentObject(node);
				}
			}
		}
	}
	catch (boost::property_tree::ptree_bad_path& exception)
	{
	}
	catch (boost::property_tree::ptree_bad_data& exception)
	{
	}
	catch (boost::property_tree::ptree_error& exception)
	{
	}

	try
	{
		for (auto& childNodeData : tree->get_child("AnimaNode.Children"))
		{
			if (childNodeData.first == "Child")
			{
				AnimaNode* child = LoadNodeFromTree(&childNodeData.second);

				if (child != nullptr)
					node->AddChild(child);
			}
		}
	}
	catch (boost::property_tree::ptree_bad_path& exception)
	{
	}
	catch (boost::property_tree::ptree_bad_data& exception)
	{
	}
	catch (boost::property_tree::ptree_error& exception)
	{
	}

	node->SetPosition(tree->get<AnimaVertex3f>("AnimaNode.SpaceData.Position"));
	node->GetTransformation()->SetTranslation(tree->get<AnimaVertex3f>("AnimaNode.SpaceData.Translation"));
	node->GetTransformation()->SetRotation(tree->get<AnimaVertex3f>("AnimaNode.SpaceData.Rotation"));
	node->GetTransformation()->SetScale(tree->get<AnimaVertex3f>("AnimaNode.SpaceData.Scale"));
	
	return node;
}

void AnimaNodesManager::SaveNodeToFile(const AnimaString& nodeName, const AnimaString& destinationPath, bool createFinalPath)
{
	AnimaNode* node = _nodes[nodeName];
	SaveNodeToFile(node, destinationPath, createFinalPath);
}

void AnimaNodesManager::SaveNodeToFile(AnimaNode* node, const AnimaString& destinationPath, bool createFinalPath)
{
	if (node == nullptr)
		return;

	namespace fs = boost::filesystem;

	AnimaString saveFileName = destinationPath;

	if (createFinalPath)
	{
		fs::path firstPart(destinationPath);
		fs::path secondPart(node->GetName() + ".anode");
		fs::path completePath = firstPart / secondPart;

		saveFileName = completePath.string();
	}

	node->SaveObject(saveFileName);
}

boost::property_tree::ptree AnimaNodesManager::GetNodeTree(AnimaNode* node)
{
	using boost::property_tree::ptree;

	if (node == nullptr)
		return ptree();

	ptree pt;

	// Salvo il nome della geometry come attributo
	pt.put("AnimaNode.Name", node->GetName());

	// Salvo i nomi delle geometry
	ptree ptGeometries;
	AInt geometriesCount = node->GetGeometriesCount();
	for (AInt i = 0; i < geometriesCount; i++)
		ptGeometries.add("Geometry", node->GetGeometry(i)->GetName());
	pt.add_child("AnimaNode.Geometries", ptGeometries);

	// Salvo i dati di posizionamento del nodelo
	pt.add("AnimaNode.SpaceData.Position", node->GetPosition());
	pt.add("AnimaNode.SpaceData.Translation", node->GetTransformation()->GetTranslation());
	pt.add("AnimaNode.SpaceData.Rotation", node->GetTransformation()->GetRotation());
	pt.add("AnimaNode.SpaceData.Scale", node->GetTransformation()->GetScale());

	ptree ptChildren;
	AInt childrenCount = node->GetChildrenCount();
	for (AInt i = 0; i < childrenCount; i++)
		ptChildren.add_child("Child", GetNodeTree((AnimaNode*)node->GetChild(i)));

	pt.add_child("AnimaNode.Children", ptChildren);

	return pt;
}

bool AnimaNodesManager::LoadNodes(const AnimaString& nodesPath)
{
	namespace fs = boost::filesystem;
	fs::path directory(nodesPath);

	if (fs::exists(directory) && fs::is_directory(directory))
	{
		fs::directory_iterator endIterator;
		for (fs::directory_iterator directoryIterator(directory); directoryIterator != endIterator; directoryIterator++)
		{
			if (directoryIterator->path().extension().string() == ".anode")
			{
				if(LoadNodeFromFile(directoryIterator->path().string()) == nullptr)
					return false;
			}
		}
	}
	return true;
}

void AnimaNodesManager::SaveNodes(const AnimaString& destinationPath)
{
	AInt count = _nodes.GetSize();
	for(AInt i = 0; i < count; i++)
	{
		SaveNodeToFile(_nodes[i], destinationPath, true);
	}
}

bool AnimaNodesManager::FinalizeObjectsAfterRead()
{
	AInt count = _nodes.GetSize();
	for (AInt i = 0; i < count; i++)
		_nodes[i]->FinalizeAfterRead(_scene);

	return true;
}

END_ANIMA_ENGINE_NAMESPACE