#pragma once

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaNamedObject.h"
#include "AnimaShaderData.h"
#include "AnimaShaderGroupData.h"
#include "AnimaMappedArray.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaShaderInclude : public AnimaNamedObject
{
	DECLARE_ANIMA_CLASS(AnimaShaderInclude);

public:
	AnimaShaderInclude(const AnimaString& name, AnimaAllocator* allocator);
	AnimaShaderInclude(const AnimaString& name, AnimaAllocator* allocator, AnimaString shaderText);
	AnimaShaderInclude(const AnimaShaderInclude& src);
	AnimaShaderInclude(AnimaShaderInclude&& src);
	~AnimaShaderInclude();
	
	AnimaShaderInclude& operator=(const AnimaShaderInclude& src);
	AnimaShaderInclude& operator=(AnimaShaderInclude&& src);
	
public:	
	void SetText(AnimaString text);
	AnimaString GetText();

	void AddShaderData(const AnimaShaderData& data);
	AInt GetShaderDataCount() const;
	AnimaShaderData* GetShaderData(const AInt& index);
	AnimaShaderData* GetShaderData(const AnimaString& name);

	void AddShaderStaticGroupData(const AnimaShaderGroupData& groupData);
	AInt GetShaderStaticGroupDataCount() const;
	AnimaShaderGroupData* GetShaderStaticGroupData(const AInt& index);
	AnimaShaderGroupData* GetShaderStaticGroupData(const AnimaString& name);

	void AddShaderDynamicGroupData(const AnimaShaderGroupData& groupData);
	AInt GetShaderDynamicGroupDataCount() const;
	AnimaShaderGroupData* GetShaderDynamicGroupData(const AInt& index);
	AnimaShaderGroupData* GetShaderDynamicGroupData(const AnimaString& name);

private:
	AnimaString	_text;
	AnimaMappedArray<AnimaShaderData> _data;
	AnimaMappedArray<AnimaShaderGroupData> _staticGroupData;
	AnimaMappedArray<AnimaShaderGroupData> _dynamicGroupData;
};

END_ANIMA_ENGINE_NAMESPACE