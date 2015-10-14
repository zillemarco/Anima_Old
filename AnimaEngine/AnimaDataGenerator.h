//
//  AnimaDataGenerator.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaDataGenerator__
#define __Anima__AnimaDataGenerator__

#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaVertex.h"
#include "AnimaTexture.h"
#include "AnimaNamedObject.h"
#include "AnimaString.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaDataGenerator : public AnimaNamedObject
{
	DECLARE_ANIMA_CLASS(AnimaDataGenerator);
	
	friend class AnimaMappedValues;
	
public:
	AnimaDataGenerator(const AnimaString& name, AnimaAllocator* allocator);
	AnimaDataGenerator(const AnimaDataGenerator& src);
	AnimaDataGenerator(AnimaDataGenerator&& src);
	virtual ~AnimaDataGenerator();

	AnimaDataGenerator& operator=(const AnimaDataGenerator& src);
	AnimaDataGenerator& operator=(AnimaDataGenerator&& src);
	
public:
	ptree GetObjectTree(bool saveName = true) const override;
	bool ReadObject(const ptree& objectTree, bool readName = true) override;

public:
	virtual void UpdateValue() = 0;

	virtual void SetColor(const AnimaColor3f& color);
	virtual void SetColor(AFloat r, AFloat g, AFloat b);
	virtual void SetColor(const AnimaColor4f& color);
	virtual void SetColor(AFloat r, AFloat g, AFloat b, AFloat a);
	virtual AnimaColor3f GetColor3f();
	virtual AnimaColor4f GetColor4f();

	virtual void SetVector(const AnimaVertex2f& color);
	virtual void SetVector(AFloat x, AFloat y);
	virtual void SetVector(const AnimaVertex3f& color);
	virtual void SetVector(AFloat x, AFloat y, AFloat z);
	virtual void SetVector(const AnimaVertex4f& color);
	virtual void SetVector(AFloat x, AFloat y, AFloat z, AFloat w);
	virtual AnimaVertex2f GetVector2f();
	virtual AnimaVertex3f GetVector3f();
	virtual AnimaVertex4f GetVector4f();
	
	virtual void SetTexture(AnimaTexture* texture);
	virtual AnimaTexture* GetTexture();

	virtual void StopValueUpdate();
	virtual void StartValueUpdate();

	virtual void SetCanUpdateValue(bool canUpdateValue) { _canUpdateValue = CanUpdateValue(); }
	virtual bool CanUpdateValue() const { return _canUpdateValue; }
	
	bool IsGeneratedFromMappedValues() const { return _generatedFromMappedValues; }
	
protected:
	void SetGeneratedFromMappedValues(bool generatedFromMappedValues) { _generatedFromMappedValues = generatedFromMappedValues; }

protected:
	bool _canUpdateValue;
	bool _updatingValue;
	bool _generatedFromMappedValues;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaDataGenerator__) */
