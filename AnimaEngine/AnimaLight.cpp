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

BEGIN_ANIMA_ENGINE_NAMESPACE

//----------------------------------------------------------------
//						ANIMA LIGHT
//----------------------------------------------------------------
AnimaLight::AnimaLight(AnimaAllocator* allocator, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name)
	: AnimaMappedValues(allocator, dataGeneratorManager, name)
{
	_shadowTexture = AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*_allocator, _allocator, GL_TEXTURE_2D, 1024, 1024, nullptr, 0, 0, GL_LINEAR, GL_RG32F, GL_RGBA, GL_UNSIGNED_BYTE, GL_CLAMP_TO_EDGE, GL_COLOR_ATTACHMENT0);
	_tempShadowTexture = AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*_allocator, _allocator, GL_TEXTURE_2D, 1024, 1024, nullptr, 0, 0, GL_LINEAR, GL_RG32F, GL_RGBA, GL_UNSIGNED_BYTE, GL_CLAMP_TO_EDGE, GL_COLOR_ATTACHMENT0);
	ComputeProjectionMatrix();
	ComputeViewMatrix();
}

AnimaLight::AnimaLight(const AnimaLight& src)
	: AnimaMappedValues(src)
{
	_type = src._type;
	_shadowTexture = src._shadowTexture;
	_tempShadowTexture = src._tempShadowTexture;
	_viewMatrix = src._viewMatrix;
	_projectionMatrix = src._projectionMatrix;
	_projectionViewMatrix = src._projectionViewMatrix;
}

AnimaLight::AnimaLight(AnimaLight&& src)
	: AnimaMappedValues(src)
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
	AnimaMappedValues::operator=(src);
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
	AnimaMappedValues::operator=(src);
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
	ANIMA_ASSERT(false);
}

void AnimaLight::SetColor(AFloat r, AFloat g, AFloat b)
{
	ANIMA_ASSERT(false);
}

void AnimaLight::SetIntensity(AFloat intensity)
{
	ANIMA_ASSERT(false);
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
	ANIMA_ASSERT(false);
	return AnimaColor3f();
}

AFloat AnimaLight::GetIntensity()
{
	ANIMA_ASSERT(false);
	return 0.0f;
}

AnimaVertex3f AnimaLight::GetDirection()
{
	ANIMA_ASSERT(false);
	return AnimaVertex3f();
}

void AnimaLight::SetPosition(const AnimaVertex3f& position)
{
	ANIMA_ASSERT(false);
}

void AnimaLight::SetPosition(AFloat x, AFloat y, AFloat z)
{
	ANIMA_ASSERT(false);
}

AnimaVertex3f AnimaLight::GetPosition()
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

bool AnimaLight::IsAmbientLight()
{
	return _type == AMBIENT;
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
//						ANIMA AMBIENT LIGHT
//----------------------------------------------------------------
AnimaAmbientLight::AnimaAmbientLight(AnimaAllocator* allocator, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name)
	: AnimaLight(allocator, dataGeneratorManager, name)
{
	AnimaMappedValues::SetColor("color", 1.0f, 1.0f, 1.0f);
	AnimaMappedValues::SetFloat("intensity", 1.0f);
	
	ComputeProjectionMatrix();
	ComputeViewMatrix();
	
	_type = AMBIENT;
}

AnimaAmbientLight::~AnimaAmbientLight()
{
}

void AnimaAmbientLight::SetColor(const AnimaColor3f& color)
{
	AnimaMappedValues::SetColor("color", color);
}

void AnimaAmbientLight::SetColor(AFloat r, AFloat g, AFloat b)
{
	AnimaMappedValues::SetColor("color", r, g, b);
}

AnimaColor3f AnimaAmbientLight::GetColor()
{
	return AnimaMappedValues::GetColor3f("color");
}

void AnimaAmbientLight::UpdateMeshTransformation(AnimaTransformation* meshTransformation)
{
}

void AnimaAmbientLight::UpdateCullFace(AnimaCamera* activeCamera)
{
}

const char* AnimaAmbientLight::GetShaderPrefix()
{
	return "AML";
}

const char* AnimaAmbientLight::GetShaderName()
{
	return "";
}

bool AnimaAmbientLight::CreateShader(AnimaShadersManager* shadersManager)
{
	// La luce ambientale non ha uno shader (vale solo per il deferred shading [standard]) 
	return true;
}

//----------------------------------------------------------------
//						ANIMA DIRECTIONAL LIGHT
//----------------------------------------------------------------
AnimaDirectionalLight::AnimaDirectionalLight(AnimaAllocator* allocator, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name)
	: AnimaAmbientLight(allocator, dataGeneratorManager, name)
{
	AnimaMappedValues::SetVector("direction", AnimaVertex3f(-1.0f, -1.0f, -1.0f).Normalized());
	AnimaMappedValues::SetFloat("intensity", 0.0f);
	
	ComputeProjectionMatrix();
	ComputeViewMatrix();

	_type = DIRECTIONAL;
}

AnimaDirectionalLight::~AnimaDirectionalLight()
{
}

void AnimaDirectionalLight::SetDirection(const AnimaVertex3f& direction)
{
	AnimaMappedValues::SetVector("direction", direction.Normalized());
	ComputeViewMatrix();
}

void AnimaDirectionalLight::SetDirection(AFloat x, AFloat y, AFloat z)
{
	AnimaMappedValues::SetVector("direction", AnimaVertex3f(x, y, z).Normalized());
	ComputeViewMatrix();
}

void AnimaDirectionalLight::SetIntensity(AFloat intensity)
{
	AnimaMappedValues::SetFloat("intensity", intensity);
}

AnimaVertex3f AnimaDirectionalLight::GetDirection()
{
	return AnimaMappedValues::GetVector3f("direction");
}

AFloat AnimaDirectionalLight::GetIntensity()
{
	return AnimaMappedValues::GetFloat("intensity");
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
	: AnimaAmbientLight(allocator, dataGeneratorManager, name)
{
	AnimaMappedValues::SetVector("position", 0.0f, 0.0f, 0.0f);
	AnimaMappedValues::SetFloat("intensity", 0.0f);
	AnimaMappedValues::SetFloat("constantAttenuation", 0.0f);
	AnimaMappedValues::SetFloat("linearAttenuation", 0.0f);
	AnimaMappedValues::SetFloat("exponentAttenuation", 1.0f);
	AnimaMappedValues::SetFloat("range", 20.0f);

	_type = POINT;
}

AnimaPointLight::~AnimaPointLight()
{
}

void AnimaPointLight::SetPosition(const AnimaVertex3f& position)
{
	AnimaMappedValues::SetVector("position", position);
}

void AnimaPointLight::SetPosition(AFloat x, AFloat y, AFloat z)
{
	AnimaMappedValues::SetVector("position", x, y, z);
}

void AnimaPointLight::SetIntensity(AFloat intensity)
{
	AnimaMappedValues::SetFloat("intensity", intensity);
}

void AnimaPointLight::SetConstantAttenuation(AFloat attenuation)
{
	AnimaMappedValues::SetFloat("constantAttenuation", attenuation);
}

void AnimaPointLight::SetLinearAttenuation(AFloat attenuation)
{
	AnimaMappedValues::SetFloat("linearAttenuation", attenuation);
}

void AnimaPointLight::SetExponentAttenuation(AFloat attenuation)
{
	AnimaMappedValues::SetFloat("exponentAttenuation", attenuation);
}

void AnimaPointLight::SetRange(AFloat range)
{
	AnimaMappedValues::SetFloat("range", range);
}

AnimaVertex3f AnimaPointLight::GetPosition()
{
	return AnimaMappedValues::GetVector3f("position");
}

AFloat AnimaPointLight::GetIntensity()
{
	return AnimaMappedValues::GetFloat("intensity");
}

AFloat AnimaPointLight::GetConstantAttenuation()
{
	return AnimaMappedValues::GetFloat("constantAttenuation");
}

AFloat AnimaPointLight::GetLinearAttenuation()
{
	return AnimaMappedValues::GetFloat("linearAttenuation");
}

AFloat AnimaPointLight::GetExponentAttenuation()
{
	return AnimaMappedValues::GetFloat("exponentAttenuation");
}

AFloat AnimaPointLight::GetRange()
{
	return AnimaMappedValues::GetFloat("range");
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
	AFloat range = GetRange();
	AnimaVertex3f position = GetPosition();
	AnimaVertex3f cameraPosition = activeCamera->GetPosition();

	float dist = (position - cameraPosition).Length();
	if (dist < range)
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
	AnimaMappedValues::SetVector("direction", AnimaVertex3f(0.0f, -1.0f, 0.0f).Normalized());
	AnimaMappedValues::SetFloat("cutoff", 0.7f);

	_type = SPOT;
}

AnimaSpotLight::~AnimaSpotLight()
{
}

void AnimaSpotLight::SetDirection(const AnimaVertex3f& direction)
{
	AnimaMappedValues::SetVector("direction", direction.Normalized());
	UpdateConeRotation();
}

void AnimaSpotLight::SetDirection(AFloat x, AFloat y, AFloat z)
{
	AnimaMappedValues::SetVector("direction", AnimaVertex3f(x, y, z).Normalized());
	UpdateConeRotation();
}

void AnimaSpotLight::SetCutoff(AFloat c)
{
	AnimaMappedValues::SetFloat("cutoff", c);
}

AnimaVertex3f AnimaSpotLight::GetDirection()
{
	return AnimaMappedValues::GetVector3f("direction");
}

AFloat AnimaSpotLight::GetCutoff()
{
	return AnimaMappedValues::GetFloat("cutoff");
}

void AnimaSpotLight::UpdateMeshTransformation(AnimaTransformation* meshTransformation)
{
	AFloat cutoff = 2.0f * GetCutoff();
	AFloat range = GetRange();
	
	meshTransformation->SetRotation(_coneRotation);
	meshTransformation->SetScale(cutoff, range, cutoff);
	meshTransformation->SetTranslation(GetPosition());
}

void AnimaSpotLight::UpdateCullFace(AnimaCamera* activeCamera)
{
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
