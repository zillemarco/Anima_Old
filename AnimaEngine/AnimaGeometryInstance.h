//
//  AnimaGeometryInstance.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaGeometryInstance__
#define __Anima__AnimaGeometryInstance__

#include "AnimaEngine.h"
#include "AnimaVertex.h"
#include "AnimaTypes.h"
#include "AnimaSceneObject.h"
#include "AnimaMaterial.h"
#include "AnimaMappedArray.h"
#include "AnimaGeometry.h"
#include "AnimaNode.h"

#include <btBulletDynamicsCommon.h>

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaGeometryInstancesManager;

class ANIMA_ENGINE_EXPORT AnimaGeometryInstance : public AnimaSceneObject
{
	DECLARE_ANIMA_CLASS(AnimaGeometryInstance);

	friend class AnimaGeometryInstancesManager;

public:
	AnimaGeometryInstance(const AnimaString& name, AnimaDataGeneratorsManager* dataGeneratorsManager, AnimaAllocator* allocator);
	AnimaGeometryInstance(const AnimaGeometryInstance& src);
	AnimaGeometryInstance(AnimaGeometryInstance&& src);
	~AnimaGeometryInstance();
	
	AnimaGeometryInstance& operator=(const AnimaGeometryInstance& src);
	AnimaGeometryInstance& operator=(AnimaGeometryInstance&& src);
	
public:
	ptree GetObjectTree(bool saveName = true) const override;
	bool ReadObject(const ptree& objectTree, AnimaScene* scene, bool readName = true) override;

public:
	void Draw(AnimaRenderer* renderer, AnimaShaderProgram* program, bool start, bool end, bool updateMaterial = true);

public:
	void SetMaterial(AnimaMaterial* material);
	AnimaMaterial* GetMaterial();	
	AnimaGeometry* GetGeometry() const;

	// Gestione dati necessari al disegno
	void AddShader(AnimaShader* shader);
	void AddShader(const AnimaString& shaderName);
	AInt GetShadersCount() const;
	AnimaString GetShaderName(AInt index) const;

	void SetShaderProgram(AnimaShaderProgram* program);
	AnimaShaderProgram* GetShaderProgram();

	void SetShaderProgramName(const AnimaString& shaderProgramName);
	AnimaString GetShaderProgramName() const;
	
	inline btCollisionShape* GetPhysCollisionShape() { return _physCollisionShape; }
	inline btMotionState* GetPhysMotionState() { return _physMotionState; }
	inline btRigidBody* GetPhysRigidBody() { return _physRigidBody; }
	
	virtual void InitializePhysicData();
	
	void SetGeometry(AnimaGeometry* geometry);

protected:
	AnimaMaterial*	_material;
	AnimaString		_materialName;

	AnimaGeometry* _geometry;
	AnimaString _geometryName;

	// Dati necessari al disegno
	AnimaArray<AnimaString> _shadersNames;

	AnimaShaderProgram* _shaderProgram;
	AnimaString _shaderProgramName;
	
	btCollisionShape* _physCollisionShape;
	btMotionState* _physMotionState;
	btRigidBody* _physRigidBody;

	AnimaArray<AInt> _indices;
	AnimaArray<AFloat> _vertices;
	btTriangleIndexVertexArray* _geometryInterface;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaGeometryInstance__) */
