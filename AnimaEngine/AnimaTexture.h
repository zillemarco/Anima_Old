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
	POSITIVE_X = 0, NEGATIVE_X, POSITIVE_Y, NEGATIVE_Y, POSITIVE_Z, NEGATIVE_Z
};

enum AnimaTextureTarget
{
	TEXTURE_TARGET_NONE = 0, TEXTURE_TARGET_2D, TEXTURE_TARGET_3D, TEXTURE_TARGET_CUBE
};

enum AnimaTextureFormat
{
	TEXTURE_FORMAT_NONE = 0, TEXTURE_FORMAT_RED, TEXTURE_FORMAT_GREEN, TEXTURE_FORMAT_BLUE, TEXTURE_FORMAT_RG, TEXTURE_FORMAT_RGB, TEXTURE_FORMAT_RGBA,
	TEXTURE_FORMAT_BGR, TEXTURE_FORMAT_BGRA, TEXTURE_FORMAT_RED_INT, TEXTURE_FORMAT_GREEN_INT, TEXTURE_FORMAT_BLUE_INT,
	TEXTURE_FORMAT_RG_INT, TEXTURE_FORMAT_RGB_INT, TEXTURE_FORMAT_RGBA_INT, TEXTURE_FORMAT_BGR_INT, TEXTURE_FORMAT_BGRA_INT,
	TEXTURE_FORMAT_DEPTH, TEXTURE_FORMAT_DEPTH_STENCIL, TEXTURE_FORMAT_STENCIL,
	TEXTURE_FORMAT_COMPRESSED_RGBA_S3TC_DXT1_EXT, TEXTURE_FORMAT_COMPRESSED_RGBA_S3TC_DXT3_EXT, TEXTURE_FORMAT_COMPRESSED_RGBA_S3TC_DXT5_EXT
};

enum AnimaTextureInternalFormat
{
	TEXTURE_INTERNAL_FORMAT_NONE = 0, TEXTURE_INTERNAL_FORMAT_R8, TEXTURE_INTERNAL_FORMAT_R8_SNORM, TEXTURE_INTERNAL_FORMAT_R16, TEXTURE_INTERNAL_FORMAT_R16_SNORM, TEXTURE_INTERNAL_FORMAT_RG8, TEXTURE_INTERNAL_FORMAT_RG8_SNORM,
	TEXTURE_INTERNAL_FORMAT_RG16, TEXTURE_INTERNAL_FORMAT_RG16_SNORM, TEXTURE_INTERNAL_FORMAT_R3_G3_B2, TEXTURE_INTERNAL_FORMAT_RGB4, TEXTURE_INTERNAL_FORMAT_RGB5, TEXTURE_INTERNAL_FORMAT_RGB565,
	TEXTURE_INTERNAL_FORMAT_RGB8, TEXTURE_INTERNAL_FORMAT_RGB8_SNORM, TEXTURE_INTERNAL_FORMAT_RGB10, TEXTURE_INTERNAL_FORMAT_RGB12, TEXTURE_INTERNAL_FORMAT_RGB16, TEXTURE_INTERNAL_FORMAT_RGB16_SNORM,
	TEXTURE_INTERNAL_FORMAT_RGBA2, TEXTURE_INTERNAL_FORMAT_RGBA4, TEXTURE_INTERNAL_FORMAT_RGB5_A1, TEXTURE_INTERNAL_FORMAT_RGBA8, TEXTURE_INTERNAL_FORMAT_RGBA8_SNORM, TEXTURE_INTERNAL_FORMAT_RGB10_A2,
	TEXTURE_INTERNAL_FORMAT_RGB10_A2UI, TEXTURE_INTERNAL_FORMAT_RGBA12, TEXTURE_INTERNAL_FORMAT_RGBA16, TEXTURE_INTERNAL_FORMAT_RGBA16_SNORM, TEXTURE_INTERNAL_FORMAT_SRGB8,
	TEXTURE_INTERNAL_FORMAT_SRGB8_A8, TEXTURE_INTERNAL_FORMAT_R16F, TEXTURE_INTERNAL_FORMAT_RG16F, TEXTURE_INTERNAL_FORMAT_RGB16F, TEXTURE_INTERNAL_FORMAT_RGBA16F, TEXTURE_INTERNAL_FORMAT_R32F, TEXTURE_INTERNAL_FORMAT_RG32F,
	TEXTURE_INTERNAL_FORMAT_RGB32F, TEXTURE_INTERNAL_FORMAT_RGBA32F, TEXTURE_INTERNAL_FORMAT_R11F_G11F_B10F, TEXTURE_INTERNAL_FORMAT_RGB9_E5, TEXTURE_INTERNAL_FORMAT_R8I, TEXTURE_INTERNAL_FORMAT_R8UI,
	TEXTURE_INTERNAL_FORMAT_R16I, TEXTURE_INTERNAL_FORMAT_R16UI, TEXTURE_INTERNAL_FORMAT_R32I, TEXTURE_INTERNAL_FORMAT_R32UI, TEXTURE_INTERNAL_FORMAT_RG8I, TEXTURE_INTERNAL_FORMAT_RG8UI, TEXTURE_INTERNAL_FORMAT_RG16I, TEXTURE_INTERNAL_FORMAT_RG16UI, TEXTURE_INTERNAL_FORMAT_RG32I,
	TEXTURE_INTERNAL_FORMAT_RG32UI, TEXTURE_INTERNAL_FORMAT_RGB8I, TEXTURE_INTERNAL_FORMAT_RGB8UI, TEXTURE_INTERNAL_FORMAT_RGB16I, TEXTURE_INTERNAL_FORMAT_RGB16UI, TEXTURE_INTERNAL_FORMAT_RGB32I, TEXTURE_INTERNAL_FORMAT_RGB32UI,
	TEXTURE_INTERNAL_FORMAT_RGBA8I, TEXTURE_INTERNAL_FORMAT_RGBA8UI, TEXTURE_INTERNAL_FORMAT_RGBA16I, TEXTURE_INTERNAL_FORMAT_RGBA16UI, TEXTURE_INTERNAL_FORMAT_RGBA32I, TEXTURE_INTERNAL_FORMAT_RGBA32UI,
	TEXTURE_INTERNAL_FORMAT_DEPTH_COMPONENT, TEXTURE_INTERNAL_FORMAT_DEPTH_STENCIL, TEXTURE_INTERNAL_FORMAT_STENCIL,
	TEXTURE_INTERNAL_FORMAT_DEPTH16, TEXTURE_INTERNAL_FORMAT_DEPTH24, TEXTURE_INTERNAL_FORMAT_DEPTH32, TEXTURE_INTERNAL_FORMAT_DEPTH32F, TEXTURE_INTERNAL_FORMAT_DEPTH24_STENCIL8, TEXTURE_INTERNAL_FORMAT_DEPTH32F_STENCIL8, TEXTURE_INTERNAL_FORMAT_STENCIL8,
	TEXTURE_INTERNAL_FORMAT_RED, TEXTURE_INTERNAL_FORMAT_RG, TEXTURE_INTERNAL_FORMAT_RGB, TEXTURE_INTERNAL_FORMAT_RGBA,
	TEXTURE_INTERNAL_FORMAT_1, TEXTURE_INTERNAL_FORMAT_2, TEXTURE_INTERNAL_FORMAT_3, TEXTURE_INTERNAL_FORMAT_4
};

enum AnimaTextureClampMode
{
	TEXTURE_CLAMP_REPEAT = 0, TEXTURE_CLAMP_MIRRORED_REPEAT, TEXTURE_CLAMP_TO_EDGE, TEXTURE_CLAMP_TO_BORDER
};

enum AnimaTextureMinFilterMode
{
	TEXTURE_MIN_FILTER_MODE_NEAREST = 0, TEXTURE_MIN_FILTER_MODE_LINEAR, TEXTURE_MIN_FILTER_MODE_NEAREST_MIPMAP_NEAREST, 
	TEXTURE_MIN_FILTER_MODE_NEAREST_MIPMAP_LINEAR, TEXTURE_MIN_FILTER_MODE_LINEAR_MIPMAP_NEAREST, TEXTURE_MIN_FILTER_MODE_LINEAR_MIPMAP_LINEAR
};

enum AnimaTextureMagFilterMode
{
	TEXTURE_MAG_FILTER_MODE_NEAREST = 0, TEXTURE_MAG_FILTER_MODE_LINEAR
};

enum AnimaTextureAttachment
{
	TEXTURE_ATTACHMENT_NONE = 0, TEXTURE_ATTACHMENT_DEPTH, TEXTURE_ATTACHMENT_STENCIL, TEXTURE_ATTACHMENT_DEPTH_STENCIL, 
	TEXTURE_ATTACHMENT_COLOR0, TEXTURE_ATTACHMENT_COLOR1, TEXTURE_ATTACHMENT_COLOR2, TEXTURE_ATTACHMENT_COLOR3,
	TEXTURE_ATTACHMENT_COLOR4, TEXTURE_ATTACHMENT_COLOR5, TEXTURE_ATTACHMENT_COLOR6, TEXTURE_ATTACHMENT_COLOR7,
	TEXTURE_ATTACHMENT_COLOR8, TEXTURE_ATTACHMENT_COLOR9, TEXTURE_ATTACHMENT_COLOR10, TEXTURE_ATTACHMENT_COLOR11,
	TEXTURE_ATTACHMENT_COLOR12, TEXTURE_ATTACHMENT_COLOR13, TEXTURE_ATTACHMENT_COLOR14, TEXTURE_ATTACHMENT_COLOR15
};

enum AnimaTextureDataType
{
	TEXTURE_DATA_TYPE_UNSIGNED_BYTE, TEXTURE_DATA_TYPE_BYTE, TEXTURE_DATA_TYPE_UNSIGNED_SHORT, TEXTURE_DATA_TYPE_SHORT, 
	TEXTURE_DATA_TYPE_UNSIGNED_INT, TEXTURE_DATA_TYPE_INT, TEXTURE_DATA_TYPE_FLOAT, TEXTURE_DATA_TYPE_UNSIGNED_BYTE_3_3_2,
	TEXTURE_DATA_TYPE_UNSIGNED_BYTE_2_3_3_REV, TEXTURE_DATA_TYPE_UNSIGNED_SHORT_5_6_5, TEXTURE_DATA_TYPE_UNSIGNED_SHORT_5_6_5_REV,
	TEXTURE_DATA_TYPE_UNSIGNED_SHORT_4_4_4_4, TEXTURE_DATA_TYPE_UNSIGNED_SHORT_4_4_4_4_REV, TEXTURE_DATA_TYPE_UNSIGNED_SHORT_5_5_5_1,
	TEXTURE_DATA_TYPE_UNSIGNED_SHORT_1_5_5_5_REV, TEXTURE_DATA_TYPE_UNSIGNED_INT_8_8_8_8, TEXTURE_DATA_TYPE_UNSIGNED_INT_8_8_8_8_REV,
	TEXTURE_DATA_TYPE_UNSIGNED_INT_10_10_10_2, TEXTURE_DATA_TYPE_UNSIGNED_INT_2_10_10_10_REV, TEXTURE_DATA_TYPE_UNSIGNED_INT_24_8
};

enum AnimaTextureMaxSurfaces
{
	TEXUTRE_MAX_SURFACES = 16
};

class AnimaTextureSurface;

class ANIMA_ENGINE_EXPORT AnimaTexture : public AnimaNamedObject
{
	DECLARE_ANIMA_CLASS(AnimaTexture);

public:
	AnimaTexture(AnimaAllocator* allocator, const AnimaString& sourceFileName = "");
	AnimaTexture(AnimaAllocator* allocator, const AnimaString& name, AUint width, AUint height, const AnimaString& sourceFileName = "");
	AnimaTexture(AnimaAllocator* allocator, const AnimaString& name, AUint width, AUint height, AUint depth, const AnimaString& sourceFileName = "");
	AnimaTexture(AnimaAllocator* allocator, const AnimaString& name, AUint width, AUint height, AUchar* data, AUint dataSize, const AnimaString& sourceFileName = "");
	AnimaTexture(AnimaAllocator* allocator, const AnimaString& name, AUint width, AUint height, AUint depth, AUchar* data, AUint dataSize, const AnimaString& sourceFileName = "");
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

	void SetDepth(AUint depth);
	AUint GetDepth() const;

	void SetMipMapLevels(AUint levels);
	AUint GetMipMapLevels() const;

	void SetGenerateMipMap(bool generate);
	bool IsGenerateMipMap() const;

	void SetFormat(AnimaTextureFormat format);
	AnimaTextureFormat GetFormat() const;

	void SetMinFilter(AnimaTextureMinFilterMode filter);
	AnimaTextureMinFilterMode GetMinFilter() const;

	void SetMagFilter(AnimaTextureMagFilterMode filter);
	AnimaTextureMagFilterMode GetMagFilter() const;

	void SetAttachment(AnimaTextureAttachment attachment);
	AnimaTextureAttachment GetAttachment() const;

	void SetInternalFormat(AnimaTextureInternalFormat internalFormat);
	AnimaTextureInternalFormat GetInternalFormat() const;

	void SetDataType(AnimaTextureDataType dataType);
	AnimaTextureDataType GetDataType() const;
	
	void SetSourceFileName(const AnimaString& sourceFileName);
	AnimaString GetSourceFileName() const;

	/*!
	 *	\brief		Imposta il buffer con i dati della texture
	 *	\details	Imposta il buffer con i dati della texture
					I dati vengono copiati, quindi è possibile deallocare il buffer dopo la chiamata a questa funzione
	 *	\param[in]	data		Buffer con i dati della texture
	 *	\param[in]	dataSize	Lunghezza del buffer con i dati della texture
	 *	\param[in]	mipMapIndex	Indice della mipmap a cui appartengono i nuovi dati (compreso tra 0 e AnimaTextureMaxSurfaces::MAX_SURFACES)
	 *	\return		Torna true se il target della texture è stato impostato per essere non una texture 3D, false altrimenti
	 *	\author		Zille Marco
	 */
	bool SetData(AUchar* data, AUint dataSize, AUint mipMapIndex);

	/*!
	 *	\brief		Imposta il buffer con i dati della texture all'indice passato
	 *	\details	Imposta il buffer con i dati della texture all'indice passato. Serve per impostare i dati delle varie texture nel caso
					di texture 3D.
					I dati vengono copiati, quindi è possibile deallocare il buffer dopo la chiamata a questa funzione
	 *	\param[in]	data		Buffer con i dati della texture
	 *	\param[in]	dataSize	Lunghezza del buffer con i dati della texture
	 *	\param[in]	index		Indice su cui andare a copiare il buffer
	 *	\param[in]	mipMapIndex	Indice della mipmap a cui appartengono i nuovi dati (compreso tra 0 e AnimaTextureMaxSurfaces::MAX_SURFACES)
	 *	\return		Torna true se il target della texture è stato impostato per essere una texture 3D e l'indice è valido, false altrimenti
	 *	\author		Zille Marco
	 */
	bool SetData(AUchar* data, AUint dataSize, AnimaTextureCubeIndex index, AUint mipMapIndex);
	const AUchar* GetData(AUint surfaceIndex) const;
	AnimaArray<AUchar>* GetDataAsArray(AUint surfaceIndex);
	const AUchar* GetData(AnimaTextureCubeIndex index, AUint surfaceIndex) const;
	AnimaArray<AUchar>* GetDataAsArray(AnimaTextureCubeIndex index, AUint surfaceIndex);

	void SetTextureTarget(AnimaTextureTarget target);
	AnimaTextureTarget GetTextureTarget() const;

	void SetClampS(AnimaTextureClampMode clamp);
	AnimaTextureClampMode GetClampS() const;

	void SetClampT(AnimaTextureClampMode clamp);
	AnimaTextureClampMode GetClampT() const;

	void SetClampR(AnimaTextureClampMode clamp);
	AnimaTextureClampMode GetClampR() const;

	void SetBorderColor(const AnimaColor4f& color);
	void SetBorderColor(const AFloat& r, const AFloat& g, const AFloat& b, const AFloat& a);
	AnimaColor4f GetBorderColor() const;
	
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
	AUint _depth;

	AUint _textureID;
	AUint _frameBuffer;
	AUint _renderBuffer;
	
	AnimaString _sourceFileName;

	AnimaArray<AnimaArray<AnimaTextureSurface> > _faces;
		
	AnimaTextureTarget			_textureTarget;
	AnimaTextureMinFilterMode	_minFilter;
	AnimaTextureMagFilterMode	_magFilter;
	AnimaTextureFormat			_format;
	AnimaTextureInternalFormat	_internalFormat;
	AnimaTextureAttachment		_attachment;
	AnimaTextureDataType		_dataType;
	AnimaTextureClampMode		_clampS;
	AnimaTextureClampMode		_clampT;
	AnimaTextureClampMode		_clampR;
	
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
	static AUint MinFilterToPlatform(const AnimaTextureMinFilterMode& filter);
	static AUint MagFilterToPlatform(const AnimaTextureMagFilterMode& filter);
	static AUint CubeIndexToPlatform(const AnimaTextureCubeIndex& index);
	static AUint FormatToPlatform(const AnimaTextureFormat& format);
	static AUint InternalFormatToPlatform(const AnimaTextureInternalFormat& internalFormat);
	static AUint AttachmentToPlatform(const AnimaTextureAttachment& attachment);
	static AUint DataTypeToPlatform(const AnimaTextureDataType& dataType);
};

class ANIMA_ENGINE_EXPORT AnimaTextureSurface
{
public:
	AnimaTextureSurface();
	AnimaTextureSurface(AUint width, AUint height, AUchar* data, AUint dataSize);
	AnimaTextureSurface(const AnimaTextureSurface& src);
	AnimaTextureSurface(AnimaTextureSurface&& src);
	~AnimaTextureSurface();

	AnimaTextureSurface& operator=(const AnimaTextureSurface& src);
	AnimaTextureSurface& operator=(AnimaTextureSurface&& src);

public:
	void SetData(AUchar* data, AUint dataSize);
	void CopyData(const AnimaTextureSurface& src);
	const AUchar* GetData() const;
	AnimaArray<AUchar>* GetDataAsArray();

	void SetWidth(AUint width);
	AUint GetWidth() const;

	void SetHeight(AUint height);
	AUint GetHeight() const;
	
protected:
	AnimaArray<AUchar> _data;

	AUint _width;
	AUint _height;
};

END_ANIMA_ENGINE_NAMESPACE