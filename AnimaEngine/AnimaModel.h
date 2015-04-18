//
//  AnimaModel.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaModel__
#define __Anima__AnimaModel__

#include "AnimaEngine.h"
#include "AnimaVertex.h"
#include "AnimaTypes.h"
#include "AnimaSceneObject.h"
#include "AnimaMaterial.h"
#include "AnimaMappedArray.h"
#include "AnimaMesh.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaModel : public AnimaSceneObject
{
public:
	AnimaModel(const AnimaString& name, AnimaDataGeneratorsManager* dataGeneratorsManager, AnimaAllocator* allocator);
	AnimaModel(const AnimaModel& src);
	AnimaModel(AnimaModel&& src);
	~AnimaModel();
	
	AnimaModel& operator=(const AnimaModel& src);
	AnimaModel& operator=(AnimaModel&& src);

public:
	void SetMaterial(AnimaMaterial* material);
	AnimaMaterial* GetMaterial();

	AInt AddMesh(AnimaMesh* mesh);
	AnimaMesh* GetMesh(AInt index);
	AnimaMesh* GetMeshFromName(const AnimaString& name);
	AnimaMesh* GetMeshFromName(const char* name);

	void SetOriginFileName(const AnimaString& fileName);
	void SetOriginFileName(const char* fileName);
	AnimaString GetAnimaOriginFileName() const;
	const char* GetOriginFileName() const;

protected:
	const char* GetShaderPrefix() { return ""; }

protected:
	AnimaMaterial*	_material;
	AnimaString		_materialName;
	AnimaString		_originFileName;

	AnimaMappedArray<AnimaMesh*> _meshes;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaModel__) */
