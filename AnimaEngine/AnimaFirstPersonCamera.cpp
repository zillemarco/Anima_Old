//
//  AnimaFirstPersonCamera.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaFirstPersonCamera.h"
#include "AnimaMath.h"
#include "AnimaCamerasManager.h"
#include "AnimaXmlTranslators.h"

#define _USE_MATH_DEFINES
#include <math.h>

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaFirstPersonCamera::AnimaFirstPersonCamera(AnimaAllocator* allocator, AnimaCamerasManager* camerasManager, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name)
	: AnimaCamera(allocator, camerasManager, dataGeneratorManager, name)
{
	IMPLEMENT_ANIMA_CLASS(AnimaFirstPersonCamera);
	_zAxis.x = 0.0f;
	_zAxis.y = 0.0f;
	_zAxis.z = 1.0f;
}

AnimaFirstPersonCamera::AnimaFirstPersonCamera(AnimaAllocator* allocator, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name)
	: AnimaCamera(allocator, nullptr, dataGeneratorManager, name)
{
	IMPLEMENT_ANIMA_CLASS(AnimaFirstPersonCamera);
	_zAxis.x = 0.0f;
	_zAxis.y = 0.0f;
	_zAxis.z = 1.0f;
}

AnimaFirstPersonCamera::AnimaFirstPersonCamera(AnimaAllocator* allocator, AnimaCamerasManager* camerasManager, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name, const AnimaVertex3f& position, const AnimaVertex3f& forward, const AnimaVertex3f& up)
	: AnimaCamera(allocator, camerasManager, dataGeneratorManager, name)
{
	IMPLEMENT_ANIMA_CLASS(AnimaFirstPersonCamera);
	_position = position;
	_yAxis = up;
	_zAxis = forward;

	_yAxis.Normalize();
	_zAxis.Normalize();
}

AnimaFirstPersonCamera::AnimaFirstPersonCamera(const AnimaFirstPersonCamera& src)
	: AnimaCamera(src)
{
}

AnimaFirstPersonCamera::AnimaFirstPersonCamera(AnimaFirstPersonCamera&& src)
	: AnimaCamera(src)
{
}

AnimaFirstPersonCamera::~AnimaFirstPersonCamera()
{
}

AnimaFirstPersonCamera& AnimaFirstPersonCamera::operator=(const AnimaFirstPersonCamera& src)
{
	AnimaCamera::operator=(src);
	return *this;
}

AnimaFirstPersonCamera& AnimaFirstPersonCamera::operator=(AnimaFirstPersonCamera&& src)
{
	AnimaCamera::operator=(src);
	return *this;
}

ptree AnimaFirstPersonCamera::GetObjectTree(bool saveName) const
{
	ptree tree;
	
	if (saveName)
		tree.add("AnimaFirstPersonCamera.Name", GetName());
	
	tree.add_child("AnimaFirstPersonCamera.Camera", AnimaCamera::GetObjectTree(false));
	
	return tree;
}

bool AnimaFirstPersonCamera::ReadObject(const ptree& objectTree, AnimaScene* scene, bool readName)
{
	try
	{
		if (readName)
			SetName(objectTree.get<AnimaString>("AnimaFirstPersonCamera.Name"));
		
		ptree cameraTree = objectTree.get_child("AnimaFirstPersonCamera.Camera");
		return AnimaCamera::ReadObject(cameraTree, scene, false);
	}
	catch (boost::property_tree::ptree_bad_path& exception)
	{
		AnimaLogger::LogMessageFormat("ERROR - Error parsing first person camera: %s", exception.what());
		return false;
	}
	catch (boost::property_tree::ptree_bad_data& exception)
	{
		AnimaLogger::LogMessageFormat("ERROR - Error parsing first person camera: %s", exception.what());
		return false;
	}
}

void AnimaFirstPersonCamera::Zoom(AFloat amount)
{
	Move(_zAxis, amount);
}

void AnimaFirstPersonCamera::Move(const AnimaVertex3f& direction, AFloat amount)
{
	_position += direction * amount;

	LookAt(_position, _zAxis);
}

void AnimaFirstPersonCamera::Move(const AFloat& xDirection, const AFloat& yDirection, const AFloat& zDirection, AFloat amount)
{
	_position.x += xDirection * amount;
	_position.y += yDirection * amount;
	_position.z += zDirection * amount;

	LookAt(_position, _zAxis);
}

void AnimaFirstPersonCamera::RotateX(AFloat angle)
{
	AnimaVertex3f hAxis = _worldYAxis ^ _zAxis;
	hAxis.Normalize();
	
	AnimaMath::RotateVector(_zAxis, angle, hAxis);
	_zAxis.Normalize();

	AnimaVertex3f newUp = _zAxis ^ hAxis;
	newUp.Normalize();
	_yAxis = newUp;

	CalculateViewMatrix();
}

void AnimaFirstPersonCamera::RotateXDeg(AFloat angle)
{
	RotateX(angle * (float)M_PI / 180.0f);
}

void AnimaFirstPersonCamera::RotateY(AFloat angle)
{
	AnimaVertex3f hAxis = _worldYAxis ^ _zAxis;
	hAxis.Normalize();
	
	AnimaMath::RotateVector(_zAxis, angle, _worldYAxis);
	_zAxis.Normalize();

	AnimaVertex3f newUp = _zAxis ^ hAxis;
	newUp.Normalize();
	_yAxis = newUp;

	CalculateViewMatrix();
}

void AnimaFirstPersonCamera::RotateYDeg(AFloat angle)
{
	RotateY(angle * (float)M_PI / 180.0f);
}

void AnimaFirstPersonCamera::LookAt(const AnimaVertex3f& position, const AnimaVertex3f& forward)
{
	SetPosition(position);
	
	_zAxis = forward;
	_zAxis.Normalize();

	_xAxis = _zAxis ^ _yAxis;
	_xAxis.Normalize();

	_yAxis = _xAxis ^ _zAxis;
	_yAxis.Normalize();

	CalculateViewMatrix();
}

void AnimaFirstPersonCamera::LookAt(AFloat xPosition, AFloat yPosition, AFloat zPosition, AFloat xForward, AFloat yForward, AFloat zForward)
{
	AnimaVertex3f position(xPosition, yPosition, zPosition);
	AnimaVertex3f forward(xForward, yForward, zForward);
	LookAt(position, forward);
}

void AnimaFirstPersonCamera::CalculateViewMatrix()
{
	_viewMatrix.LookAt(_position, _zAxis, _yAxis);
	
	_projectionViewMatrix = _projectionMatrix * _viewMatrix;
	_inverseProjectionViewMatrix = _projectionViewMatrix.Inversed();

	_frustum.ComputeFrustum(_projectionViewMatrix);

	SetMatrix("ViewMatrix", _viewMatrix);
	SetMatrix("ProjectionViewMatrix", _projectionViewMatrix);
	SetMatrix("InverseProjectionViewMatrix", _inverseProjectionViewMatrix);
}

END_ANIMA_ENGINE_NAMESPACE
