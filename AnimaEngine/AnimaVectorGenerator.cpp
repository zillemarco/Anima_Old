//
//  AnimaVectorGenerator.cpp
//  Anima
//
//  Created zy Marco Zille on 26/11/14.
//
//

#include "AnimaVectorGenerator.h"
#include "AnimaMappedValues.h"
#include "AnimaXmlTranslators.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaVectorGenerator::AnimaVectorGenerator(const AnimaString& name, AnimaAllocator* allocator)
	: AnimaDataGenerator(name, allocator)
	, _vector(0.0f, 0.0f, 0.0f, 0.0f)
{
	IMPLEMENT_ANIMA_CLASS(AnimaVectorGenerator);
}

AnimaVectorGenerator::AnimaVectorGenerator(const AnimaString& name, AnimaAllocator* allocator, const AnimaVertex2f& vector)
	: AnimaDataGenerator(name, allocator)
	, _vector(vector, 0.0f, 0.0f)
{
	IMPLEMENT_ANIMA_CLASS(AnimaVectorGenerator);
}

AnimaVectorGenerator::AnimaVectorGenerator(const AnimaString& name, AnimaAllocator* allocator, AFloat x, AFloat y)
	: AnimaDataGenerator(name, allocator)
	, _vector(x, y, 0.0f, 0.0f)
{
	IMPLEMENT_ANIMA_CLASS(AnimaVectorGenerator);
}

AnimaVectorGenerator::AnimaVectorGenerator(const AnimaString& name, AnimaAllocator* allocator, const AnimaVertex3f& vector)
	: AnimaDataGenerator(name, allocator)
	, _vector(vector, 1.0f)
{
	IMPLEMENT_ANIMA_CLASS(AnimaVectorGenerator);
}

AnimaVectorGenerator::AnimaVectorGenerator(const AnimaString& name, AnimaAllocator* allocator, AFloat x, AFloat y, AFloat z)
	: AnimaDataGenerator(name, allocator)
	, _vector(x, y, z, 1.0f)
{
	IMPLEMENT_ANIMA_CLASS(AnimaVectorGenerator);
}

AnimaVectorGenerator::AnimaVectorGenerator(const AnimaString& name, AnimaAllocator* allocator, const AnimaVertex4f& vector)
	: AnimaDataGenerator(name, allocator)
	, _vector(vector)
{
	IMPLEMENT_ANIMA_CLASS(AnimaVectorGenerator);
}

AnimaVectorGenerator::AnimaVectorGenerator(const AnimaString& name, AnimaAllocator* allocator, AFloat x, AFloat y, AFloat z, AFloat w)
	: AnimaDataGenerator(name, allocator)
	, _vector(x, y, z, w)
{
	IMPLEMENT_ANIMA_CLASS(AnimaVectorGenerator);
}

AnimaVectorGenerator::AnimaVectorGenerator(const AnimaVectorGenerator& src)
	: AnimaDataGenerator(src)
	, _vector(src._vector)
{
}

AnimaVectorGenerator::AnimaVectorGenerator(AnimaVectorGenerator&& src)
	: AnimaDataGenerator(src)
	, _vector(src._vector)
{
}

AnimaVectorGenerator::~AnimaVectorGenerator()
{
}

AnimaVectorGenerator& AnimaVectorGenerator::operator=(const AnimaVectorGenerator& src)
{
	if (this != &src)
	{
		AnimaDataGenerator::operator=(src);

		_vector = src._vector;
	}

	return *this;
}

AnimaVectorGenerator& AnimaVectorGenerator::operator=(AnimaVectorGenerator&& src)
{
	if (this != &src)
	{
		AnimaDataGenerator::operator=(src);

		_vector = src._vector;
	}

	return *this;
}

ptree AnimaVectorGenerator::GetObjectTree(bool saveName) const
{
	ptree tree;
	
	if(saveName)
	{
		if(IsGeneratedFromMappedValues())
			tree.add("AnimaVectorGenerator.Name", AnimaMappedValues::ExtractName(GetName()));
		else
			tree.add("AnimaVectorGenerator.Name", GetName());
	}
	
	tree.add("AnimaVectorGenerator.Vector", _vector);
	
	tree.add_child("AnimaVectorGenerator.DataGenerator", AnimaDataGenerator::GetObjectTree(false));
	
	return tree;
}

bool AnimaVectorGenerator::ReadObject(const ptree& objectTree, bool readName)
{
	try
	{
		if(readName)
			SetName(objectTree.get<AnimaString>("AnimaVectorGenerator.Name"));
		
		SetVector(objectTree.get<AnimaVertex4f>("AnimaVectorGenerator.Vector"));
		
		ptree dataGeneratorTree = objectTree.get_child("AnimaVectorGenerator.DataGenerator");
		
		return AnimaDataGenerator::ReadObject(dataGeneratorTree, false);
	}
	catch (boost::property_tree::ptree_bad_path& exception)
	{
		AnimaLogger::LogMessageFormat("ERROR - Error parsing color generator: %s", exception.what());
		return false;
	}
	catch (boost::property_tree::ptree_bad_data& exception)
	{
		AnimaLogger::LogMessageFormat("ERROR - Error parsing color generator: %s", exception.what());
		return false;
	}
}

void AnimaVectorGenerator::UpdateValue()
{
}

void AnimaVectorGenerator::SetVector(const AnimaVertex2f& vector)
{
	_vector.x = vector.x;
	_vector.y = vector.y;
	_vector.z = 0.0f;
	_vector.w = 0.0f;
}

void AnimaVectorGenerator::SetVector(AFloat x, AFloat y)
{
	_vector.x = x;
	_vector.y = y;
	_vector.z = 0.0f;
	_vector.w = 0.0f;
}

void AnimaVectorGenerator::SetVector(const AnimaVertex3f& vector)
{
	_vector.x = vector.x;
	_vector.y = vector.y;
	_vector.z = vector.z;
	_vector.w = 0.0f;
}

void AnimaVectorGenerator::SetVector(AFloat x, AFloat y, AFloat z)
{
	_vector.x = x;
	_vector.y = y;
	_vector.z = z;
	_vector.w = 0.0f;
}

void AnimaVectorGenerator::SetVector(const AnimaVertex4f& vector)
{
	_vector = vector;
}

void AnimaVectorGenerator::SetVector(AFloat x, AFloat y, AFloat z, AFloat w)
{
	_vector.x = x;
	_vector.y = y;
	_vector.z = z;
	_vector.w = w;
}

AnimaVertex2f AnimaVectorGenerator::GetVector2f()
{
	AnimaVertex2f c(_vector.x, _vector.y);
	return c;
}

AnimaVertex3f AnimaVectorGenerator::GetVector3f()
{
	AnimaVertex3f c(_vector.x, _vector.y, _vector.z);
	return c;
}

AnimaVertex4f AnimaVectorGenerator::GetVector4f()
{
	return _vector;
}

END_ANIMA_ENGINE_NAMESPACE
