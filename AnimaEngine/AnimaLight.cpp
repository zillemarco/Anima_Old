//
//  AnimaLight.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaLight.h"
#include "AnimaDataGeneratorsManager.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

//----------------------------------------------------------------
//						ANIMA LIGHT
//----------------------------------------------------------------
AnimaLight::AnimaLight(AnimaEngine* engine)
	: AnimaMappedValues(engine)
{
}

AnimaLight::AnimaLight(const AnimaLight& src)
	: AnimaMappedValues(src)
{
}

AnimaLight::AnimaLight(AnimaLight&& src)
	: AnimaMappedValues(src)
{
}

AnimaLight::~AnimaLight()
{
}

AnimaLight& AnimaLight::operator=(const AnimaLight& src)
{
	AnimaMappedValues::operator=(src);
	return *this;
}

AnimaLight& AnimaLight::operator=(AnimaLight&& src)
{
	AnimaMappedValues::operator=(src);
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
	return nullptr;
}

AFloat AnimaLight::GetIntensity()
{
	ANIMA_ASSERT(false);
	return 0.0f;
}

AnimaVertex3f AnimaLight::GetDirection()
{
	ANIMA_ASSERT(false);
	return nullptr;
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
	return nullptr;
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

//----------------------------------------------------------------
//						ANIMA AMBIENT LIGHT
//----------------------------------------------------------------
AnimaAmbientLight::AnimaAmbientLight(AnimaEngine* engine)
	: AnimaLight(engine)
{
	AnimaMappedValues::SetColor("color", 1.0f, 1.0f, 1.0f);
	AnimaMappedValues::SetFloat("intensity", 1.0f);
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

//----------------------------------------------------------------
//						ANIMA DIRECTIONAL LIGHT
//----------------------------------------------------------------
AnimaDirectionalLight::AnimaDirectionalLight(AnimaEngine* engine)
	: AnimaAmbientLight(engine)
{
	AnimaMappedValues::SetVector("direction", -1.0f, -1.0f, -1.0f);
	AnimaMappedValues::SetFloat("intensity", 0.0f);
}

AnimaDirectionalLight::~AnimaDirectionalLight()
{
}

void AnimaDirectionalLight::SetDirection(const AnimaVertex3f& direction)
{
	AnimaMappedValues::SetVector("direction", direction);
}

void AnimaDirectionalLight::SetDirection(AFloat x, AFloat y, AFloat z)
{
	AnimaMappedValues::SetVector("direction", x, y, z);
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

//----------------------------------------------------------------
//						ANIMA POINT LIGHT
//----------------------------------------------------------------
AnimaPointLight::AnimaPointLight(AnimaEngine* engine)
	: AnimaAmbientLight(engine)
{
	AnimaMappedValues::SetVector("position", 0.0f, 0.0f, 0.0f);
	AnimaMappedValues::SetFloat("intensity", 0.0f);
	AnimaMappedValues::SetFloat("constantAttenuation", 0.0f);
	AnimaMappedValues::SetFloat("linearAttenuation", 0.0f);
	AnimaMappedValues::SetFloat("exponentAttenuation", 1.0f);
	AnimaMappedValues::SetFloat("range", 20.0f);
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

END_ANIMA_ENGINE_NAMESPACE
