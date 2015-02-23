#pragma once

#ifndef __Anima__AnimaRenderingManager__
#define __Anima__AnimaRenderingManager__

#include <stdio.h>
#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaModel.h"
#include "AnimaMaterial.h"
#include "AnimaLight.h"
#include "AnimaShaderProgram.h"
#include "AnimaStage.h"
#include "AnimaString.h"
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
	
protected:
	class DeferredData
	{
	public:
		DeferredData(const AnimaString& name, AUint index, AUint attachment, AUint internalFormat, AUint format, AUint dataType, AUint filter) {
			_index = index;
			_attachment = attachment;
			_internalFormat = internalFormat;
			_format = format;
			_dataType = dataType;
			_filter = filter;
			_name = name;
		}

		DeferredData(const DeferredData& src) {
			_index = src._index;
			_attachment = src._attachment;
			_internalFormat = src._internalFormat;
			_format = src._format;
			_dataType = src._dataType;
			_filter = src._filter;
			_name = src._name;
		}

		DeferredData(const DeferredData&& src) {
			_index = src._index;
			_attachment = src._attachment;
			_internalFormat = src._internalFormat;
			_format = src._format;
			_dataType = src._dataType;
			_filter = src._filter;
			_name = src._name;
		}

	public:
		AnimaString _name;
		AUint _index;
		AUint _attachment;
		AUint _internalFormat;
		AUint _format;
		AUint _dataType;
		AUint _filter;
	};

	typedef multi_index_container<
		DeferredData*,
		indexed_by<
		ordered_unique<BOOST_MULTI_INDEX_MEMBER(DeferredData, AUint, _index)>,
		hashed_unique<BOOST_MULTI_INDEX_MEMBER(DeferredData, AnimaString, _name), AnimaString::Hasher> >
	> DeferredDataSet;

	typedef DeferredDataSet::nth_index<0>::type DeferredDataSetByIndex;
	typedef DeferredDataSet::nth_index<1>::type DeferredDataSetByName;
	
public:
	void Start(AnimaStage* stage);
	void Finish(AnimaStage* stage);

	void DrawAllModels(AnimaStage* stage);
	void DrawSingleModel(AnimaStage* stage, AnimaModel* model);

	void ForwardDrawAllModels(AnimaStage* stage);
	void ForwardDrawSingleModel(AnimaStage* stage, AnimaModel* model);

	void DeferredDrawAllModels(AnimaStage* stage);
	void DeferredDrawSingleModel(AnimaStage* stage, AnimaModel* model);
	
public:
	void InitTextureSlots();
	void InitRenderingTargets(AInt screenWidth, AInt screenHeight);
	void InitRenderingUtilities(AInt screenWidth, AInt screenHeight);

protected:
	void DrawModel(AnimaStage* stage, AnimaModel* model, AnimaShaderProgram* program);
	void DrawModel(AnimaStage* stage, AnimaModel* model, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation);
	void DrawModelMesh(AnimaStage* stage, AnimaMesh* mesh, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation);
	
	void ForwardAmbientPass(AnimaStage* stage, AnimaShaderProgram* program, AnimaModel* model = nullptr);
	void ForwardDirectionalPass(AnimaStage* stage, AnimaShaderProgram* program, AnimaModel* model = nullptr);
	void ForwardPointPass(AnimaStage* stage, AnimaShaderProgram* program, AnimaModel* model = nullptr);
	void ForwardSpotPass(AnimaStage* stage, AnimaShaderProgram* program, AnimaModel* model = nullptr);

	void ForwardDrawModel(AnimaStage* stage, AnimaModel* model, AnimaShaderProgram* program);
	void ForwardDrawModel(AnimaStage* stage, AnimaModel* model, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation);
	void ForwardDrawModelMesh(AnimaStage* stage, AnimaMesh* mesh, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation);

	void DeferredPreparePass(AnimaStage* stage, AnimaShaderProgram* program, AnimaModel* model = nullptr);
	void DeferredAmbientPass(AnimaStage* stage, AnimaShaderProgram* program);
	void DeferredDirectionalPass(AnimaStage* stage, AnimaShaderProgram* program);
	void DeferredPointPass(AnimaStage* stage, AnimaShaderProgram* program);
	void DeferredSpotPass(AnimaStage* stage, AnimaShaderProgram* program);

	void DeferredDrawModel(AnimaStage* stage, AnimaModel* model, AnimaShaderProgram* program);
	void DeferredDrawModel(AnimaStage* stage, AnimaModel* model, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation);
	void DeferredDrawModelMesh(AnimaStage* stage, AnimaMesh* mesh, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation);
	
	void Clear();

	void ApplyFilter(AnimaShaderProgram* filterProgram, AnimaTexture* src, AnimaTexture* dst);

protected:
	void SetTexture(AnimaString propertyName, AnimaTexture* value, bool deleteExistent = true);
	void SetTexture(const char* propertyName, AnimaTexture* value, bool deleteExistent = true);

	void SetTextureSlot(AnimaString slotName, AUint value);
	void SetTextureSlot(const char* slotName, AUint value);

	void SetDeferredData(DeferredData* value);

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
	AnimaTexture* GetTexture(AnimaString propertyName);
	AnimaTexture* GetTexture(const char* propertyName);

	AUint GetTextureSlot(const AnimaString& slotName);
	AUint GetTextureSlot(const char* slotName);

	AUint GetTextureIndex(const AnimaString& textureName);
	AUint GetTextureIndex(const char* textureName);

	DeferredData* GetDeferredData(const AnimaString& dataName);
	DeferredData* GetDeferredData(const char* dataName);

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

	AnimaMesh* _filterMesh;
	AnimaTransformation _filterMeshTransform;

	AnimaCamera* _filterCamera;

#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, AUint, AnimaString::Hasher>			_textureSlotsMap;
	boost::unordered_map<AnimaString, AnimaTexture*, AnimaString::Hasher>	_texturesMap;
	DeferredDataSet															_deferredDataMap;

	boost::unordered_map<AnimaString, AnimaColor3f, AnimaString::Hasher> _colors3fMap;
	boost::unordered_map<AnimaString, AnimaColor4f, AnimaString::Hasher> _colors4fMap;
	
	boost::unordered_map<AnimaString, AnimaVertex2f, AnimaString::Hasher> _vectors2fMap;
	boost::unordered_map<AnimaString, AnimaVertex3f, AnimaString::Hasher> _vectors3fMap;
	boost::unordered_map<AnimaString, AnimaVertex4f, AnimaString::Hasher> _vectors4fMap;

	boost::unordered_map<AnimaString, AFloat, AnimaString::Hasher>	_floatsMap;
	boost::unordered_map<AnimaString, AInt, AnimaString::Hasher>	_integersMap;
	boost::unordered_map<AnimaString, bool, AnimaString::Hasher>	_booleansMap;
#pragma warning (default: 4251)
};

END_ANIMA_ENGINE_NAMESPACE

#endif __Anima__AnimaRenderingManager__