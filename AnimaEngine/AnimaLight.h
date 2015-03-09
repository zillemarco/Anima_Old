//
//  AnimaLight.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaLight__
#define __Anima__AnimaLight__

#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaMappedValues.h"
#include "AnimaMesh.h"
#include "AnimaTexture.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

//----------------------------------------------------------------
//						ANIMA LIGHT
//----------------------------------------------------------------
class ANIMA_ENGINE_EXPORT AnimaLight : protected AnimaMappedValues
{
public:
	enum AnimaLightType {
		AMBIENT,
		DIRECTIONAL,
		POINT,
		SPOT
	};

protected:
	AnimaLight(AnimaAllocator* allocator, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name);
	AnimaLight(const AnimaLight& src);
	AnimaLight(AnimaLight&& src);

public:
	virtual ~AnimaLight();

	AnimaLight& operator=(const AnimaLight& src);
	AnimaLight& operator=(AnimaLight&& src);

public:
	virtual void SetColor(const AnimaColor3f& color);
	virtual void SetColor(AFloat r, AFloat g, AFloat b);
	virtual AnimaColor3f GetColor();

	virtual void SetIntensity(AFloat intensity);
	virtual AFloat GetIntensity();

	virtual void SetDirection(const AnimaVertex3f& direction);
	virtual void SetDirection(AFloat x, AFloat y, AFloat z);
	virtual AnimaVertex3f GetDirection();

	virtual void SetPosition(const AnimaVertex3f& position);
	virtual void SetPosition(AFloat x, AFloat y, AFloat z);
	virtual AnimaVertex3f GetPosition();

	virtual void SetConstantAttenuation(AFloat attenuation);
	virtual void SetLinearAttenuation(AFloat attenuation);
	virtual void SetExponentAttenuation(AFloat attenuation);
	virtual AFloat GetConstantAttenuation();
	virtual AFloat GetLinearAttenuation();
	virtual AFloat GetExponentAttenuation();

	virtual void SetRange(AFloat range);
	virtual AFloat GetRange();

	virtual void SetCutoff(AFloat c);
	virtual AFloat GetCutoff();

	virtual void ComputeViewMatrix();
	virtual void ComputeProjectionMatrix();

	bool IsAmbientLight();
	bool IsDirectionalLight();
	bool IsPointLight();
	bool IsSpotLight();

	AnimaTexture* GetShadowTexture();
	AnimaTexture* GetTempShadowTexture();

	virtual AnimaMatrix GetViewMatrix();
	virtual AnimaMatrix GetProjectionMatrix();
	virtual AnimaMatrix GetProjectionViewMatrix();

protected:
	AnimaLightType	_type;
	AnimaTexture*	_shadowTexture;
	AnimaTexture*	_tempShadowTexture;
	AnimaMatrix		_viewMatrix;
	AnimaMatrix		_projectionMatrix;
	AnimaMatrix		_projectionViewMatrix;
};

//----------------------------------------------------------------
//						ANIMA AMBIENT LIGHT
//----------------------------------------------------------------
class ANIMA_ENGINE_EXPORT AnimaAmbientLight : public AnimaLight
{
public:
	AnimaAmbientLight(AnimaAllocator* allocator, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name);
	virtual ~AnimaAmbientLight();

	void SetColor(const AnimaColor3f& color) override;
	void SetColor(AFloat r, AFloat g, AFloat b) override;

	AnimaColor3f GetColor() override;
};

//----------------------------------------------------------------
//						ANIMA DIRECTIONAL LIGHT
//----------------------------------------------------------------
class ANIMA_ENGINE_EXPORT AnimaDirectionalLight : public AnimaAmbientLight
{
public:
	AnimaDirectionalLight(AnimaAllocator* allocator, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name);
	virtual ~AnimaDirectionalLight();
	
	void SetDirection(const AnimaVertex3f& direction) override;
	void SetDirection(AFloat x, AFloat y, AFloat z) override;
	void SetIntensity(AFloat intensity) override;

	AnimaVertex3f GetDirection() override;
	AFloat GetIntensity() override;

	void ComputeViewMatrix() override;
	void ComputeProjectionMatrix() override;
};

//----------------------------------------------------------------
//						ANIMA POINT LIGHT
//----------------------------------------------------------------
class ANIMA_ENGINE_EXPORT AnimaPointLight : public AnimaAmbientLight
{
public:
	AnimaPointLight(AnimaAllocator* allocator, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name);
	virtual ~AnimaPointLight();

	void SetPosition(const AnimaVertex3f& position) override;
	void SetPosition(AFloat x, AFloat y, AFloat z) override;
	void SetIntensity(AFloat intensity) override;
	void SetConstantAttenuation(AFloat attenuation) override;
	void SetLinearAttenuation(AFloat attenuation) override;
	void SetExponentAttenuation(AFloat attenuation) override;
	void SetRange(AFloat range) override;

	AnimaVertex3f GetPosition() override;
	AFloat GetIntensity() override;
	AFloat GetConstantAttenuation() override;
	AFloat GetLinearAttenuation() override;
	AFloat GetExponentAttenuation() override;
	AFloat GetRange() override;
};

//----------------------------------------------------------------
//						ANIMA SPOT LIGHT
//----------------------------------------------------------------
class ANIMA_ENGINE_EXPORT AnimaSpotLight : public AnimaPointLight
{
public:
	AnimaSpotLight(AnimaAllocator* allocator, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name);
	virtual ~AnimaSpotLight();

	void SetDirection(const AnimaVertex3f& position) override;
	void SetDirection(AFloat x, AFloat y, AFloat z) override;
	void SetCutoff(AFloat c) override;

	AnimaVertex3f GetDirection() override;
	AFloat GetCutoff() override;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaLight__) */
