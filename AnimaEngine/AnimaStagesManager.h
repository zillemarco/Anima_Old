//
//  AnimaStagesManager.h
//  Anima
//
//  Created by Marco Zille on 14/01/15.
//
//

#ifndef __Anima__AnimaStagesManager__
#define __Anima__AnimaStagesManager__

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaStage.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaStagesManager
{
public:
	AnimaStagesManager(AnimaAllocator* allocator);
	~AnimaStagesManager();
	
	AnimaStage* CreateStage(const AnimaString& name);
	AnimaStage* CreateStage(const char* name);

	AnimaStage* GetStage(AUint index);
	AnimaStage* GetStage(const AnimaString& name);
	AnimaStage* GetStage(const char* name);

private:
	void ClearStages(bool bDeleteObjects = true, bool bResetNumber = true);
	
private:
	AnimaAllocator* _allocator;
	
	AnimaStage** _stages;
	ASizeT _stagesNumber;
	
#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, AUint, AnimaString::Hasher> _stagesMap;
#pragma warning (default: 4251) 
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaStagesManager__) */
