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
	DECLARE_ANIMA_CLASS(AnimaMeshInstance);

	friend class AnimaMeshInstancesManager;

public:
	AnimaMeshInstance(const AnimaString& name, AnimaDataGeneratorsManager* dataGeneratorsManager, AnimaAllocator* allocator);
	AnimaMeshInstance(const AnimaMeshInstance& src);
	AnimaMeshInstance(AnimaMeshInstance&& src);
	~AnimaMeshInstance();
	
	AnimaMeshInstance& operator=(const AnimaMeshInstance& src);
	AnimaMeshInstance& operator=(AnimaMeshInstance&& src);
	
public:
	ptree GetObjectTree(bool saveName = true) const override;
	bool ReadObject(const ptree& objectTree, AnimaScene* scene, bool readName = true) override;

public:
	void Draw(AnimaRenderer* renderer, AnimaShaderProgram* program, bool start, bool end, bool updateMaterial = true);

public:
	void SetMaterial(AnimaMaterial* material);
	AnimaMaterial* GetMaterial();	
	AnimaMesh* GetMesh() const;

	// Gestione dati necessari al disegno
	void AddShader(AnimaShader* shader);
	void AddShader(const AnimaString& shaderName);
	AInt GetShadersCount() const;
	AnimaString GetShaderName(AInt index) const;

	void SetShaderProgram(AnimaShaderProgram* program);
	AnimaShaderProgram* GetShaderProgram();

	void SetShaderProgramName(const AnimaString& shaderProgramName);
	AnimaString GetShaderProgramName() const;

protected:
	const char* GetShaderPrefix() override { return "MOD"; }
	
	void SetMesh(AnimaMesh* mesh);

protected:
	AnimaMaterial*	_material;
	AnimaString		_materialName;

	AnimaMesh* _mesh;
	AnimaString _meshName;

	// Dati necessari al disegno
	AnimaArray<AnimaString> _shadersNames;

	AnimaShaderProgram* _shaderProgram;
	AnimaString _shaderProgramName;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaMeshInstance__) */
