//
//  AnimaNamedObject.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaNamedObject.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaNamedObject::AnimaNamedObject(const AnimaString& name, AnimaAllocator* allocator)
	: _allocator(allocator)
{
	IMPLEMENT_ANIMA_CLASS(AnimaNamedObject);

	_name = name;
	_savingDirectory = "";
}

AnimaNamedObject::AnimaNamedObject(const AnimaNamedObject& src)
	: _allocator(src._allocator)
	, _name(src._name)
	, _className(src._className)
	, _derivedClassNames(src._derivedClassNames)
{
	_savingDirectory = "";
}

AnimaNamedObject::AnimaNamedObject(AnimaNamedObject&& src)
	: _allocator(src._allocator)
	, _name(src._name)
	, _className(src._className)
	, _derivedClassNames(src._derivedClassNames)
{
	_savingDirectory = "";
}

AnimaNamedObject::~AnimaNamedObject()
{
}

AnimaNamedObject& AnimaNamedObject::operator=(const AnimaNamedObject& src)
{
	if (this != &src)
	{
		_allocator = src._allocator;
		_name = src._name;
		_className = src._className;
		_derivedClassNames = src._derivedClassNames;
		
		_savingDirectory = "";
	}

	return *this;
}

AnimaNamedObject& AnimaNamedObject::operator=(AnimaNamedObject&& src)
{
	if (this != &src)
	{
		_allocator = src._allocator;
		_name = src._name;
		_className = src._className;
		_derivedClassNames = src._derivedClassNames;
		
		_savingDirectory = "";
	}

	return *this;
}

void AnimaNamedObject::SetName(const AnimaString& name)
{
	_name = name;
}

AnimaString AnimaNamedObject::GetName() const
{
	return _name;
}

void AnimaNamedObject::_AddDerivedClassName(AnimaString derivedClassName)
{
	_derivedClassNames.push_back(derivedClassName);
}

void AnimaNamedObject::_SetClassName(AnimaString className)
{
	_className = className;
}

bool AnimaNamedObject::IsOfClass(AnimaString className)
{
	if (_className == className)
		return true;

	for (auto val : _derivedClassNames)
	{
		if (val == className)
			return true;
	}

	return false;
}

void AnimaNamedObject::SaveObject(const AnimaString& destinationPath, const AnimaString& extension, bool createCompletePath)
{	
	namespace fs = boost::filesystem;
	
	AnimaString saveFileName = destinationPath;
	if(createCompletePath)
	{
		fs::path firstPart(destinationPath);
		fs::path secondPart(this->GetName() + extension);
		fs::path completePath = firstPart / secondPart;
		
		saveFileName = completePath.string();
	}
	
	_savingDirectory = fs::path(saveFileName).parent_path().string();
	ptree tree = GetObjectTree();
	_savingDirectory = "";
	
	boost::property_tree::write_xml(saveFileName, tree, std::locale(), boost::property_tree::xml_writer_make_settings<ptree::key_type>('\t', 1));
}

AnimaString AnimaNamedObject::GetObjectAsXML() const
{
	ptree tree = GetObjectTree();
	
	std::stringstream ss;
	boost::property_tree::write_xml(ss, tree);
	
	return ss.str();
}

ptree AnimaNamedObject::GetObjectTree(bool saveName) const
{
	ptree tree;
	
	if(saveName)
		tree.add("AnimaNamedObject.Name", GetName());
	
	return tree;
}

bool AnimaNamedObject::ReadObject(const AnimaString& sourcePath)
{
	std::ifstream fileStream(sourcePath);
	AnimaString xml((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
	fileStream.close();
	
	return ReadObject(xml);
}

bool AnimaNamedObject::ReadObjectFromXML(const AnimaString& xml)
{
	try
	{
		boost::property_tree::ptree pt;
		std::stringstream ss(xml);
		boost::property_tree::read_xml(ss, pt);
		
		return ReadObject(pt);
	}
	catch (boost::property_tree::ptree_error& exception)
	{
		AnimaLogger::LogMessageFormat("ERROR - Error parsing named object: %s", exception.what());
		return false;
	}
}

bool AnimaNamedObject::ReadObject(const ptree& objectTree, bool readName)
{
	try
	{
		if(readName)
			_name = objectTree.get<AnimaString>("AnimaNamedObject.Name");
	}
	catch (boost::property_tree::ptree_bad_path& exception)
	{
		AnimaLogger::LogMessageFormat("ERROR - Error parsing named object: %s", exception.what());
		return false;
	}
	catch (boost::property_tree::ptree_bad_data& exception)
	{
		AnimaLogger::LogMessageFormat("ERROR - Error parsing named object: %s", exception.what());
		return false;
	}
	
	return true;
}

END_ANIMA_ENGINE_NAMESPACE
