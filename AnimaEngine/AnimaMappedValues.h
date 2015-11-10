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
#include "AnimaTextureGenerator.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaMatrix.h"
#include "AnimaNamedObject.h"
#include <boost/unordered_map.hpp>

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaShaderProgram;
class AnimaScene;
class AnimaDataGeneratorsManager;

class ANIMA_ENGINE_EXPORT AnimaMappedValues : public AnimaNamedObject
{
	friend class AnimaShaderProgram;
	friend class AnimaScene;

	DECLARE_ANIMA_CLASS(AnimaMappedValues);

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
	ptree GetObjectTree(bool saveName = true) const override;
	bool ReadObject(const ptree& objectTree, AnimaScene* scene, bool readName = true) override;
	
public:
	virtual const char* GetShaderPrefix() = 0;
	
public:
	void AddTexture(const AnimaString& propertyName, AnimaTextureGenerator* value);
	void AddTexture(const AnimaString& propertyName, AnimaTexture* value);
	void SetTexture(const AnimaString& propertyName, AnimaTextureGenerator* value);
	void SetTexture(const AnimaString& propertyName, AnimaTexture* value);
	AnimaTexture* GetTexture(const AnimaString& propertyName) const;
	bool HasTexture(const AnimaString& propertyName) const;
	void CopyTextures(const AnimaMappedValues& src);
	inline boost::unordered_map<AnimaString, AnimaTextureGenerator*, AnimaStringHasher>* GetTextures() { return &_texturesMap; }
	
	void AddColor(const AnimaString& propertyName, AnimaColorGenerator* value);
	void AddColor(const AnimaString& propertyName, AnimaColor3f value);
	void AddColor(const AnimaString& propertyName, AFloat r, AFloat g, AFloat b);
	void AddColor(const AnimaString& propertyName, AnimaColor4f value);
	void AddColor(const AnimaString& propertyName, AFloat r, AFloat g, AFloat b, AFloat a);
	void SetColor(const AnimaString& propertyName, AnimaColorGenerator* value);
	void SetColor(const AnimaString& propertyName, AnimaColor3f value);
	void SetColor(const AnimaString& propertyName, AFloat r, AFloat g, AFloat b);
	void SetColor(const AnimaString& propertyName, AnimaColor4f value);
	void SetColor(const AnimaString& propertyName, AFloat r, AFloat g, AFloat b, AFloat a);
	AnimaColor3f GetColor3f(const AnimaString& propertyName) const;
	AnimaColor4f GetColor4f(const AnimaString& propertyName) const;
	bool HasColor(const AnimaString& propertyName) const;
	void CopyColors(const AnimaMappedValues& src);
	inline boost::unordered_map<AnimaString, AnimaColorGenerator*, AnimaStringHasher>* GetColors() { return &_colorsMap; }
	
	void AddVector(const AnimaString& propertyName, AnimaVectorGenerator* value);
	void AddVector(const AnimaString& propertyName, AnimaVertex2f value);
	void AddVector(const AnimaString& propertyName, AFloat x, AFloat y);
	void AddVector(const AnimaString& propertyName, AnimaVertex3f value);
	void AddVector(const AnimaString& propertyName, AFloat x, AFloat y, AFloat z);
	void AddVector(const AnimaString& propertyName, AnimaVertex4f value);
	void AddVector(const AnimaString& propertyName, AFloat x, AFloat y, AFloat z, AFloat w);
	void SetVector(const AnimaString& propertyName, AnimaVectorGenerator* value);
	void SetVector(const AnimaString& propertyName, AnimaVertex2f value);
	void SetVector(const AnimaString& propertyName, AFloat x, AFloat y);
	void SetVector(const AnimaString& propertyName, AnimaVertex3f value);
	void SetVector(const AnimaString& propertyName, AFloat x, AFloat y, AFloat z);
	void SetVector(const AnimaString& propertyName, AnimaVertex4f value);
	void SetVector(const AnimaString& propertyName, AFloat x, AFloat y, AFloat z, AFloat w);
	AnimaVertex2f GetVector2f(const AnimaString& propertyName) const;
	AnimaVertex3f GetVector3f(const AnimaString& propertyName) const;
	AnimaVertex4f GetVector4f(const AnimaString& propertyName) const;
	bool HasVector(const AnimaString& propertyName) const;
	void CopyVectors(const AnimaMappedValues& src);
	inline boost::unordered_map<AnimaString, AnimaVectorGenerator*, AnimaStringHasher>* GetVectors() { return &_vectorsMap; }
	
	void AddVectorArray(const AnimaString& propertyName, AnimaArray<AnimaVectorGenerator*>* value);
	void AddVectorArray(const AnimaString& propertyName, AnimaArray<AnimaVertex2f>* value);
	void AddVectorArray(const AnimaString& propertyName, AnimaArray<AnimaVertex3f>* value);
	void AddVectorArray(const AnimaString& propertyName, AnimaArray<AnimaVertex4f>* value);
	void SetVectorArray(const AnimaString& propertyName, AnimaArray<AnimaVectorGenerator*>* value);
	void SetVectorArray(const AnimaString& propertyName, AnimaArray<AnimaVertex2f>* value);
	void SetVectorArray(const AnimaString& propertyName, AnimaArray<AnimaVertex3f>* value);
	void SetVectorArray(const AnimaString& propertyName, AnimaArray<AnimaVertex4f>* value);
	AnimaArray<AnimaVectorGenerator*>* GetVectorArray(const AnimaString& propertyName) const;
	bool HasVectorArray(const AnimaString& propertyName) const;
	void CopyVectorsArrays(const AnimaMappedValues& src);
	inline boost::unordered_map<AnimaString, AnimaArray<AnimaVectorGenerator*>*, AnimaStringHasher>* GetVectorArrays() { return &_vectorsArraysMap; }
	
	void AddFloat(const AnimaString& propertyName, AFloat value);
	void SetFloat(const AnimaString& propertyName, AFloat value);
	AFloat GetFloat(const AnimaString& propertyName) const;
	bool HasFloat(const AnimaString& propertyName) const;
	void CopyFloats(const AnimaMappedValues& src);
	inline boost::unordered_map<AnimaString, AFloat, AnimaStringHasher>* GetFloats() { return &_floatsMap; }
	
	void AddBoolean(const AnimaString& propertyName, bool value);
	void SetBoolean(const AnimaString& propertyName, bool value);
	bool GetBoolean(const AnimaString& propertyName) const;
	bool HasBoolean(const AnimaString& propertyName) const;
	void CopyBooleans(const AnimaMappedValues& src);
	inline boost::unordered_map<AnimaString, bool, AnimaStringHasher>* GetBooleans() { return &_booleansMap; }
	
	void AddInteger(const AnimaString& propertyName, AInt value);
	void SetInteger(const AnimaString& propertyName, AInt value);
	AInt GetInteger(const AnimaString& propertyName) const;
	bool HasInteger(const AnimaString& propertyName) const;
	void CopyIntegers(const AnimaMappedValues& src);
	inline boost::unordered_map<AnimaString, AInt, AnimaStringHasher>* GetIntegers() { return &_integersMap; }
	
	void AddMatrix(const AnimaString& propertyName, const AnimaMatrix& value);
	void AddMatrix(const AnimaString& propertyName, AFloat value[16]);
	void SetMatrix(const AnimaString& propertyName, const AnimaMatrix& value);
	void SetMatrix(const AnimaString& propertyName, AFloat value[16]);
	AnimaMatrix GetMatrix(const AnimaString& propertyName) const;
	bool HasMatrix(const AnimaString& propertyName) const;
	void CopyMatrices(const AnimaMappedValues& src);
	inline boost::unordered_map<AnimaString, AnimaMatrix, AnimaStringHasher>* GetMatrices() { return &_matricesMap; }
	
	void AddMatrixArray(const AnimaString& propertyName, AnimaArray<AnimaMatrix>* value);
	void SetMatrixArray(const AnimaString& propertyName, AnimaArray<AnimaMatrix>* value);
	AnimaArray<AnimaMatrix>* GetMatrixArray(const AnimaString& propertyName) const;
	bool HasMatrixArray(const AnimaString& propertyName) const;
	void CopyMatricesArrays(const AnimaMappedValues& src);
	inline boost::unordered_map<AnimaString, AnimaArray<AnimaMatrix>*, AnimaStringHasher>* GetMatrixArrays() { return &_matricesArraysMap; }

public:
	static AnimaString ExtractName(const AnimaString& src);
	
protected:
	void ClearAllVectorsArrays();
	void ClearAllMatricesArrays();
	
protected:
	AnimaDataGeneratorsManager* _dataGeneratorManager;

	AnimaString _uniqueName;

#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, AnimaTextureGenerator*, AnimaStringHasher> _texturesMap;
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
