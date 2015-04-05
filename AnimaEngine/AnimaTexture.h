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
	AnimaTexture(AnimaAllocator* allocator);
	AnimaTexture(AnimaAllocator* allocator, const char* name, AUint textureTarget, AUint width, AUint height, AUchar* data, ASizeT dataSize, AUint mipMapLevels, AUint filter, AUint internalFormat, AUint format, AUint dataType, AUint clamp, AUint attachment = GL_NONE);
	AnimaTexture(AnimaAllocator* allocator, const AnimaString& name, AUint textureTarget, AUint width, AUint height, AUchar* data, ASizeT dataSize, AUint mipMapLevels, AUint filter, AUint internalFormat, AUint format, AUint dataType, AUint clamp, AUint attachment = GL_NONE);
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

	void SetFormat(AUint format);
	AUint GetFormat() const;

	void SetFilter(AUint filter);
	AUint GetFilter() const;

	void SetAttachment(AUint attachment);
	AUint GetAttachment() const;

	void SetInternalFormat(AUint internalFormat);
	AUint GetInternalFormat() const;

	void SetDataType(AUint dataType);
	AUint GetDataType() const;

	void SetData(AUchar* data, ASizeT dataSize);
	AUchar* GetData() const;
	const AUchar* GetConstData() const;

	void SetTextureTarget(AUint target);
	AUint GetTextureTarget() const;

	void SetClamp(AUint clamp);
	AUint GetClamp() const;
	
	bool Load();
	void Unload();
	bool LoadRenderTargets();

	bool IsReady();
	bool AreRenderTargetsReady();

	void Resize(AUint width, AUint height);
	
	void Bind(AUint unit) const;
	void BindAsRenderTarget() const;

	void SetName(const AnimaString& name);
	void SetName(const char* name);

	AnimaString GetAnimaName() const;
	const char* GetName() const;

private:
	AnimaAllocator* _allocator;

	AnimaString _name;
	
	AUint _width;
	AUint _height;

	AUint _textureID;
	AUint _frameBuffer;
	AUint _renderBuffer;

	AUchar* _data;
	ASizeT _dataSize;
		
	AUint _textureTarget;
	AUint _filter;
	AUint _internalFormat;
	AUint _format;
	AUint _attachment;
	AUint _dataType;
	AUint _clamp;
	AUint _mipMapLevels;

	bool _texturesReady;
	bool _needsResize;
	bool _renderTargetsReady;
};

END_ANIMA_ENGINE_NAMESPACE