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
	virtual const char* GetShaderPrefix() = 0;
	
public:
	void AddTexture(const AnimaString& propertyName, AnimaTexture* value);
	
	void AddColor(const AnimaString& propertyName, AnimaColorGenerator* value);
	void AddColor(const AnimaString& propertyName, AnimaColor3f value);
	void AddColor(const AnimaString& propertyName, AFloat r, AFloat g, AFloat b);
	void AddColor(const AnimaString& propertyName, AnimaColor4f value);
	void AddColor(const AnimaString& propertyName, AFloat r, AFloat g, AFloat b, AFloat a);
	
	void AddVector(const AnimaString& propertyName, AnimaVectorGenerator* value);
	void AddVector(const AnimaString& propertyName, AnimaVertex2f value);
	void AddVector(const AnimaString& propertyName, AFloat x, AFloat y);
	void AddVector(const AnimaString& propertyName, AnimaVertex3f value);
	void AddVector(const AnimaString& propertyName, AFloat x, AFloat y, AFloat z);
	void AddVector(const AnimaString& propertyName, AnimaVertex4f value);
	void AddVector(const AnimaString& propertyName, AFloat x, AFloat y, AFloat z, AFloat w);
	
	void AddVectorArray(const AnimaString& propertyName, AnimaArray<AnimaVectorGenerator*>* value);
	void AddVectorArray(const AnimaString& propertyName, AnimaArray<AnimaVertex2f>* value);
	void AddVectorArray(const AnimaString& propertyName, AnimaArray<AnimaVertex3f>* value);
	void AddVectorArray(const AnimaString& propertyName, AnimaArray<AnimaVertex4f>* value);
	
	void AddFloat(const AnimaString& propertyName, AFloat value);
	
	void AddBoolean(const AnimaString& propertyName, bool value);
	
	void AddInteger(const AnimaString& propertyName, AInt value);
	
	void AddMatrix(const AnimaString& propertyName, const AnimaMatrix& value);
	void AddMatrix(const AnimaString& propertyName, AFloat value[16]);
	
	void AddMatrixArray(const AnimaString& propertyName, AnimaArray<AnimaMatrix>* value);
	
	void SetTexture(const AnimaString& propertyName, AnimaTexture* value);
	
	void SetColor(const AnimaString& propertyName, AnimaColorGenerator* value);
	void SetColor(const AnimaString& propertyName, AnimaColor3f value);
	void SetColor(const AnimaString& propertyName, AFloat r, AFloat g, AFloat b);
	void SetColor(const AnimaString& propertyName, AnimaColor4f value);
	void SetColor(const AnimaString& propertyName, AFloat r, AFloat g, AFloat b, AFloat a);
	
	void SetVector(const AnimaString& propertyName, AnimaVectorGenerator* value);
	void SetVector(const AnimaString& propertyName, AnimaVertex2f value);
	void SetVector(const AnimaString& propertyName, AFloat x, AFloat y);
	void SetVector(const AnimaString& propertyName, AnimaVertex3f value);
	void SetVector(const AnimaString& propertyName, AFloat x, AFloat y, AFloat z);
	void SetVector(const AnimaString& propertyName, AnimaVertex4f value);
	void SetVector(const AnimaString& propertyName, AFloat x, AFloat y, AFloat z, AFloat w);
	
	void SetVectorArray(const AnimaString& propertyName, AnimaArray<AnimaVectorGenerator*>* value);
	void SetVectorArray(const AnimaString& propertyName, AnimaArray<AnimaVertex2f>* value);
	void SetVectorArray(const AnimaString& propertyName, AnimaArray<AnimaVertex3f>* value);
	void SetVectorArray(const AnimaString& propertyName, AnimaArray<AnimaVertex4f>* value);
	
	void SetFloat(const AnimaString& propertyName, AFloat value);
	
	void SetBoolean(const AnimaString& propertyName, bool value);
	
	void SetInteger(const AnimaString& propertyName, AInt value);
	
	void SetMatrix(const AnimaString& propertyName, const AnimaMatrix& value);
	void SetMatrix(const AnimaString& propertyName, AFloat value[16]);
	
	void SetMatrixArray(const AnimaString& propertyName, AnimaArray<AnimaMatrix>* value);
	
	AnimaTexture* GetTexture(const AnimaString& propertyName);
	
	AnimaColor3f GetColor3f(const AnimaString& propertyName);
	AnimaColor4f GetColor4f(const AnimaString& propertyName);
	
	AnimaVertex2f GetVector2f(const AnimaString& propertyName);
	AnimaVertex3f GetVector3f(const AnimaString& propertyName);
	AnimaVertex4f GetVector4f(const AnimaString& propertyName);
	
	AnimaArray<AnimaVectorGenerator*>* GetVectorArray(const AnimaString& propertyName);
	
	AFloat GetFloat(const AnimaString& propertyName);
	
	AInt GetInteger(const AnimaString& propertyName);
	
	bool GetBoolean(const AnimaString& propertyName);
	
	AnimaMatrix GetMatrix(const AnimaString& propertyName);
	
	AnimaArray<AnimaMatrix>* GetMatrixArray(const AnimaString& propertyName);
	
	bool HasTexture(const AnimaString& propertyName);
	
	bool HasColor(const AnimaString& propertyName);
	
	bool HasVector(const AnimaString& propertyName);
	
	bool HasVectorArray(const AnimaString& propertyName);
	
	bool HasFloat(const AnimaString& propertyName);
	
	bool HasInteger(const AnimaString& propertyName);
	
	bool HasBoolean(const AnimaString& propertyName);
	
	bool HasMatrix(const AnimaString& propertyName);
	
	bool HasMatrixArray(const AnimaString& propertyName);
	
	void CopyTextures(const AnimaMappedValues& src);
	void CopyColors(const AnimaMappedValues& src);
	void CopyVectors(const AnimaMappedValues& src);
	void CopyVectorsArrays(const AnimaMappedValues& src);
	void CopyMatrices(const AnimaMappedValues& src);
	void CopyMatricesArrays(const AnimaMappedValues& src);
	void CopyFloats(const AnimaMappedValues& src);
	void CopyIntegers(const AnimaMappedValues& src);
	void CopyBooleans(const AnimaMappedValues& src);

protected:
	AnimaString ExtractName(const AnimaString& src) const;
	void ClearAllVectorsArrays();
	void ClearAllMatricesArrays();
	
protected:
	AnimaDataGeneratorsManager* _dataGeneratorManager;

	AnimaString _uniqueName;

#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, AnimaTexture*, AnimaStringHasher> _texturesMap;
	boost::unordered_map<AnimaString, AnimaColorGenerator*, AnimaStringHasher> _colorsMap;
	boost::unordered_map<AnimaString, AnimaVectorGenerator*, AnimaStringHasher> _vectorsMap;
	boost::unordered_map<AnimaString, AnimaMatrix, AnimaStringHasher> _matricesMap;
	boost::unordered_map<AnimaString, AFloat, AnimaStringHasher> _floatsMap;
	boost::unordered_map<AnimaString, AInt, AnimaStringHasher> _integersMap;
	boost::unordered_map<AnimaString, bool, AnimaStringHasher> _booleansMap;
	boost::unordered_map<AnimaString, AnimaArray<AnimaVectorGenerator*>*, AnimaStringHasher> _vectorsArraysMap;
	boost::unordered_map<AnimaString, AnimaArray<AnimaMatrix>*, AnimaStringHasher> _matricesArraysMap;
#pragma warning (default: 4251) 
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaMappedValues__) */
