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
	AnimaTexture(AnimaEngine* engine);
	AnimaTexture(AnimaEngine* engine, AUchar* data, ASizeT dataSize, AUint width, AUint height, AUint mipMapLevels, AUint format);
	AnimaTexture(const AnimaTexture& src);
	AnimaTexture(AnimaTexture&& src);
	~AnimaTexture();
	
	AnimaTexture& operator=(const AnimaTexture& src);
	AnimaTexture& operator=(AnimaTexture&& src);

public:
	AUint GetID() const;

	void SetWidth(AUint width);
	AUint GetWidth() const;

	void SetHeight(AUint height);
	AUint GetHeight() const;

	void SetMipMapLevels(AUint levels);
	AUint GetMipMapLevels() const;

	void SetFormat(AUint format);
	AUint GetFormat() const;

	void SetData(AUchar* data, ASizeT dataSize);
	AUchar* GetData() const;
	const AUchar* GetConstData() const;

	bool Load();
	bool IsLoaded();

	void Unload();

	void Bind(AUint unit);

private:
	AnimaEngine* _engine;
	
	AUchar* _data;
	ASizeT _dataSize;

	AUint _textureId;
	AUint _samplerId;

	AUint _width;
	AUint _height;
	AUint _mipMapLevels;
	AUint _format;

	bool _isLoaded;
};

END_ANIMA_ENGINE_NAMESPACE