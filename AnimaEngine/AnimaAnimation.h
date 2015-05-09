//
//  AnimaAnimation.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaAnimation__
#define __Anima__AnimaAnimation__

#include "AnimaEngine.h"
#include "AnimaVertex.h"
#include "AnimaQuaternion.h"
#include "AnimaTypes.h"
#include "AnimaMappedArray.h"
#include "AnimaMappedValues.h"
#include "AnimaMesh.h"
#include "AnimaNamedObject.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaAnimationPositionKey;
class AnimaAnimationRotationKey;
class AnimaAnimationScalingKey;
class AnimaAnimationNode;
class AnimaModel;

class ANIMA_ENGINE_EXPORT AnimaAnimation : public AnimaNamedObject
{
public:
	AnimaAnimation(const AnimaString& name, AnimaAllocator* allocator);
	AnimaAnimation(const AnimaAnimation& src);
	AnimaAnimation(AnimaAnimation&& src);
	~AnimaAnimation();
	
	AnimaAnimation& operator=(const AnimaAnimation& src);
	AnimaAnimation& operator=(AnimaAnimation&& src);
	
public:
	void UpdateAnimation(AnimaModel* model, AFloat time);

protected:
	void UpdateModelNodesAnimation(AnimaModel* model, AFloat animationTime, const AnimaMatrix& parentMatrix, const AnimaMatrix& globalInverseMatrix, AnimaMappedArray<AnimaMeshBoneInfo*>* mesheBonesInfo);

public:
	AInt GetAnimationNodesCount() const;
	AnimaAnimationNode* CreateAnimationNode(const AnimaString& name);
	AnimaAnimationNode* CreateAnimationNode(const char* name);
	AnimaAnimationNode* GetAnimationNode(AInt index);
	AnimaMappedArray<AnimaAnimationNode*>* GetAnimationNodes();
	void ClearAnimationNodes();

	void SetDuration(AFloat duration);
	AFloat GetDuration() const;

	void SetTicksPerSecond(AFloat ticksPerSecond);
	AFloat GetTicksPerSecond() const;

protected:
	AFloat _duration;
	AFloat _ticksPerSecond;

	AnimaMappedArray<AnimaAnimationNode*> _animationNodes;
};

class ANIMA_ENGINE_EXPORT AnimaAnimationNode : public AnimaNamedObject
{
public:
	AnimaAnimationNode(const AnimaString& name, AnimaAllocator* allocator);
	~AnimaAnimationNode();

public:
	AnimaVertex3f ComputeInterpolatedScaling(AFloat animationTime) const;
	AnimaQuaternion ComputeInterpolatedRotation(AFloat animationTime) const;
	AnimaVertex3f ComputeInterpolatedPosition(AFloat animationTime) const;

	AInt FindScaling(AFloat animationTime) const;
	AInt FindRotation(AFloat animationTime) const;
	AInt FindPosition(AFloat animationTime) const;

public:
	AInt GetPositionKeysCount() const;
	void AddPositionKey(AnimaAnimationPositionKey key);
	AnimaAnimationPositionKey GetPositionKey(AInt index) const;
	AnimaArray<AnimaAnimationPositionKey>* GetPositionKeys();
	void ClearPositionKeys();

	AInt GetRotationKeysCount() const;
	void AddRotationKey(AnimaAnimationRotationKey key);
	AnimaAnimationRotationKey GetRotationKey(AInt index) const;
	AnimaArray<AnimaAnimationRotationKey>* GetRotationKeys();
	void ClearRotationKeys();

	AInt GetScalingKeysCount() const;
	void AddScalingKey(AnimaAnimationScalingKey key);
	AnimaAnimationScalingKey GetScalingKey(AInt index) const;
	AnimaArray<AnimaAnimationScalingKey>* GetScalingKeys();
	void ClearScalingKeys();

protected:
	AnimaArray<AnimaAnimationPositionKey> _positionKeys;
	AnimaArray<AnimaAnimationRotationKey> _rotationKeys;
	AnimaArray<AnimaAnimationScalingKey> _scalingKeys;
};

class ANIMA_ENGINE_EXPORT AnimaAnimationPositionKey
{
public:
	AnimaAnimationPositionKey();
	AnimaAnimationPositionKey(AFloat time, AnimaVertex3f value);
	~AnimaAnimationPositionKey();

	void SetTime(AFloat time);
	AFloat GetTime() const;

	void SetValue(AnimaVertex3f value);
	AnimaVertex3f GetValue() const;

protected:
	AFloat _time;
	AnimaVertex3f _value;
};

class ANIMA_ENGINE_EXPORT AnimaAnimationRotationKey
{
public:
	AnimaAnimationRotationKey();
	AnimaAnimationRotationKey(AFloat time, AnimaQuaternion value);
	~AnimaAnimationRotationKey();

	void SetTime(AFloat time);
	AFloat GetTime() const;

	void SetValue(AnimaQuaternion value);
	AnimaQuaternion GetValue() const;

protected:
	AFloat _time;
	AnimaQuaternion _value;
};

class ANIMA_ENGINE_EXPORT AnimaAnimationScalingKey
{
public:
	AnimaAnimationScalingKey();
	AnimaAnimationScalingKey(AFloat time, AnimaVertex3f value);
	~AnimaAnimationScalingKey();

	void SetTime(AFloat time);
	AFloat GetTime() const;

	void SetValue(AnimaVertex3f value);
	AnimaVertex3f GetValue() const;

protected:
	AFloat _time;
	AnimaVertex3f _value;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaAnimation__) */
