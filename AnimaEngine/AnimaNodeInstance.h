//
//  AnimaNodeInstance.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaNodeInstance__
#define __Anima__AnimaNodeInstance__

#include "AnimaEngine.h"
#include "AnimaVertex.h"
#include "AnimaTypes.h"
#include "AnimaSceneObject.h"
#include "AnimaMaterial.h"
#include "AnimaMappedArray.h"
#include "AnimaGeometryInstance.h"
#include "AnimaNode.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaScene;
class AnimaNodeInstancesManager;

class ANIMA_ENGINE_EXPORT AnimaNodeInstance : public AnimaSceneObject
{
	DECLARE_ANIMA_CLASS(AnimaNodeInstance);

	friend class AnimaNodeInstancesManager;

public:
	AnimaNodeInstance(const AnimaString& name, AnimaDataGeneratorsManager* dataGeneratorsManager, AnimaAllocator* allocator);
	AnimaNodeInstance(const AnimaNodeInstance& src);
	AnimaNodeInstance(AnimaNodeInstance&& src);
	~AnimaNodeInstance();
	
	AnimaNodeInstance& operator=(const AnimaNodeInstance& src);
	AnimaNodeInstance& operator=(AnimaNodeInstance&& src);
	
public:
	ptree GetObjectTree(bool saveName = true) const override;
	bool ReadObject(const ptree& objectTree, AnimaScene* scene, bool readName = true) override;
	
protected:
	void SetGeometries(AnimaArray<AnimaGeometryInstance*>* geometries);
	void AddGeometry(AnimaGeometryInstance* geometry);
	void SetNode(AnimaNode* node);

public:
	AInt GetGeometriesCount() const;
	AnimaGeometryInstance* GetGeometry(AInt index);
	AnimaArray<AnimaGeometryInstance*>* GetGeometries() const;

	void GetAllGeometries(AnimaArray<AnimaGeometryInstance*> *geometries);

	AnimaNode* GetNode();
	
	virtual void SetIsAsset(bool isAsset) { _isAsset = isAsset; }
	virtual bool IsAsset() const { return _isAsset; }

protected:
	void UpdateChildrenTransformation() override;

protected:
	AnimaNode* _node;
	AnimaString _nodeName;
	
	bool _isAsset;

	AnimaArray<AnimaGeometryInstance*> _geometries;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaNodeInstance__) */
