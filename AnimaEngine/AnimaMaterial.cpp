//
//  AnimaMaterial.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaMaterial.h"
#include "AnimaDataGeneratorsManager.h"
#include "AnimaXmlTranslators.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaMaterial::AnimaMaterial(AnimaAllocator* allocator, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name)
	: AnimaMappedValues(allocator, dataGeneratorManager, name)
{
	IMPLEMENT_ANIMA_CLASS(AnimaMaterial);
	//AddShader("base-material-fs");

//	SetInteger("FrontFace", GL_CCW);
//	SetInteger("CullFace", GL_BACK);
//
//	SetColor("Albedo", 0.560f, 0.570f, 0.580f, 1.0f);
//	SetFloat("Specular", 0.5f);
//	SetFloat("Metallic", 0.0f);
//	SetFloat("Roughness", 0.5f);
//	SetFloat("ReflectionIntensity", 0.8f);
}

AnimaMaterial::AnimaMaterial(const AnimaMaterial& src)
	: AnimaMappedValues(src)
	, _shadersNames(src._shadersNames)
{
}

AnimaMaterial::AnimaMaterial(AnimaMaterial&& src)
	: AnimaMappedValues(src)
	, _shadersNames(src._shadersNames)
{
}

AnimaMaterial::~AnimaMaterial()
{
}

AnimaMaterial& AnimaMaterial::operator=(const AnimaMaterial& src)
{
	if (this != &src)
	{
		AnimaMappedValues::operator=(src);
		_shadersNames = src._shadersNames;
	}
	return *this;
}

AnimaMaterial& AnimaMaterial::operator=(AnimaMaterial&& src)
{
	if (this != &src)
	{
		AnimaMappedValues::operator=(src);
		_shadersNames = src._shadersNames;
	}
	return *this;
}

ptree AnimaMaterial::GetObjectTree(bool saveName) const
{
	ptree tree;
	
	if(saveName)
	{
		tree.add("AnimaMaterial.Name", GetName());
	}
	
	ptree shadersNamesTree;
	for(auto& shaderName : _shadersNames)
		shadersNamesTree.add("ShaderName", shaderName);
	
	tree.add_child("AnimaMaterial.ShaderNames", shadersNamesTree);
	tree.add_child("AnimaMaterial.MappedValues", AnimaMappedValues::GetObjectTree(false));
	
	return tree;
}

bool AnimaMaterial::ReadObject(const ptree& objectTree, bool readName)
{
	try
	{
		if(readName)
			SetName(objectTree.get<AnimaString>("AnimaMaterial.Name"));
		
		for(auto& shadersName : objectTree.get_child("AnimaMaterial.ShaderNames"))
		{
			if(shadersName.first == "ShaderName")
			{
				AnimaString shaderName = shadersName.second.get_value<AnimaString>("");
				_shadersNames.push_back(shaderName);
			}
		}
		
		ptree mappedValuesTree = objectTree.get_child("AnimaMaterial.MappedValues");
		
		return AnimaMappedValues::ReadObject(mappedValuesTree, false);
	}
	catch (boost::property_tree::ptree_bad_path& exception)
	{
		AnimaLogger::LogMessageFormat("ERROR - Error parsing material: %s", exception.what());
		return false;
	}
	catch (boost::property_tree::ptree_bad_data& exception)
	{
		AnimaLogger::LogMessageFormat("ERROR - Error parsing material: %s", exception.what());
		return false;
	}
}

void AnimaMaterial::AddShader(AnimaShader* shader)
{
	_shadersNames.push_back(shader->GetName());
}

void AnimaMaterial::AddShader(const AnimaString& shaderName)
{
	_shadersNames.push_back(shaderName);
}

AInt AnimaMaterial::GetShadersCount() const
{
	return _shadersNames.size();
}

AnimaString AnimaMaterial::GetShaderName(AInt index) const
{
	return _shadersNames[index];
}

END_ANIMA_ENGINE_NAMESPACE
