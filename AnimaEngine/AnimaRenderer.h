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

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaPrimitiveData
{
public:
	AnimaPrimitiveData(AnimaAllocator* allocator);
	AnimaPrimitiveData(AnimaPrimitiveData& src);
	AnimaPrimitiveData(AnimaPrimitiveData&& src);
	~AnimaPrimitiveData();
	
public:
	void SetVertices(AnimaArray<AnimaVertex3f>* vertices);
	AnimaArray<AnimaVertex3f>* GetVertices();
	
	void SetIndices(AnimaArray<AUint>* indices);
	AnimaArray<AUint>* GetIndices();
	
	void SetColor(const AnimaColor4f& color);
	AnimaColor4f GetColor();
	
	void SetType(AUint type);
	AUint GetType();
	
	void SetModelMatrix(const AnimaMatrix& modelMatrix);
	AnimaMatrix GetModelMatrix();
	
protected:
	AnimaArray<AnimaVertex3f>	_vertices;
	AnimaArray<AUint>			_indices;
	AnimaColor4f				_color;
	AUint						_type;
	AnimaMatrix					_modelMatrix;
};

class AnimaEngine;

class ANIMA_ENGINE_EXPORT AnimaRenderer
{
public:
	AnimaRenderer(AnimaEngine* engine, AnimaAllocator* allocator);
	AnimaRenderer(AnimaRenderer& src);
	AnimaRenderer(AnimaRenderer&& src);
	virtual ~AnimaRenderer();

	AnimaRenderer& operator=(const AnimaRenderer& src);
	AnimaRenderer& operator=(AnimaRenderer&& src);
	
public:
	virtual void Start(AnimaScene* scene);

	AnimaScene* GetActiveScene() { return _scene; }

	virtual void DrawAll();
	virtual void DrawMesh(AnimaMesh* mesh);
	virtual void DrawMesh(AnimaMeshInstance* instance);
	virtual void DrawModel(AnimaModel* model);
	virtual void DrawModel(AnimaModelInstance* instance);
	
	virtual void AddPrimitive(AnimaArray<AnimaVertex3f>* vertices, AnimaArray<AUint>* indices, AnimaColor4f color, AnimaMatrix modelMatrix, AUint primitiveType);

	virtual void UpdateModelsVisibility();
	
public:
	virtual void InitTextureSlots();
	virtual void InitRenderingTargets(AInt screenWidth, AInt screenHeight);
	virtual void InitRenderingUtilities(AInt screenWidth, AInt screenHeight);

protected:
	virtual void Start();
	virtual void Finish();

protected:
	virtual void PreparePass(AnimaShaderProgram* program);
	virtual void PreparePass(AnimaShaderProgram* program, AnimaMesh* mesh);
	virtual void PreparePass(AnimaShaderProgram* program, AnimaMeshInstance* instance);
	virtual void PreparePass(AnimaShaderProgram* program, AnimaModel* model);
	virtual void PreparePass(AnimaShaderProgram* program, AnimaModelInstance* instance);

	virtual void UpdateShadowMaps(AnimaShaderProgram* program);
	virtual void UpdateShadowMaps(AnimaShaderProgram* program, AnimaMesh* mesh);
	virtual void UpdateShadowMaps(AnimaShaderProgram* program, AnimaMeshInstance* instance);
	virtual void UpdateShadowMaps(AnimaShaderProgram* program, AnimaModel* model);
	virtual void UpdateShadowMaps(AnimaShaderProgram* program, AnimaModelInstance* instance);

	virtual void LightPass(AnimaArray<AnimaLight*>* lights);
	virtual void CombinePass(AnimaShaderProgram* program);

	virtual void DrawMesh(AnimaMesh* mesh, AnimaShaderProgram* program, bool updateMaterial = true, bool forceDraw = false, AnimaFrustum* frustum = nullptr, bool useInstances = true);
	virtual void DrawMesh(AnimaMeshInstance* instance, AnimaShaderProgram* program, bool updateMaterial = true, bool forceDraw = false, AnimaFrustum* frustum = nullptr);
	virtual void DrawModel(AnimaModel* model, AnimaShaderProgram* program, bool updateMaterial = true, bool forceDraw = false, AnimaFrustum* frustum = nullptr, bool useInstances = true);
	virtual void DrawModel(AnimaModelInstance* instance, AnimaShaderProgram* program, bool updateMaterial = true, bool forceDraw = false, AnimaFrustum* frustum = nullptr);

protected:
	void Clear();
	void ClearPrimitives();
	
	void DrawPrimitives(AnimaShaderProgram* program);
	void DrawPrimitive(AnimaPrimitiveData* primitive, AnimaShaderProgram* program);
	void CombinePrimitives(AnimaShaderProgram* program);

	void ApplyEffectFromTextureToTexture(AnimaShaderProgram* filterProgram, AnimaTexture* src, AnimaTexture* dst);
	void ApplyEffectFromTextureToGBuffer(AnimaShaderProgram* filterProgram, AnimaTexture* src, AnimaGBuffer* dst);
	void ApplyEffectFromGBufferToGBuffer(AnimaShaderProgram* filterProgram, AnimaGBuffer* src, AnimaGBuffer* dst);
	void ApplyEffectFromGBufferToTexture(AnimaShaderProgram* filterProgram, AnimaGBuffer* src, AnimaTexture* dst);

	void UpdateModelVisibility(AnimaFrustum* frustum, AnimaMesh* mesh, AnimaMatrix parentMeshMatrix);

protected:
	template<class T> AnimaMesh* CreateMeshForLightType();

	void SetTextureSlot(AnimaString slotName, AUint value);
	void SetTextureSlot(const char* slotName, AUint value);
	
	void SetTexture(AnimaString propertyName, AnimaTexture* value, bool deleteExistent = true);
	void SetTexture(const char* propertyName, AnimaTexture* value, bool deleteExistent = true);
	
	void SetGBuffer(const AnimaString& name, AnimaGBuffer* value, bool deleteExistent = true);
	void SetGBuffer(const char* name, AnimaGBuffer* value, bool deleteExistent = true);

	void SetColor(AnimaString propertyName, AnimaColor3f value);
	void SetColor(const char* propertyName, AnimaColor3f value);
	void SetColor(AnimaString propertyName, AFloat r, AFloat g, AFloat b);
	void SetColor(const char* propertyName, AFloat r, AFloat g, AFloat b);
	void SetColor(AnimaString propertyName, AnimaColor4f value);
	void SetColor(const char* propertyName, AnimaColor4f value);
	void SetColor(AnimaString propertyName, AFloat r, AFloat g, AFloat b, AFloat a);
	void SetColor(const char* propertyName, AFloat r, AFloat g, AFloat b, AFloat a);

	void SetVector(AnimaString propertyName, AnimaVertex2f value);
	void SetVector(const char* propertyName, AnimaVertex2f value);
	void SetVector(AnimaString propertyName, AFloat x, AFloat y);
	void SetVector(const char* propertyName, AFloat x, AFloat y);
	void SetVector(AnimaString propertyName, AnimaVertex3f value);
	void SetVector(const char* propertyName, AnimaVertex3f value);
	void SetVector(AnimaString propertyName, AFloat x, AFloat y, AFloat z);
	void SetVector(const char* propertyName, AFloat x, AFloat y, AFloat z);
	void SetVector(AnimaString propertyName, AnimaVertex4f value);
	void SetVector(const char* propertyName, AnimaVertex4f value);
	void SetVector(AnimaString propertyName, AFloat x, AFloat y, AFloat z, AFloat w);
	void SetVector(const char* propertyName, AFloat x, AFloat y, AFloat z, AFloat w);

	void SetFloat(AnimaString propertyName, AFloat value);
	void SetFloat(const char* propertyName, AFloat value);

	void SetBoolean(AnimaString propertyName, bool value);
	void SetBoolean(const char* propertyName, bool value);

	void SetInteger(AnimaString propertyName, AInt value);
	void SetInteger(const char* propertyName, AInt value);

public:
	AUint GetTextureSlot(const AnimaString& slotName);
	AUint GetTextureSlot(const char* slotName);
	
	AnimaTexture* GetTexture(AnimaString propertyName);
	AnimaTexture* GetTexture(const char* propertyName);

	AnimaGBuffer* GetGBuffer(const AnimaString& gBufferName);
	AnimaGBuffer* GetGBuffer(const char* gBufferName);

	AnimaColor3f GetColor3f(AnimaString propertyName);
	AnimaColor3f GetColor3f(const char* propertyName);
	AnimaColor4f GetColor4f(AnimaString propertyName);
	AnimaColor4f GetColor4f(const char* propertyName);

	AnimaVertex2f GetVector2f(AnimaString propertyName);
	AnimaVertex2f GetVector2f(const char* propertyName);
	AnimaVertex3f GetVector3f(AnimaString propertyName);
	AnimaVertex3f GetVector3f(const char* propertyName);
	AnimaVertex4f GetVector4f(AnimaString propertyName);
	AnimaVertex4f GetVector4f(const char* propertyName);

	AFloat GetFloat(AnimaString propertyName);
	AFloat GetFloat(const char* propertyName);

	AInt GetInteger(AnimaString propertyName);
	AInt GetInteger(const char* propertyName);

	bool GetBoolean(AnimaString propertyName);
	bool GetBoolean(const char* propertyName);

protected:
	AnimaAllocator* _allocator;
	AnimaScene*		_scene;
	AnimaEngine*	_engine;

	AnimaMesh*		_filterMesh;
	AnimaCamera*	_filterCamera;

	AnimaArray<AnimaPrimitiveData*> _primitives;
	AUint _vertexArrayObject;
	AUint _verticesBufferObject;
	AUint _indexesBufferObject;

	AnimaModel* _lastUpdatedModel;
	AnimaModelInstance* _lastUpdatedModelInstance;
	
#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, AUint, AnimaStringHasher>			_textureSlotsMap;
	boost::unordered_map<AnimaString, AnimaTexture*, AnimaStringHasher>	_texturesMap;
	boost::unordered_map<AnimaString, AnimaGBuffer*, AnimaStringHasher>	_gBuffersMap;
		
	boost::unordered_map<AnimaString, AnimaVertex2f, AnimaStringHasher> _vectors2fMap;
	boost::unordered_map<AnimaString, AnimaVertex3f, AnimaStringHasher> _vectors3fMap;
	boost::unordered_map<AnimaString, AnimaVertex4f, AnimaStringHasher> _vectors4fMap;

	boost::unordered_map<AnimaString, AFloat, AnimaStringHasher>	_floatsMap;
	boost::unordered_map<AnimaString, AInt, AnimaStringHasher>	_integersMap;
	boost::unordered_map<AnimaString, bool, AnimaStringHasher>	_booleansMap;

	boost::unordered_map<AnimaString, AnimaMesh*, AnimaStringHasher>	_lightsMeshMap;
#pragma warning (default: 4251)
};

template<class T> 
AnimaMesh* AnimaRenderer::CreateMeshForLightType()
{
	AnimaString type = typeid(T).name();

	auto pair = _lightsMeshMap.find(type);
	if (pair != _lightsMeshMap.end())
		return pair->second;

	AnimaString name = type + "_RENMESH";

	AnimaMesh* lightMesh = AnimaAllocatorNamespace::AllocateNew<AnimaMesh>(*_allocator, name, _engine->GetDataGeneratorsManager(), _allocator);
	_lightsMeshMap[type] = lightMesh;

	return lightMesh;
}

END_ANIMA_ENGINE_NAMESPACE

#endif //__Anima__AnimaRenderer__