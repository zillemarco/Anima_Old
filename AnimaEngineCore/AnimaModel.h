//
//  AnimaEngineModel.h
//  Anima
//
//  Created by Marco Zille on 20/11/14.
//
//

#ifndef __Anima__AnimaModel__
#define __Anima__AnimaModel__

#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaAllocators.h"
#include "AnimaMesh.h"
#include "AnimaEngine.h"

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

class ANIMA_ENGINE_CORE_EXPORT AnimaModel
{
public:
	AnimaModel(AnimaEngine* engine);
	AnimaModel(const AnimaModel& src);
	AnimaModel(AnimaModel&& src);
	~AnimaModel();
	
	AnimaModel& operator=(const AnimaModel& src);
	AnimaModel& operator=(AnimaModel&& src);
	
public:
	void SetChildren(AnimaModel* children, ASizeT n);
	void AddChild(const AnimaModel& child);
	ASizeT GetChildrenNumber();
	AnimaModel GetChild(ASizeT index);
	AnimaModel* GetPChild(ASizeT index);
	AnimaModel* GetChildren();
	void ClearChildren();
	
	void SetMeshes(AnimaMesh* meshes, ASizeT n);
	void AddMesh(const AnimaMesh& mesh);
	ASizeT GetMeshesNumber();
	AnimaMesh GetMesh(ASizeT index);
	AnimaMesh* GetPMesh(ASizeT index);
	AnimaMesh* GetMeshes();
	void ClearMeshes();
	
protected:
	AnimaModel*		_modelChildren;
	ASizeT			_modelChildrenNumber;
	
	AnimaMesh*		_modelMeshes;
	ASizeT			_modelMeshesNumber;
	
	AnimaEngine* _engine;
};

END_ANIMA_ENGINE_CORE_NAMESPACE

#endif /* defined(__Anima__AnimaModel__) */
