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
#include "AnimaString.h"
#include "AnimaMatrix.h"
#include "AnimaTransformation.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaModel
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
	
	void SetModelName(const AnimaString& name);
	void SetModelName(const char* name);
	
	void SetModelFileName(const AnimaString& name);
	void SetModelFileName(const char* name);
	
	AnimaString GetAnimaModelName();
	const char* GetModelName();
	
	AnimaString GetAnimaModelFileName();
	const char* GetModelFileName();

	//AnimaMatrix GetTransformationMatrix();
	//AnimaMatrix* GetPTransformationMatrix();
	//const AFloat* GetTransformationMatrixFlt();
	//void GetTransformationMatrixFlt(AFloat* m);

	//void SetTransformationMatrix(AnimaMatrix m);
	//void SetTransformationMatrix(AFloat m[16]);

	AnimaTransformation* GetTransformation();
	AnimaTransformation GetTransformationCopy();

protected:
	AnimaModel*		_modelChildren;
	ASizeT			_modelChildrenNumber;
	
	AnimaMesh*		_modelMeshes;
	ASizeT			_modelMeshesNumber;
	
	AnimaString		_modelName;
	AnimaString		_modelFileName;

	//AnimaMatrix		_transformationMatrix;
	AnimaTransformation _transformation;
	
	AnimaEngine* _engine;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaModel__) */
