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
#include "AnimaSceneObject.h"
#include "AnimaMesh.h"
#include "AnimaTexture.h"
#include "AnimaCamera.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaShaderProgram;
class AnimaShadersManager;

//----------------------------------------------------------------
//						ANIMA LIGHT
//----------------------------------------------------------------
class ANIMA_ENGINE_EXPORT AnimaLight : public AnimaSceneObject
{
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

	AnimaTexture* GetShadowTexture();
	AnimaTexture* GetTempShadowTexture();

	virtual AnimaMatrix GetViewMatrix();
	virtual AnimaMatrix GetProjectionMatrix();
	virtual AnimaMatrix GetProjectionViewMatrix();

	virtual void UpdateMeshTransformation(AnimaTransformation* meshTransformation) = 0;
	virtual void UpdateCullFace(AnimaCamera* activeCamera) = 0;

	virtual const char* GetShaderName() = 0;
	virtual bool CreateShader(AnimaShadersManager* shadersManager) = 0;

protected:
};

//----------------------------------------------------------------
//						ANIMA DIRECTIONAL LIGHT
//----------------------------------------------------------------
class ANIMA_ENGINE_EXPORT AnimaDirectionalLight : public AnimaLight
{
public:
	AnimaDirectionalLight(AnimaAllocator* allocator, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name);
	virtual ~AnimaDirectionalLight();
	
	void SetDirection(const AnimaVertex3f& direction) override;
	void SetDirection(AFloat x, AFloat y, AFloat z) override;
	
	AnimaVertex3f GetDirection() override;
	
	void ComputeViewMatrix() override;
	void ComputeProjectionMatrix() override;

	void UpdateMeshTransformation(AnimaTransformation* meshTransformation) override;
	void UpdateCullFace(AnimaCamera* activeCamera) override;

	const char* GetShaderPrefix() override;
	const char* GetShaderName() override;
	bool CreateShader(AnimaShadersManager* shadersManager) override;
};

//----------------------------------------------------------------
//						ANIMA POINT LIGHT
//----------------------------------------------------------------
class ANIMA_ENGINE_EXPORT AnimaPointLight : public AnimaLight
{
public:
	AnimaPointLight(AnimaAllocator* allocator, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name);
	virtual ~AnimaPointLight();

	void SetConstantAttenuation(AFloat attenuation) override;
	void SetLinearAttenuation(AFloat attenuation) override;
	void SetExponentAttenuation(AFloat attenuation) override;
	void SetRange(AFloat range) override;

	AFloat GetConstantAttenuation() override;
	AFloat GetLinearAttenuation() override;
	AFloat GetExponentAttenuation() override;
	AFloat GetRange() override;
	
	void UpdateMeshTransformation(AnimaTransformation* meshTransformation) override;
	void UpdateCullFace(AnimaCamera* activeCamera) override;

	const char* GetShaderPrefix() override;
	const char* GetShaderName() override;
	bool CreateShader(AnimaShadersManager* shadersManager) override;
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

	void UpdateMeshTransformation(AnimaTransformation* meshTransformation) override;
	void UpdateCullFace(AnimaCamera* activeCamera) override;

	const char* GetShaderPrefix() override;
	const char* GetShaderName() override;
	bool CreateShader(AnimaShadersManager* shadersManager) override;
	
protected:
	void UpdateConeRotation();
	
protected:
	AnimaVertex3f _coneRotation;
};

//----------------------------------------------------------------
//						ANIMA HEMISPHERE LIGHT
//----------------------------------------------------------------
class ANIMA_ENGINE_EXPORT AnimaHemisphereLight : public AnimaLight
{
public:
	AnimaHemisphereLight(AnimaAllocator* allocator, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name);
	virtual ~AnimaHemisphereLight();

public:
	void SetSkyColor(const AnimaColor3f& color);
	void SetSkyColor(AFloat r, AFloat g, AFloat b);
	AnimaColor3f GetSkyColor();

	void SetGroundColor(const AnimaColor3f& color);
	void SetGroundColor(AFloat r, AFloat g, AFloat b);
	AnimaColor3f GetGroundColor();

public:
	void UpdateMeshTransformation(AnimaTransformation* meshTransformation) override;
	void UpdateCullFace(AnimaCamera* activeCamera) override;

	const char* GetShaderPrefix() override;
	const char* GetShaderName() override;
	bool CreateShader(AnimaShadersManager* shadersManager) override;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaLight__) */
