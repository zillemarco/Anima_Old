//
//  AnimaEngineNodesManager.h
//  Anima
//
//  Created by Marco Zille on 20/11/14.
//
//

#ifndef __Anima__AnimaGeometryInstancesManager__
#define __Anima__AnimaGeometryInstancesManager__

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaGeometry.h"
#include "AnimaGeometry.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaArray.h"
#include "AnimaMappedArray.h"
#include "AnimaGeometriesManager.h"
#include "AnimaGeometryInstance.h"
#include "AnimaNode.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaScene;

class ANIMA_ENGINE_EXPORT AnimaGeometryInstancesManager
{
public:
	AnimaGeometryInstancesManager(AnimaScene* scene, AnimaGeometriesManager* geometriesManager);
	~AnimaGeometryInstancesManager();

public:
	AnimaGeometryInstance* CreateGeometryInstance(const AnimaString& instanceName, AnimaGeometry* srcGeometry);
	AnimaGeometryInstance* CreateGeometryInstance(const AnimaString& instanceName, const AnimaString& srcGeometryName);

	AnimaArray<AnimaGeometryInstance*>* CreateInstancesFromNode(AnimaNode* srcNode);

	AInt GetGeometryInstancesCount();

	AnimaGeometryInstance* GetGeometryInstance(AInt index);
	AnimaGeometryInstance* GetGeometryInstanceFromName(const AnimaString& name);
	
	AnimaGeometryInstance* LoadGeometryInstanceFromFile(const AnimaString& filePath);
	AnimaGeometryInstance* LoadGeometryInstanceFromXml(const AnimaString& geometryXmlDefinition);
	bool LoadGeometriesInstances(const AnimaString& geometriesInstancesPath);
	
	void SaveGeometryInstanceToFile(const AnimaString& geometryInstanceName, const AnimaString& destinationPath, bool createFinalPath = false);
	void SaveGeometryInstanceToFile(AnimaGeometryInstance* geometryInstance, const AnimaString& destinationPath, bool createFinalPath = false);
	void SaveGeometriesInstances(const AnimaString& destinationPath);
	
	bool FinalizeObjectsAfterRead();

protected:
	AnimaGeometryInstance* CreateEmptyInstance(const AnimaString& instanceName);

public:
	void ClearInstances();
	void ClearLastInstancesFromNode();
			
private:
	AnimaScene*			_scene;
	AnimaGeometriesManager* _geometriesManager;

	AnimaMappedArray<AnimaGeometryInstance*> _geometryInstances;

	AnimaArray<AnimaGeometryInstance*> _lastInstancesFromNode;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaGeometryInstancesManager__) */
