#pragma once

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaNamedObject.h"
#include "AnimaVertex.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

enum AnimaTextureCubeIndex
{
	POSITIVE_X = 0,
	NEGATIVE_X,
	POSITIVE_Y,
	NEGATIVE_Y,
	POSITIVE_Z,
	NEGATIVE_Z
};

enum AnimaTextureTarget
{
	TEXTURE_2D = 0,
	TEXTURE_3D,
	TEXTURE_CUBE
};

enum AnimaTextureClampMode
{
	REPEAT = 0,
	MIRRORED_REPEAT,
	TO_EDGE,
	TO_BORDER
};

enum AnimaTextureFilterMode
{
	NEAREST = 0,
	LINEAR,
	NEAREST_MIPMAP_NEAREST,
	NEAREST_MIPMAP_LINEAR,
	LINEAR_MIPMAP_NEAREST,
	LINEAR_MIPMAP_LINEAR
};

class ANIMA_ENGINE_EXPORT AnimaTexture : public AnimaNamedObject
{
public:
	AnimaTexture(AnimaAllocator* allocator);
	AnimaTexture(AnimaAllocator* allocator, const AnimaString& name, AUint width, AUint height, AUchar* data, AUint dataSize);
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

	void SetMipMapLevels(AUint levels, bool generate);
	AUint GetMipMapLevels() const;

	void SetFormat(AUint format);
	AUint GetFormat() const;

	void SetFilter(AnimaTextureFilterMode filter);
	AnimaTextureFilterMode GetFilter() const;

	void SetAttachment(AUint attachment);
	AUint GetAttachment() const;

	void SetInternalFormat(AUint internalFormat);
	AUint GetInternalFormat() const;

	void SetDataType(AUint dataType);
	AUint GetDataType() const;

	/*!
	 *	\brief		Imposta il buffer con i dati della texture
	 *	\details	Imposta il buffer con i dati della texture
					I dati vengono copiati, quindi è possibile deallocare il buffer dopo la chiamata a questa funzione
	 *	\param[in]	data		Buffer con i dati della texture
	 *	\param[in]	dataSize	Lunghezza del buffer con i dati della texture
	 *	\return		Torna true se il target della texture è stato impostato per essere non una texture 3D, false altrimenti
	 *	\author		Zille Marco
	 */
	bool SetData(AUchar* data, AUint dataSize);

	/*!
	 *	\brief		Imposta il buffer con i dati della texture all'indice passato
	 *	\details	Imposta il buffer con i dati della texture all'indice passato. Serve per impostare i dati delle varie texture nel caso
					di texture 3D.
					I dati vengono copiati, quindi è possibile deallocare il buffer dopo la chiamata a questa funzione
	 *	\param[in]	data		Buffer con i dati della texture
	 *	\param[in]	dataSize	Lunghezza del buffer con i dati della texture
	 *	\param[in]	index		Indice su cui andare a copiare il buffer
	 *	\return		Torna true se il target della texture è stato impostato per essere una texture 3D e l'indice è valido, false altrimenti
	 *	\author		Zille Marco
	 */
	bool SetData(AUchar* data, AUint dataSize, AnimaTextureCubeIndex index);
	const AUchar* GetData() const;
	const AUchar* GetData(AnimaTextureCubeIndex index) const;

	void SetTextureTarget(AnimaTextureTarget target);
	AnimaTextureTarget GetTextureTarget() const;

	void SetClamp(AnimaTextureClampMode clamp);
	AnimaTextureClampMode GetClamp() const;

	void SetBorderColor(const AnimaColor4f& color);
	void SetBorderColor(const AFloat& r, const AFloat& g, const AFloat& b, const AFloat& a);
	AnimaColor4f GetColor() const;
	
	bool Load();
	void Unload();
	bool LoadRenderTargets();

	bool IsReady();
	bool AreRenderTargetsReady();

	void Resize(AUint width, AUint height);
	
	void Bind(AUint unit) const;
	void BindAsRenderTarget();

private:
	void CopyData(const AnimaTexture& src);
	bool ResizeTexture();

private:
	AUint _width;
	AUint _height;

	AUint _textureID;
	AUint _frameBuffer;
	AUint _renderBuffer;

	AnimaArray<AnimaArray<AUchar> > _data;
		
	AnimaTextureTarget _textureTarget;
	AnimaTextureFilterMode _filter;
	AUint _internalFormat;
	AUint _format;
	AUint _attachment;
	AUint _dataType;
	AnimaTextureClampMode _clamp;
	AUint _mipMapLevels;
	AnimaColor4f _borderColor;
	bool _generateMipMaps;

	bool _texturesReady;
	bool _needsResize;
	bool _renderTargetsReady;

	// Funzioni di utilità di conversione tra dati AnimaEngine e dati piattaforma (OpenGL o DirectX)
public:
	static AUint TargetToPlatform(const AnimaTextureTarget& target);
	static AUint ClampToPlatform(const AnimaTextureClampMode& clamp);
	static AUint FilterToPlatform(const AnimaTextureFilterMode& filter);
	static AUint CubeIndexToPlatform(const AnimaTextureCubeIndex& index);
};

END_ANIMA_ENGINE_NAMESPACE