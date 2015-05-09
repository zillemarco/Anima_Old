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
#include "AnimaMatrix.h"
#include "AnimaNamedObject.h"
#include <boost/unordered_map.hpp>

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaShaderProgram;
class AnimaScene;

class ANIMA_ENGINE_EXPORT AnimaMappedValues : public AnimaNamedObject
{
	friend class AnimaShaderProgram;
	friend class AnimaScene;
	friend class AnimaMappedValues;

protected:
	AnimaMappedValues(AnimaAllocator* allocator, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name);
	AnimaMappedValues(const AnimaMappedValues& src);
	AnimaMappedValues(AnimaMappedValues&& src);

public:
	~AnimaMappedValues();
	
	AnimaMappedValues& operator=(const AnimaMappedValues& src);
	AnimaMappedValues& operator=(AnimaMappedValues&& src);
	
	virtual void CopyData(const AnimaMappedValues& src);
	
public:
	void AddTexture(const AnimaString& propertyName, AnimaTexture* value);
	void AddTexture(const char* propertyName, AnimaTexture* value);

	void AddColor(const AnimaString& propertyName, AnimaColorGenerator* value);
	void AddColor(const char* propertyName, AnimaColorGenerator* value);
	void AddColor(const AnimaString& propertyName, AnimaColor3f value);
	void AddColor(const char* propertyName, AnimaColor3f value);
	void AddColor(const AnimaString& propertyName, AFloat r, AFloat g, AFloat b);
	void AddColor(const char* propertyName, AFloat r, AFloat g, AFloat b);
	void AddColor(const AnimaString& propertyName, AnimaColor4f value);
	void AddColor(const char* propertyName, AnimaColor4f value);
	void AddColor(const AnimaString& propertyName, AFloat r, AFloat g, AFloat b, AFloat a);
	void AddColor(const char* propertyName, AFloat r, AFloat g, AFloat b, AFloat a);

	void AddVector(const AnimaString& propertyName, AnimaVectorGenerator* value);
	void AddVector(const char* propertyName, AnimaVectorGenerator* value);
	void AddVector(const AnimaString& propertyName, AnimaVertex2f value);
	void AddVector(const char* propertyName, AnimaVertex2f value);
	void AddVector(const AnimaString& propertyName, AFloat x, AFloat y);
	void AddVector(const char* propertyName, AFloat x, AFloat y);
	void AddVector(const AnimaString& propertyName, AnimaVertex3f value);
	void AddVector(const char* propertyName, AnimaVertex3f value);
	void AddVector(const AnimaString& propertyName, AFloat x, AFloat y, AFloat z);
	void AddVector(const char* propertyName, AFloat x, AFloat y, AFloat z);
	void AddVector(const AnimaString& propertyName, AnimaVertex4f value);
	void AddVector(const char* propertyName, AnimaVertex4f value);
	void AddVector(const AnimaString& propertyName, AFloat x, AFloat y, AFloat z, AFloat w);
	void AddVector(const char* propertyName, AFloat x, AFloat y, AFloat z, AFloat w);

	void AddFloat(const AnimaString& propertyName, AFloat value);
	void AddFloat(const char* propertyName, AFloat value);

	void AddBoolean(const AnimaString& propertyName, bool value);
	void AddBoolean(const char* propertyName, bool value);

	void AddInteger(const AnimaString& propertyName, AInt value);
	void AddInteger(const char* propertyName, AInt value);

	void AddMatrix(const AnimaString& propertyName, const AnimaMatrix& value);
	void AddMatrix(const char* propertyName, const AnimaMatrix& value);
	void AddMatrix(const AnimaString& propertyName, AFloat value[16]);
	void AddMatrix(const char* propertyName, AFloat value[16]);

	void SetTexture(const AnimaString& propertyName, AnimaTexture* value);
	void SetTexture(const char* propertyName, AnimaTexture* value);

	void SetColor(const AnimaString& propertyName, AnimaColorGenerator* value);
	void SetColor(const char* propertyName, AnimaColorGenerator* value);
	void SetColor(const AnimaString& propertyName, AnimaColor3f value);
	void SetColor(const char* propertyName, AnimaColor3f value);
	void SetColor(const AnimaString& propertyName, AFloat r, AFloat g, AFloat b);
	void SetColor(const char* propertyName, AFloat r, AFloat g, AFloat b);
	void SetColor(const AnimaString& propertyName, AnimaColor4f value);
	void SetColor(const char* propertyName, AnimaColor4f value);
	void SetColor(const AnimaString& propertyName, AFloat r, AFloat g, AFloat b, AFloat a);
	void SetColor(const char* propertyName, AFloat r, AFloat g, AFloat b, AFloat a);

	void SetVector(const AnimaString& propertyName, AnimaVectorGenerator* value);
	void SetVector(const char* propertyName, AnimaVectorGenerator* value);
	void SetVector(const AnimaString& propertyName, AnimaVertex2f value);
	void SetVector(const char* propertyName, AnimaVertex2f value);
	void SetVector(const AnimaString& propertyName, AFloat x, AFloat y);
	void SetVector(const char* propertyName, AFloat x, AFloat y);
	void SetVector(const AnimaString& propertyName, AnimaVertex3f value);
	void SetVector(const char* propertyName, AnimaVertex3f value);
	void SetVector(const AnimaString& propertyName, AFloat x, AFloat y, AFloat z);
	void SetVector(const char* propertyName, AFloat x, AFloat y, AFloat z);
	void SetVector(const AnimaString& propertyName, AnimaVertex4f value);
	void SetVector(const char* propertyName, AnimaVertex4f value);
	void SetVector(const AnimaString& propertyName, AFloat x, AFloat y, AFloat z, AFloat w);
	void SetVector(const char* propertyName, AFloat x, AFloat y, AFloat z, AFloat w);

	void SetFloat(const AnimaString& propertyName, AFloat value);
	void SetFloat(const char* propertyName, AFloat value);

	void SetBoolean(const AnimaString& propertyName, bool value);
	void SetBoolean(const char* propertyName, bool value);

	void SetInteger(const AnimaString& propertyName, AInt value);
	void SetInteger(const char* propertyName, AInt value);

	void SetMatrix(const AnimaString& propertyName, const AnimaMatrix& value);
	void SetMatrix(const char* propertyName, const AnimaMatrix& value);
	void SetMatrix(const AnimaString& propertyName, AFloat value[16]);
	void SetMatrix(const char* propertyName, AFloat value[16]);

	AnimaTexture* GetTexture(const AnimaString& propertyName);
	AnimaTexture* GetTexture(const char* propertyName);

	AnimaColor3f GetColor3f(const AnimaString& propertyName);
	AnimaColor3f GetColor3f(const char* propertyName);
	AnimaColor4f GetColor4f(const AnimaString& propertyName);
	AnimaColor4f GetColor4f(const char* propertyName);

	AnimaVertex2f GetVector2f(const AnimaString& propertyName);
	AnimaVertex2f GetVector2f(const char* propertyName);
	AnimaVertex3f GetVector3f(const AnimaString& propertyName);
	AnimaVertex3f GetVector3f(const char* propertyName);
	AnimaVertex4f GetVector4f(const AnimaString& propertyName);
	AnimaVertex4f GetVector4f(const char* propertyName);

	AFloat GetFloat(const AnimaString& propertyName);
	AFloat GetFloat(const char* propertyName);

	AInt GetInteger(const AnimaString& propertyName);
	AInt GetInteger(const char* propertyName);

	bool GetBoolean(const AnimaString& propertyName);
	bool GetBoolean(const char* propertyName);

	AnimaMatrix GetMatrix(const AnimaString& propertyName);
	AnimaMatrix GetMatrix(const char* propertyName);

	bool HasTexture(const AnimaString& propertyName);
	bool HasTexture(const char* propertyName);
	
	bool HasColor(const AnimaString& propertyName);
	bool HasColor(const char* propertyName);
	
	bool HasVector(const AnimaString& propertyName);
	bool HasVector(const char* propertyName);
	
	bool HasFloat(const AnimaString& propertyName);
	bool HasFloat(const char* propertyName);
	
	bool HasInteger(const AnimaString& propertyName);
	bool HasInteger(const char* propertyName);
	
	bool HasBoolean(const AnimaString& propertyName);
	bool HasBoolean(const char* propertyName);

	bool HasMatrix(const AnimaString& propertyName);
	bool HasMatrix(const char* propertyName);

	void CopyTextures(const AnimaMappedValues& src);
	void CopyColors(const AnimaMappedValues& src);
	void CopyVectors(const AnimaMappedValues& src);
	void CopyMatrices(const AnimaMappedValues& src);
	void CopyFloats(const AnimaMappedValues& src);
	void CopyIntegers(const AnimaMappedValues& src);
	void CopyBooleans(const AnimaMappedValues& src);

protected:
	AnimaString ExtractName(const AnimaString& src) const;
	
protected:
	AnimaDataGeneratorsManager* _dataGeneratorManager;

	AnimaString _uniqueName;

#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, AnimaTexture*, AnimaString::Hasher> _texturesMap;
	boost::unordered_map<AnimaString, AnimaColorGenerator*, AnimaString::Hasher> _colorsMap;
	boost::unordered_map<AnimaString, AnimaVectorGenerator*, AnimaString::Hasher> _vectorsMap;
	boost::unordered_map<AnimaString, AnimaMatrix, AnimaString::Hasher> _matricesMap;
	boost::unordered_map<AnimaString, AFloat, AnimaString::Hasher> _floatsMap;
	boost::unordered_map<AnimaString, AInt, AnimaString::Hasher> _integersMap;
	boost::unordered_map<AnimaString, bool, AnimaString::Hasher> _booleansMap;
#pragma warning (default: 4251) 
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaMappedValues__) */
