//
//  AnimaMeshInstance.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaMeshInstance__
#define __Anima__AnimaMeshInstance__

#include "AnimaEngine.h"
#include "AnimaVertex.h"
#include "AnimaTypes.h"
#include "AnimaSceneObject.h"
#include "AnimaMaterial.h"
#include "AnimaMappedArray.h"
#include "AnimaMesh.h"
#include "AnimaModel.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaMeshInstancesManager;

class ANIMA_ENGINE_EXPORT AnimaMeshInstance : public AnimaSceneObject
{
	friend class AnimaMeshInstancesManager;

public:
	AnimaMeshInstance(const AnimaString& name, AnimaDataGeneratorsManager* dataGeneratorsManager, AnimaAllocator* allocator);
	AnimaMeshInstance(const AnimaMeshInstance& src);
	AnimaMeshInstance(AnimaMeshInstance&& src);
	~AnimaMeshInstance();
	
	AnimaMeshInstance& operator=(const AnimaMeshInstance& src);
	AnimaMeshInstance& operator=(AnimaMeshInstance&& src);
	
public:
	void SetMaterial(AnimaMaterial* material);
	AnimaMaterial* GetMaterial();	
	AnimaMesh* GetMesh() const;

protected:
	const char* GetShaderPrefix() { return ""; }
	
	void SetMesh(AnimaMesh* mesh);

protected:
	AnimaMaterial*	_material;
	AnimaString		_materialName;

	AnimaMesh* _mesh;
	AnimaString _meshName;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaMeshInstance__) */
