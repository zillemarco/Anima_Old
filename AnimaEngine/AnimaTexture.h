#pragma once

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaString.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaTexture
{
public:
	AnimaTexture(AnimaAllocator* allocator, AUint texturesNumber);
	AnimaTexture(AnimaAllocator* allocator, AUint textureTarget, AUint width, AUint height, AUint texturesNumber, AUchar** data, ASizeT* dataSize, AUint mipMapLevels, AUint* filters, AUint* internalFormats, AUint* formats, bool clamp, AUint* attachments);
	AnimaTexture(const AnimaTexture& src);
	AnimaTexture(AnimaTexture&& src);
	~AnimaTexture();
	
	AnimaTexture& operator=(const AnimaTexture& src);
	AnimaTexture& operator=(AnimaTexture&& src);

public:
	AUint GetID(AUint index = 0) const;

	void SetWidth(AUint width);
	AUint GetWidth() const;

	void SetHeight(AUint height);
	AUint GetHeight() const;

	void SetMipMapLevels(AUint levels);
	AUint GetMipMapLevels() const;

	void SetFormats(AUint* formats);
	void SetFormat(AUint format, AUint index);
	AUint GetFormat(AUint index) const;
	AUint* GetFormats() const;

	void SetFilters(AUint* filters);
	void SetFilter(AUint filter, AUint index);
	AUint GetFilter(AUint index) const;
	AUint* GetFilters() const;

	void SetAttachments(AUint* attachments);
	void SetAttachment(AUint attachment, AUint index);
	AUint GetAttachment(AUint index) const;
	AUint* GetAttachments() const;

	void SetInternalFormats(AUint* internalFormats);
	void SetInternalFormat(AUint internalFormat, AUint index);
	AUint GetInternalFormat(AUint index) const;
	AUint* GetInternalFormats() const;

	void SetDatas(AUchar** datas, ASizeT* datasSize);
	void SetData(AUchar* data, ASizeT dataSize, AUint index);
	AUchar* GetData(AUint index) const;
	const AUchar* GetConstData(AUint index) const;
	AUchar** GetDatas() const;
	const AUchar** GetConstDatas() const;

	void SetTextureTarget(AUint target);
	AUint GetTextureTarget() const;

	void EnableClamp(bool bEnable);
	bool ClampEnabled() const;
	
	bool LoadTextures();
	bool LoadRenderTargets();
	bool IsReady();

	void Unload();

	void Bind(AUint unit, AUint index = 0) const;
	void BindAsRenderTarget() const;

private:
	AnimaAllocator* _allocator;
	
	AUint _width;
	AUint _height;
	AUint _texturesNumber;

	AUint* _textureIDs;
	AUint _frameBuffer;
	AUint _renderBuffer;

	AUchar** _datas;
	ASizeT* _datasSize;
		
	AUint _textureTarget;
	AUint* _filters;
	AUint* _internalFormats;
	AUint* _formats;
	AUint* _attachments;

	bool _enableClamp;

	AUint _mipMapLevels;

	bool _texturesReady;
	bool _renderTargetsReady;
};

END_ANIMA_ENGINE_NAMESPACE