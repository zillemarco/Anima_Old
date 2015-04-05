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
#include "AnimaFrustum.h"

#ifndef min
#	define min(a,b) (a < b ? a : b)
#endif

#ifndef max
#	define max(a,b) (a > b ? a : b)
#endif

BEGIN_ANIMA_ENGINE_NAMESPACE

//----------------------------------------------------------------
//						ANIMA LIGHT
//----------------------------------------------------------------
AnimaLight::AnimaLight(AnimaAllocator* allocator, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name)
	: AnimaSceneObject(name, dataGeneratorManager, allocator)
{
	AnimaSceneObject::SetTexture("ShadowMap", AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*_allocator, _allocator, "ShadowMap", GL_TEXTURE_2D, 1024, 1024, nullptr, 0, 0, GL_LINEAR, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT, GL_CLAMP_TO_BORDER, GL_DEPTH_ATTACHMENT));
	//AnimaSceneObject::SetTexture("ShadowMap", AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*_allocator, _allocator, GL_TEXTURE_2D, 1024, 1024, nullptr, 0, 0, GL_LINEAR, GL_RG32F, GL_RGBA, GL_FLOAT, GL_CLAMP_TO_EDGE, GL_COLOR_ATTACHMENT0));
	//AnimaSceneObject::SetTexture("TempShadowMap", AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*_allocator, _allocator, GL_TEXTURE_2D, 1024, 1024, nullptr, 0, 0, GL_LINEAR, GL_RG32F, GL_RGBA, GL_FLOAT, GL_CLAMP_TO_EDGE, GL_COLOR_ATTACHMENT0));
	
	ComputeLightMatrix(nullptr);

	AnimaSceneObject::SetColor("Color", 1.0f, 1.0f, 1.0f);
	AnimaSceneObject::SetFloat("Intensity", 1.0f);
}

AnimaLight::AnimaLight(const AnimaLight& src)
	: AnimaSceneObject(src)
{
}

AnimaLight::AnimaLight(AnimaLight&& src)
	: AnimaSceneObject(src)
{
}

AnimaLight::~AnimaLight()
{
	AnimaTexture* shadowMap = AnimaSceneObject::GetTexture("ShadowMap");
	if (shadowMap != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_allocator, shadowMap);
		shadowMap = nullptr;
	}

	AnimaTexture* tempShadowMap = AnimaSceneObject::GetTexture("TempShadowMap");
	if (tempShadowMap != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_allocator, tempShadowMap);
		tempShadowMap = nullptr;
	}
}

AnimaLight& AnimaLight::operator=(const AnimaLight& src)
{
	if (this != &src)
		AnimaSceneObject::operator=(src);
	return *this;
}

AnimaLight& AnimaLight::operator=(AnimaLight&& src)
{
	if (this != &src)
		AnimaSceneObject::operator=(src);
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

AnimaTexture* AnimaLight::GetShadowTexture()
{
	return AnimaSceneObject::GetTexture("ShadowMap");
}

AnimaTexture* AnimaLight::GetTempShadowTexture()
{
	return AnimaSceneObject::GetTexture("TempShadowMap");
}

AnimaMatrix AnimaLight::GetViewMatrix()
{
	return AnimaSceneObject::GetMatrix("ViewMatrix");
}

AnimaMatrix AnimaLight::GetProjectionMatrix()
{
	return AnimaSceneObject::GetMatrix("ProjectionMatrix");
}

AnimaMatrix AnimaLight::GetProjectionViewMatrix()
{
	return AnimaSceneObject::GetMatrix("ProjectionViewMatrix");
}

void AnimaLight::ComputeLightMatrix(AnimaCamera* activeCamera)
{
	AnimaSceneObject::SetMatrix("ViewMatrix", AnimaMatrix());
	AnimaSceneObject::SetMatrix("ProjectionMatrix", AnimaMatrix());
	AnimaSceneObject::SetMatrix("ProjectionViewMatrix", AnimaMatrix());
}

AnimaFrustum* AnimaLight::GetFrustum()
{
	return &_frustum;
}

//----------------------------------------------------------------
//						ANIMA DIRECTIONAL LIGHT
//----------------------------------------------------------------
AnimaDirectionalLight::AnimaDirectionalLight(AnimaAllocator* allocator, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name)
	: AnimaLight(allocator, dataGeneratorManager, name)
{
	AnimaLight::SetVector("Direction", AnimaVertex3f(-1.0f, -1.0f, -1.0f).Normalized());
	AnimaLight::SetVector("ShadowMapTexelSize", AnimaVertex2f(1.0f / 1024.0f));
	AnimaLight::SetFloat("ShadowMapBias", 1.0f / 1024.0f);
	
	ComputeLightMatrix(nullptr);
}

AnimaDirectionalLight::~AnimaDirectionalLight()
{
}

void AnimaDirectionalLight::SetDirection(const AnimaVertex3f& direction)
{
	AnimaSceneObject::SetVector("Direction", direction.Normalized());
	ComputeLightMatrix(nullptr);	// TODO
}

void AnimaDirectionalLight::SetDirection(AFloat x, AFloat y, AFloat z)
{
	AnimaSceneObject::SetVector("Direction", AnimaVertex3f(x, y, z).Normalized());
	ComputeLightMatrix(nullptr);	// TODO
}

AnimaVertex3f AnimaDirectionalLight::GetDirection()
{
	return AnimaSceneObject::GetVector3f("Direction");
}

void AnimaDirectionalLight::ComputeLightMatrix(AnimaCamera* activeCamera)
{
	if (activeCamera == nullptr)
		return;

	AnimaFrustum* cameraFrustum = activeCamera->GetFrustum();

	AnimaVertex3f direction = GetDirection();												// asse z
	AnimaVertex3f perpVec1 = (direction ^ AnimaVertex3f(0.0f, 0.0f, 1.0f)).Normalized();	// asse y
	AnimaVertex3f perpVec2 = (direction ^ perpVec1).Normalized();							// asse x

	AnimaMatrix rotMat;	
	rotMat.m[0] = perpVec2.x;	rotMat.m[1] = perpVec1.x;	rotMat.m[2] =	direction.x;
	rotMat.m[4] = perpVec2.y;	rotMat.m[5] = perpVec1.y;	rotMat.m[6] =	direction.y;
	rotMat.m[8] = perpVec2.z;	rotMat.m[9] = perpVec1.z;	rotMat.m[10] =	direction.z;
	
	AnimaVertex3f frustumVertices[8];
	cameraFrustum->GetFrustumVertices(frustumVertices);
	
	for (AInt i = 0; i < 8; i++)
		frustumVertices[i] = rotMat * frustumVertices[i];

	AnimaVertex3f minV = frustumVertices[0], maxV = frustumVertices[0];
	for (AInt i = 1; i < 8; i++)
	{
		minV.x = min(minV.x, frustumVertices[i].x);
		minV.y = min(minV.y, frustumVertices[i].y);
		minV.z = min(minV.z, frustumVertices[i].z);
		maxV.x = max(maxV.x, frustumVertices[i].x);
		maxV.y = max(maxV.y, frustumVertices[i].y);
		maxV.z = max(maxV.z, frustumVertices[i].z);
	}

	AnimaVertex3f extends = maxV - minV;
	extends *= 0.5f;
	
	AnimaMatrix viewMatrix = AnimaMatrix::MakeLookAt(cameraFrustum->GetBoundingBoxCenter(), direction, perpVec1);
	AnimaMatrix projectionMatrix = AnimaMatrix::MakeOrtho(-extends.x, extends.x, -extends.y, extends.y, -extends.z, extends.z);
	AnimaMatrix projectionViewMatrix = projectionMatrix * viewMatrix;

	AnimaSceneObject::SetMatrix("ViewMatrix", viewMatrix);
	AnimaSceneObject::SetMatrix("ProjectionMatrix", projectionMatrix);
	AnimaSceneObject::SetMatrix("ProjectionViewMatrix", projectionViewMatrix);

	_frustum.ComputeFrustum(projectionViewMatrix);
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


//----------------------------------------------------------------
//						ANIMA HEMISPHERE LIGHT
//----------------------------------------------------------------
AnimaHemisphereLight::AnimaHemisphereLight(AnimaAllocator* allocator, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name)
	: AnimaLight(allocator, dataGeneratorManager, name)
{
	AnimaSceneObject::SetColor("SkyColor", AnimaColor3f(1.0f, 1.0f, 1.0f));
	AnimaSceneObject::SetColor("GroundColor", AnimaColor3f(0.0f, 0.0f, 0.0f));
}

AnimaHemisphereLight::~AnimaHemisphereLight()
{
}

void AnimaHemisphereLight::SetSkyColor(const AnimaColor3f& color)
{
	AnimaSceneObject::SetColor("SkyColor", color);
}

void AnimaHemisphereLight::SetSkyColor(AFloat r, AFloat g, AFloat b)
{
	AnimaSceneObject::SetColor("SkyColor", AnimaColor3f(r, g, b));
}

AnimaColor3f AnimaHemisphereLight::GetSkyColor()
{
	return AnimaSceneObject::GetColor3f("SkyColor");
}

void AnimaHemisphereLight::SetGroundColor(const AnimaColor3f& color)
{
	AnimaSceneObject::SetColor("GroundColor", color);
}

void AnimaHemisphereLight::SetGroundColor(AFloat r, AFloat g, AFloat b)
{
	AnimaSceneObject::SetColor("GroundColor", AnimaColor3f(r, g, b));
}

AnimaColor3f AnimaHemisphereLight::GetGroundColor()
{
	return AnimaSceneObject::GetColor3f("GroundColor");
}

void AnimaHemisphereLight::UpdateMeshTransformation(AnimaTransformation* meshTransformation)
{
	meshTransformation->SetRotationDeg(90.0f, 0.0f, 0.0f);
	meshTransformation->SetScale(1.0f, 1.0f, 1.0f);
	meshTransformation->SetTranslation(0.0f, 0.0f, 0.0f);
}

void AnimaHemisphereLight::UpdateCullFace(AnimaCamera* activeCamera)
{
}

const char* AnimaHemisphereLight::GetShaderPrefix()
{
	return "HEL";
}

const char* AnimaHemisphereLight::GetShaderName()
{
	return "deferred-hemisphere";
}

bool AnimaHemisphereLight::CreateShader(AnimaShadersManager* shadersManager)
{
	if (shadersManager->GetProgramFromName("deferred-hemisphere"))
		return true;

	AnimaShaderProgram* pgr = shadersManager->CreateProgram("deferred-hemisphere");

	if (pgr == nullptr)
		return false;

	if (!pgr->Create())
		return false;

	pgr->AddShader(shadersManager->LoadShaderFromFile("deferred-hemisphere-vs", ANIMA_ENGINE_SHADERS_PATH "Deferred/deferred-hemisphere-vs.glsl", Anima::AnimaShader::VERTEX));
	pgr->AddShader(shadersManager->LoadShaderFromFile("deferred-hemisphere-fs", ANIMA_ENGINE_SHADERS_PATH "Deferred/deferred-hemisphere-fs.glsl", Anima::AnimaShader::FRAGMENT));

	if (!pgr->Link())
		return false;

	return true;
}

END_ANIMA_ENGINE_NAMESPACE
