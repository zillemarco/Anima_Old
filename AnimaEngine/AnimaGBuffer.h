//
//  AnimaGBuffer.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaGBuffer__
#define __Anima__AnimaGBuffer__

#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaAllocator.h"
#include "AnimaTexture.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaGBuffer
{
public:
	AnimaGBuffer(AnimaAllocator* allocator);
	AnimaGBuffer(AnimaAllocator* allocator, AUint width, AUint height);
	AnimaGBuffer(const AnimaGBuffer& src);
	AnimaGBuffer(AnimaGBuffer&& src);
	~AnimaGBuffer();

	AnimaGBuffer& operator=(const AnimaGBuffer& src);
	AnimaGBuffer& operator=(AnimaGBuffer&& src);

public:
	bool AddTexture(const AnimaString& name, AUint target, AUint attachment, AUint internalFormat, AUint format, AUint dataType, AUint filter, AUint clamp);
	bool AddTexture(const char* name, AUint target, AUint attachment, AUint internalFormat, AUint format, AUint dataType, AUint filter, AUint clamp);

	void Resize(AUint width, AUint height);

	bool Create();
	void Destroy();

	void BindAsRenderTarget() const;

	AnimaTexture* GetTexture(const AnimaString& name);
	AnimaTexture* GetTexture(const char* name);

protected:
	void ClearTextures();

protected:
	class AnimaGBufferData
	{
	public:
		AnimaGBufferData(const AnimaString& name, AnimaTexture* texture, AUint index, AUint attachment) {
			_name = name;
			_index = index;
			_texture = texture;
			_attachment = attachment;
		}

		AnimaGBufferData(const AnimaGBufferData& src) {
			_name = src._name;
			_index = src._index;
			_texture = src._texture;
			_attachment = src._attachment;
		}

		AnimaGBufferData(const AnimaGBufferData&& src) {
			_name = src._name;
			_index = src._index;
			_texture = src._texture;
			_attachment = src._attachment;
		}

	public:
		AnimaString		_name;
		AUint			_index;
		AUint			_attachment;
		AnimaTexture*	_texture;
	};

	typedef boost::multi_index_container<
		AnimaGBufferData*,
		boost::multi_index::indexed_by<
		boost::multi_index::ordered_unique<BOOST_MULTI_INDEX_MEMBER(AnimaGBufferData, AUint, _index)>,
		boost::multi_index::hashed_unique<BOOST_MULTI_INDEX_MEMBER(AnimaGBufferData, AnimaString, _name), AnimaString::Hasher> >
	> AnimaGBufferDataSet;

	typedef AnimaGBufferDataSet::nth_index<0>::type AnimaGBufferDataSetByIndex;
	typedef AnimaGBufferDataSet::nth_index<1>::type AnimaGBufferDataSetByName;

protected:
	AnimaAllocator* _allocator;
	AUint	_width;
	AUint	_height;
	
	AUint	_frameBuffer;
	AUint	_renderBuffer;
	bool	_created;
	bool	_needsResize;
	
#pragma warning (disable: 4251)
	AnimaGBufferDataSet	_texturesSet;
#pragma warning (default: 4251) 
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaGBuffer__) */
