#pragma once

#ifndef __Anima__AnimaRenderer__
#define __Anima__AnimaRenderer__

#include <stdio.h>
#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaArray.h"
#include "AnimaGeometry.h"
#include "AnimaMaterial.h"
#include "AnimaLight.h"
#include "AnimaShaderProgram.h"
#include "AnimaString.h"
#include "AnimaGBuffer.h"
#include "AnimaFrustum.h"
#include "AnimaMatrix.h"
#include "AnimaGeometryInstance.h"
#include "AnimaNode.h"
#include "AnimaNodeInstance.h"
#include "AnimaRendererDrawableGeometry.h"
#include "AnimaPhysicsDebugDrawer.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <mutex>

BEGIN_ANIMA_ENGINE_NAMESPACE

//class ANIMA_ENGINE_EXPORT AnimaPrimitiveData
//{
//public:
//	AnimaPrimitiveData(AnimaAllocator* allocator);
//	AnimaPrimitiveData(AnimaPrimitiveData& src);
//	AnimaPrimitiveData(AnimaPrimitiveData&& src);
//	~AnimaPrimitiveData();
//	
//public:
//	void SetVertices(AnimaArray<AnimaVertex3f>* vertices);
//	AnimaArray<AnimaVertex3f>* GetVertices();
//	
//	void SetIndices(AnimaArray<AUint>* indices);
//	AnimaArray<AUint>* GetIndices();
//	
//	void SetColor(const AnimaColor4f& color);
//	AnimaColor4f GetColor();
//	
//	void SetType(AUint type);
//	AUint GetType();
//	
//	void SetNodeMatrix(const AnimaMatrix& nodeMatrix);
//	AnimaMatrix GetNodeMatrix();
//	
//protected:
//	AnimaArray<AnimaVertex3f>	_vertices;
//	AnimaArray<AUint>			_indices;
//	AnimaColor4f				_color;
//	AUint						_type;
//	AnimaMatrix					_nodeMatrix;
//};

struct AnimaRendererMaterialInstances
{
	AnimaMaterial* _material;
	AnimaArray<AnimaGeometryInstance*> _instances;
};

struct AnimaRendererGeometryInstances
{
	AnimaGeometry* _geometry;
	AnimaArray<AnimaGeometryInstance*> _instances;
};

struct AnimaRendererProgramData
{
	AnimaShaderProgram* _program;
	AnimaArray<AnimaRendererMaterialInstances> _materials;	/*!< Usato se il programma NON supporta le istanze */
	AnimaArray<AnimaRendererGeometryInstances> _geometries;			/*!< Usato se il programma supporta le istanze */
};

class AnimaEngine;

typedef void(*AnimaRenderPassFunc)(AnimaRenderer* renderer);

class ANIMA_ENGINE_EXPORT AnimaRenderer
{
public:
	AnimaRenderer(AnimaEngine* engine, AnimaAllocator* allocator);
	AnimaRenderer(AnimaRenderer& src);
	AnimaRenderer(AnimaRenderer&& src);
	virtual ~AnimaRenderer();

	AnimaRenderer& operator=(const AnimaRenderer& src);
	AnimaRenderer& operator=(AnimaRenderer&& src);
	
protected:
	virtual bool InitializeShaders();

public:
	virtual void Start(AnimaScene* scene);

	void CheckPrograms(AnimaScene* scene);

	AnimaScene* GetActiveScene() { return _scene; }

	virtual void Render();
	//virtual void AddPrimitive(AnimaArray<AnimaVertex3f>* vertices, AnimaArray<AUint>* indices, AnimaColor4f color, AnimaMatrix nodeMatrix, AUint primitiveType);
	virtual void UpdateNodesVisibility();
	
public:
	virtual void InitTextureSlots();
	virtual void InitRenderingTargets(AInt screenWidth, AInt screenHeight);
	virtual void InitRenderingUtilities(AInt screenWidth, AInt screenHeight);

protected:
	virtual void Start();
	virtual void Finish();

protected:
	virtual void UpdateShadowMaps(AnimaShaderProgram* program);
	virtual void UpdateShadowMap(AnimaLight* light);
	virtual void UpdateDirectionalLightShadowMap(AnimaDirectionalLight* light);
	
	virtual void DirectionalLightsPass(AnimaArray<AnimaLight*>* directionalLights);
	virtual void PointLightsPass(AnimaArray<AnimaLight*>* pointLights);
	
	virtual void DrawGeometry(AnimaGeometry* geometry, AnimaShaderProgram* program, bool updateMaterial = true, bool forceDraw = false, AnimaFrustum* frustum = nullptr, bool useInstances = true);

	void BuildDrawableObjectsArray(AnimaArray<AnimaRendererDrawableGeometry>* drawableGeometries, AnimaCamera* camera);
	AInt FindDrawableObjecsFromProgram(AnimaArray<AnimaRendererDrawableGeometryInstances>* drawableGeometryInstances, AnimaShaderProgram* program);

	// Funzioni usate dal prepare pass
	void BuildProgramsData(AnimaArray<AnimaRendererProgramData>* programs, AnimaCamera* camera);
	AInt FindProgramData(AnimaArray<AnimaRendererProgramData>* programs, AnimaShaderProgram* program);

	AInt FindMaterialInstances(AnimaArray<AnimaRendererMaterialInstances>* materials, AnimaMaterial* material);
	AInt FindGeometryInstances(AnimaArray<AnimaRendererGeometryInstances>* geometries, AnimaGeometry* geometry);

	void SetupProgramDataStaticBuffers(AnimaArray<AnimaRendererProgramData>* programs, AnimaCamera* camera);
	void SetupProgramDataInstancedStaticBuffers(AnimaShaderProgram* program, AnimaRendererGeometryInstances* geometryInstances, AnimaCamera* camera);

	// Funzioni usate da update shadow map pass
	void BuildShadowMapGeometries(AnimaArray<AnimaRendererGeometryInstances>* geometries);

	void SetupShadowMapStaticBuffers(AnimaShaderProgram* program, AnimaArray<AnimaRendererGeometryInstances>* geometries, AnimaLight* light);
	void SetupShadowMapInstancedStaticBuffers(AnimaShaderProgram* program, AnimaRendererGeometryInstances* geometryInstances, AnimaLight* light);

protected:
	static void PreparePass(AnimaRenderer* renderer);
	static void LightPass(AnimaRenderer* renderer);
	static void CombinePass(AnimaRenderer* renderer);
	static void BloomCreationPass(AnimaRenderer* renderer);
	static void FinalPass(AnimaRenderer* renderer);
	static void DirectionalLightShadowMap(AnimaRenderer* renderer);

protected:
	void Clear();
	//void ClearPrimitives();
	
	//void DrawPrimitives(AnimaShaderProgram* program);
	//void DrawPrimitive(AnimaPrimitiveData* primitive, AnimaShaderProgram* program);
	//void CombinePrimitives(AnimaShaderProgram* program);

	void ApplyEffectFromTextureToTexture(AnimaShaderProgram* filterProgram, AnimaTexture* src, AnimaTexture* dst);
	void ApplyEffectFromTextureToGBuffer(AnimaShaderProgram* filterProgram, AnimaTexture* src, AnimaGBuffer* dst);
	void ApplyEffectFromGBufferToGBuffer(AnimaShaderProgram* filterProgram, AnimaGBuffer* src, AnimaGBuffer* dst);
	void ApplyEffectFromGBufferToTexture(AnimaShaderProgram* filterProgram, AnimaGBuffer* src, AnimaTexture* dst);

	void UpdateNodeVisibility(AnimaFrustum* frustum, AnimaGeometry* geometry, AnimaMatrix parentGeometryMatrix);

protected:
	template<class T> AnimaGeometry* CreateGeometryForLightType();

	void SetTextureSlot(AnimaString slotName, AUint value);	
	void SetGBuffer(const AnimaString& name, AnimaGBuffer* value, bool deleteExistent = true);
	
public:
	AUint GetTextureSlot(const AnimaString& slotName);
	void SetTexture(AnimaString propertyName, AnimaTexture* value, bool deleteExistent = true);
	AnimaTexture* GetTexture(AnimaString propertyName);
	AnimaGBuffer* GetGBuffer(const AnimaString& gBufferName);

	AnimaColor3f GetColor3f(AnimaString propertyName);
	AnimaColor4f GetColor4f(AnimaString propertyName);

	AnimaVertex2f GetVector2f(AnimaString propertyName);
	AnimaVertex3f GetVector3f(AnimaString propertyName);
	AnimaVertex4f GetVector4f(AnimaString propertyName);

	AFloat GetFloat(AnimaString propertyName);
	AInt GetInteger(AnimaString propertyName);
	bool GetBoolean(AnimaString propertyName);

	void SetColor(AnimaString propertyName, AnimaColor3f value);
	void SetColor(AnimaString propertyName, AFloat r, AFloat g, AFloat b);
	void SetColor(AnimaString propertyName, AnimaColor4f value);
	void SetColor(AnimaString propertyName, AFloat r, AFloat g, AFloat b, AFloat a);

	void SetVector(AnimaString propertyName, AnimaVertex2f value);
	void SetVector(AnimaString propertyName, AFloat x, AFloat y);
	void SetVector(AnimaString propertyName, AnimaVertex3f value);
	void SetVector(AnimaString propertyName, AFloat x, AFloat y, AFloat z);
	void SetVector(AnimaString propertyName, AnimaVertex4f value);
	void SetVector(AnimaString propertyName, AFloat x, AFloat y, AFloat z, AFloat w);

	void SetFloat(AnimaString propertyName, AFloat value);
	void SetBoolean(AnimaString propertyName, bool value);
	void SetInteger(AnimaString propertyName, AInt value);

	void AddRenderPassFunction(AnimaRenderPassFunc function) { _renderPassesFunction.push_back(function); }
	void InsertRenderPassFunction(AnimaRenderPassFunc function, AInt index) { _renderPassesFunction.insert(_renderPassesFunction.begin() + index, function); }
	void RemoveRenderPassFunction(AInt index) { _renderPassesFunction.erase(_renderPassesFunction.begin() + index); }

	AnimaPhysicsDebugDrawer* GetPhysicsDebugDrawer() { return _physicsDebugDrawer; }
	
	void EnablePhysicsDebugDrawing() { SetBoolean("PhysicsDebugDrawingEnabled", true); }
	void DisablePhysicsDebugDrawing() { SetBoolean("PhysicsDebugDrawingEnabled", false); }
	bool IsPhysicsDebugDrawingEnabled() { return GetBoolean("PhysicsDebugDrawingEnabled"); }
	
protected:
	AnimaAllocator* _allocator;
	AnimaScene*		_scene;
	AnimaEngine*	_engine;

	AnimaGeometry*		_filterGeometry;
	AnimaCamera*	_filterCamera;

	//AnimaArray<AnimaPrimitiveData*> _primitives;
	//AUint _vertexArrayObject;
	//AUint _verticesBufferObject;
	//AUint _indexesBufferObject;
	
	AnimaNode* _lastUpdatedNode;
	AnimaNodeInstance* _lastUpdatedNodeInstance;

	AnimaArray<AnimaRenderPassFunc> _renderPassesFunction;

	AnimaShaderProgram* _defaultShaderProgram;

	AUint _programsBufferIndex;
	
	std::mutex _renderMutex;
	
	AnimaPhysicsDebugDrawer* _physicsDebugDrawer;
	
#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, AUint, AnimaStringHasher>			_textureSlotsMap;
	boost::unordered_map<AnimaString, AnimaTexture*, AnimaStringHasher>	_texturesMap;
	boost::unordered_map<AnimaString, AnimaGBuffer*, AnimaStringHasher>	_gBuffersMap;
		
	boost::unordered_map<AnimaString, AnimaVertex2f, AnimaStringHasher> _vectors2fMap;
	boost::unordered_map<AnimaString, AnimaVertex3f, AnimaStringHasher> _vectors3fMap;
	boost::unordered_map<AnimaString, AnimaVertex4f, AnimaStringHasher> _vectors4fMap;

	boost::unordered_map<AnimaString, AFloat, AnimaStringHasher>	_floatsMap;
	boost::unordered_map<AnimaString, AInt, AnimaStringHasher>		_integersMap;
	boost::unordered_map<AnimaString, bool, AnimaStringHasher>		_booleansMap;

	boost::unordered_map<AnimaString, AnimaGeometry*, AnimaStringHasher>	_geometriesMap;
#pragma warning (default: 4251)
};

template<class T> 
AnimaGeometry* AnimaRenderer::CreateGeometryForLightType()
{
	AnimaString type = ANIMA_CLASS_NAME(T);

	auto pair = _geometriesMap.find(type);
	if (pair != _geometriesMap.end())
		return pair->second;

	AnimaString name = type + "_LIGHT_RENGEOMETRY";

	AnimaGeometry* lightGeometry = AnimaAllocatorNamespace::AllocateNew<AnimaGeometry>(*_allocator, name, _engine->GetDataGeneratorsManager(), _allocator);
	_geometriesMap[type] = lightGeometry;

	return lightGeometry;
}

END_ANIMA_ENGINE_NAMESPACE

#endif //__Anima__AnimaRenderer__