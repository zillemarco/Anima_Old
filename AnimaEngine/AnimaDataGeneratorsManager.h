//
//  AnimaDataGeneratorsManager.h
//  Anima
//
//  Created by Marco Zille on 14/01/15.
//
//

#ifndef __Anima__AnimaDataGeneratorsManager__
#define __Anima__AnimaDataGeneratorsManager__

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaColorGenerator.h"
#include "AnimaVectorGenerator.h"
#include "AnimaDataGenerator.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaDataGeneratorsManager
{
public:
	AnimaDataGeneratorsManager(AnimaEngine* engine);
	~AnimaDataGeneratorsManager();
	
	AnimaColorGenerator* CreateColorGenerator(const AnimaString& name);
	AnimaColorGenerator* CreateColorGenerator(const char* name);

	AnimaVectorGenerator* CreateVectorGenerator(const AnimaString& name);
	AnimaVectorGenerator* CreateVectorGenerator(const char* name);

	AnimaDataGenerator* GetGenerator(AUint index);
	AnimaDataGenerator* GetGenerator(const AnimaString& name);
	AnimaDataGenerator* GetGenerator(const char* name);

	void UpdateValues();

private:
	void ClearGenerators(bool bDeleteObjects = true, bool bResetNumber = true);
	
private:
	AnimaEngine* _engine;
	
	AnimaDataGenerator** _generators;
	ASizeT _generatorsNumber;
	
#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, AUint, AnimaString::Hasher> _generatorsMap;
#pragma warning (default: 4251) 
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaDataGeneratorsManager__) */
