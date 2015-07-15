//
//  AnimaAnimationsManager.cpp
//  Anima
//
//  Created by Marco Zille on 20/11/14.
//
//

#include "AnimaAnimationsManager.h"
#include "AnimaTimer.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaAnimationsManager::AnimaAnimationsManager(AnimaScene* scene)
{
	ANIMA_ASSERT(scene != nullptr);
	_scene = scene;
}

AnimaAnimationsManager::~AnimaAnimationsManager()
{
	ClearAnimations();
	ClearLastLoadedAnimations();
}

bool AnimaAnimationsManager::LoadAnimations(const aiScene* scene)
{
	ClearLastLoadedAnimations();

	if (!scene->HasAnimations())
		return false;

	for (AUint na = 0; na < scene->mNumAnimations; na++)
	{
		aiAnimation* animation = scene->mAnimations[na];
		
		AnimaString animationName;
		AnimaString originalAnimationName(animation->mName.C_Str());

		if (originalAnimationName.empty())
			originalAnimationName = "Animation";

		AnimaAnimation* newAnimation = nullptr;
		int nameOffset = 0;
		while (newAnimation == nullptr)
		{
			if (nameOffset > 0)
				animationName = FormatString("%s%d", originalAnimationName.c_str(), nameOffset);
			else
				animationName = FormatString("%s", originalAnimationName.c_str());

			newAnimation = CreateEmptyAnimation(animationName);
			nameOffset++;
		}

		AInt animationNodesCount = animation->mNumChannels;
		for (AInt nc = 0; nc < animationNodesCount; nc++)
		{
			aiNodeAnim* animationNode = animation->mChannels[nc];
			AnimaAnimationNode* newAnimationNode = newAnimation->CreateAnimationNode(animationNode->mNodeName.C_Str());

			AInt animationNodePositionKeysCount = animationNode->mNumPositionKeys;
			for (AInt npk = 0; npk < animationNodePositionKeysCount; npk++)
			{
				aiVectorKey key = animationNode->mPositionKeys[npk];
				newAnimationNode->AddPositionKey(AnimaAnimationPositionKey((AFloat)key.mTime, AnimaVertex3f(key.mValue.x, key.mValue.y, key.mValue.z)));
			}

			AInt animationNodeRotationKeysCount = animationNode->mNumRotationKeys;
			for (AInt nrk = 0; nrk < animationNodeRotationKeysCount; nrk++)
			{
				aiQuatKey key = animationNode->mRotationKeys[nrk];
				newAnimationNode->AddRotationKey(AnimaAnimationRotationKey((AFloat)key.mTime, AnimaQuaternion(key.mValue.x, key.mValue.y, key.mValue.z, key.mValue.w)));
			}

			AInt animationNodeScalingKeysCount = animationNode->mNumScalingKeys;
			for (AInt nsk = 0; nsk < animationNodeScalingKeysCount; nsk++)
			{
				aiVectorKey key = animationNode->mScalingKeys[nsk];
				newAnimationNode->AddScalingKey(AnimaAnimationScalingKey((AFloat)key.mTime, AnimaVertex3f(key.mValue.x, key.mValue.y, key.mValue.z)));
			}
		}

		newAnimation->SetTicksPerSecond((AFloat)animation->mTicksPerSecond);
		newAnimation->SetDuration((AFloat)animation->mDuration);

		_lastLoadedAnimations.Add(animationName, newAnimation);
		_animations.Add(animationName, newAnimation);
	}

	return true;
}

AnimaAnimation* AnimaAnimationsManager::CreateEmptyAnimation(const AnimaString& animationName)
{
	AInt index = _animations.Contains(animationName);
	if (index >= 0)
		return nullptr;

	AnimaAnimation* newAnimation = AnimaAllocatorNamespace::AllocateNew<AnimaAnimation>(*(_scene->GetAnimationsAllocator()), animationName, _scene->GetAnimationsAllocator());
	_animations.Add(animationName, newAnimation);

	return newAnimation;
}

AInt AnimaAnimationsManager::GetAnimationsCount()
{
	return _animations.GetSize();
}

AnimaAnimation* AnimaAnimationsManager::GetAnimation(AInt index)
{
	return _animations[index];
}

AnimaAnimation* AnimaAnimationsManager::GetAnimationFromName(const AnimaString& name)
{
	return _animations[name];
}

void AnimaAnimationsManager::ClearAnimations()
{
	AInt count = _animations.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaAnimation* animation = _animations[i];
		AnimaAllocatorNamespace::DeallocateObject(*(_scene->GetAnimationsAllocator()), animation);
		animation = nullptr;
	}

	_animations.RemoveAll();
}

void AnimaAnimationsManager::ClearLastLoadedAnimations()
{
	_lastLoadedAnimations.RemoveAll();
}

AnimaMappedArray<AnimaAnimation*>* AnimaAnimationsManager::GetLastLoadedAnimations()
{
	return &_lastLoadedAnimations;
}

END_ANIMA_ENGINE_NAMESPACE