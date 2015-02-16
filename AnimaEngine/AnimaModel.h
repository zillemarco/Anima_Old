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
	AnimaModel(AnimaAllocator* allocator);
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
	AnimaMesh* CreateMesh();
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

	void SetParent(AnimaModel* parent);
	AnimaModel* GetParent() const;

	void ComputeBoundingBox(bool updateRecursively = false);
	AnimaVertex3f GetBoundingBoxMin() const;
	AnimaVertex3f GetBoundingBoxMax() const;

	void EnableDrawing(bool bEnable = true);
	bool DrawingEnabled() const;

protected:
	AnimaModel*		_modelChildren;
	ASizeT			_modelChildrenNumber;

	AnimaModel*		_parentModel;
	AnimaVertex3f	_boundingBoxMin;
	AnimaVertex3f	_boundingBoxMax;
	
	AnimaMesh*		_modelMeshes;
	ASizeT			_modelMeshesNumber;
	
	AnimaString		_modelName;
	AnimaString		_modelFileName;

	bool			_drawingEnabled;

	//AnimaMatrix		_transformationMatrix;
	AnimaTransformation _transformation;
	
	AnimaAllocator* _allocator;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaModel__) */
