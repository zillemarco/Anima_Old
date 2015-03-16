#pragma once

#ifndef __Anima__AnimaRenderingManager__
#define __Anima__AnimaRenderingManager__

#include <stdio.h>
#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaMesh.h"
#include "AnimaMaterial.h"
#include "AnimaLight.h"
#include "AnimaShaderProgram.h"
#include "AnimaScene.h"
#include "AnimaString.h"
#include "AnimaGBuffer.h"
#include "AnimaVertex.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>

using boost::multi_index_container;
using namespace boost::multi_index;

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaRenderingManager
{
public:
	AnimaRenderingManager(AnimaAllocator* allocator);
	AnimaRenderingManager(const AnimaRenderingManager& src);
	AnimaRenderingManager(AnimaRenderingManager&& src);
	~AnimaRenderingManager();

	AnimaRenderingManager& operator=(const AnimaRenderingManager& src);
	AnimaRenderingManager& operator=(AnimaRenderingManager&& src);
	
public:
	void Start(AnimaScene* scene);
	void Finish(AnimaScene* scene);

	void DrawAllModels(AnimaScene* scene);
	void DrawSingleModel(AnimaScene* scene, AnimaMesh* model);

	void ForwardDrawAllModels(AnimaScene* scene);
	void ForwardDrawSingleModel(AnimaScene* scene, AnimaMesh* model);

	void DeferredDrawAllModels(AnimaScene* scene);
	void DeferredDrawSingleModel(AnimaScene* scene, AnimaMesh* model);
	
public:
	void InitTextureSlots();
	void InitRenderingTargets(AInt screenWidth, AInt screenHeight);
	void InitRenderingUtilities(AInt screenWidth, AInt screenHeight);

protected:
	void DrawModel(AnimaScene* scene, AnimaMesh* model, AnimaShaderProgram* program);
	void DrawModel(AnimaScene* scene, AnimaMesh* model, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation);
	void DrawModelMesh(AnimaScene* scene, AnimaMesh* mesh, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation);
	
	void ForwardAmbientPass(AnimaScene* scene, AnimaShaderProgram* program, AnimaMesh* model = nullptr);
	void ForwardDirectionalPass(AnimaScene* scene, AnimaShaderProgram* program, AnimaMesh* model = nullptr);
	void ForwardPointPass(AnimaScene* scene, AnimaShaderProgram* program, AnimaMesh* model = nullptr);
	void ForwardSpotPass(AnimaScene* scene, AnimaShaderProgram* program, AnimaMesh* model = nullptr);

	void ForwardDrawModel(AnimaScene* scene, AnimaMesh* model, AnimaShaderProgram* program);
	void ForwardDrawModel(AnimaScene* scene, AnimaMesh* model, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation);
	void ForwardDrawModelMesh(AnimaScene* scene, AnimaMesh* mesh, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation);

	void DeferredPreparePass(AnimaScene* scene, AnimaShaderProgram* program, AnimaMesh* model = nullptr);
	void DeferredDirectionalPass(AnimaScene* scene, AnimaShaderProgram* program);
	void DeferredPointPass(AnimaScene* scene, AnimaShaderProgram* program);
	void DeferredSpotPass(AnimaScene* scene, AnimaShaderProgram* program);
	void DeferredCombinePass(AnimaScene* scene, AnimaShaderProgram* program);
	void DeferredUpdateShadowMaps(AnimaScene* scene, AnimaShaderProgram* program);

	void DeferredDrawModel(AnimaScene* scene, AnimaMesh* model, AnimaShaderProgram* program, bool updateMaterial = true);
	void DeferredDrawModel(AnimaScene* scene, AnimaMesh* model, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation, bool updateMaterial = true);
	void DeferredDrawModelMesh(AnimaScene* scene, AnimaMesh* mesh, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation, bool updateMaterial = true);
	
	void Clear();

	void ApplyEffectFromTextureToTexture(AnimaShaderProgram* filterProgram, AnimaTexture* src, AnimaTexture* dst);
	void ApplyEffectFromTextureToGBuffer(AnimaShaderProgram* filterProgram, AnimaTexture* src, AnimaGBuffer* dst);
	void ApplyEffectFromGBufferToGBuffer(AnimaShaderProgram* filterProgram, AnimaGBuffer* src, AnimaGBuffer* dst);
	void ApplyEffectFromGBufferToTexture(AnimaShaderProgram* filterProgram, AnimaGBuffer* src, AnimaTexture* dst);

protected:
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

	AnimaMesh*		_filterMesh;
	AnimaCamera*	_filterCamera;

	AnimaMesh*		_pointLightMesh;
	AnimaMesh*		_spotLightMesh;

#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, AUint, AnimaString::Hasher>			_textureSlotsMap;
	boost::unordered_map<AnimaString, AnimaTexture*, AnimaString::Hasher>	_texturesMap;
	boost::unordered_map<AnimaString, AnimaGBuffer*, AnimaString::Hasher>	_gBuffersMap;
		
	boost::unordered_map<AnimaString, AnimaVertex2f, AnimaString::Hasher> _vectors2fMap;
	boost::unordered_map<AnimaString, AnimaVertex3f, AnimaString::Hasher> _vectors3fMap;
	boost::unordered_map<AnimaString, AnimaVertex4f, AnimaString::Hasher> _vectors4fMap;

	boost::unordered_map<AnimaString, AFloat, AnimaString::Hasher>	_floatsMap;
	boost::unordered_map<AnimaString, AInt, AnimaString::Hasher>	_integersMap;
	boost::unordered_map<AnimaString, bool, AnimaString::Hasher>	_booleansMap;
#pragma warning (default: 4251)
};

END_ANIMA_ENGINE_NAMESPACE

#endif //__Anima__AnimaRenderingManager__