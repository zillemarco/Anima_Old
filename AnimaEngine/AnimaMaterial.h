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
#include "AnimaMappedValues.h"
#include "AnimaArray.h"
#include "AnimaShader.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaMaterial : public AnimaMappedValues
{
	DECLARE_ANIMA_CLASS(AnimaMaterial);
public:
	AnimaMaterial(AnimaAllocator* allocator, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name);
	AnimaMaterial(const AnimaMaterial& src);
	AnimaMaterial(AnimaMaterial&& src);
	~AnimaMaterial();
	
	AnimaMaterial& operator=(const AnimaMaterial& src);
	AnimaMaterial& operator=(AnimaMaterial&& src);

	void AddShader(AnimaShader* shader);
	void AddShader(const AnimaString& shaderName);
	AInt GetShadersCount() const;
	AnimaString GetShaderName(AInt index) const;
	
public:
	ptree GetObjectTree(bool saveName = true) const override;
	bool ReadObject(const ptree& objectTree, AnimaScene* scene, bool readName = true) override;
	
protected:
	AnimaArray<AnimaString> _shadersNames;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaMaterial__) */
