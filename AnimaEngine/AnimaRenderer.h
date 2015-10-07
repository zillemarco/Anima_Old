#pragma once

#ifndef __Anima__AnimaRenderer__
#define __Anima__AnimaRenderer__

#include <stdio.h>
#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaArray.h"
#include "AnimaMesh.h"
#include "AnimaMaterial.h"
#include "AnimaLight.h"
#include "AnimaShaderProgram.h"
#include "AnimaString.h"
#include "AnimaGBuffer.h"
#include "AnimaFrustum.h"
#include "AnimaMatrix.h"
#include "AnimaMeshInstance.h"
#include "AnimaModel.h"
#include "AnimaModelInstance.h"
#include "AnimaRendererDrawableMesh.h"

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
//	void SetModelMatrix(const AnimaMatrix& modelMatrix);
//	AnimaMatrix GetModelMatrix();
//	
//protected:
//	AnimaArray<AnimaVertex3f>	_vertices;
//	AnimaArray<AUint>			_indices;
//	AnimaColor4f				_color;
//	AUint						_type;
//	AnimaMatrix					_modelMatrix;
//};

struct AnimaRendererMaterialInstances
{
	AnimaMaterial* _material;
	AnimaArray<AnimaMeshInstance*> _instances;
};

struct AnimaRendererMeshInstances
{
	AnimaMesh* _mesh;
	AnimaArray<AnimaMeshInstance*> _instances;
};

struct AnimaRendererProgramData
{
	AnimaShaderProgram* _program;
	AnimaArray<AnimaRendererMaterialInstances> _materials;	/*!< Usato se il programma NON supporta le istanze */
	AnimaArray<AnimaRendererMeshInstances> _meshes;			/*!< Usato se il programma supporta le istanze */
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
	//virtual void AddPrimitive(AnimaArray<AnimaVertex3f>* vertices, AnimaArray<AUint>* indices, AnimaColor4f color, AnimaMatrix modelMatrix, AUint primitiveType);
	virtual void UpdateModelsVisibility();
	
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
	
	virtual void DrawMesh(AnimaMesh* mesh, AnimaShaderProgram* program, bool updateMaterial = true, bool forceDraw = false, AnimaFrustum* frustum = nullptr, bool useInstances = true);

	void BuildDrawableObjectsArray(AnimaArray<AnimaRendererDrawableMesh>* drawableMeshes, AnimaCamera* camera);
	AInt FindDrawableObjecsFromProgram(AnimaArray<AnimaRendererDrawableMeshInstances>* drawableMeshInstances, AnimaShaderProgram* program);

	// Funzioni usate dal prepare pass
	void BuildProgramsData(AnimaArray<AnimaRendererProgramData>* programs, AnimaCamera* camera);
	AInt FindProgramData(AnimaArray<AnimaRendererProgramData>* programs, AnimaShaderProgram* program);

	AInt FindMaterialInstances(AnimaArray<AnimaRendererMaterialInstances>* materials, AnimaMaterial* material);
	AInt FindMeshInstances(AnimaArray<AnimaRendererMeshInstances>* meshes, AnimaMesh* mesh);

	void SetupProgramDataStaticBuffers(AnimaArray<AnimaRendererProgramData>* programs, AnimaCamera* camera);
	void SetupProgramDataInstancedStaticBuffers(AnimaShaderProgram* program, AnimaRendererMeshInstances* meshInstances, AnimaCamera* camera);

	// Funzioni usate da update shadow map pass
	void BuildShadowMapMeshes(AnimaArray<AnimaRendererMeshInstances>* meshes);

	void SetupShadowMapStaticBuffers(AnimaShaderProgram* program, AnimaArray<AnimaRendererMeshInstances>* meshes, AnimaLight* light);
	void SetupShadowMapInstancedStaticBuffers(AnimaShaderProgram* program, AnimaRendererMeshInstances* meshInstances, AnimaLight* light);

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

	void UpdateModelVisibility(AnimaFrustum* frustum, AnimaMesh* mesh, AnimaMatrix parentMeshMatrix);

protected:
	template<class T> AnimaMesh* CreateMeshForLightType();

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

protected:
	AnimaAllocator* _allocator;
	AnimaScene*		_scene;
	AnimaEngine*	_engine;

	AnimaMesh*		_filterMesh;
	AnimaCamera*	_filterCamera;

	//AnimaArray<AnimaPrimitiveData*> _primitives;
	//AUint _vertexArrayObject;
	//AUint _verticesBufferObject;
	//AUint _indexesBufferObject;

	AnimaModel* _lastUpdatedModel;
	AnimaModelInstance* _lastUpdatedModelInstance;

	AnimaArray<AnimaRenderPassFunc> _renderPassesFunction;

	AnimaShaderProgram* _defaultShaderProgram;

	AUint _programsBufferIndex;
	
	std::mutex _renderMutex;
	
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

	boost::unordered_map<AnimaString, AnimaMesh*, AnimaStringHasher>	_meshesMap;
#pragma warning (default: 4251)
};

template<class T> 
AnimaMesh* AnimaRenderer::CreateMeshForLightType()
{
	AnimaString type = ANIMA_CLASS_NAME(T);

	auto pair = _meshesMap.find(type);
	if (pair != _meshesMap.end())
		return pair->second;

	AnimaString name = type + "_LIGHT_RENMESH";

	AnimaMesh* lightMesh = AnimaAllocatorNamespace::AllocateNew<AnimaMesh>(*_allocator, name, _engine->GetDataGeneratorsManager(), _allocator);
	_meshesMap[type] = lightMesh;

	return lightMesh;
}

END_ANIMA_ENGINE_NAMESPACE

#endif //__Anima__AnimaRenderer__