//
//  AnimaMaterial.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaMaterial__
#define __Anima__AnimaMaterial__

#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaTexture.h"
#include "AnimaVertex.h"
#include "AnimaColorGenerator.h"
#include <boost\unordered_map.hpp>

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaMaterial
{
public:
	AnimaMaterial(AnimaEngine* engine);
	AnimaMaterial(const AnimaMaterial& src);
	AnimaMaterial(AnimaMaterial&& src);
	~AnimaMaterial();
	
	AnimaMaterial& operator=(const AnimaMaterial& src);
	AnimaMaterial& operator=(AnimaMaterial&& src);

	void AddTexture(AnimaString propertyName, AnimaTexture* value);
	void AddTexture(const char* propertyName, AnimaTexture* value);

	void AddColor(AnimaString propertyName, AnimaColorGenerator* value);
	void AddColor(const char* propertyName, AnimaColorGenerator* value);
	void AddColor(AnimaString propertyName, AnimaColor3f value);
	void AddColor(const char* propertyName, AnimaColor3f value);
	void AddColor(AnimaString propertyName, AFloat r, AFloat g, AFloat b);
	void AddColor(const char* propertyName, AFloat r, AFloat g, AFloat b);

	void AddFloat(AnimaString propertyName, AFloat value);
	void AddFloat(const char* propertyName, AFloat value);

	AnimaTexture* GetTexture(AnimaString propertyName);
	AnimaTexture* GetTexture(const char* propertyName);

	AnimaColor3f GetColor(AnimaString propertyName);
	AnimaColor3f GetColor(const char* propertyName);

	AFloat GetFloat(AnimaString propertyName);
	AFloat GetFloat(const char* propertyName);
	
private:
	AnimaEngine* _engine;

#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, AnimaTexture*, AnimaString::Hasher> _texturesMap;
	boost::unordered_map<AnimaString, AnimaColorGenerator*, AnimaString::Hasher> _colorsMap;
	boost::unordered_map<AnimaString, AFloat, AnimaString::Hasher> _floatsMap;
#pragma warning (default: 4251) 
	
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaMaterial__) */
