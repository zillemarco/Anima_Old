//
//  AnimaTextureGenerator.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaTextureGenerator.h"
#include "AnimaMappedValues.h"
#include "AnimaXmlTranslators.h"
#include "AnimaTexturesManager.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaTextureGenerator::AnimaTextureGenerator(const AnimaString& name, AnimaAllocator* allocator, AnimaTexturesManager* texturesManager)
	: AnimaDataGenerator(name, allocator)
	, _texture(nullptr)
	, _texturesManager(texturesManager)
{
	IMPLEMENT_ANIMA_CLASS(AnimaTextureGenerator);
}

AnimaTextureGenerator::AnimaTextureGenerator(const AnimaString& name, AnimaAllocator* allocator, AnimaTexture* texture, AnimaTexturesManager* texturesManager)
	: AnimaDataGenerator(name, allocator)
	, _texture(texture)
	, _texturesManager(texturesManager)
{
	IMPLEMENT_ANIMA_CLASS(AnimaTextureGenerator);
}

AnimaTextureGenerator::AnimaTextureGenerator(const AnimaTextureGenerator& src)
	: AnimaDataGenerator(src)
	, _texture(src._texture)
	, _texturesManager(src._texturesManager)
{
}

AnimaTextureGenerator::AnimaTextureGenerator(AnimaTextureGenerator&& src)
	: AnimaDataGenerator(src)
	, _texture(src._texture)
	, _texturesManager(src._texturesManager)
{
}

AnimaTextureGenerator::~AnimaTextureGenerator()
{
}

AnimaTextureGenerator& AnimaTextureGenerator::operator=(const AnimaTextureGenerator& src)
{
	if (this != &src)
	{
		AnimaDataGenerator::operator=(src);

		_texture = src._texture;
		_texturesManager = src._texturesManager;
	}

	return *this;
}

AnimaTextureGenerator& AnimaTextureGenerator::operator=(AnimaTextureGenerator&& src)
{
	if (this != &src)
	{
		AnimaDataGenerator::operator=(src);

		_texture = src._texture;
		_texturesManager = src._texturesManager;
	}

	return *this;
}

ptree AnimaTextureGenerator::GetObjectTree(bool saveName) const
{
	ptree tree;
	
	if(saveName)
	{
		if(IsGeneratedFromMappedValues())
			tree.add("AnimaTextureGenerator.Name", AnimaMappedValues::ExtractName(GetName()));
		else
			tree.add("AnimaTextureGenerator.Name", GetName());
	}
	
	tree.add("AnimaTextureGenerator.TextureName", _texture->GetName());
	
	tree.add_child("AnimaTextureGenerator.DataGenerator", AnimaDataGenerator::GetObjectTree(false));
	
	return tree;
}

bool AnimaTextureGenerator::ReadObject(const ptree& objectTree, bool readName)
{
	try
	{
		if(readName)
			SetName(objectTree.get<AnimaString>("AnimaTextureGenerator.Name"));
		
		AnimaString textureName = objectTree.get<AnimaString>("AnimaTextureGenerator.TextureName", "");
		if(!textureName.empty() && _texturesManager != nullptr)
			SetTexture(_texturesManager->GetTextureFromName(textureName));
		
		ptree dataGeneratorTree = objectTree.get_child("AnimaTextureGenerator.DataGenerator");
		
		return AnimaDataGenerator::ReadObject(dataGeneratorTree, false);
	}
	catch (boost::property_tree::ptree_bad_path& exception)
	{
		AnimaLogger::LogMessageFormat("ERROR - Error parsing texture generator: %s", exception.what());
		return false;
	}
	catch (boost::property_tree::ptree_bad_data& exception)
	{
		AnimaLogger::LogMessageFormat("ERROR - Error parsing texture generator: %s", exception.what());
		return false;
	}
}

void AnimaTextureGenerator::UpdateValue()
{
}

void AnimaTextureGenerator::SetTexture(AnimaTexture* texture)
{
	_texture = texture;
}

AnimaTexture* AnimaTextureGenerator::GetTexture()
{
	return _texture;
}

END_ANIMA_ENGINE_NAMESPACE
