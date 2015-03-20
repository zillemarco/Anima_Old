//
//  AnimaLight.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaLight.h"
#include "AnimaDataGeneratorsManager.h"
#include "AnimaShaderProgram.h"
#include "AnimaShadersManager.h"
#include "AnimaMath.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

//----------------------------------------------------------------
//						ANIMA LIGHT
//----------------------------------------------------------------
AnimaLight::AnimaLight(AnimaAllocator* allocator, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name)
	: AnimaSceneObject(name, dataGeneratorManager, allocator)
{
	_shadowTexture = AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*_allocator, _allocator, GL_TEXTURE_2D, 1024, 1024, nullptr, 0, 0, GL_LINEAR, GL_RG32F, GL_RGBA, GL_UNSIGNED_BYTE, GL_CLAMP_TO_EDGE, GL_COLOR_ATTACHMENT0);
	_tempShadowTexture = AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*_allocator, _allocator, GL_TEXTURE_2D, 1024, 1024, nullptr, 0, 0, GL_LINEAR, GL_RG32F, GL_RGBA, GL_UNSIGNED_BYTE, GL_CLAMP_TO_EDGE, GL_COLOR_ATTACHMENT0);
	
	ComputeProjectionMatrix();
	ComputeViewMatrix();

	AnimaSceneObject::SetColor("Color", 1.0f, 1.0f, 1.0f);
	AnimaSceneObject::SetFloat("Intensity", 1.0f);
}

AnimaLight::AnimaLight(const AnimaLight& src)
	: AnimaSceneObject(src)
{
	_type = src._type;
	_shadowTexture = src._shadowTexture;
	_tempShadowTexture = src._tempShadowTexture;
	_viewMatrix = src._viewMatrix;
	_projectionMatrix = src._projectionMatrix;
	_projectionViewMatrix = src._projectionViewMatrix;
}

AnimaLight::AnimaLight(AnimaLight&& src)
	: AnimaSceneObject(src)
{
	_type = src._type;
	_shadowTexture = src._shadowTexture;
	_tempShadowTexture = src._tempShadowTexture;
	_viewMatrix = src._viewMatrix;
	_projectionMatrix = src._projectionMatrix;
	_projectionViewMatrix = src._projectionViewMatrix;
}

AnimaLight::~AnimaLight()
{
	if (_shadowTexture != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_allocator, _shadowTexture);
		_shadowTexture = nullptr;
	}

	if (_tempShadowTexture != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_allocator, _tempShadowTexture);
		_tempShadowTexture = nullptr;
	}
}

AnimaLight& AnimaLight::operator=(const AnimaLight& src)
{
	AnimaSceneObject::operator=(src);
	_type = src._type;
	_shadowTexture = src._shadowTexture;
	_tempShadowTexture = src._tempShadowTexture;
	_viewMatrix = src._viewMatrix;
	_projectionMatrix = src._projectionMatrix;
	_projectionViewMatrix = src._projectionViewMatrix;
	return *this;
}

AnimaLight& AnimaLight::operator=(AnimaLight&& src)
{
	AnimaSceneObject::operator=(src);
	_type = src._type;
	_shadowTexture = src._shadowTexture;
	_tempShadowTexture = src._tempShadowTexture;
	_viewMatrix = src._viewMatrix;
	_projectionMatrix = src._projectionMatrix;
	_projectionViewMatrix = src._projectionViewMatrix;
	return *this;
}

void AnimaLight::SetColor(const AnimaColor3f& color)
{
	AnimaSceneObject::SetColor("Color", color);
}

void AnimaLight::SetColor(AFloat r, AFloat g, AFloat b)
{
	AnimaSceneObject::SetColor("Color", r, g, b);
}

void AnimaLight::SetIntensity(AFloat intensity)
{
	AnimaSceneObject::SetFloat("Intensity", intensity);
}

void AnimaLight::SetDirection(const AnimaVertex3f& direction)
{
	ANIMA_ASSERT(false);
}

void AnimaLight::SetDirection(AFloat x, AFloat y, AFloat z)
{
	ANIMA_ASSERT(false);
}

AnimaColor3f AnimaLight::GetColor()
{
	return AnimaSceneObject::GetColor3f("Color");
}

AFloat AnimaLight::GetIntensity()
{
	return AnimaSceneObject::GetFloat("Intensity");
}

AnimaVertex3f AnimaLight::GetDirection()
{
	ANIMA_ASSERT(false);
	return AnimaVertex3f();
}

void AnimaLight::SetConstantAttenuation(AFloat attenuation)
{
	ANIMA_ASSERT(false);
}

void AnimaLight::SetLinearAttenuation(AFloat attenuation)
{
	ANIMA_ASSERT(false);
}

void AnimaLight::SetExponentAttenuation(AFloat attenuation)
{
	ANIMA_ASSERT(false);
}

AFloat AnimaLight::GetConstantAttenuation()
{
	ANIMA_ASSERT(false);
	return 0.0f;
}

AFloat AnimaLight::GetLinearAttenuation()
{
	ANIMA_ASSERT(false);
	return 0.0f;
}

AFloat AnimaLight::GetExponentAttenuation()
{
	ANIMA_ASSERT(false);
	return 0.0f;
}

void AnimaLight::SetRange(AFloat range)
{
	ANIMA_ASSERT(false);
}

AFloat AnimaLight::GetRange()
{
	ANIMA_ASSERT(false);
	return 0.0f;
}

void AnimaLight::SetCutoff(AFloat c)
{
	ANIMA_ASSERT(false);
}

AFloat AnimaLight::GetCutoff()
{
	ANIMA_ASSERT(false);
	return 0.0f;
}

bool AnimaLight::IsDirectionalLight()
{
	return _type == DIRECTIONAL;
}

bool AnimaLight::IsPointLight()
{
	return _type == POINT;
}

bool AnimaLight::IsSpotLight()
{
	return _type == SPOT;
}

AnimaTexture* AnimaLight::GetShadowTexture()
{
	return _shadowTexture;
}

AnimaTexture* AnimaLight::GetTempShadowTexture()
{
	return _tempShadowTexture;
}

AnimaMatrix AnimaLight::GetViewMatrix()
{
	return _viewMatrix;
}

AnimaMatrix AnimaLight::GetProjectionMatrix()
{
	return _projectionMatrix;
}

AnimaMatrix AnimaLight::GetProjectionViewMatrix()
{
	return _projectionViewMatrix;
}

void AnimaLight::ComputeProjectionMatrix()
{
	_viewMatrix.SetIdentity();
	_projectionMatrix.SetIdentity();
	_projectionViewMatrix.SetIdentity();
}

void AnimaLight::ComputeViewMatrix()
{
	_viewMatrix.SetIdentity();
	_projectionMatrix.SetIdentity();
	_projectionViewMatrix.SetIdentity();
}

//----------------------------------------------------------------
//						ANIMA DIRECTIONAL LIGHT
//----------------------------------------------------------------
AnimaDirectionalLight::AnimaDirectionalLight(AnimaAllocator* allocator, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name)
	: AnimaLight(allocator, dataGeneratorManager, name)
{
	AnimaLight::SetVector("Direction", AnimaVertex3f(-1.0f, -1.0f, -1.0f).Normalized());
	
	ComputeProjectionMatrix();
	ComputeViewMatrix();

	_type = DIRECTIONAL;
}

AnimaDirectionalLight::~AnimaDirectionalLight()
{
}

void AnimaDirectionalLight::SetDirection(const AnimaVertex3f& direction)
{
	AnimaSceneObject::SetVector("Direction", direction.Normalized());
	ComputeViewMatrix();
}

void AnimaDirectionalLight::SetDirection(AFloat x, AFloat y, AFloat z)
{
	AnimaSceneObject::SetVector("Direction", AnimaVertex3f(x, y, z).Normalized());
	ComputeViewMatrix();
}

AnimaVertex3f AnimaDirectionalLight::GetDirection()
{
	return AnimaSceneObject::GetVector3f("Direction");
}

void AnimaDirectionalLight::ComputeViewMatrix()
{
	_viewMatrix.LookAt(AnimaVertex3f(0.0f, 0.0f, 0.0f), GetDirection(), AnimaVertex3f(0.0f, 1.0f, 0.0f));
	_projectionViewMatrix = _projectionMatrix * _viewMatrix;
}

void AnimaDirectionalLight::ComputeProjectionMatrix()
{
	float t = 20.0f;
	_projectionMatrix = AnimaMatrix::MakeOrtho(-t, t, -t, t, -1000.0f, 1000.0f);
	_projectionViewMatrix = _projectionMatrix * _viewMatrix;
}

void AnimaDirectionalLight::UpdateMeshTransformation(AnimaTransformation* meshTransformation)
{
	meshTransformation->SetRotationDeg(90.0f, 0.0f, 0.0f);
	meshTransformation->SetScale(1.0f, 1.0f, 1.0f);
	meshTransformation->SetTranslation(0.0f, 0.0f, 0.0f);
}

void AnimaDirectionalLight::UpdateCullFace(AnimaCamera* activeCamera)
{
}

const char* AnimaDirectionalLight::GetShaderPrefix()
{
	return "DIL";
}

const char* AnimaDirectionalLight::GetShaderName()
{
	return "deferred-directional";
}

bool AnimaDirectionalLight::CreateShader(AnimaShadersManager* shadersManager)
{
	if (shadersManager->GetProgramFromName("deferred-directional"))
		return true;

	AnimaShaderProgram* pgr = shadersManager->CreateProgram("deferred-directional");

	if (pgr == nullptr)
		return false;

	if (!pgr->Create())
		return false;

	pgr->AddShader(shadersManager->LoadShaderFromFile("deferred-directional-vs", ANIMA_ENGINE_SHADERS_PATH "Deferred/deferred-directional-vs.glsl", Anima::AnimaShader::VERTEX));
	pgr->AddShader(shadersManager->LoadShaderFromFile("deferred-directional-fs", ANIMA_ENGINE_SHADERS_PATH "Deferred/deferred-directional-fs.glsl", Anima::AnimaShader::FRAGMENT));

	if (!pgr->Link())
		return false;

	return true;
}

//----------------------------------------------------------------
//						ANIMA POINT LIGHT
//----------------------------------------------------------------
AnimaPointLight::AnimaPointLight(AnimaAllocator* allocator, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name)
	: AnimaLight(allocator, dataGeneratorManager, name)
{
	AnimaSceneObject::SetFloat("ConstantAttenuation", 0.0f);
	AnimaSceneObject::SetFloat("LinearAttenuation", 0.0f);
	AnimaSceneObject::SetFloat("ExponentAttenuation", 1.0f);
	AnimaSceneObject::SetFloat("Range", 20.0f);

	_type = POINT;
}

AnimaPointLight::~AnimaPointLight()
{
}

void AnimaPointLight::SetConstantAttenuation(AFloat attenuation)
{
	AnimaSceneObject::SetFloat("ConstantAttenuation", attenuation);
}

void AnimaPointLight::SetLinearAttenuation(AFloat attenuation)
{
	AnimaSceneObject::SetFloat("LinearAttenuation", attenuation);
}

void AnimaPointLight::SetExponentAttenuation(AFloat attenuation)
{
	AnimaSceneObject::SetFloat("ExponentAttenuation", attenuation);
}

void AnimaPointLight::SetRange(AFloat range)
{
	AnimaSceneObject::SetFloat("Range", range);
}

AFloat AnimaPointLight::GetConstantAttenuation()
{
	return AnimaSceneObject::GetFloat("ConstantAttenuation");
}

AFloat AnimaPointLight::GetLinearAttenuation()
{
	return AnimaSceneObject::GetFloat("LinearAttenuation");
}

AFloat AnimaPointLight::GetExponentAttenuation()
{
	return AnimaSceneObject::GetFloat("ExponentAttenuation");
}

AFloat AnimaPointLight::GetRange()
{
	return AnimaSceneObject::GetFloat("Range");
}

void AnimaPointLight::UpdateMeshTransformation(AnimaTransformation* meshTransformation)
{
	AFloat range = GetRange();
	AnimaVertex3f position = GetPosition();

	meshTransformation->SetRotationDeg(0.0f, 0.0f, 0.0f);
	meshTransformation->SetScale(range, range, range);
	meshTransformation->SetTranslation(position);
}

void AnimaPointLight::UpdateCullFace(AnimaCamera* activeCamera)
{
	if (AnimaMath::PointInsideSphere(activeCamera->GetPosition(), GetPosition(), GetRange()))
		glCullFace(GL_FRONT);
	else
		glCullFace(GL_BACK);
}

const char* AnimaPointLight::GetShaderPrefix()
{
	return "PTL";
}

const char* AnimaPointLight::GetShaderName()
{
	return "deferred-point";
}

bool AnimaPointLight::CreateShader(AnimaShadersManager* shadersManager)
{
	if (shadersManager->GetProgramFromName("deferred-point"))
		return true;

	AnimaShaderProgram* pgr = shadersManager->CreateProgram("deferred-point");

	if (pgr == nullptr)
		return false;

	if (!pgr->Create())
		return false;

	pgr->AddShader(shadersManager->LoadShaderFromFile("deferred-point-vs", ANIMA_ENGINE_SHADERS_PATH "Deferred/deferred-point-vs.glsl", Anima::AnimaShader::VERTEX));
	pgr->AddShader(shadersManager->LoadShaderFromFile("deferred-point-fs", ANIMA_ENGINE_SHADERS_PATH "Deferred/deferred-point-fs.glsl", Anima::AnimaShader::FRAGMENT));

	if (!pgr->Link())
		return false;

	return true;
}

//----------------------------------------------------------------
//						ANIMA SPOT LIGHT
//----------------------------------------------------------------
AnimaSpotLight::AnimaSpotLight(AnimaAllocator* allocator, AnimaDataGeneratorsManager* dataGeneratorManager,  const AnimaString& name)
	: AnimaPointLight(allocator, dataGeneratorManager, name)
{
	AnimaSceneObject::SetVector("Direction", AnimaVertex3f(0.0f, -1.0f, 0.0f).Normalized());
	AnimaSceneObject::SetFloat("Cutoff", 0.7f);

	_type = SPOT;
}

AnimaSpotLight::~AnimaSpotLight()
{
}

void AnimaSpotLight::SetDirection(const AnimaVertex3f& direction)
{
	AnimaSceneObject::SetVector("Direction", direction.Normalized());
	UpdateConeRotation();
}

void AnimaSpotLight::SetDirection(AFloat x, AFloat y, AFloat z)
{
	AnimaSceneObject::SetVector("Direction", AnimaVertex3f(x, y, z).Normalized());
	UpdateConeRotation();
}

void AnimaSpotLight::SetCutoff(AFloat c)
{
	AnimaSceneObject::SetFloat("Cutoff", c);
}

AnimaVertex3f AnimaSpotLight::GetDirection()
{
	return AnimaSceneObject::GetVector3f("Direction");
}

AFloat AnimaSpotLight::GetCutoff()
{
	return AnimaSceneObject::GetFloat("Cutoff");
}

void AnimaSpotLight::UpdateMeshTransformation(AnimaTransformation* meshTransformation)
{
	AFloat range = GetRange();
	AFloat raggio = range * tanf(acosf(GetCutoff()) / 2.0f);
	
	meshTransformation->SetRotation(_coneRotation);
	meshTransformation->SetScale(raggio, range, raggio);
	meshTransformation->SetTranslation(GetPosition());
}

void AnimaSpotLight::UpdateCullFace(AnimaCamera* activeCamera)
{
	if (AnimaMath::PointInsideCone(activeCamera->GetPosition(), GetPosition(), GetDirection(), GetRange(), acosf(GetCutoff()) / 2.0f))
		glCullFace(GL_FRONT);
	else
		glCullFace(GL_BACK);
}

void AnimaSpotLight::UpdateConeRotation()
{
	Anima::AnimaVertex3f A = GetDirection();
	
	// Questa è la direzione in cui punta i cono quando creato
	Anima::AnimaVertex3f B(0.0f, -1.0f, 0.0f);
	
	A.Normalize();
	
	Anima::AnimaVertex3f v = A ^ B;
	float s = v.Length2();
	float c = A * B;
	
	Anima::AnimaMatrix vx;
	vx.x[0] = 0.0f;	vx.x[1] = v.z;	vx.x[2] = -v.y;
	vx.y[0] = -v.z;	vx.y[1] = 0.0f;	vx.y[2] = v.x;
	vx.z[0] = v.y;	vx.z[1] = -v.x;	vx.z[2] = 0.0f;
	
	Anima::AnimaMatrix vx2 = vx * vx;
	
	Anima::AnimaMatrix m;
	m += vx + (vx2 * ((1 - c) / s));
	
	_coneRotation = m.GetRotationAxes();
}

const char* AnimaSpotLight::GetShaderPrefix()
{
	return "SPL";
}

const char* AnimaSpotLight::GetShaderName()
{
	return "deferred-spot";
}

bool AnimaSpotLight::CreateShader(AnimaShadersManager* shadersManager)
{
	if (shadersManager->GetProgramFromName("deferred-spot"))
		return true;
	
	AnimaShaderProgram* pgr = shadersManager->CreateProgram("deferred-spot");
	
	if (pgr == nullptr)
		return false;
	
	if (!pgr->Create())
		return false;
	
	pgr->AddShader(shadersManager->LoadShaderFromFile("deferred-spot-vs", ANIMA_ENGINE_SHADERS_PATH "Deferred/deferred-spot-vs.glsl", Anima::AnimaShader::VERTEX));
	pgr->AddShader(shadersManager->LoadShaderFromFile("deferred-spot-fs", ANIMA_ENGINE_SHADERS_PATH "Deferred/deferred-spot-fs.glsl", Anima::AnimaShader::FRAGMENT));
	
	if (!pgr->Link())
		return false;
	
	return true;
}

END_ANIMA_ENGINE_NAMESPACE
