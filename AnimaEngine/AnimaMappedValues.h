//
//  AnimaMappedValues.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaMappedValues__
#define __Anima__AnimaMappedValues__

#include "AnimaEngine.h"
#include "AnimaVertex.h"
#include "AnimaTexture.h"
#include "AnimaString.h"
#include "AnimaColorGenerator.h"
#include "AnimaVectorGenerator.h"
#include "AnimaDataGeneratorsManager.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include <boost/unordered_map.hpp>

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaMappedValues
{
protected:
	AnimaMappedValues(AnimaAllocator* allocator, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name);
	AnimaMappedValues(const AnimaMappedValues& src);
	AnimaMappedValues(AnimaMappedValues&& src);

public:
	~AnimaMappedValues();
	
	AnimaMappedValues& operator=(const AnimaMappedValues& src);
	AnimaMappedValues& operator=(AnimaMappedValues&& src);
	
public:
	void AddTexture(AnimaString propertyName, AnimaTexture* value);
	void AddTexture(const char* propertyName, AnimaTexture* value);

	void AddColor(AnimaString propertyName, AnimaColorGenerator* value);
	void AddColor(const char* propertyName, AnimaColorGenerator* value);
	void AddColor(AnimaString propertyName, AnimaColor3f value);
	void AddColor(const char* propertyName, AnimaColor3f value);
	void AddColor(AnimaString propertyName, AFloat r, AFloat g, AFloat b);
	void AddColor(const char* propertyName, AFloat r, AFloat g, AFloat b);
	void AddColor(AnimaString propertyName, AnimaColor4f value);
	void AddColor(const char* propertyName, AnimaColor4f value);
	void AddColor(AnimaString propertyName, AFloat r, AFloat g, AFloat b, AFloat a);
	void AddColor(const char* propertyName, AFloat r, AFloat g, AFloat b, AFloat a);

	void AddVector(AnimaString propertyName, AnimaVectorGenerator* value);
	void AddVector(const char* propertyName, AnimaVectorGenerator* value);
	void AddVector(AnimaString propertyName, AnimaVertex2f value);
	void AddVector(const char* propertyName, AnimaVertex2f value);
	void AddVector(AnimaString propertyName, AFloat x, AFloat y);
	void AddVector(const char* propertyName, AFloat x, AFloat y);
	void AddVector(AnimaString propertyName, AnimaVertex3f value);
	void AddVector(const char* propertyName, AnimaVertex3f value);
	void AddVector(AnimaString propertyName, AFloat x, AFloat y, AFloat z);
	void AddVector(const char* propertyName, AFloat x, AFloat y, AFloat z);
	void AddVector(AnimaString propertyName, AnimaVertex4f value);
	void AddVector(const char* propertyName, AnimaVertex4f value);
	void AddVector(AnimaString propertyName, AFloat x, AFloat y, AFloat z, AFloat w);
	void AddVector(const char* propertyName, AFloat x, AFloat y, AFloat z, AFloat w);

	void AddFloat(AnimaString propertyName, AFloat value);
	void AddFloat(const char* propertyName, AFloat value);

	void AddBoolean(AnimaString propertyName, bool value);
	void AddBoolean(const char* propertyName, bool value);

	void AddInteger(AnimaString propertyName, AInt value);
	void AddInteger(const char* propertyName, AInt value);

	void SetTexture(AnimaString propertyName, AnimaTexture* value);
	void SetTexture(const char* propertyName, AnimaTexture* value);

	void SetColor(AnimaString propertyName, AnimaColorGenerator* value);
	void SetColor(const char* propertyName, AnimaColorGenerator* value);
	void SetColor(AnimaString propertyName, AnimaColor3f value);
	void SetColor(const char* propertyName, AnimaColor3f value);
	void SetColor(AnimaString propertyName, AFloat r, AFloat g, AFloat b);
	void SetColor(const char* propertyName, AFloat r, AFloat g, AFloat b);
	void SetColor(AnimaString propertyName, AnimaColor4f value);
	void SetColor(const char* propertyName, AnimaColor4f value);
	void SetColor(AnimaString propertyName, AFloat r, AFloat g, AFloat b, AFloat a);
	void SetColor(const char* propertyName, AFloat r, AFloat g, AFloat b, AFloat a);

	void SetVector(AnimaString propertyName, AnimaVectorGenerator* value);
	void SetVector(const char* propertyName, AnimaVectorGenerator* value);
	void SetVector(AnimaString propertyName, AnimaVertex2f value);
	void SetVector(const char* propertyName, AnimaVertex2f value);
	void SetVector(AnimaString propertyName, AFloat x, AFloat y);
	void SetVector(const char* propertyName, AFloat x, AFloat y);
	void SetVector(AnimaString propertyName, AnimaVertex3f value);
	void SetVector(const char* propertyName, AnimaVertex3f value);
	void SetVector(AnimaString propertyName, AFloat x, AFloat y, AFloat z);
	void SetVector(const char* propertyName, AFloat x, AFloat y, AFloat z);
	void SetVector(AnimaString propertyName, AnimaVertex4f value);
	void SetVector(const char* propertyName, AnimaVertex4f value);
	void SetVector(AnimaString propertyName, AFloat x, AFloat y, AFloat z, AFloat w);
	void SetVector(const char* propertyName, AFloat x, AFloat y, AFloat z, AFloat w);

	void SetFloat(AnimaString propertyName, AFloat value);
	void SetFloat(const char* propertyName, AFloat value);

	void SetBoolean(AnimaString propertyName, bool value);
	void SetBoolean(const char* propertyName, bool value);

	void SetInteger(AnimaString propertyName, AInt value);
	void SetInteger(const char* propertyName, AInt value);

	AnimaTexture* GetTexture(AnimaString propertyName);
	AnimaTexture* GetTexture(const char* propertyName);

	AnimaColor3f GetColor3f(AnimaString propertyName);
	AnimaColor3f GetColor3f(const char* propertyName);
	AnimaColor4f GetColor4f(AnimaString propertyName);
	AnimaColor4f GetColor4f(const char* propertyName);

	AnimaVertex2f GetVector2f(AnimaString propertyName);
	AnimaVertex2f GetVector2f(const char* propertyName);
	AnimaVertex3f GetVector3f(AnimaString propertyName);
	AnimaVertex3f GetVector3f(const char* propertyName);
	AnimaVertex4f GetVector4f(AnimaString propertyName);
	AnimaVertex4f GetVector4f(const char* propertyName);

	AFloat GetFloat(AnimaString propertyName);
	AFloat GetFloat(const char* propertyName);

	AInt GetInteger(AnimaString propertyName);
	AInt GetInteger(const char* propertyName);

	bool GetBoolean(AnimaString propertyName);
	bool GetBoolean(const char* propertyName);
	
protected:
	AnimaAllocator* _allocator;
	AnimaDataGeneratorsManager* _dataGeneratorManager;
	AnimaString _name;

#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, AnimaTexture*, AnimaString::Hasher> _texturesMap;
	boost::unordered_map<AnimaString, AnimaColorGenerator*, AnimaString::Hasher> _colorsMap;
	boost::unordered_map<AnimaString, AnimaVectorGenerator*, AnimaString::Hasher> _vectorsMap;
	boost::unordered_map<AnimaString, AFloat, AnimaString::Hasher> _floatsMap;
	boost::unordered_map<AnimaString, AInt, AnimaString::Hasher> _integersMap;
	boost::unordered_map<AnimaString, bool, AnimaString::Hasher> _booleansMap;
#pragma warning (default: 4251) 
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaMappedValues__) */
