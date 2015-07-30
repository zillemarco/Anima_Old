//
//  AnimaEngineModelsManager.cpp
//  Anima
//
//  Created by Marco Zille on 20/11/14.
//
//

#include "AnimaModelsManager.h"
#include "AnimaMaterialsManager.h"
#include "AnimaTexturesManager.h"
#include "AnimaTimer.h"
#include "AnimaXmlTranslators.h"

#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaModelsManager::AnimaModelsManager(AnimaScene* scene, AnimaMeshesManager* meshesManager, AnimaMaterialsManager* materialsManager, AnimaAnimationsManager* animationsManager)
{
	ANIMA_ASSERT(scene != nullptr);
	ANIMA_ASSERT(meshesManager != nullptr);
	ANIMA_ASSERT(materialsManager != nullptr);
	ANIMA_ASSERT(animationsManager != nullptr);

	_scene = scene;
	_meshesManager = meshesManager;
	_materialsManager = materialsManager;
	_animationsManager = animationsManager;
}

AnimaModelsManager::~AnimaModelsManager()
{
	ClearModels();
}

AnimaModel* AnimaModelsManager::LoadModelFromExternalFile(const AnimaString& modelPath, const AnimaString& name)
{
	AInt index = _topLevelModels.Contains(name);
	if (index >= 0)
		return nullptr;
	
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelPath.c_str(), /*aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs*/aiProcessPreset_TargetRealtime_Quality);
	
	if (scene == nullptr)
		return nullptr;
	
	AnimaModel* newTopLevelModel = nullptr;

	//_materialsManager->LoadMaterialsFromModel(scene, name);
	//AnimaArray<AnimaString>* materialNames = _materialsManager->GetLastMaterialsIndexMap();

	_animationsManager->LoadAnimations(scene);
	AnimaMappedArray<AnimaAnimation*>* loadedAnimations = _animationsManager->GetLastLoadedAnimations();

	if (_meshesManager->LoadMeshesFromModel(scene, name/*, materialNames*/))
	{
		AnimaMappedArray<AnimaMeshBoneInfo*>* meshesBonesInfo = _meshesManager->GetLastMeshesBonesInfo();

		AnimaArray<AnimaString>* meshesName = _meshesManager->GetLastMeshesIndexMap();		
		newTopLevelModel = LoadModelFromScene(scene, scene->mRootNode, meshesName, name);
						
		newTopLevelModel->SetOriginFileName(modelPath);
		newTopLevelModel->SetMeshesBonesInfo(meshesBonesInfo);
		newTopLevelModel->SetAnimations(loadedAnimations->GetArray());
		newTopLevelModel->GetTransformation()->SetModelNodeTransformationMatrix(AnimaMatrix());

		_topLevelModels.Add(name, newTopLevelModel);
	}

	importer.FreeScene();
	return newTopLevelModel;
}

AnimaModel* AnimaModelsManager::LoadModelFromScene(const aiScene* scene, const aiNode* sceneNode, AnimaArray<AnimaString>* meshesMap, const AnimaString& modelName)
{
	AnimaString newModelName;
	AnimaString animationNodeName;
	if (!modelName.empty())
		newModelName = modelName;
	else
		newModelName = sceneNode->mName.C_Str();

	animationNodeName = sceneNode->mName.C_Str();

	AnimaModel* currentModel = AnimaAllocatorNamespace::AllocateNew<AnimaModel>(*(_scene->GetModelsAllocator()), newModelName, _scene->GetDataGeneratorsManager(), _scene->GetModelsAllocator());
	_models.Add(newModelName, currentModel);

	currentModel->SetAnimationNodeName(animationNodeName);

	AnimaMatrix modelNodeTransformationMatrix;
	modelNodeTransformationMatrix.m[0] = sceneNode->mTransformation.a1;		modelNodeTransformationMatrix.m[1] = sceneNode->mTransformation.a2;		modelNodeTransformationMatrix.m[2] = sceneNode->mTransformation.a3;		modelNodeTransformationMatrix.m[3] = sceneNode->mTransformation.a4;
	modelNodeTransformationMatrix.m[4] = sceneNode->mTransformation.b1;		modelNodeTransformationMatrix.m[5] = sceneNode->mTransformation.b2;		modelNodeTransformationMatrix.m[6] = sceneNode->mTransformation.b3;		modelNodeTransformationMatrix.m[7] = sceneNode->mTransformation.b4;
	modelNodeTransformationMatrix.m[8] = sceneNode->mTransformation.c1;		modelNodeTransformationMatrix.m[9] = sceneNode->mTransformation.c2;		modelNodeTransformationMatrix.m[10] = sceneNode->mTransformation.c3;	modelNodeTransformationMatrix.m[11] = sceneNode->mTransformation.c4;
	modelNodeTransformationMatrix.m[12] = sceneNode->mTransformation.d1;	modelNodeTransformationMatrix.m[13] = sceneNode->mTransformation.d2;	modelNodeTransformationMatrix.m[14] = sceneNode->mTransformation.d3;	modelNodeTransformationMatrix.m[15] = sceneNode->mTransformation.d4;

	currentModel->GetTransformation()->SetModelNodeTransformationMatrix(modelNodeTransformationMatrix.Transposed());
	
	for (AUint n = 0; n < sceneNode->mNumMeshes; n++)
	{
		AInt meshIndex = (AInt)sceneNode->mMeshes[n];
		AnimaString meshName = meshesMap->at(meshIndex);
		AnimaMesh* mesh = _meshesManager->GetMeshFromName(meshName);
		
		ANIMA_ASSERT(mesh->GetParentObject() == nullptr);
		mesh->SetParentObject(currentModel);

		currentModel->AddMesh(mesh);
	}

	for (AUint n = 0; n < sceneNode->mNumChildren; n++)
	{
		AnimaModel* child = LoadModelFromScene(scene, sceneNode->mChildren[n], meshesMap, AnimaString());
		currentModel->AddChild(child);
	}

	return currentModel;
}

AnimaModel* AnimaModelsManager::CreateModel(const AnimaString& name)
{
	AInt index = _topLevelModels.Contains(name);
	if (index >= 0)
		return nullptr;

	AnimaModel* newModel = AnimaAllocatorNamespace::AllocateNew<AnimaModel>(*(_scene->GetModelsAllocator()), name, _scene->GetDataGeneratorsManager(), _scene->GetModelsAllocator());
	_models.Add(name, newModel);
	_topLevelModels.Add(name, newModel);
	return newModel;
}

AInt AnimaModelsManager::GetModelsCount()
{
	return _topLevelModels.GetSize();
}

AnimaModel* AnimaModelsManager::GetModel(AInt index)
{
	return _topLevelModels[index];
}

AnimaModel* AnimaModelsManager::GetModelFromName(const AnimaString& name)
{
	return _topLevelModels[name];
}

void AnimaModelsManager::ClearModels()
{
	AInt count = _models.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaModel* model = _models[i];
		AnimaAllocatorNamespace::DeallocateObject(*(_scene->GetModelsAllocator()), model);
		model = nullptr;
	}

	_models.RemoveAll();
	_topLevelModels.RemoveAll();
}

AnimaModel* AnimaModelsManager::LoadModelFromFile(const AnimaString& filePath)
{
	std::ifstream fileStream(filePath);
	AnimaString xml((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
	fileStream.close();

	return LoadModelFromXml(xml);
}

AnimaModel* AnimaModelsManager::LoadModelFromXml(const AnimaString& modelXmlDefinition)
{
	AnimaModel* model = nullptr;
	
	using boost::property_tree::ptree;
	ptree pt;

	std::stringstream ss(modelXmlDefinition);
	boost::property_tree::read_xml(ss, pt);

	AnimaString name = pt.get<AnimaString>("AnimaModel.Name");

	model = CreateModel(name);

	if (model)
	{
		try
		{
			for (auto& meshData : pt.get_child("AnimaModel.Meshes"))
			{
				if (meshData.first == "Mesh")
				{
					AnimaString meshName = meshData.second.get_value<AnimaString>("");
					AnimaMesh* mesh = _meshesManager->GetMeshFromName(meshName);

					if (mesh != nullptr)
					{
						model->AddMesh(mesh);
						mesh->SetParentObject(model);
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
			for (auto& childModelData : pt.get_child("AnimaModel.Children"))
			{
				if (childModelData.first == "Child")
				{
					AnimaModel* child = LoadModelFromTree(&childModelData.second);

					if (child != nullptr)
						model->AddChild(child);
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

		model->SetPosition(pt.get<AnimaVertex3f>("AnimaModel.SpaceData.Position"));
		model->GetTransformation()->SetTranslation(pt.get<AnimaVertex3f>("AnimaModel.SpaceData.Translation"));
		model->GetTransformation()->SetRotation(pt.get<AnimaVertex3f>("AnimaModel.SpaceData.Rotation"));
		model->GetTransformation()->SetScale(pt.get<AnimaVertex3f>("AnimaModel.SpaceData.Scale"));
	}

	return model;
}

AnimaModel* AnimaModelsManager::LoadModelFromTree(boost::property_tree::ptree* tree)
{
	if (tree == nullptr)
		return nullptr;

	AnimaModel* model = nullptr;

	AnimaString name = tree->get<AnimaString>("AnimaModel.Name");
	model = AnimaAllocatorNamespace::AllocateNew<AnimaModel>(*(_scene->GetModelsAllocator()), name, _scene->GetDataGeneratorsManager(), _scene->GetModelsAllocator());
	_models.Add(name, model);

	try
	{
		for (auto& meshData : tree->get_child("AnimaModel.Meshes"))
		{
			if (meshData.first == "Mesh")
			{
				AnimaString meshName = meshData.second.get_value<AnimaString>("");
				AnimaMesh* mesh = _meshesManager->GetMeshFromName(meshName);

				if (mesh != nullptr)
				{
					model->AddMesh(mesh);
					mesh->SetParentObject(model);
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
		for (auto& childModelData : tree->get_child("AnimaModel.Children"))
		{
			if (childModelData.first == "Child")
			{
				AnimaModel* child = LoadModelFromTree(&childModelData.second);

				if (child != nullptr)
					model->AddChild(child);
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

	model->SetPosition(tree->get<AnimaVertex3f>("AnimaModel.SpaceData.Position"));
	model->GetTransformation()->SetTranslation(tree->get<AnimaVertex3f>("AnimaModel.SpaceData.Translation"));
	model->GetTransformation()->SetRotation(tree->get<AnimaVertex3f>("AnimaModel.SpaceData.Rotation"));
	model->GetTransformation()->SetScale(tree->get<AnimaVertex3f>("AnimaModel.SpaceData.Scale"));
	
	return model;
}

void AnimaModelsManager::SaveModelToFile(const AnimaString& modelName, const AnimaString& filePath)
{
	AnimaModel* model = _models[modelName];
	SaveModelToFile(model, filePath);
}

void AnimaModelsManager::SaveModelToFile(AnimaModel* model, const AnimaString& filePath)
{
	if (model == nullptr)
		return;

	using boost::property_tree::ptree;

	boost::property_tree::write_xml(filePath, GetModelTree(model), std::locale(), boost::property_tree::xml_writer_make_settings<ptree::key_type>('\t', 1));
}

boost::property_tree::ptree AnimaModelsManager::GetModelTree(AnimaModel* model)
{
	using boost::property_tree::ptree;

	if (model == nullptr)
		return ptree();

	ptree pt;

	// Salvo il nome della mesh come attributo
	pt.put("AnimaModel.Name", model->GetName());

	// Salvo i nomi delle mesh
	ptree ptMeshes;
	AInt meshesCount = model->GetMeshesCount();
	for (AInt i = 0; i < meshesCount; i++)
		ptMeshes.add("Mesh", model->GetMesh(i)->GetName());
	pt.add_child("AnimaModel.Meshes", ptMeshes);

	// Salvo i dati di posizionamento del modello
	pt.add("AnimaModel.SpaceData.Position", model->GetPosition());
	pt.add("AnimaModel.SpaceData.Translation", model->GetTransformation()->GetTranslation());
	pt.add("AnimaModel.SpaceData.Rotation", model->GetTransformation()->GetRotation());
	pt.add("AnimaModel.SpaceData.Scale", model->GetTransformation()->GetScale());

	ptree ptChildren;
	AInt childrenCount = model->GetChildrenCount();
	for (AInt i = 0; i < childrenCount; i++)
		ptChildren.add_child("Child", GetModelTree((AnimaModel*)model->GetChild(i)));

	pt.add_child("AnimaModel.Children", ptChildren);

	return pt;
}

void AnimaModelsManager::LoadModels(const AnimaString& modelsPath)
{
	namespace fs = boost::filesystem;
	fs::path directory(modelsPath);

	if (fs::exists(directory) && fs::is_directory(directory))
	{
		fs::directory_iterator endIterator;
		for (fs::directory_iterator directoryIterator(directory); directoryIterator != endIterator; directoryIterator++)
		{
			if (directoryIterator->path().extension().string() == ".amodel")
			{
				LoadModelFromFile(directoryIterator->path().string());
				printf("Read %s\n", directoryIterator->path().string().c_str());
			}
		}
	}
}

END_ANIMA_ENGINE_NAMESPACE