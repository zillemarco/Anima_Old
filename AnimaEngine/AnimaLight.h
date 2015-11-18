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
#include "AnimaGeometry.h"
#include "AnimaTexture.h"
#include "AnimaCamera.h"
#include "AnimaFrustum.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaShaderProgram;
class AnimaShadersManager;

//----------------------------------------------------------------
//						ANIMA LIGHT
//----------------------------------------------------------------
class ANIMA_ENGINE_EXPORT AnimaLight : public AnimaSceneObject
{
	DECLARE_ANIMA_CLASS(AnimaLight);

protected:
	AnimaLight(AnimaAllocator* allocator, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name);
	AnimaLight(const AnimaLight& src);
	AnimaLight(AnimaLight&& src);

public:
	virtual ~AnimaLight();

	AnimaLight& operator=(const AnimaLight& src);
	AnimaLight& operator=(AnimaLight&& src);
	
public:
	ptree GetObjectTree(bool saveName = true) const override;
	bool ReadObject(const ptree& objectTree, AnimaScene* scene, bool readName = true) override;

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

	virtual void SetExtension(const AnimaVertex3f& extension);
	virtual AnimaVertex3f GetExtension() const;

	virtual void SetRange(AFloat range);
	virtual AFloat GetRange();

	virtual void SetCutoff(AFloat c);
	virtual AFloat GetCutoff();

	virtual void ComputeLightMatrix(AnimaCamera* activeCamera);
	
	virtual AnimaMatrix GetViewMatrix();
	virtual AnimaMatrix GetProjectionMatrix();
	virtual AnimaMatrix GetProjectionViewMatrix();

	AnimaFrustum* GetFrustum();

	virtual void UpdateCullFace(AnimaCamera* activeCamera) = 0;
	
	virtual const char* GetShaderName() = 0;
	virtual bool CreateShader(AnimaShadersManager* shadersManager) = 0;

protected:
	virtual void UpdateLightGeometryMatrix();

protected:
	AnimaFrustum _frustum;
};

//----------------------------------------------------------------
//						ANIMA DIRECTIONAL LIGHT
//----------------------------------------------------------------
class ANIMA_ENGINE_EXPORT AnimaDirectionalLight : public AnimaLight
{
	DECLARE_ANIMA_CLASS(AnimaDirectionalLight);

public:
	AnimaDirectionalLight(AnimaAllocator* allocator, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name);
	virtual ~AnimaDirectionalLight();
	
public:
	ptree GetObjectTree(bool saveName = true) const override;
	bool ReadObject(const ptree& objectTree, AnimaScene* scene, bool readName = true) override;
	
public:
	void SetDirection(const AnimaVertex3f& direction) override;
	void SetDirection(AFloat x, AFloat y, AFloat z) override;
	
	AnimaVertex3f GetDirection() override;
	
	void ComputeLightMatrix(AnimaCamera* activeCamera) override;

	void UpdateCullFace(AnimaCamera* activeCamera) override;

	const char* GetShaderName() override;
	bool CreateShader(AnimaShadersManager* shadersManager) override;

protected:
	void UpdateLightGeometryMatrix() override;
};

//----------------------------------------------------------------
//						ANIMA POINT LIGHT
//----------------------------------------------------------------
class ANIMA_ENGINE_EXPORT AnimaPointLight : public AnimaLight
{
	DECLARE_ANIMA_CLASS(AnimaPointLight);

public:
	AnimaPointLight(AnimaAllocator* allocator, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name);
	virtual ~AnimaPointLight();
	
public:
	ptree GetObjectTree(bool saveName = true) const override;
	bool ReadObject(const ptree& objectTree, AnimaScene* scene, bool readName = true) override;
	
public:
	void SetConstantAttenuation(AFloat attenuation) override;
	void SetLinearAttenuation(AFloat attenuation) override;
	void SetExponentAttenuation(AFloat attenuation) override;
	void SetRange(AFloat range) override;

	AFloat GetConstantAttenuation() override;
	AFloat GetLinearAttenuation() override;
	AFloat GetExponentAttenuation() override;
	AFloat GetRange() override;
	
	void UpdateCullFace(AnimaCamera* activeCamera) override;

	const char* GetShaderName() override;
	bool CreateShader(AnimaShadersManager* shadersManager) override;

	virtual void SetPosition(AFloat x, AFloat y, AFloat z);

protected:
	void UpdateLightGeometryMatrix() override;
};

//----------------------------------------------------------------
//						ANIMA SPOT LIGHT
//----------------------------------------------------------------
class ANIMA_ENGINE_EXPORT AnimaSpotLight : public AnimaPointLight
{
	DECLARE_ANIMA_CLASS(AnimaSpotLight);

public:
	AnimaSpotLight(AnimaAllocator* allocator, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name);
	virtual ~AnimaSpotLight();
	
public:
	ptree GetObjectTree(bool saveName = true) const override;
	bool ReadObject(const ptree& objectTree, AnimaScene* scene, bool readName = true) override;
	
public:
	void SetDirection(const AnimaVertex3f& position) override;
	void SetDirection(AFloat x, AFloat y, AFloat z) override;
	void SetCutoff(AFloat c) override;

	AnimaVertex3f GetDirection() override;
	AFloat GetCutoff() override;
	
	void UpdateCullFace(AnimaCamera* activeCamera) override;

	const char* GetShaderName() override;
	bool CreateShader(AnimaShadersManager* shadersManager) override;
	
protected:
	void UpdateConeRotation();

protected:
	void UpdateLightGeometryMatrix() override;
	
protected:
	AnimaVertex3f _coneRotation;
};

//----------------------------------------------------------------
//						ANIMA HEMISPHERE LIGHT
//----------------------------------------------------------------
class ANIMA_ENGINE_EXPORT AnimaHemisphereLight : public AnimaLight
{
	DECLARE_ANIMA_CLASS(AnimaHemisphereLight);

public:
	AnimaHemisphereLight(AnimaAllocator* allocator, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name);
	virtual ~AnimaHemisphereLight();
	
public:
	ptree GetObjectTree(bool saveName = true) const override;
	bool ReadObject(const ptree& objectTree, AnimaScene* scene, bool readName = true) override;
	
public:
	void SetSkyColor(const AnimaColor3f& color);
	void SetSkyColor(AFloat r, AFloat g, AFloat b);
	AnimaColor3f GetSkyColor();

	void SetGroundColor(const AnimaColor3f& color);
	void SetGroundColor(AFloat r, AFloat g, AFloat b);
	AnimaColor3f GetGroundColor();

protected:
	void UpdateLightGeometryMatrix() override;

public:
	void UpdateCullFace(AnimaCamera* activeCamera) override;

	const char* GetShaderName() override;
	bool CreateShader(AnimaShadersManager* shadersManager) override;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaLight__) */
